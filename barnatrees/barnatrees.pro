TEMPLATE = app
QT += sql quick quickcontrols2 qml network positioning location
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    src/basemodel.h \
    src/plantmodel.h \
    src/speciesmodel.h

SOURCES = \
    src/basemodel.cpp \
    src/main.cpp \
    src/plantmodel.cpp  \
    src/speciesmodel.cpp
	
RESOURCES += \
    qml/qml.qrc \
    images/images.qrc \
    translations/translations.qrc

DISTFILES += \
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
    barnatrees.db

TRANSLATIONS = \
    translations/barnatrees_ca.ts \
    translations/barnatrees_en.ts \
    translations/barnatrees_es.ts

include(openssl.pri)

win32:RC_ICONS = images/barnatrees.ico
macx:ICON = images/barnatrees.icns

datafiles.files = $$PWD/barnatrees.db
android {
    datafiles.path = /assets
    INSTALLS += datafiles
    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        ANDROID_PACKAGE_SOURCE_DIR = \
            $$PWD/android
    }
} else {
    CONFIG += file_copies
    datafiles.path = $$OUT_PWD
    COPIES += datafiles
}
