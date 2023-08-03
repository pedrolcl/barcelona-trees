project ( barnatrees
    VERSION 0.0.5
    LANGUAGES CXX
)

include(TranslationUtils)
include(FetchContent)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/barnatrees.txt)
    FetchContent_Declare ( barnatrees_timestamp
        DOWNLOAD_NAME "barnatrees.txt"
        DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}
        DOWNLOAD_NO_EXTRACT true
        URL "https://www.dropbox.com/s/e485ypwbdy113v9/barnatrees.txt?dl=1" )
    FetchContent_MakeAvailable(barnatrees_timestamp)
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/barnatrees.db.7z)
    FetchContent_Declare ( barnatrees_database
        DOWNLOAD_NAME "barnatrees.db.7z"
        DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}
        DOWNLOAD_NO_EXTRACT true
        URL "https://www.dropbox.com/s/7o9aa8744mwjx7i/barnatrees.db.7z?dl=1" )
    FetchContent_MakeAvailable(barnatrees_database)
endif()

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

set_target_properties( barnatrees PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER io.github.pedrolcl.barnatrees
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

if (ANDROID)
    include( ../android_openssl/CMakeLists.txt )
    if( QT_VERSION VERSION_GREATER_EQUAL 6.5 )
        add_target_openssl_libs( barnatrees )
    endif()
endif()

install(TARGETS barnatrees
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if (UNIX AND NOT APPLE)
    install( FILES barnatrees.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )
    install( FILES images/barnatrees_icon128.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons
        RENAME barnatrees.png
    )
endif()

if(ANDROID)
    set_property(TARGET barnatrees APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
    set ( QT_ANDROID_VERSION_CODE "${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR}${PROJECT_VERSION_PATCH}" )
    set ( QT_ANDROID_VERSION_NAME "v${PROJECT_VERSION}" )
endif()
