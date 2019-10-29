import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: Math.round(Math.min(window.width, window.height) / 3 * 2)
    modal: true
    focus: true
    title: qsTr("Settings")
    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        settings.language = langBox.model.get(langBox.currentIndex).value
        settings.links = linkBox.displayText
        settings.style = styleBox.displayText
        close()
    }
    onRejected: {
        styleBox.currentIndex = styleBox.styleIndex
        langBox.currentIndex = langBox.langIndex
        close()
    }

    Column {
        anchors.fill: parent
        spacing: 20

        Row {
            spacing: 10
            width: parent.width
            Label {
                text: qsTr("Style:")
                anchors.verticalCenter: parent.verticalCenter
            }
            ComboBox {
                id: styleBox
                anchors.verticalCenter: parent.verticalCenter
                property int styleIndex: -1
                model: availableStyles
                Component.onCompleted: {
                    styleIndex = find(settings.style, Qt.MatchFixedString)
                    if (styleIndex !== -1)
                        currentIndex = styleIndex
                }
            }
        }

        Row {
            spacing: 10
            width: parent.width
            Label {
                text: qsTr("Language:")
                anchors.verticalCenter: parent.verticalCenter
            }
            ComboBox {
                id: langBox
                anchors.verticalCenter: parent.verticalCenter
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
            }
        }

        Row {
            spacing: 10
            width: parent.width
            Label {
                text: qsTr("Wiki:")
                anchors.verticalCenter: parent.verticalCenter
            }
            ComboBox {
                id: linkBox
                property int linkIndex: -1
                anchors.verticalCenter: parent.verticalCenter
                model:  ListModel {
                    ListElement { text: qsTr("Wikipedia") }
                    ListElement { text: qsTr("WikiSpecies") }
                    ListElement { text: qsTr("WikiCommons") }
                }
                Component.onCompleted: {
                    linkIndex = find(settings.links, Qt.MatchFixedString)
                    if (linkIndex !== -1)
                        currentIndex = linkIndex
                }
            }
        }

        Label {
            text: qsTr("Restart required")
            color: "#e41e25"
            opacity: ( styleBox.currentIndex !== styleBox.styleIndex ||
                       langBox.currentIndex !== langBox.langIndex ) ? 1.0 : 0.0
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignVCenter
            width: parent.width
        }
    }
}
