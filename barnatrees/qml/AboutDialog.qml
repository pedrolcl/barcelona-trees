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
                         + "version: %1 git: %2.<br/>Copyright © 2019-2020 Pedro López-Cabanillas.<br/>"
                         + "This program comes with ABSOLUTELY NO WARRANTY. "
                         + "This is free software, and you are welcome to redistribute it "
                         + "under the terms of the license "
                         + "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>GPLv3</a>")
                         .arg(Qt.application.version).arg(gitversion)
                wrapMode: Label.Wrap
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("This program uses Qt version %1 (<a href='https://www.qt.io/'>www.qt.io</a>)<br/>"
                         + "This application includes software developed by the OpenSSL Project for use in "
                         + "the <a href='http://www.openssl.org/'>OpenSSL Toolkit</a>. This program is released under "
                         + "the GPL with the additional exemption that compiling, linking, and/or using OpenSSL is allowed.<br/>"
                         + "This application contains source code from the <a href='https://www.7-zip.org'>7-zip</a> "
                         + "and <a href='http://p7zip.sourceforge.net'>p7zip</a> projects and some classes from the "
                         + "<a href='https://doc.qt.io/qtinstallerframework/'>Qt Installer Framework</a>.")
                        .arg(qtversion);
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
                    + "Creative Commons Attribution 4.0 International (CC BY 4.0)</a> license.");
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
