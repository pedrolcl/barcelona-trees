QT = core sql network
CONFIG += c++11 cmdline
win32:VERSION = 0.0.4.0
else:VERSION = 0.0.4
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
