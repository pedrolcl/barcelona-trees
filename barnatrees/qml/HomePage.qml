import QtQuick 2.12
import QtQuick.Controls 2.12
import QtPositioning 5.12
import QtLocation 5.12

Page {
    title: qsTr("Barcelona Trees")

    property variant locationBarna: QtPositioning.coordinate( 41.403216, 2.186674 )

    function togglePositioning(checked) {
        locationCircle.visible = checked
        if (checked) {
            positionSource.update()
        }
    }

    function changeMapCenter(newcenter) {
        map.center = newcenter
        map.zoomLevel = map.maximumZoomLevel
        //map.zoomLevel = 18
    }

    PositionSource {
        id: positionSource
        active: true
        updateInterval: 120000 // 2 mins
        property variant lastSearchPosition: locationBarna
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
        onCopyrightLinkActivated: Qt.openUrlExternally(link)

        MapQuickItem {
            id: locationCircle
            sourceItem: Rectangle {
                width: 14;
                height: 14;
                color: 'deepskyblue'
                border.width: 3.0
                border.color: 'paleturquoise'
                smooth: true;
                radius: 7
            }
            visible: true
            coordinate: locationBarna
            opacity:1.0
            anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
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
                            return "<a href='https://es.wikipedia.org/wiki/%1'><i>%2</i></a>".arg(s).arg(model.scientificName);
                        }
                        function streetViewLink()
                        {
                            var pos = "viewpoint=%1,%2".arg(model.latitude).arg(model.longitude);
                            return "<a href='https://www.google.com/maps/@?api=1&map_action=pano&%1'>%2</a>".arg(pos).arg(model.plantAddress);
                        }
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            specimenDialog.specieScientificName = wikipediaLink()
                            specimenDialog.specieCommonNames = model.commonName1 + '\n' + model.commonName2
                            specimenDialog.specimenLocation = streetViewLink()
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
                        font.italic: true
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
}
