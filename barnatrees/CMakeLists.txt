project ( barnatrees
    VERSION 0.0.7
    LANGUAGES CXX
)

include(TranslationUtils)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/barnatrees.txt)
    file(DOWNLOAD "https://pedrolcl.github.io/barcelona-trees/barnatrees.txt" "${CMAKE_CURRENT_SOURCE_DIR}/barnatrees.txt" )
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/barnatrees.db.7z)
    file(DOWNLOAD "https://pedrolcl.github.io/barcelona-trees/barnatrees.db.7z" "${CMAKE_CURRENT_SOURCE_DIR}/barnatrees.db.7z" )
endif()

set (HEADERS
    src/basemodel.h
    src/gendermodel.h
    src/plantmodel.h
    src/plantproxymodel.h
    src/speciesmodel.h
    src/splashwindow.h
    src/streetlistmodel.h
    src/summarymodel.h
    src/webdownloader.h
)

set (SOURCES
    src/basemodel.cpp
    src/gendermodel.cpp
    src/main.cpp
    src/plantmodel.cpp
    src/plantproxymodel.cpp
    src/speciesmodel.cpp
    src/splashwindow.cpp
    src/streetlistmodel.cpp
    src/summarymodel.cpp
    src/webdownloader.cpp
)

add_qt_translations_resource(QT_RES ca en es nl)

set (RESOURCES
    data.qrc
    images/images.qrc
    ${QT_RES}
)

if (WIN32)
    configure_file(barnatrees.rc.in barnatrees.rc @ONLY)
    list ( APPEND RESOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/barnatrees.rc
    )
endif()

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

set_source_files_properties(qml/AboutDialog.qml PROPERTIES QT_RESOURCE_ALIAS AboutDialog.qml)
set_source_files_properties(qml/BalloonTip.qml PROPERTIES QT_RESOURCE_ALIAS BalloonTip.qml)
set_source_files_properties(qml/GenderSearchDialog.qml PROPERTIES QT_RESOURCE_ALIAS GenderSearchDialog.qml)
set_source_files_properties(qml/MainWindow.qml PROPERTIES QT_RESOURCE_ALIAS MainWindow.qml)
set_source_files_properties(qml/MsgDialog.qml PROPERTIES QT_RESOURCE_ALIAS MsgDialog.qml)
set_source_files_properties(qml/ResultsPage.qml PROPERTIES QT_RESOURCE_ALIAS ResultsPage.qml)
set_source_files_properties(qml/SettingsDialog.qml PROPERTIES QT_RESOURCE_ALIAS SettingsDialog.qml)
set_source_files_properties(qml/SpecieSearchDialog.qml PROPERTIES QT_RESOURCE_ALIAS SpecieSearchDialog.qml)
set_source_files_properties(qml/SpecimenDialog.qml PROPERTIES QT_RESOURCE_ALIAS SpecimenDialog.qml)
set_source_files_properties(qml/StreetSearchDialog.qml PROPERTIES QT_RESOURCE_ALIAS StreetSearchDialog.qml)
set_source_files_properties(qml/qtquickcontrols2.conf PROPERTIES QT_RESOURCE_ALIAS qtquickcontrols2.conf)

qt_add_qml_module( barnatrees
    URI "io.github.pedrolcl"
    RESOURCE_PREFIX "/"
    QML_FILES
        qml/AboutDialog.qml
        qml/BalloonTip.qml
        qml/GenderSearchDialog.qml
        qml/MainWindow.qml
        qml/MsgDialog.qml
        qml/ResultsPage.qml
        qml/SettingsDialog.qml
        qml/SpecieSearchDialog.qml
        qml/SpecimenDialog.qml
        qml/StreetSearchDialog.qml
    RESOURCES
        qml/qtquickcontrols2.conf
    NO_RESOURCE_TARGET_PATH
)

if(WIN32)
    set_target_properties( barnatrees PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

if (ANDROID)
    include( ../android_openssl/android_openssl.cmake )
    add_android_openssl_libraries( barnatrees )
endif()

if(ANDROID)
    set_target_properties(barnatrees PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
        QT_ANDROID_VERSION_CODE "${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR}${PROJECT_VERSION_PATCH}"
        QT_ANDROID_VERSION_NAME "v${PROJECT_VERSION}"
        QT_ANDROID_PACKAGE_NAME "io.github.pedrolcl.barnatrees"
        QT_ANDROID_APP_ICON "@drawable/icon"
        QT_ANDROID_APP_NAME "Barcelona Trees for Android"
    )
    set ( QT_ANDROID_BUILD_ALL_ABIS TRUE )
endif()

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "barnatrees.icns")
    set(APP_ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/images/barnatrees.icns")
    set_source_files_properties( ${APP_ICON_PATH} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources" )
    target_sources( barnatrees PUBLIC ${APP_ICON_PATH} )
    set_target_properties( barnatrees PROPERTIES
        BUNDLE TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE ${MACOSX_BUNDLE_ICON_FILE}
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.pedrolcl.barnatrees"
        MACOSX_BUNDLE_BUNDLE_NAME "barnatrees"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_COPYRIGHT "Copyright © 2019-2026 Pedro López-Cabanillas"
        MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    )
endif()

target_link_libraries( barnatrees PRIVATE
    Qt6::Sql
    Qt6::Network
    Qt6::Quick
    Qt6::Positioning
    Qt6::Location
    Qt6::QuickControls2
    QArchive
)

install( TARGETS barnatrees
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    BUNDLE DESTINATION .
)

if (APPLE)
    if(TARGET q7z)
        install( TARGETS q7z
                 LIBRARY DESTINATION barnatrees.app/Contents/Frameworks )
    else()
        install( IMPORTED_RUNTIME_ARTIFACTS Q7Zip::q7z
                 LIBRARY DESTINATION barnatrees.app/Contents/Frameworks )
    endif()
endif()

if (UNIX AND NOT APPLE)
    install( FILES barnatrees.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )
    install( FILES images/barnatrees_icon256.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps
        RENAME barnatrees.png
    )
    install( FILES io.github.pedrolcl.barnatrees.metainfo.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo
    )
endif()
