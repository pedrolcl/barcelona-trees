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

if (USE_QT5)
    find_package( QT NAMES Qt5 REQUIRED)
    find_package( Qt5 5.15 COMPONENTS Core Sql Network Gui Quick QuickControls2 Qml Positioning Location LinguistTools REQUIRED )
else()
    find_package( QT NAMES Qt6 REQUIRED)
    find_package( Qt6 6.5 COMPONENTS Core Sql Network Gui Quick QuickControls2 Qml Positioning Location LinguistTools REQUIRED )
endif()

find_package( Q7Zip CONFIG )
if ( NOT Q7Zip_FOUND )
    message(STATUS "Using the libq7zip git submodule")
    add_subdirectory( libq7zip )
endif()

include( SCMRevision )
include( GNUInstallDirs )

if (NOT ANDROID)
    add_subdirectory ( treedbcreator )
endif()

add_subdirectory ( barnatrees )
