# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/wb/SCC/working_copies/facies/serverside

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/wb/SCC/working_copies/facies/serverside

# Include any dependencies generated for this target.
include CMakeFiles/wwwserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wwwserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wwwserver.dir/flags.make

CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o: wwwserver1/connection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/connection.cpp

CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/connection.cpp > CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/connection.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o: wwwserver1/connection_manager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/connection_manager.cpp

CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/connection_manager.cpp > CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/connection_manager.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o: wwwserver1/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/main.cpp

CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/main.cpp > CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/main.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o: wwwserver1/mime_types.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/mime_types.cpp

CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/mime_types.cpp > CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/mime_types.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o: wwwserver1/reply.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/reply.cpp

CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/reply.cpp > CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/reply.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o: wwwserver1/request_handler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/request_handler.cpp

CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/request_handler.cpp > CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/request_handler.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o: wwwserver1/request_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/request_parser.cpp

CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/request_parser.cpp > CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/request_parser.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o


CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o: CMakeFiles/wwwserver.dir/flags.make
CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o: wwwserver1/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o -c /data/wb/SCC/working_copies/facies/serverside/wwwserver1/server.cpp

CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/wb/SCC/working_copies/facies/serverside/wwwserver1/server.cpp > CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.i

CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/wb/SCC/working_copies/facies/serverside/wwwserver1/server.cpp -o CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.s

CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.requires:

.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.requires

CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.provides: CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.requires
	$(MAKE) -f CMakeFiles/wwwserver.dir/build.make CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.provides.build
.PHONY : CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.provides

CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.provides.build: CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o


# Object files for target wwwserver
wwwserver_OBJECTS = \
"CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o" \
"CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o"

# External object files for target wwwserver
wwwserver_EXTERNAL_OBJECTS =

wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o
wwwserver: CMakeFiles/wwwserver.dir/build.make
wwwserver: /usr/lib/x86_64-linux-gnu/libboost_system.so
wwwserver: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
wwwserver: /usr/lib/x86_64-linux-gnu/libboost_system.so
wwwserver: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
wwwserver: CMakeFiles/wwwserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/wb/SCC/working_copies/facies/serverside/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable wwwserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wwwserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wwwserver.dir/build: wwwserver

.PHONY : CMakeFiles/wwwserver.dir/build

CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/connection.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/connection_manager.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/main.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/mime_types.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/reply.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/request_handler.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/request_parser.cpp.o.requires
CMakeFiles/wwwserver.dir/requires: CMakeFiles/wwwserver.dir/wwwserver1/server.cpp.o.requires

.PHONY : CMakeFiles/wwwserver.dir/requires

CMakeFiles/wwwserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wwwserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wwwserver.dir/clean

CMakeFiles/wwwserver.dir/depend:
	cd /data/wb/SCC/working_copies/facies/serverside && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/wb/SCC/working_copies/facies/serverside /data/wb/SCC/working_copies/facies/serverside /data/wb/SCC/working_copies/facies/serverside /data/wb/SCC/working_copies/facies/serverside /data/wb/SCC/working_copies/facies/serverside/CMakeFiles/wwwserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wwwserver.dir/depend

