/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("Species Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal dialogAccepted()
    signal resultsFound()
    signal resultsNotFound()

    onAccepted: {
        dialogAccepted()
        var id = speciesModel.data(speciesCombo.currentIndex, "idSpecies");
        plantModel.setSpecies(id)
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
            wrapMode: Label.Wrap
            text: qsTr("Select a Scientific Name:")
            width: parent.width
        }

        ComboBox {
            id: speciesCombo
            width: parent.width
            font.italic: true
            popup.font.italic: true
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
            width: parent.width
            text: ""
        }
    }
}
