import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: 250
    standardButtons: Dialog.Close

    property string dlgMessage: null

    Label {
        text: dlgMessage
        width: parent.width
        wrapMode: Label.Wrap
    }
}
