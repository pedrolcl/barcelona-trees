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

import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtPositioning
import QtLocation
import Qt.labs.animation

ApplicationWindow {
    id: window
    flags:  Qt.FramelessWindowHint | Qt.Window
    visible: true
    width: 1000
    height: 800
    title: qsTr("Barcelona Trees")

    property var locationBarna: QtPositioning.coordinate( 41.403216, 2.186674 )
    property var barnaRegion: QtPositioning.rectangle(QtPositioning.coordinate(41.45,2.09), QtPositioning.coordinate(41.33, 2.23))
    property double defaultZoom: map.maximumZoomLevel - 2.0
    property var globalItems: []
    property int numberOfRows: 0

    function toggleMaximized() {
        if (window.visibility === Window.Maximized) {
            window.showNormal();
        } else {
            window.showMaximized();
        }
    }

    DragHandler {
        enabled: Qt.platform.os !== "android"
        id: resizeHandler
        grabPermissions: TapHandler.TakeOverForbidden
        target: null
        onActiveChanged: if (active) {
            const p = resizeHandler.centroid.position;
            let e = 0;
            if (p.x / width < 0.10) { e |= Qt.LeftEdge }
            if (p.x / width > 0.90) { e |= Qt.RightEdge }
            if (p.y / height < 0.10) { e |= Qt.TopEdge }
            if (p.y / height > 0.90) { e |= Qt.BottomEdge }
            window.startSystemResize(e);
        }
    }

    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        width: 5
        color: "transparent"
        HoverHandler {
            cursorShape: Qt.SizeHorCursor
        }
    }
    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        width: 5
        color: "transparent"
        HoverHandler {
            cursorShape: Qt.SizeHorCursor
        }
    }
    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        height: 5
        color: "transparent"
        HoverHandler {
            cursorShape: Qt.SizeVerCursor
        }
    }
    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        height: 5
        color: "transparent"
        HoverHandler {
            cursorShape: Qt.SizeVerCursor
        }
    }

    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.top: parent.top
        anchors.left: parent.left
        width: 6
        height: 6
        radius: 3
        color: "white"
        HoverHandler {
            cursorShape: Qt.SizeFDiagCursor
        }
    }
    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.top: parent.top
        anchors.right: parent.right
        width: 6
        height: 6
        radius: 3
        color: "white"
        HoverHandler {
            cursorShape: Qt.SizeBDiagCursor
        }
    }
    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 6
        height: 6
        radius: 3
        color: "white"
        HoverHandler {
            cursorShape: Qt.SizeBDiagCursor
        }
    }
    Rectangle{
        visible: window.visibility !== Window.Maximized && Qt.platform.os !== "android"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 6
        height: 6
        radius: 3
        color: "white"
        HoverHandler {
            cursorShape: Qt.SizeFDiagCursor
        }
    }

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

        MenuItem {
            id: closeItem
            text: qsTr("Quit")
            onTriggered: window.close()
        }
    }

    Page {
        id: homePage
        title: qsTr("Barcelona Trees")
        anchors.fill: parent
        anchors.margins: window.visibility === Window.Windowed ? 5 : 0

        function clearItems() {
            //console.log("cleaning", map.mapItems.length, "mapItems and",globalItems.length,"globalItems")
            resultsPage.currentIndex = -1
            globalItems = []
            map.clearMapItems()
            gc()
            map.addMapItem(locationCircle)
        }

        function togglePositioning(checked) {
            if (checked) {
                positionSource.update()
            }
        }

        function changeMapCenter(newcenter) {
            map.center = newcenter
            map.zoomLevel = map.maximumZoomLevel
        }

        function changeGlobalCenter(coordinate) {
            if (barnaRegion.contains(coordinate)) {
                clearItems()
                map.zoomLevel = defaultZoom
                map.center = coordinate
                locationCircle.coordinate = coordinate
                plantModel.setCenter(coordinate)
                locationTip.open()
            }
        }

        function showCurrentLocation() {
            if (map.mapReady) {
                locationTip.open()
            }
        }

        function showBalloonTip(ix) {
            //console.log("showBalloonTip:", ix, "globalItems.length:", globalItems.length);
            if (ix < globalItems.length ) {
                var item = globalItems[ix]
                if (item && item.modelIndex >= 0) {
                    item.showTip()
                } //else
                    //console.log("Warning!",ix,item)
            } //else
                //console.log("Warning!",ix)
            mapTimer.pendingTreeTip = false
            mapTimer.pendingTreeRow = -1
        }

        function delayedBalloonTip(ix) {
            mapTimer.pendingTreeRow = ix
            mapTimer.pendingTreeTip = true
            mapTimer.start()
        }

        function resultsFound() {
            changeMapCenter(plantProxy.nearestPlantCoordinate())
            numberOfRows = plantModel.rowCount()
            mapTimer.pendingTreeTip = true
            mapTimer.pendingSuccessMsg = true
            mapTimer.start()
        }

        header: ToolBar {
            id: windowHeader
            contentHeight: toolButton.implicitHeight
            property string title: qsTr("Barcelona Trees")

            HoverHandler {
                cursorShape: Qt.SizeAllCursor
            }

            RowLayout {
                spacing: 20
                anchors.fill: parent

                TapHandler {
                    enabled: Qt.platform.os !== "android"
                    onDoubleTapped: window.toggleMaximized()
                }

                DragHandler {
                    enabled: Qt.platform.os !== "android"
                    grabPermissions: TapHandler.CanTakeOverFromAnything
                    onActiveChanged: if (active) { window.startSystemMove() }
                }

                ToolButton {
                    visible: Qt.platform.os !== "android"
                    icon.name: "close"
                    onClicked: window.close()
                }

                ToolButton {
                    visible: Qt.platform.os !== "android"
                    icon.name: window.visibility == Window.Maximized ? "resize" : "maximize"
                    onClicked: window.toggleMaximized()
                }

                ToolButton {
                    visible: Qt.platform.os !== "android"
                    icon.name: "minimize"
                    onClicked: window.showMinimized();
                }

                Image {
                    horizontalAlignment: Image.AlignLeft
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/barnatrees_icon32.png"
                }

                Label {
                    text: windowHeader.title
                    font.pixelSize: Qt.application.font.pixelSize * 1.6
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignLeft
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

        PositionSource {
            id: positionSource
            active: true
            updateInterval: 60000 // 1 min
            property variant lastSearchPosition: locationBarna
            onPositionChanged:  {
                var currentPosition = positionSource.position.coordinate
                var distance1 = currentPosition.distanceTo(lastSearchPosition)
                var distance2 = currentPosition.distanceTo(locationBarna)
                //console.log("Position: " + currentPosition + " distance: " + distance1 + " enabled: " + positionEnabled.checked)
                if (positionEnabled && positionEnabled.checked && distance1 > 250 && distance2 < 8000) { // 250 m from last location and 8 km from Glories
                    lastSearchPosition = currentPosition
                    homePage.changeGlobalCenter(currentPosition)
                }
            }
        }

        Map {
            id: map
            anchors.fill: parent
            maximumZoomLevel: zlBr.maximum
            minimumZoomLevel: zlBr.minimum
            plugin: Plugin {
                name: "osm"
                PluginParameter {
                    name: "osm.mapping.providersrepository.disabled"
                    value: "true"
                }
                PluginParameter {
                    name: "osm.mapping.providersrepository.address"
                    value: "http://maps-redirect.qt.io/osm/5.8/"
                }
            }
            center: locationBarna
            zoomLevel: defaultZoom

            onZoomLevelChanged: zlBr.returnToBounds()
            onCopyrightLinkActivated: (link)=> Qt.openUrlExternally(link)
            onMapItemsChanged: {
                if (globalItems.length === 0 && mapItems.length === (1 + plantModel.rowCount())) {
                    //console.log("mapItems.length:", mapItems.length)
                    for(var j=1; j<mapItems.length; ++j) {
                        if(mapItems[j].modelIndex >= 0) {
                            var ix = mapItems[j].modelIndex
                            globalItems[ix] = mapItems[j]
                        }
                    }
                    //console.log("globalItems.length", globalItems.length)
                    numberOfRows = plantModel.rowCount()
                    mapTimer.start()
                }
            }

            BoundaryRule on zoomLevel {
                id: zlBr
                minimum: 13.0
                maximum: 20.0
            }

            MapQuickItem {
                id: locationCircle
                coordinate: locationBarna
                opacity: 1.0
                anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
                function showTip() {
                    locationTip.open()
                }
                sourceItem: Rectangle {
                    width: 14
                    height: 14
                    color: 'deepskyblue'
                    border.width: 3.0
                    border.color: 'paleturquoise'
                    smooth: true;
                    radius: 7
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: locationTip.visible = true
                        onExited: locationTip.visible = false
                    }
                    BalloonTip {
                        id: locationTip
                        text: qsTr("Current Location")
                        timeout: 15000
                    }
                }
            }

            MapItemView {
                id: itemView
                model: plantModel

                delegate: MapQuickItem {
                    id: item
                    anchorPoint.x: mark.width / 2
                    anchorPoint.y: mark.height / 2
                    coordinate: QtPositioning.coordinate(model.latitude, model.longitude)

                    property int modelIndex: model.index

                    function showTip() {
                        tip.open()
                    }

                    sourceItem: Rectangle {
                        id: mark
                        radius: 4
                        smooth: true
                        width: 8
                        height: 8
                        color: "green"
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                specimenDialog.specieScientificName = plantModel.wikiLink(index)
                                specimenDialog.specieCommonNames = model.commonName1 + '\n' + model.commonName2
                                specimenDialog.specimenLocation = plantModel.streetLink(index)
                                specimenDialog.specimenDistance = plantModel.formattedDistance(index)
                                specimenDialog.open()
                            }
                            onEntered: tip.visible = true
                            onExited: tip.visible = false
                        }
                        BalloonTip {
                            id: tip
                            parent: mark
                            text: plantModel.formattedScientificName(index)
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onPressAndHold: (mouse)=> {
                    homePage.changeGlobalCenter(map.toCoordinate(Qt.point(mouse.x, mouse.y)));
                }
            }

            Column {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
                Button {
                    width: 50
                    height: 50
                    icon.name: "zoomin"
                    onPressed: map.zoomLevel += 1;
                }
                Button {
                    width: 50
                    height: 50
                    icon.name: "zoomout"
                    onPressed: map.zoomLevel -= 1;
                }
            }

            PinchHandler {
                id: pinch
                target: null
                onActiveChanged: if (active) {
                    map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                }
                onScaleChanged: (delta) => {
                    map.zoomLevel += Math.log2(delta)
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
                onRotationChanged: (delta) => {
                    map.bearing -= delta
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
                grabPermissions: PointerHandler.TakeOverForbidden
            }
            WheelHandler {
                id: wheel
                rotationScale: 1/120
                property: "zoomLevel"
            }
            DragHandler {
                id: drag
                target: null
                onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
            }
            Shortcut {
                sequence: StandardKey.ZoomIn
                onActivated: map.zoomLevel += 1
            }
            Shortcut {
                sequence: StandardKey.ZoomOut
                onActivated: map.zoomLevel -= 1
            }
        }

        Timer {
            id: mapTimer
            interval: 200
            repeat: false
            running: false
            property int pendingTreeRow: -1
            property bool pendingTreeTip: false
            property bool pendingLocationTip: true
            property bool pendingSuccessMsg: false
            property bool pendingNewDatabase: newDatabase
            onTriggered: {
                if (pendingLocationTip) {
                    homePage.showCurrentLocation()
                    pendingLocationTip = false
                }
                if (pendingTreeTip) {
                    homePage.showBalloonTip(pendingTreeRow > -1 ? pendingTreeRow : plantProxy.nearestRow())
                    pendingTreeTip = false
                    pendingTreeRow = -1
                }
                if (pendingSuccessMsg && numberOfRows > 0) {
                    resultsFoundDialog.open()
                    pendingSuccessMsg = false
                }
                if (pendingNewDatabase) {
                    newDatabaseDialog.open()
                    pendingNewDatabase = false
                }
            }
        }

        MsgDialog {
            id: resultsFoundDialog
            y: 100
            title: qsTr("Success!")
            dlgMessage: qsTr("Found %Ln tree(s)", "", numberOfRows)
        }

        MsgDialog {
            id: newDatabaseDialog
            y: 100
            title: qsTr("New Database")
            dlgMessage: qsTr("A new database file has been downloaded. It will become active the next time you start this program.")
        }
    }

    ResultsPage {
        id: resultsPage
        onClosed: {
            windowHeader.title = qsTr("Barcelona Trees")
        }
        onRowSelected: (lat, lon, idx)=> {
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
        if (Qt.platform.os === "android") {
            optionsMenu.removeItem(closeItem)
        } else {
            optionsMenu.removeItem(positionEnabled)
        }
    }
}
