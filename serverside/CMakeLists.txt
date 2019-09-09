cmake_minimum_required(VERSION 2.8)

project(facjata)

set( SRCPATH "./")

FIND_PACKAGE( Threads REQUIRED)
FIND_PACKAGE( Boost 1.60 COMPONENTS system program_options regex REQUIRED )
SET(Boost_USE_MULTITHREADED ON)

include_directories(
                    "${SRCPATH}"
                    "${SRCPATH}/tools/"
                    "${SRCPATH}/fasada/"
                    "${SRCPATH}/tools/URLparserLib"
                    "${SRCPATH}/tools/PTREEWalkersLib"
                     ${Boost_INCLUDE_DIR}
                    )

add_library(fasada
    "${SRCPATH}/fasada/tree_processor.cpp"
    "${SRCPATH}/fasada/processor_get.cpp"
    "${SRCPATH}/fasada/processor_ls.cpp"
    "${SRCPATH}/fasada/processor_dfs.cpp"
    "${SRCPATH}/tools/URLparserLib/URLparser.cpp"
    )

add_executable( treeserver
    "${SRCPATH}/treeserver1/main.cpp"
                )

add_executable( treeedit
    "${SRCPATH}/tools/treeedit/main.cpp"
                )
                    
add_executable( wwwserver
    "${SRCPATH}/wwwserver1/connection.cpp"
    "${SRCPATH}/wwwserver1/connection_manager.cpp"
    "${SRCPATH}/wwwserver1/main.cpp"
    "${SRCPATH}/wwwserver1/mime_types.cpp"
    "${SRCPATH}/wwwserver1/reply.cpp"
    "${SRCPATH}/wwwserver1/request_handler.cpp"
    "${SRCPATH}/wwwserver1/request_parser.cpp"
    "${SRCPATH}/wwwserver1/server.cpp"
    "${SRCPATH}/tools/URLparserLib/URLparser.cpp"
                )
       
target_compile_options( treeserver PRIVATE "${CMAKE_CXX_FLAGS}" )
target_compile_options( treeedit   PRIVATE "${CMAKE_CXX_FLAGS}" )
target_compile_options( wwwserver  PRIVATE "${CMAKE_CXX_FLAGS}" )

target_link_libraries(treeserver LINK_PUBLIC
    fasada
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    )

target_link_libraries(wwwserver LINK_PUBLIC
    fasada
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    )

target_link_libraries(treeedit  LINK_PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    )




