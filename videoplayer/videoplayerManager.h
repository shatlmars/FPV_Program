#pragma once

#include <QObject>
#include <QString>
#include <QVideoSink>
#include <QDebug>
#include <QTimer>
#include <QVideoFrameFormat>
#include <QVideoFrame>

struct _GstElement;


class VideoPlayer: public QObject{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink  WRITE setVideoSink  NOTIFY videoSinkChanged)
    Q_PROPERTY(bool playing READ playing NOTIFY playingChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
public:
    explicit VideoPlayer(QObject* parent = nullptr);
    ~VideoPlayer() override;

    QString url() const { return _url;}
    void setUrl(const QString& url);

    QVideoSink* videoSink() const {return _videoSink;}
    void setVideoSink(QVideoSink* sink);

    bool playing() const {return _playing;}
    QString errorString() const {return _error;}

    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
signals:
    void urlChanged();
    void videoSinkChanged();
    void playingChanged();
    void errorStringChanged();

private:
    void setError(const QString &text);
    void setPlaying(bool on);
    void releasePipeline();
    void pollBus();

    QString _url;
    QVideoSink* _videoSink;
    QString _error;
    bool _playing{false};
    _GstElement *_pipeline{nullptr};
    QTimer* _busTimer{nullptr};


};
