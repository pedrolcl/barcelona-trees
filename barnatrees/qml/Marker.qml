import QtQuick 2.12

Rectangle {
    width: image.width
    property string markText: "0"
    Image {
        id: image
        anchors.centerIn: parent
        source: "marker.png"
        Text {
            y: parent.height/10
            width: parent.width
            color: "white"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            text: markText
        }
    }
}
