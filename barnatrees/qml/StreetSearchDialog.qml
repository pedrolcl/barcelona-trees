import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("Street Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal dialogAccepted()
    signal resultsFound()

    onOpened: streetText.clear()
    onAccepted: {
        dialogAccepted()
        plantModel.setStreet(streetText.text)
        if (plantModel.rowCount() > 0) {
            resultsFound();
        }
    }

    Column {
        anchors.fill: parent
        spacing: 20

        Label {
            text: qsTr("Type the street (or partial) name:")
            wrapMode: Label.Wrap
            width: parent.width
        }

        TextField {
            id: streetText
            focus: true
            width: parent.width
        }
    }
}
