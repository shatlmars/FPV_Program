#include "videoplayer/videoplayerManager.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

namespace {

bool gstReady{false};

void ensureGst(){
    if(gstReady){
        return;
    }
    gst_init(nullptr, nullptr);
    gstReady = true;
}

GstFlowReturn onNewSample(GstAppSink* appSink, gpointer user){
    auto* self = static_cast<VideoPlayer*>(user);

    GstSample* sample = gst_app_sink_pull_sample(appSink);
    if(!sample)
        return GST_FLOW_ERROR;
    GstBuffer* buffer = gst_sample_get_buffer(sample);
    GstCaps *caps = gst_sample_get_caps(sample);

    GstVideoInfo info;
    if(!gst_video_info_from_caps(&info, caps)){
        gst_sample_unref(sample);
        return GST_FLOW_ERROR;
    }

    GstMapInfo map;
    if(!gst_buffer_map(buffer, &map, GST_MAP_READ)){
        gst_sample_unref(sample);
        return GST_FLOW_ERROR;
    }

    QVideoFrameFormat fmt(
        QSize(info.width, info.height),
        QVideoFrameFormat::Format_BGRX8888);

    QVideoFrame frame(fmt);
    if(frame.map(QVideoFrame::WriteOnly)){
        const gsize n = qMin<gsize>(map.size, gsize(frame.mappedBytes(0)));
        memcpy(frame.bits(0), map.data, n);
        frame.unmap();

        if(QVideoSink* sink = self->videoSink())
            sink->setVideoFrame(frame);
    }

    gst_buffer_unmap(buffer, &map);
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

}

VideoPlayer::VideoPlayer(QObject* parent): QObject(parent), _busTimer(new QTimer(this)){
    ensureGst();

    _busTimer->setInterval(100);
    connect(_busTimer, &QTimer::timeout, this, &VideoPlayer::pollBus);
}

VideoPlayer::~VideoPlayer(){
    stop();
}

void VideoPlayer::setUrl(const QString &url){
    if(_url == url) return;
    _url = url;
    emit urlChanged();
}

void VideoPlayer::setVideoSink(QVideoSink* sink){
    if(_videoSink == sink) return;
    _videoSink = sink;
    emit videoSinkChanged();
}

void VideoPlayer::setError(const QString& text){
    if(_error == text) return;
    _error = text;
    emit errorStringChanged();
}


void VideoPlayer::setPlaying(bool on){
    if(_playing == on) return;
    _playing = on;
    emit playingChanged();
}

void VideoPlayer::releasePipeline(){
    _busTimer->stop();
    if(!_pipeline) return;
    gst_element_set_state(_pipeline, GST_STATE_NULL);
    gst_object_unref(_pipeline);
    _pipeline = nullptr;
}

void VideoPlayer::play(){
    stop();

    if(_url.isEmpty()){
        setError(QStringLiteral("RTSP URL EMPTY"));
        return;
    }

    const QString desc =
        QStringLiteral(
            "rtspsrc location=\"%1\" latency=80 protocols=udp ! "
            "decodebin ! videoconvert ! videoscale ! "
            "video/x-raw, format=BGRx ! "
            "appsink name=mysink emit-signals=true sync=false "
            "max-buffers=2 drop=true").arg(_url);
    GError* err = nullptr;
    _pipeline = gst_parse_launch(desc.toUtf8().constData(), &err);
    if(!_pipeline){
        setError(err ? QString::fromUtf8(err->message) : QStringLiteral("Cannot create pipeline"));
        if (err)
            g_error_free(err);
        return;
    }

    GstElement* sink = gst_bin_get_by_name(GST_BIN(_pipeline), "mysink");
    if(!sink){
        setError(QStringLiteral("appsink not found"));
        releasePipeline();
        return;
    }

    g_signal_connect(sink, "new-sample", G_CALLBACK(onNewSample), this);
    gst_object_unref(sink);
    const GstStateChangeReturn ret =
        gst_element_set_state(_pipeline, GST_STATE_PLAYING);
    if(ret == GST_STATE_CHANGE_FAILURE){
        setError(QStringLiteral("Cannot start pipeline"));
        releasePipeline();
        return;
    }

    setError({});
    setPlaying(true);
    _busTimer->start();
}

void VideoPlayer::stop()
{
    releasePipeline();
    setPlaying(false);
}

void VideoPlayer::pollBus(){
    if(!_pipeline) return;
    GstBus* bus = gst_element_get_bus(_pipeline);
    GstMessage* msg = nullptr;

    while((msg = gst_bus_pop(bus))){
        switch(GST_MESSAGE_TYPE(msg)){
        case GST_MESSAGE_ERROR: {
            GError* err = nullptr;
            gchar* dbg = nullptr;
            gst_message_parse_error(msg, &err, &dbg);
            const QString text = err ? QString::fromUtf8(err->message) : QStringLiteral("GStreamer error");
            qWarning() << "GST error:" << text << dbg;
            setError(text);
            if(err) g_error_free(err);
            g_free(dbg);
            stop();
            break;
        }
        case GST_MESSAGE_EOS:
            setError(QStringLiteral("Stream stoped"));
            stop();
            break;
        default:
            break;
        }
        gst_message_unref(msg);

    }
    gst_object_unref(bus);
}






