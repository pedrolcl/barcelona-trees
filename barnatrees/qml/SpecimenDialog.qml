import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Label {
            text: specieScientificName
            Layout.fillWidth: true
            wrapMode: Label.Wrap
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            text: specieCommonNames
            Layout.fillWidth: true
            wrapMode: Label.Wrap
        }

        Label {
            text: specimenLocation
            Layout.fillWidth: true
            wrapMode: Label.Wrap
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            text: specimenDistance
            Layout.fillWidth: true
            wrapMode: Label.Wrap
        }
    }
}
