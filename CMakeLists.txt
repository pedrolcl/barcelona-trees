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
    )
    include ( InstallRequiredSystemLibraries )
else()
    qt_generate_deploy_qml_app_script(
        TARGET barnatrees
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
endif()
install(SCRIPT ${deploy_script})

if (CMAKE_SYSTEM_NAME MATCHES "Linux" AND DEFINED ENV{DESTDIR})
    # AppDir symlinks
    string(REPLACE "/" "" RELPREF ${CMAKE_INSTALL_PREFIX})
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${RELPREF}/${CMAKE_INSTALL_BINDIR}/barnatrees $ENV{DESTDIR}/AppRun)"
        COMPONENT sonivoxeas_runtime)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${RELPREF}/${CMAKE_INSTALL_DATAROOTDIR}/applications/barnatrees.desktop $ENV{DESTDIR}/barnatrees.desktop)"
        COMPONENT sonivoxeas_runtime)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${RELPREF}/${CMAKE_INSTALL_DATAROOTDIR}/icons/barnatrees.png $ENV{DESTDIR}/barnatrees.png)"
        COMPONENT sonivoxeas_runtime)
endif()

if (DEFINED ENV{GITHUB_OUTPUT})
    file(APPEND
        "$ENV{GITHUB_OUTPUT}"
        "config_version=${PROJECT_VERSION}\n"
    )
endif()
