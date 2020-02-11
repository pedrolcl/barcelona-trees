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
    signal resultsNotFound()

    onOpened: {
        streetCombo.currentIndex = -1
        streetCombo.contentItem.clear()
    }

    onAccepted: {
        dialogAccepted()
        plantModel.setStreet(streetCombo.editText)
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
            text: qsTr("Type the street (or partial) name:")
            wrapMode: Label.Wrap
            width: parent.width
        }

        ComboBox {
            id: streetCombo
            width: parent.width
            editable: true
            focus: true
            model: streetModel
            textRole: "display"
            onEditTextChanged: {
                if (editText.length === 1) {
                    var filterText = editText
                    streetModel.setFilterString(filterText)
                    currentIndex = -1
                    contentItem.text = filterText
                } else {
                    contentItem.text = editText
                }
            }
        }
    }
}
