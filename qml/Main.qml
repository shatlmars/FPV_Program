import QtQuick

Window {
    id: root
    width: 800
    height: 480
    visible: true
    title: qsTr("FPV Гарантия развития")
    color: "#121212"

    // function knobX(axis, size) { return size / 2 - 16 + axis * (size / 2 - 24) }
    // function knobY(axis, size) { return size / 2 - 16 + axis * (size / 2 - 24) }

    // Row {
    //     anchors.centerIn: parent
    //     spacing: 80

    //     Repeater {
    //         model: [
    //             { title: "Левый",  x: gamepad.leftX,  y: gamepad.leftY },
    //             { title: "Правый", x: gamepad.rightX, y: gamepad.rightY }
    //         ]

    //         delegate: Column {
    //             spacing: 12

    //             Text {
    //                 text: modelData.title
    //                 color: "white"
    //                 font.pixelSize: 18
    //                 anchors.horizontalCenter: parent.horizontalCenter
    //             }

    //             Item {
    //                 width: 220
    //                 height: 220

    //                 Rectangle {
    //                     anchors.fill: parent
    //                     radius: width / 2
    //                     color: "#1e1e1e"
    //                     border.color: gamepad.connected ? "#4ea1ff" : "#555"
    //                     border.width: 2
    //                 }

    //                 Rectangle {
    //                     width: 32
    //                     height: 32
    //                     radius: 16
    //                     color: "#4ea1ff"
    //                     x: knobX(modelData.x, parent.width)
    //                     y: knobY(modelData.y, parent.height)
    //                 }
    //             }

    //             Text {
    //                 text: "X " + modelData.x.toFixed(2) + "   Y " + modelData.y.toFixed(2)
    //                 color: "#ccc"
    //                 font.pixelSize: 14
    //                 anchors.horizontalCenter: parent.horizontalCenter
    //             }
    //         }
    //     }
    // }

    // Text {
    //     anchors.bottom: parent.bottom
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     anchors.bottomMargin: 20
    //     text: gamepad.connected ? gamepad.name : "Подключи геймпад"
    //     color: gamepad.connected ? "#8fd18f" : "#e07070"
    //     font.pixelSize: 16
    // }
}