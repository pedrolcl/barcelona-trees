TEMPLATE = app
QT += sql quick quickcontrols2 qml network positioning location
CONFIG += c++11 lrelease embed_translations
LRELEASE_DIR='.'
QM_FILES_RESOURCE_PREFIX='/'
win32:VERSION = 0.0.1.0
else:VERSION = 0.0.1
DEFINES += APPVER=$$VERSION
DEFINES += GITVER=$$system(git describe --always)
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    src/basemodel.h \
    src/plantmodel.h \
    src/speciesmodel.h \
    src/summarymodel.h

SOURCES = \
    src/basemodel.cpp \
    src/main.cpp \
    src/plantmodel.cpp  \
    src/speciesmodel.cpp \
    src/summarymodel.cpp
	
RESOURCES += \
    qml/qml.qrc \
    images/images.qrc

exists(barnatrees.db) {
    # create barnatrees.txt
    # linux*:system($$PWD/barnatrees.sh)
    RESOURCES += data.qrc
} else {
    error("barnatrees.db missing")
}


DISTFILES += \
    qml/BalloonTip.qml \
    qml/MainWindow.qml \
    qml/AboutDialog.qml \
    qml/GenderSearchDialog.qml \
    qml/HomePage.qml \
    qml/Marker.qml \
    qml/ResultsPage.qml \
    qml/DetailsPage.qml \
    qml/SettingsDialog.qml \
    qml/SpecieSearchDialog.qml \
    qml/SpecimenDialog.qml \
    qml/StreetSearchDialog.qml \
    qml/qtquickcontrols2.conf \
    images/barnatrees_icon32.png \
    images/barnatrees_icon64.png \
    images/icons/example/index.theme \
    images/icons/example/20x20/back.png \
    images/icons/example/20x20/drawer.png \
    images/icons/example/20x20/menu.png \
    images/icons/example/20x20@2/back.png \
    images/icons/example/20x20@2/drawer.png \
    images/icons/example/20x20@2/menu.png \
    images/icons/example/20x20@3/back.png \
    images/icons/example/20x20@3/drawer.png \
    images/icons/example/20x20@3/menu.png \
    images/icons/example/20x20@4/back.png \
    images/icons/example/20x20@4/drawer.png \
    images/icons/example/20x20@4/menu.png \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/drawable-hdpi/icon.png \
    android/drawable-ldpi/icon.png \
    android/drawable-mdpi/icon.png \
    barnatrees.db

TRANSLATIONS = \
    translations/barnatrees_ca.ts \
    translations/barnatrees_en.ts \
    translations/barnatrees_es.ts

win32:RC_ICONS = images/barnatrees.ico
macx:ICON = images/barnatrees.icns

LCONVERT_LANGS=ca es
include(../lconvert.pri)
include(../openssl.pri)
