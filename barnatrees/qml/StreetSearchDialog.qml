import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("Street Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal resultsFound()

    onOpened: streetText.clear()
    onAccepted: {
        plantModel.setStreet(streetText.text)
        if (plantModel.rowCount() > 0) {
            resultsFound();
        }
        //map.center = plantModel.nearestPlant();
        //map.zoomLevel = 18
    }

    ColumnLayout {
        spacing: 20
        anchors.fill: parent
        Label {
            elide: Label.ElideRight
            text: qsTr("Type the street (or partial) name:")
            Layout.fillWidth: true
        }
        TextField {
            id: streetText
            focus: true
            Layout.fillWidth: true
        }
    }
}
