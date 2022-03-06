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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Controls.Universal 2.15
import QtPositioning 5.15
import Qt.labs.settings 1.1

ApplicationWindow {
    id: window
    visible: true
    width: 1000
    height: 800
    title: qsTr("Barcelona Trees")

    Settings {
        id: settings
        property string language: "en"
        property string links: "Wikipedia"
        property string style: "Material"
        property alias window_x: window.x
        property alias window_y: window.y
        property alias window_width: window.width
        property alias window_height: window.height
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: resultsPage.visible
        onActivated: {
            resultsPage.close()
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenu.open()
    }

    header: ToolBar {
        id: windowHeader
        Material.foreground: "white"
        contentHeight: toolButton.implicitHeight
        property string title: qsTr("Barcelona Trees")

        RowLayout {
            spacing: 20
            anchors.fill: parent

            Image {
                horizontalAlignment: Image.AlignLeft
                fillMode: Image.PreserveAspectFit
                source: "qrc:/barnatrees_icon32.png"
            }

            Label {
                text: windowHeader.title
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: toolButton
                icon.name: resultsPage.visible ? "back" : "drawer"
                onClicked: {
                    if (resultsPage.visible) {
                        resultsPage.close()
                    } else {
                        optionsMenu.open()
                    }
                }
            }
        }
    }

    Menu {
        id: optionsMenu
        width: {
            var result = 0;
            var padding = 0;
            for (var i = 0; i < count; ++i) {
                var item = itemAt(i);
                result = Math.max(item.contentItem.implicitWidth, result);
                padding = Math.max(item.padding, padding);
            }
            return result + padding * 2;
        }
        x: parent.width - width
        transformOrigin: Menu.TopRight

        MenuItem {
            id: positionEnabled
            text: qsTr("Update Position")
            checkable: true
            checked: false
            onToggled: homePage.togglePositioning(checked)
        }

        MenuItem {
            text: qsTr("Search by Street")
            onTriggered: streetSearchDialog.open()
        }

        MenuItem {
            text: qsTr("Search by Gender/Name")
            onTriggered: genderSearchDialog.open()
        }

        MenuItem {
            text: qsTr("Search Species")
            onTriggered: specieSearchDialog.open()
        }

        MenuItem {
            text: qsTr("View Results List")
            onTriggered: resultsPage.open()
        }

        MenuItem {
            text: qsTr("Settings")
            onTriggered: settingsDialog.open()
        }

        MenuItem {
            text: qsTr("About")
            onTriggered: aboutDialog.open()
        }
    }

    HomePage {
        id: homePage
        anchors.fill: parent
    }

    ResultsPage {
        id: resultsPage
        onClosed: {
            windowHeader.title = qsTr("Barcelona Trees")
        }
        onRowSelected: {
            homePage.changeMapCenter(QtPositioning.coordinate(lat, lon))
            close()
            homePage.delayedBalloonTip(idx)
        }
    }

    AboutDialog {
        id: aboutDialog
    }

    GenderSearchDialog {
        id: genderSearchDialog
        onDialogAccepted: homePage.clearItems()
        onResultsFound: homePage.resultsFound()
        onResultsNotFound: resultsNotFoundDialog.open()
    }

    SettingsDialog {
        id: settingsDialog
    }

    SpecieSearchDialog {
        id: specieSearchDialog
        onDialogAccepted: homePage.clearItems()
        onResultsFound: homePage.resultsFound()
        onResultsNotFound: resultsNotFoundDialog.open()
    }

    SpecimenDialog {
        id: specimenDialog
    }

    StreetSearchDialog {
        id: streetSearchDialog
        onDialogAccepted: homePage.clearItems()
        onResultsFound: homePage.resultsFound()
        onResultsNotFound: resultsNotFoundDialog.open()
    }

    MsgDialog {
        id: resultsNotFoundDialog
        title: qsTr("Sorry!")
        dlgMessage: qsTr("Your search returned no results.")
    }

    Component.onCompleted: {
        if (Qt.platform.os !== "android") {
            optionsMenu.removeItem(positionEnabled)
        }
    }
}
