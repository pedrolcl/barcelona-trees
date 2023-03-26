cmake_minimum_required(VERSION 3.16)

project ( barcelona-trees 
    VERSION 0.0.5 
    LANGUAGES CXX
)

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package( Qt6 6.5 COMPONENTS Core Sql Network Quick QuickControls2 Qml Positioning Location REQUIRED )
find_package( Q7Zip REQUIRED )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

include( SCMRevision )
include( GNUInstallDirs )

add_subdirectory ( treedbcreator )
add_subdirectory ( barnatrees )

