import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("About")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 5 * 4)
    contentHeight: aboutColumn.height
    standardButtons: Dialog.Close

    ScrollView {
            anchors.fill: parent

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("<h1>Barcelona Trees</h1>A guide of the trees of Barcelona. "
                         + "Copyright © 2019 Pedro López-Cabanillas.<br/>"
                         + "This program comes with ABSOLUTELY NO WARRANTY. "
                         + "This is free software, and you are welcome to redistribute it "
                         + "under the terms of the license "
                         + "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>GPLv3</a>" )
                wrapMode: Label.Wrap
                font.pixelSize: 12
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Image {
                id: logo
                width: aboutDialog.availableWidth
                horizontalAlignment: Image.AlignHCenter
                fillMode: Image.PreserveAspectFit
                source: "qrc:/barnatrees_icon64.png"
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("This application contains data provided by the Barcelona's city council: "
                    + "<a href='https://opendata-ajuntament.barcelona.cat/en/'>Open Data BCN</a> "
                    + "using the datasets <b>Zone trees of the city of Barcelona</b> and "
                    + "<b>Street trees of the city of Barcelona</b>, "
                    + "under the therms of the <a href='https://creativecommons.org/licenses/by/4.0/'>"
                    + "Creative Commons Attribution 4.0 International (CC BY 4.0)</a> license.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
}
