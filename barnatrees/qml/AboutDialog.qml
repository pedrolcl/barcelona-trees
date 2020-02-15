/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("About")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 5 * 4)
    height: Math.round(window.height / 10 * 8)
    standardButtons: Dialog.Close

    Flickable {
        anchors.fill: parent
        contentHeight: col.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Column {
            id: col
            spacing: 10

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("<h1>Barcelona Trees</h1>A guide of the trees of Barcelona. "
                         + "v%1 git:%2. Copyright © 2019 Pedro López-Cabanillas.<br/>"
                         + "This program comes with ABSOLUTELY NO WARRANTY. "
                         + "This is free software, and you are welcome to redistribute it "
                         + "under the terms of the license "
                         + "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>GPLv3</a>")
                         .arg(Qt.application.version).arg(gitversion)
                wrapMode: Label.Wrap
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Image {
                width: aboutDialog.availableWidth
                horizontalAlignment: Image.AlignHCenter
                fillMode: Image.PreserveAspectFit
                source: "qrc:/barnatrees_icon64.png"
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("This application contains data provided by the Barcelona's city council: "
                    + "<a href='https://opendata-ajuntament.barcelona.cat/en/'>Open Data BCN</a> "
                    + "using the datasets <b>Zone trees of the city of Barcelona</b> and "
                    + "<b>Street trees of the city of Barcelona</b>, "
                    + "under the therms of the <a href='https://creativecommons.org/licenses/by/4.0/'>"
                    + "Creative Commons Attribution 4.0 International (CC BY 4.0)</a> license.")
                wrapMode: Label.Wrap
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Repeater {
                model: summaryModel
                width: aboutDialog.availableWidth
                delegate: Column {
                    width: parent.width
                    spacing: 4
                    Label {
                        width: parent.width
                        font.bold: true
                        text: name
                    }
                    Label {
                        width: parent.width
                        text: qsTr("Processed: %1").arg(description)
                    }
                    Label {
                        width: parent.width
                        text: qsTr("Trees: %L1").arg(specimens)
                    }
                    Label {
                        width: parent.width
                        text: qsTr("Species: %L1").arg(species)
                    }
                }
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {}
    }
}
