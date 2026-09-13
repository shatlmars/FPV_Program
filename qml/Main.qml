import QtQuick
import QtMultimedia
import FPV_GR 1.0

Window {
    id: root
    width: 1280
    height: 720
    visible: true
    color: "black"
    title: "FPV_GR"

    VideoOutput {
        id: vo
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit
    }

    VideoPlayer {
        id: player
        url: "rtsp://127.0.0.1:554/live"
        videoSink: vo.videoSink
        Component.onCompleted: play()
    }

    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 12
        color: "white"
        text: player.errorString !== "" ? player.errorString
              : (player.playing ? "PLAY" : "STOP")
    }
}