/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
import QtQuick
import QtQuick.Controls

Dialog {
    parent: Overlay.overlay
    modal: true
    focus: true
    title: qsTr("Street Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Qt.platform.os === "android" ? Screen.width : Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal dialogAccepted()
    signal resultsFound()
    signal resultsNotFound()

    onOpened: {
        streetCombo.currentIndex = -1
        streetCombo.contentItem.clear()
        streetCombo.forceActiveFocus()
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
