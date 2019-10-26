TEMPLATE = app

QT += sql quick quickcontrols2 qml network positioning location

SOURCES = main.cpp \
    basemodel.cpp \
    plantmodel.cpp  \
    speciesmodel.cpp
	
RESOURCES += \
    barnatrees.qrc

HEADERS += \
    basemodel.h \
    plantmodel.h \
    speciesmodel.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    barnatrees.db \
    barnatrees.qml \
    qtquickcontrols2.conf

TRANSLATIONS = \
    translations/barnatrees_ca.ts \
    translations/barnatrees_en.ts \
    translations/barnatrees_es.ts

include(openssl.pri)

win32:RC_ICONS = barnatrees.ico
macx:ICON = barnatrees.icns

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
