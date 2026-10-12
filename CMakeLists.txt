cmake_minimum_required(VERSION 3.19)

project ( barcelona-trees
    VERSION 0.0.7
    LANGUAGES CXX
)

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( SCMRevision )
include( GNUInstallDirs )

option(BUILD_TREEDBCREATOR "Build the treedbcreator program" ON)

find_package( QT NAMES Qt6 REQUIRED)
find_package( Qt6 6.5 COMPONENTS Core Sql Network Gui Quick QuickControls2 Qml Positioning Location LinguistTools REQUIRED )

message(STATUS "Using the QArchive git submodule")
set(QARCHIVE_QT_VERSION_MAJOR 6)
add_subdirectory( QArchive )

qt_standard_project_setup(REQUIRES 6.5)

if (BUILD_TREEDBCREATOR AND NOT ANDROID AND NOT APPLE)
    add_subdirectory ( treedbcreator )
endif()

add_subdirectory ( barnatrees )

find_package(LibArchive CONFIG)
message(STATUS "LibArchive_FOUND: ${LibArchive_FOUND}")
if (LibArchive_FOUND)
    install( IMPORTED_RUNTIME_ARTIFACTS LibArchive::LibArchive
        RUNTIME_DEPENDENCY_SET libarchive_dependencies
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
    if(WIN32)
      set(_pre_exclude api-ms ext-ms wpaxholder hvsi azureattest pdmutilities
                       vcruntime msvcp concrt vccorlib ucrtbase )
      set(_post_exclude [[.*(\\|/)system32(\\|/).*\.dll]])
    elseif(APPLE)
      set(_pre_exclude "/usr/lib" "/System/")
      set(_post_exclude "")
    else()
      set(_pre_exclude "")
      set(_post_exclude ".*/ld-linux-.*" ".*/linux-vdso.*" ".*/libm\\..*" ".*/libc\\..*" ".*/libpthread\\..*" ".*/libdl\\..*" ".*/libstdc\\+\\+\\..*" ".*/libgcc_s.*" ".*/libgomp\\..*" ".*/libQt.*")
    endif()
    install(RUNTIME_DEPENDENCY_SET libarchive_dependencies
            PRE_EXCLUDE_REGEXES ${_pre_exclude}
            POST_EXCLUDE_REGEXES ${_post_exclude}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(MSVC)
    qt_generate_deploy_qml_app_script(
        TARGET barnatrees
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
        NO_COMPILER_RUNTIME
        NO_TRANSLATIONS
        EXCLUDE_PLUGINS "qtposition_nmea;qsqlodbc;qsqlpsql;qsqlmimer;qsqlibase;qsqlmysql;qsqloci"
    )
    include ( InstallRequiredSystemLibraries )
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    qt_generate_deploy_qml_app_script(
        TARGET barnatrees
        OUTPUT_SCRIPT deploy_script
        INCLUDE_PLUGINS xcb wayland
        NO_UNSUPPORTED_PLATFORM_ERROR
        NO_TRANSLATIONS
        EXCLUDE_PLUGINS "qtposition_nmea;libqsqlodbc;libqsqlpsql;libqsqlmimer;libqsqlibase;libqsqlmysql;libqsqloci"
    )
else()
    qt_generate_deploy_qml_app_script(
        TARGET barnatrees
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
        NO_TRANSLATIONS
    )
endif()
if(DEFINED deploy_script)
    install(SCRIPT ${deploy_script})
endif()

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

if(APPLE)
    set (CPACK_SYSTEM_NAME "macOS")
    set (CPACK_PRE_BUILD_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ThinFrameworks.cmake")
    set (CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${PROJECT_VERSION}-macOS-${CMAKE_SYSTEM_PROCESSOR}")
    set (CPACK_BINARY_ZIP ON)
    set (CPACK_BINARY_TGZ OFF)
    set (CPACK_BINARY_STGZ OFF)
endif()

include ( CPack )
