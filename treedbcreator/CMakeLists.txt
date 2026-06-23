
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
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE FALSE
)

target_link_libraries( treedbcreator PRIVATE
    Qt6::Core
    Qt6::Sql
    Qt6::Network
    QArchive
)

install(TARGETS treedbcreator
    RUNTIME_DEPENDENCY_SET treedbcreator-dependencies
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    BUNDLE DESTINATION .
)

if (DEFINED DEPENDENCY_DIRS AND WIN32)
    install(RUNTIME_DEPENDENCY_SET treedbcreator-dependencies
        PRE_INCLUDE_REGEXES "archive.*" "z.*" "bz2.*" "liblzma.*" "libcrypto-.*" "lz4.*" "zstd.*"
        PRE_EXCLUDE_REGEXES ".*"
        DIRECTORIES ${DEPENDENCY_DIRS}
    )
endif()