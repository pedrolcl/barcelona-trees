import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("Gender Name Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal dialogAccepted()
    signal resultsFound()
    signal resultsNotFound()

    onOpened: genderText.clear()
    onAccepted: {
        dialogAccepted()
        plantModel.setGender(genderText.text)
        if (plantModel.rowCount() > 0) {
            resultsFound();
        } else {
            resultsNotFound()
        }
    }

    Column {
        anchors.fill: parent
        spacing: 20
        Label {
            text: qsTr("Type a gender (or partial) name:")
            wrapMode: Label.Wrap
            width: parent.width
        }
        TextField {
            id: genderText
            focus: true
            width: parent.width
        }
    }
}
