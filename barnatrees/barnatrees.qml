import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12
import QtPositioning 5.12
import QtLocation 5.12
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("Barcelona Trees")

    Settings {
        id: settings
        property string style: "Material"
        property string language: "en"
        //property string plugin: "osm"
    }

    header: ToolBar {
        Material.foreground: "white"

        RowLayout {
            spacing: 20
            anchors.fill: parent

            Image {
                horizontalAlignment: Image.AlignLeft
                fillMode: Image.PreserveAspectFit
                source: "barnatrees_icon32.png"
            }

            Label {
                id: titleLabel
                text: window.title
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                icon.source: "menu.png"
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        id: positionEnabled
                        text: qsTr("Update Position")
                        checkable: true
                        checked: false
                        onToggled: {
                            locationCircle.visible = checked
                            if (checked) {
                                positionSource.update()
                            }
                        }
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
                        text: qsTr("Settings")
                        onTriggered: settingsDialog.open()
                    }

                    MenuItem {
                        text: qsTr("About")
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    property variant locationBarna: QtPositioning.coordinate( 41.403216, 2.186674 )
    property variant lastSearchPosition: locationBarna
    //property variant mapsProvider: settings.plugin

    PositionSource {
        id: positionSource
        active: true
        updateInterval: 120000 // 2 mins
        onPositionChanged:  {
            var currentPosition = positionSource.position.coordinate
            var distance1 = currentPosition.distanceTo(lastSearchPosition)
            var distance2 = currentPosition.distanceTo(locationBarna)
            //console.log("Position: " + currentPosition + " distance: " + distance1 + " enabled: " + positionEnabled.checked)
            if (positionEnabled.checked && distance1 > 500 && distance2 < 8000) { // 500m from last and 8km from centre
                lastSearchPosition = currentPosition
                map.center = currentPosition
                locationCircle.center = currentPosition
                locationCircle.visible = true
                plantModel.setCenter(currentPosition)
            }
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin {
            name: "osm" // esri, osm, mapboxgl
            //PluginParameter {...}
        }
        center: locationBarna
        zoomLevel: 17 //maximumZoomLevel
        //onZoomLevelChanged: console.log("ZoomLevel:" + zoomLevel);
        onCopyrightLinkActivated: Qt.openUrlExternally(link)

        MapCircle {
            id: locationCircle
            visible: true;
            center: locationBarna
            radius: 5.0
            color: 'deepskyblue'
            border.width: 2.0
            border.color: 'paleturquoise'
        }

        MapItemView {
            model: plantModel
            delegate: MapQuickItem {
                id: item
                zoomLevel: 17
                anchorPoint.x: mark.width * 0.5
                anchorPoint.y: mark.height * 0.5
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
                sourceItem: Rectangle {
                    id: mark
                    radius: 2
                    width: 5
                    height: 5
                    color: "green"
                    MouseArea {
                        function wikipediaLink()
                        {
                            var s = "";
                            if (model.scientificName.indexOf(" x ") > -1) {
                                s = model.scientificName.replace(" x ", "_x_");
                            } else {
                                s = model.scientificName.replace(" ", "_");
                            }
                            var idx = s.indexOf(" ");
                            if ( idx > -1) {
                                s = s.substring(0, idx);
                            }
                            return "<a href='https://es.wikipedia.org/wiki/%1'>%2</a>".arg(s).arg(model.scientificName);
                        }
                        function streetViewLink()
                        {
                            var pos = "viewpoint=%1,%2".arg(model.latitude).arg(model.longitude);
                            return "<a href='https://www.google.com/maps/@?api=1&map_action=pano&%1'>%2</a>".arg(pos).arg(model.plantAddress);
                        }
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            specieScientificName.text = wikipediaLink()
                            specieCommonNames.text = model.commonName1 + '\n' + model.commonName2
                            specimenLocation.text = streetViewLink()
                            specimenDialog.open()
                        }
                        onEntered: {
                            tip.visible = true
                        }
                        onExited: {
                            tip.visible = false
                        }
                    }
                    ToolTip {
                        id: tip
                        parent: mark
                        text: model.scientificName
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onPressAndHold: {
                var coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                map.center = coordinate
                plantModel.setCenter(coordinate)
            }
        }
    }

    Dialog {
        id: streetSearchDialog
        parent: Overlay.overlay
        modal: true
        focus: true
        title: qsTr("Street Search")
        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        standardButtons: Dialog.Ok | Dialog.Cancel

        onOpened: streetText.clear()
        onAccepted: {
            plantModel.setStreet(streetText.text)
            map.center = plantModel.nearestPlant();
            map.zoomLevel = 18
        }

        ColumnLayout {
            spacing: 20
            anchors.fill: parent
            Label {
                elide: Label.ElideRight
                text: qsTr("Type the street (or partial) name:")
                Layout.fillWidth: true
            }
            TextField {
                id: streetText
                focus: true
                Layout.fillWidth: true
            }
        }
    }

    Dialog {
        id: genderSearchDialog
        parent: Overlay.overlay
        modal: true
        focus: true
        title: qsTr("Gender Name Search")
        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        standardButtons: Dialog.Ok | Dialog.Cancel

        onOpened: genderText.clear()
        onAccepted: {
            plantModel.setGender(genderText.text)
            map.center = plantModel.nearestPlant();
            map.zoomLevel = 18
        }

        ColumnLayout {
            spacing: 20
            anchors.fill: parent
            Label {
                elide: Label.ElideRight
                text: qsTr("Type a gender (or partial) name:")
                Layout.fillWidth: true
            }
            TextField {
                id: genderText
                focus: true
                Layout.fillWidth: true
            }
        }
    }

    Dialog {
        id: specieSearchDialog
        parent: Overlay.overlay
        modal: true
        focus: true
        title: qsTr("Species Search")
        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        standardButtons: Dialog.Ok | Dialog.Cancel

        onOpened: specieSummary.text = ""
        onAccepted: {
            var id = speciesModel.data(speciesCombo.currentIndex, "idSpecies");
            plantModel.setSpecies(id)
            map.center = plantModel.nearestPlant();
            map.zoomLevel = 18
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
                model: speciesModel
                textRole: "scientificName"
                onCurrentIndexChanged: {
                    //console.log("current index:" + currentIndex);
                    var num = speciesModel.data(currentIndex, "numSpecimens");
                    specieSummary.text = qsTr("Number of specimens: %1").arg(num);
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

    Dialog {
        id: specimenDialog
        parent: Overlay.overlay
        modal: true
        focus: true
        title: qsTr("<h1>Specimen</h1>")
        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        standardButtons: Dialog.Close

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            Label {
                id: specieScientificName
                Layout.fillWidth: true
                font.pointSize: 18
                wrapMode: Label.Wrap
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label {
                id: specieCommonNames
                Layout.fillWidth: true
                font.pointSize: 16
                wrapMode: Label.Wrap
            }

            Label {
                id: specimenLocation
                Layout.fillWidth: true
                font.pointSize: 16
                wrapMode: Label.Wrap
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        title: qsTr("Settings")

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settings.language = langBox.model.get(langBox.currentIndex).value
            //settings.plugin = mapBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            langBox.currentIndex = langBox.langIndex
            //mapBox.currentIndex = mapBox.mapIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: qsTr("Style:")
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: availableStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                spacing: 10
                Label {
                    text: qsTr("Language:")
                }
                ComboBox {
                    id: langBox
                    textRole: "key"
                    property int langIndex: -1
                    model:  ListModel {
                        ListElement { key: qsTr("English"); value: "en" }
                        ListElement { key: qsTr("Catalan"); value: "ca" }
                        ListElement { key: qsTr("Spanish"); value: "es" }
                    }
                    Component.onCompleted: {
                        for(var i=0; i<model.count; ++i) {
                            if (model.get(i).value === settings.language) {
                                langIndex = i
                                break;
                            }
                        }
                        if (langIndex !== -1)
                            currentIndex = langIndex
                    }
                    Layout.fillWidth: true
                }
            }

//            RowLayout {
//                spacing: 10
//                Label {
//                    text: "Maps:"
//                }
//                ComboBox {
//                    id: mapBox
//                    property int mapIndex: -1
//                    model: ListModel {
//                        ListElement { text: "esri" }
//                        ListElement { text: "mapboxgl" }
//                        ListElement { text: "osm" }
//                    }
//                    Component.onCompleted: {
//                        mapIndex = find(settings.plugin, Qt.MatchFixedString)
//                        if (mapIndex !== -1)
//                            currentIndex = mapIndex
//                    }
//                    Layout.fillWidth: true
//                }
//            }

            Label {
                text: qsTr("Restart required")
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex || langBox.currentIndex !== langBox.langIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Dialog {
        id: aboutDialog
        parent: Overlay.overlay
        modal: true
        focus: true
        title: qsTr("About")
        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(Math.min(window.width, window.height) / 5 * 4)
        contentHeight: aboutColumn.height
        standardButtons: Dialog.Close

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
                source: "barnatrees_icon64.png"
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
