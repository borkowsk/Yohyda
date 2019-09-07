cmake_minimum_required(VERSION 2.8)

project(facjata)

set( SRCPATH "./")

FIND_PACKAGE( Threads REQUIRED)
FIND_PACKAGE( Boost 1.60 COMPONENTS system program_options REQUIRED )
SET(Boost_USE_MULTITHREADED ON)

include_directories(
                    "${SRCPATH}"
                     ${Boost_INCLUDE_DIR}
                    )

add_executable(wwwserver
    "${SRCPATH}connection.cpp"
    "${SRCPATH}connection_manager.cpp"
    "${SRCPATH}main.cpp"
    "${SRCPATH}mime_types.cpp"
    "${SRCPATH}reply.cpp"
    "${SRCPATH}request_handler.cpp"
    "${SRCPATH}request_parser.cpp"
    "${SRCPATH}server.cpp"
                )

target_compile_options(wwwserver PRIVATE "${CMAKE_CXX_FLAGS}")

target_link_libraries(wwwserver LINK_PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )


