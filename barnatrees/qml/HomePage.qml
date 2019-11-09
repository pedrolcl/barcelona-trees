import QtQuick 2.12
import QtQuick.Controls 2.12
import QtPositioning 5.12
import QtLocation 5.12

Page {
    title: qsTr("Barcelona Trees")

    property variant locationBarna: QtPositioning.coordinate( 41.403216, 2.186674 )
    property double defaultZoom: map.maximumZoomLevel - 2
    property bool pendingTreeTip: false
    property bool pendingLocationTip: true
    property var globalItems: []

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
        if(map.mapReady && pendingLocationTip) {
            locationTip.open()
            pendingLocationTip = false;
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
        pendingTreeTip = false
    }

    function resultsFound() {
        pendingTreeTip = true
        changeMapCenter(plantModel.nearestPlantCoordinate())
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
            if (positionEnabled.checked && distance1 > 250 && distance2 < 8000) { // 250 m from last location and 8 km from Glories
                lastSearchPosition = currentPosition
                changeGlobalCenter(currentPosition)
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
        zoomLevel: defaultZoom

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
                        onEntered: tip.visible = (map.zoomLevel === map.maximumZoomLevel)
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
    }

    Timer {
        id: mapTimer
        interval: 200
        repeat: false
        running: false
        onTriggered: {
            if (pendingLocationTip) {
                showCurrentLocation()
            }
            if (pendingTreeTip) {
                showBalloonTip(plantModel.nearestRow())
            }
        }
    }
}
