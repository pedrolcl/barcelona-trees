QT = core sql network
CONFIG += c++11 cmdline
win32:VERSION = 0.0.5.0
else:VERSION = 0.0.5
DEFINES += APPVER=$$VERSION
DEFINES += GITVER=$$system(git describe --always)
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    downloadmanager.cpp \
    main.cpp \
    treedata.cpp \
    jsonparser.cpp

HEADERS += \
    downloadmanager.h \
    treedata.h \
    jsonparser.h

include(../openssl.pri)

DISTFILES += \
    readme.md

win32 {
    QMAKE_TARGET_COMPANY = "Pedro Lopez-Cabanillas"
    QMAKE_TARGET_DESCRIPTION = "Barcelona Trees DB Creator" 
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2019-2023 Pedro Lopez-Cabanillas"
    QMAKE_TARGET_PRODUCT = "Barcelona Trees"
}
