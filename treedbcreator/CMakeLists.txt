
set( SOURCES
    downloadmanager.cpp
    main.cpp
    treedata.cpp
    jsonparser.cpp
)

set ( HEADERS
    downloadmanager.h
    treedata.h
    jsonparser.h
)

qt_add_executable( treedbcreator
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions( treedbcreator PRIVATE
    APPVER=${PROJECT_VERSION}
    GITVER=${PROJECT_WC_REVISION}
    QT_DEPRECATED_WARNINGS
)

set_target_properties( treedbcreator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries( treedbcreator PRIVATE
    Qt6::Core
    Qt6::Sql
    Qt6::Network
    QArchive
)

install(TARGETS treedbcreator
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    BUNDLE DESTINATION .
)
