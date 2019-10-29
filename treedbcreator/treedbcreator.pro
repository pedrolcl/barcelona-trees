QT = core sql network
CONFIG += c++11 console
win32:VERSION = 0.0.1.0
else:VERSION = 0.0.1
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

include(openssl.pri)

DISTFILES += \
    readme.md
