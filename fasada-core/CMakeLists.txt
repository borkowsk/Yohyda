# This file is a part of Fasada toolbox
# ---------------------------------------------------
# @author Wojciech Borkowski <wborkowsk@gmail.com>
# @footnote Technical mail: <fasada.wb@gmail.com>
# @copyright 2019
# @version 0.01
#
#  See CURRENT licence file!
#

cmake_minimum_required(VERSION 2.8)

project(facjata)

set( SRCPATH "./") #Directory where this CMakeList.txt is put!

FIND_PACKAGE( Threads REQUIRED)
FIND_PACKAGE( Boost 1.58 COMPONENTS system program_options filesystem regex locale REQUIRED )
SET(Boost_USE_MULTITHREADED ON)

include_directories(
                    "${SRCPATH}"
                    "${SRCPATH}/lib/fasada/"
                    "${SRCPATH}/lib/facebookspec/"
                    "${SRCPATH}/tools/"
                     ${Boost_INCLUDE_DIR}
                    )

#How to make version.h from GIT versioning?
#See: https://gist.github.com/rkitover/3fe24bbf8f3a0e32a19259aca384680a

add_executable( treeserver
    "${SRCPATH}/treeserver1/treeserver1_main.cpp"
                )

add_executable( indexer
    "${SRCPATH}/tools/indexer/indexer_main.cpp"
                )

add_executable( treeedit
    "${SRCPATH}/tools/treeedit/treeedit_main.cpp"
                )

add_executable( wwwserver
    "${SRCPATH}/wwwserver1/connection.cpp"
    "${SRCPATH}/wwwserver1/connection_manager.cpp"
    "${SRCPATH}/wwwserver1/mime_types.cpp"
    "${SRCPATH}/wwwserver1/reply.cpp"
    "${SRCPATH}/wwwserver1/request_handler.cpp"
    "${SRCPATH}/wwwserver1/request_parser.cpp"
    "${SRCPATH}/wwwserver1/server.cpp"
    "${SRCPATH}/wwwserver1/commwithfasada.cpp"
    "${SRCPATH}/wwwserver1/wwwserver1_main.cpp"
                )

add_library(fasada
    #base
    "${SRCPATH}/lib/fasada/fasada.cpp"
    "${SRCPATH}/lib/fasada/tree_processor.cpp"
    "${SRCPATH}/lib/fasada/tree_attributes.cpp"
    #first set of processors
    "${SRCPATH}/lib/fasada/processor_info.cpp"
    "${SRCPATH}/lib/fasada/processor_get.cpp"
    "${SRCPATH}/lib/fasada/processor_set.cpp"
    "${SRCPATH}/lib/fasada/processor_add.cpp"
    "${SRCPATH}/lib/fasada/processor_del.cpp"
    "${SRCPATH}/lib/fasada/processor_ren.cpp"
    "${SRCPATH}/lib/fasada/processor_ls.cpp"
    "${SRCPATH}/lib/fasada/processor_dfs.cpp"
    "${SRCPATH}/lib/fasada/processor_find.cpp"
    "${SRCPATH}/lib/fasada/processor_check_file.cpp"
    "${SRCPATH}/lib/fasada/loader_txt.cpp"
    "${SRCPATH}/lib/fasada/loader_csv.cpp"
    "${SRCPATH}/lib/fasada/loader_json.cpp"
    #generated processors
    "${SRCPATH}/lib/fasada/view_csv.cpp"
    "${SRCPATH}/lib/fasada/run_on_tree.cpp"
    "${SRCPATH}/lib/fasada/save_as_txt.cpp"
    "${SRCPATH}/lib/fasada/save_as_csv.cpp"
    "${SRCPATH}/lib/fasada/save_as_xml.cpp"
    "${SRCPATH}/lib/fasada/save_as_json.cpp"
    "${SRCPATH}/lib/fasada/view_processor.cpp"
    "${SRCPATH}/lib/fasada/saver_processor.cpp"
    "${SRCPATH}/lib/fasada/loader_processor.cpp"
    #general
    "${SRCPATH}/lib/fasada/register_processors.cpp"
    "${SRCPATH}/lib/fasada/http/URLparser.cpp"
    "${SRCPATH}/lib/fasada/tree/tree.cpp"
    )

add_library(facebookspec
    "${SRCPATH}/lib/facebookspec/facebookspec.cpp"
    "${SRCPATH}/lib/facebookspec/loader_facebookjson.cpp"
    )

target_compile_options( treeserver PRIVATE "${CMAKE_CXX_FLAGS}" "-std=c++11" )
target_compile_options( treeedit   PRIVATE "${CMAKE_CXX_FLAGS}" "-std=c++11" )
target_compile_options( indexer    PRIVATE "${CMAKE_CXX_FLAGS}" "-std=c++11" )
target_compile_options( wwwserver  PRIVATE "${CMAKE_CXX_FLAGS}" "-std=c++11" )
target_compile_options( fasada     PRIVATE "${CMAKE_CXX_FLAGS}" "-std=c++11" )
target_compile_options( facebookspec     PRIVATE "${CMAKE_CXX_FLAGS}" "-std=c++11" )

target_link_libraries(treeserver LINK_PUBLIC
    fasada
    facebookspec
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LOCALE}
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

target_link_libraries(indexer  LINK_PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    )


