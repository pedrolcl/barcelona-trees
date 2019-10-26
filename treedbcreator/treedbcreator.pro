QT = core sql network
CONFIG += c++11 console
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
