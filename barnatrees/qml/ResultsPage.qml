import QtQuick 2.12
import QtQuick.Controls 2.12
import QtPositioning 5.12

Page {
    property int numberOfRows: 0
    signal rowSelected(real lat, real lon)

    title: qsTr("Found %Ln tree(s)", "", numberOfRows)

    Component {
        id: plantDelegate
        Rectangle {
            width: parent.width
            height: row.height
            color: "ivory"
            Row {
                id: row
                spacing: 5
                Text {
                    color: "green"
                    padding: 5
                    topPadding: (parent.height - contentHeight) / 2
                    font.bold: true
                    font.pixelSize: 14
                    text: index + 1
                }
                Column {
                    Text {
                        text: scientificName
                        font.bold: true
                        font.italic: true
                    }
                    Text {
                        text: plantAddress
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: rowSelected(latitude, longitude)
            }
        }
    }

    ListView {
        id: plantsListView
        anchors.fill: parent
        spacing: 2
        model: plantModel
        delegate: plantDelegate
        ScrollIndicator.horizontal: ScrollIndicator {}
        ScrollIndicator.vertical: ScrollIndicator {}
    }
}
