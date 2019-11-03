import QtQuick 2.12
import QtQuick.Controls 2.12

ToolTip {
    id: control
    x: clcx
    y: -implicitHeight - 8

    property double clcx: parent ? (parent.width - implicitWidth) / 2 : 0

    background: Rectangle {
        color: control.palette.toolTipBase
        border.color: control.palette.toolTipBase
        radius: 6
        Rectangle {
            id: arrow
            x: (clcx === control.x) ? (control.width / 2) - 5 : -control.x
            z: -1
            width: 10
            height: 10
            rotation: 45
            color: control.palette.toolTipBase
            border.color: control.palette.toolTipBase
            anchors.verticalCenter: (control.y < 0) ? parent.bottom : parent.top
        }
    }
}
