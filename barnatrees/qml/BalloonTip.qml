import QtQuick 2.12
import QtQuick.Controls 2.12

ToolTip {
    id: control
    x: parent ? (parent.width - implicitWidth) / 2 : 0
    y: -implicitHeight - 8

    background: Rectangle {
        color: "black"
        border.color: "black"
        radius: 6
        Rectangle {
            x: -control.x
            z: -1
            width: 10
            height: 10
            rotation: 45
            color: "black"
            border.color: "black"
            anchors.verticalCenter: (control.y < 0) ? parent.bottom : parent.top
        }
    }
}
