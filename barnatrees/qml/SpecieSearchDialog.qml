import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("Species Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal resultsFound()

    onAccepted: {
        var id = speciesModel.data(speciesCombo.currentIndex, "idSpecies");
        plantModel.setSpecies(id)
        if (plantModel.rowCount() > 0) {
            resultsFound();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        Label {
            Layout.fillWidth: true
            font.pixelSize: 12
            wrapMode: Label.Wrap
            text: qsTr("Select a Scientific Name:")
        }
        ComboBox {
            id: speciesCombo
            Layout.fillWidth: true
            font.pixelSize: 12
            font.italic: true
            model: speciesModel
            textRole: "scientificName"
            onCurrentIndexChanged: {
                //console.log("current index:" + currentIndex);
                var num = speciesModel.data(currentIndex, "numSpecimens");
                specieSummary.text = qsTr("Number of specimens: %L1").arg(num);
            }
        }
        Label {
            id: specieSummary
            Layout.fillWidth: true
            font.pixelSize: 12
            text: ""
        }
    }
}
