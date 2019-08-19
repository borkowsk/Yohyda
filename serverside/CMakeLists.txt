cmake_minimum_required(VERSION 2.8)

project(facjata)

set( SRCPATH "./")

FIND_PACKAGE( Threads REQUIRED)
FIND_PACKAGE( Boost 1.60 COMPONENTS system program_options REQUIRED )
SET(Boost_USE_MULTITHREADED ON)

include_directories(
                    "${SRCPATH}"
                    "${SRCPATH}/wwwserver1/"
                    "${SRCPATH}/treeserver1/"
                     ${Boost_INCLUDE_DIR}
                    )

add_executable(treeserver
    "${SRCPATH}/treeserver1/main.cpp"
            )
                    
add_executable(wwwserver
    "${SRCPATH}/wwwserver1/connection.cpp"
    "${SRCPATH}/wwwserver1/connection_manager.cpp"
    "${SRCPATH}/wwwserver1/main.cpp"
    "${SRCPATH}/wwwserver1/mime_types.cpp"
    "${SRCPATH}/wwwserver1/reply.cpp"
    "${SRCPATH}/wwwserver1/request_handler.cpp"
    "${SRCPATH}/wwwserver1/request_parser.cpp"
    "${SRCPATH}/wwwserver1/server.cpp"
                )
                
target_compile_options( treeserver PRIVATE "${CMAKE_CXX_FLAGS}" )
target_compile_options( wwwserver  PRIVATE "${CMAKE_CXX_FLAGS}" )

target_link_libraries(treeserver LINK_PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )

target_link_libraries(wwwserver LINK_PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )


