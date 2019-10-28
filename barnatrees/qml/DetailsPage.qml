import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    title: qsTr("Page 2")

    property string detailsText: ""

    Label {
        text: detailsText
        anchors.centerIn: parent
    }

}
