QT = core sql network
CONFIG += c++11 cmdline
win32:VERSION = 0.0.3.0
else:VERSION = 0.0.3
DEFINES += APPVER=$$VERSION
DEFINES += GITVER=$$system(git describe --always)
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    downloadmanager.cpp \
    main.cpp \
    treedata.cpp \
    xmlparser.cpp

HEADERS += \
    downloadmanager.h \
    treedata.h \
    xmlparser.h

include(../openssl.pri)

DISTFILES += \
    readme.md
