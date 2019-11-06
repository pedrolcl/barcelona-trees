import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12
import QtPositioning 5.12
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
        enabled: stackView.depth > 1
        onActivated: {
            stackView.pop()
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

        RowLayout {
            spacing: 20
            anchors.fill: parent

            Image {
                horizontalAlignment: Image.AlignLeft
                fillMode: Image.PreserveAspectFit
                source: "qrc:/barnatrees_icon32.png"
            }

            Label {
                text: stackView.currentItem.title
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: toolButton
                icon.name: stackView.depth > 1 ? "back" : "drawer"
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
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
            onTriggered: {
                resultsPage.numberOfRows = plantModel.rowCount()
                stackView.push(resultsPage)
            }
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

    ResultsPage {
        id: resultsPage
        onRowSelected: {
            homePage.changeMapCenter(QtPositioning.coordinate(lat, lon))
            stackView.pop();
            homePage.showBalloonTip(idx)
        }
    }

    StackView {
        id: stackView
        initialItem: HomePage {
            id: homePage
        }
        anchors.fill: parent
    }

    AboutDialog {
        id: aboutDialog
    }

    GenderSearchDialog {
        id: genderSearchDialog
        onResultsFound: {
            homePage.clearItems()
            homePage.changeMapCenter(plantModel.nearestPlantCoordinate())
            resTimer.running = true
        }
    }

    SettingsDialog {
        id: settingsDialog
    }

    SpecieSearchDialog {
        id: specieSearchDialog
        onResultsFound: {
            homePage.clearItems()
            homePage.changeMapCenter(plantModel.nearestPlantCoordinate())
            resTimer.running = true
        }
    }

    SpecimenDialog {
        id: specimenDialog
    }

    StreetSearchDialog {
        id: streetSearchDialog
        onResultsFound: {
            homePage.clearItems()
            homePage.changeMapCenter(plantModel.nearestPlantCoordinate())
            resTimer.running = true
        }
    }

    Timer {
      id: resTimer
      running: false
      repeat: false
      interval: 1000
      onTriggered: homePage.showBalloonTip(plantModel.nearestRow())
    }

    Timer {
      running: true
      repeat: false
      interval: 500
      onTriggered: homePage.showCurrentLocation()
    }
}
