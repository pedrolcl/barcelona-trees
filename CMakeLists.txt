cmake_minimum_required(VERSION 3.16)

project ( barcelona-trees
    VERSION 0.0.6
    LANGUAGES CXX
)

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( SCMRevision )
include( GNUInstallDirs )

find_package( QT NAMES Qt6 REQUIRED)
find_package( Qt6 6.5 COMPONENTS Core Sql Network Gui Quick QuickControls2 Qml Positioning Location LinguistTools REQUIRED )
find_package( Q7Zip CONFIG )
if ( NOT Q7Zip_FOUND )
    message(STATUS "Using the libq7zip git submodule")
    add_subdirectory( libq7zip )
endif()

qt_standard_project_setup(REQUIRES 6.5)

if (NOT ANDROID)
    set(QARCHIVE_QT_VERSION_MAJOR 6)
    add_subdirectory( QArchive )
    add_subdirectory ( treedbcreator )
endif()

add_subdirectory ( barnatrees )

if(MSVC)
    qt_generate_deploy_qml_app_script(
        TARGET barnatrees
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
        NO_COMPILER_RUNTIME
        NO_TRANSLATIONS
        EXCLUDE_PLUGINS qtposition_nmea
    )
    include ( InstallRequiredSystemLibraries )
else()
    qt_generate_deploy_qml_app_script(
        TARGET barnatrees
        OUTPUT_SCRIPT deploy_script
        INCLUDE_PLUGINS xcb wayland
        NO_UNSUPPORTED_PLATFORM_ERROR
        NO_TRANSLATIONS
        EXCLUDE_PLUGINS qtposition_nmea
    )
endif()
install(SCRIPT ${deploy_script})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    install( CODE
      "include(${CMAKE_SOURCE_DIR}/cmake/CreateAppImage.cmake)
       create_appimage(
            EXE \"barnatrees\"
            ICON \"barnatrees.png\"
            DESKTOP \"barnatrees.desktop\"
            WORKDIR \"${CMAKE_BINARY_DIR}\"
            APPDIR \"${PROJECT_NAME}-v${PROJECT_VERSION}-${CMAKE_SYSTEM_PROCESSOR}.AppDir\"
            ARCH \"${CMAKE_SYSTEM_PROCESSOR}\"
       )"
    )
endif()

if (DEFINED ENV{GITHUB_OUTPUT})
    file(APPEND
        "$ENV{GITHUB_OUTPUT}"
        "config_version=${PROJECT_VERSION}\n"
    )
endif()

# CPack support
set ( CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Barcelona Trees" )
set ( CPACK_PACKAGE_VENDOR "https://pedrolcl.github.io/barcelona-trees" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md" )
set ( CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE" )
set ( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set ( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set ( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
set ( CPACK_PACKAGE_EXECUTABLES "barnatrees" "Barcelona Trees" )

# source packages
set ( CPACK_SOURCE_IGNORE_FILES "/.git/;/build/;~$;${CPACK_SOURCE_IGNORE_FILES}" )
set ( CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT-NAME}-${PROJECT_VERSION}" )
set ( CPACK_SOURCE_STRIP_FILES OFF )

# binary packages
set ( CPACK_PACKAGE_NAME ${PROJECT-NAME} )
set ( CPACK_STRIP_FILES ON )

# Windows NSIS setup package
if (WIN32)
    set (CPACK_NSIS_DISPLAY_NAME "Barcelona Trees")
    set (CPACK_NSIS_WELCOME_TITLE_3LINES ON)
    set (CPACK_NSIS_FINISH_TITLE_3LINES ON)
    set (CPACK_NSIS_HELP_LINK "https://github.com/pedrolcl/barcelona-trees")
    set (CPACK_NSIS_URL_INFO_ABOUT "https://pedrolcl.github.io/barcelona-trees")
    set (CPACK_NSIS_CONTACT "plcl@users.sf.net")
endif ()

include ( CPack )
