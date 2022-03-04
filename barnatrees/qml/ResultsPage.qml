/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2022 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15

Drawer {
    y: windowHeader.height
    width: window.width > 300 ? 300 : window.width
    height: window.height - windowHeader.height
    edge: Qt.RightEdge

    property alias currentIndex: plantsListView.currentIndex

    signal rowSelected(real lat, real lon, int idx)

    onOpened: {
        windowHeader.title = qsTr("Found %Ln tree(s)", "", plantProxy.rowCount())
    }

    Component {
        id: plantDelegate
        Rectangle {
            width: window.width > 300 ? 300 : window.width
            height: row.height
            color: "#505050"
            Row {
                id: row
                spacing: 5
                Text {
                    color: "lime"
                    padding: 5
                    topPadding: (parent.height - contentHeight) / 2
                    font.bold: true
                    text: fmtDistance
                }
                Column {
                    Text {
                        text: fmtName
                        color: "white"
                        font.bold: true
                    }
                    Text {
                        text: plantAddress
                        color: "white"
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    plantsListView.currentIndex = index
                    rowSelected(latitude, longitude, plantProxy.sourceRow(index))
                }
            }
        }
    }

    ListView {
        id: plantsListView
        anchors.fill: parent
        spacing: 2
        model: plantProxy
        delegate: plantDelegate
        focus: true
        clip: true
        ScrollIndicator.horizontal: ScrollIndicator {z: Infinity}
        ScrollIndicator.vertical: ScrollIndicator {z: Infinity}
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 50
        highlight: Rectangle {
            color: 'lightsteelblue'
            opacity: 0.5
            z: 2
        }
        //onCurrentItemChanged: console.log('item', plantsListView.currentIndex, 'selected')
    }
}
