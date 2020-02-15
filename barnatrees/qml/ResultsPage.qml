import QtQuick 2.12
import QtQuick.Controls 2.12
import QtPositioning 5.12

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
            width: parent.width
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
