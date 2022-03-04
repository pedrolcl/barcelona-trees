/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2022 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
    title: qsTr("Gender Name Search")
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    standardButtons: Dialog.Ok | Dialog.Cancel

    signal dialogAccepted()
    signal resultsFound()
    signal resultsNotFound()

    onOpened: {
        genderCombo.currentIndex = -1
        genderCombo.contentItem.clear()
        genderCombo.forceActiveFocus()
    }

    onAccepted: {
        dialogAccepted()
        plantModel.setGender(genderCombo.editText)
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
        ComboBox {
            id: genderCombo
            width: parent.width
            focus: true
            editable: true
            model: genderModel
            textRole: "gender"
        }
    }
}
