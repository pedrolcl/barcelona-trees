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
import QtPositioning 5.12
import QtLocation 5.12

Page {
    title: qsTr("Barcelona Trees")

    property variant locationBarna: QtPositioning.coordinate( 41.403216, 2.186674 )
    property double defaultZoom: map.maximumZoomLevel - 2
    property var globalItems: []
    property int numberOfRows: 0

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
        clearItems()
        map.zoomLevel = defaultZoom
        map.center = coordinate
        locationCircle.coordinate = coordinate
        plantModel.setCenter(coordinate)
        locationTip.open()
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
                changeGlobalCenter(currentPosition)
            }
        }
    }

    Map {
        id: map
        maximumZoomLevel: 20.0
        minimumZoomLevel: 13.0
        anchors.fill: parent
        plugin: Plugin {
            name: "osm" // esri, osm, mapboxgl
            //PluginParameter {...}
        }
        center: locationBarna
        zoomLevel: defaultZoom

        //onZoomLevelChanged: console.log("Zoom: " + map.zoomLevel)
        onCopyrightLinkActivated: Qt.openUrlExternally(link)

        onMapItemsChanged: {
            if (globalItems.length == 0 && mapItems.length === (1 + plantModel.rowCount())) {
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

        MapQuickItem {
            id: locationCircle
            coordinate: locationBarna
            opacity:1.0
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
                zoomLevel: map.maximumZoomLevel
                anchorPoint.x: mark.width / 2
                anchorPoint.y: mark.height / 2
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)

                property int modelIndex: model.index

                function showTip() {
                    tip.open()
                }

                sourceItem: Rectangle {
                    id: mark
                    radius: 5
                    smooth: true
                    width: 10
                    height: 10
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
                        onEntered: tip.visible = (map.zoomLevel >= map.maximumZoomLevel)
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
            onPressAndHold: changeGlobalCenter(map.toCoordinate(Qt.point(mouse.x, mouse.y)))
        }

        Column {
            anchors.top: parent.top
            anchors.left: parent.left
            Button {
                width: 50
                height: 50
                icon.name: "zoomin"
                onPressed: if (map.zoomLevel < 20.0) map.zoomLevel += 0.1;
            }
            Button {
                width: 50
                height: 50
                icon.name: "zoomout"
                onPressed: if (map.zoomLevel > 13.0) map.zoomLevel -= 0.1;
            }
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
                showCurrentLocation()
                pendingLocationTip = false
            }
            if (pendingTreeTip) {
                showBalloonTip(pendingTreeRow > -1 ? pendingTreeRow : plantProxy.nearestRow())
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
