/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
import QtQuick
import QtQuick.Controls

ToolTip {
    id: control
    x: parent ? (parent.width - implicitWidth) / 2 : 0
    y: -implicitHeight - 8

    onVisibleChanged: {
        if (visible) console.log("x=",control.x,"y=",control.y);
    }

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
