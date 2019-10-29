import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("<h1>Specimen</h1>")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Close

    property string specieScientificName: null
    property string specieCommonNames: null
    property string specimenLocation: null
    property string specimenDistance: null

    Column {
        anchors.fill: parent
        spacing: 20

        Label {
            text: specieScientificName
            width: parent.width
            wrapMode: Label.Wrap
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            text: specieCommonNames
            width: parent.width
            wrapMode: Label.Wrap
        }

        Label {
            text: specimenLocation
            width: parent.width
            wrapMode: Label.Wrap
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            text: specimenDistance
            width: parent.width
            wrapMode: Label.Wrap
        }
    }
}
