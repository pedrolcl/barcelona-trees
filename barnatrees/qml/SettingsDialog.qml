import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
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
        close()
    }
    onRejected: {
        styleBox.currentIndex = styleBox.styleIndex
        langBox.currentIndex = langBox.langIndex
        close()
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
