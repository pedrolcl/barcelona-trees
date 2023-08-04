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

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("<h1>Specimen</h1>")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Qt.platform.os === "android" ? Screen.width : Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Close

    property string specieScientificName: ""
    property string specieCommonNames: ""
    property string specimenLocation: ""
    property string specimenDistance: ""

    Column {
        anchors.fill: parent
        spacing: 20

        Label {
            text: specieScientificName
            width: parent.width
            wrapMode: Label.Wrap
            onLinkActivated: function(link) { Qt.openUrlExternally(link) }
        }

        Label {
            text: specieCommonNames
            width: parent.width
            wrapMode: Label.Wrap
        }

        Label {
            text: specimenLocation
            width: parent.width
            wrapMode: Label.Wrap
            onLinkActivated: function(link) { Qt.openUrlExternally(link) }
        }

        Label {
            text: specimenDistance
            width: parent.width
            wrapMode: Label.Wrap
        }
    }
}
