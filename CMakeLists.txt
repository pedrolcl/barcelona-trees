cmake_minimum_required(VERSION 3.16)

project ( barcelona-trees
    VERSION 0.0.5
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
    add_subdirectory ( treedbcreator )
endif()

add_subdirectory ( barnatrees )
