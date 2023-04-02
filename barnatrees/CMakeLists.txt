set (HEADERS
    src/basemodel.h
    src/dropboxdownloader.h
    src/gendermodel.h
    src/plantmodel.h
    src/plantproxymodel.h
    src/speciesmodel.h
    src/splashwindow.h
    src/streetlistmodel.h
    src/summarymodel.h
)

set (SOURCES
    src/basemodel.cpp
    src/dropboxdownloader.cpp
    src/gendermodel.cpp
    src/main.cpp
    src/plantmodel.cpp
    src/plantproxymodel.cpp
    src/speciesmodel.cpp
    src/splashwindow.cpp
    src/streetlistmodel.cpp
    src/summarymodel.cpp
)

set (RESOURCES
    data.qrc
    qml/qml.qrc
    images/images.qrc
)

set (TRANSLATIONS
    translations/barnatrees_ca.ts
    translations/barnatrees_en.ts
    translations/barnatrees_es.ts
    translations/barnatrees_nl.ts
)

qt_add_executable( barnatrees
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
)

qt_add_translations( barnatrees
    TS_FILES ${TRANSLATIONS}
    RESOURCE_PREFIX "/"
)

target_compile_definitions( barnatrees PRIVATE
    APPVER=${PROJECT_VERSION}
    GITVER=${PROJECT_WC_REVISION}
)

set_target_properties( barnatrees PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries( barnatrees PRIVATE
    Qt6::Sql
    Qt6::Network
    Qt6::Quick
    Qt6::Positioning
    Qt6::Location
    Qt6::QuickControls2
    Q7Zip::q7z
)

install(TARGETS barnatrees
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
