# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/michael/cppCode/ClientTCP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michael/cppCode/ClientTCP/build

# Include any dependencies generated for this target.
include CMakeFiles/TcpProg.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TcpProg.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TcpProg.dir/flags.make

CMakeFiles/TcpProg.dir/src/main.cpp.o: CMakeFiles/TcpProg.dir/flags.make
CMakeFiles/TcpProg.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/cppCode/ClientTCP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TcpProg.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TcpProg.dir/src/main.cpp.o -c /home/michael/cppCode/ClientTCP/src/main.cpp

CMakeFiles/TcpProg.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TcpProg.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/cppCode/ClientTCP/src/main.cpp > CMakeFiles/TcpProg.dir/src/main.cpp.i

CMakeFiles/TcpProg.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TcpProg.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/cppCode/ClientTCP/src/main.cpp -o CMakeFiles/TcpProg.dir/src/main.cpp.s

CMakeFiles/TcpProg.dir/src/TcpClient.cpp.o: CMakeFiles/TcpProg.dir/flags.make
CMakeFiles/TcpProg.dir/src/TcpClient.cpp.o: ../src/TcpClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/cppCode/ClientTCP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TcpProg.dir/src/TcpClient.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TcpProg.dir/src/TcpClient.cpp.o -c /home/michael/cppCode/ClientTCP/src/TcpClient.cpp

CMakeFiles/TcpProg.dir/src/TcpClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TcpProg.dir/src/TcpClient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/cppCode/ClientTCP/src/TcpClient.cpp > CMakeFiles/TcpProg.dir/src/TcpClient.cpp.i

CMakeFiles/TcpProg.dir/src/TcpClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TcpProg.dir/src/TcpClient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/cppCode/ClientTCP/src/TcpClient.cpp -o CMakeFiles/TcpProg.dir/src/TcpClient.cpp.s

CMakeFiles/TcpProg.dir/src/TcpServer.cpp.o: CMakeFiles/TcpProg.dir/flags.make
CMakeFiles/TcpProg.dir/src/TcpServer.cpp.o: ../src/TcpServer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/cppCode/ClientTCP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TcpProg.dir/src/TcpServer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TcpProg.dir/src/TcpServer.cpp.o -c /home/michael/cppCode/ClientTCP/src/TcpServer.cpp

CMakeFiles/TcpProg.dir/src/TcpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TcpProg.dir/src/TcpServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/cppCode/ClientTCP/src/TcpServer.cpp > CMakeFiles/TcpProg.dir/src/TcpServer.cpp.i

CMakeFiles/TcpProg.dir/src/TcpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TcpProg.dir/src/TcpServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/cppCode/ClientTCP/src/TcpServer.cpp -o CMakeFiles/TcpProg.dir/src/TcpServer.cpp.s

# Object files for target TcpProg
TcpProg_OBJECTS = \
"CMakeFiles/TcpProg.dir/src/main.cpp.o" \
"CMakeFiles/TcpProg.dir/src/TcpClient.cpp.o" \
"CMakeFiles/TcpProg.dir/src/TcpServer.cpp.o"

# External object files for target TcpProg
TcpProg_EXTERNAL_OBJECTS =

TcpProg: CMakeFiles/TcpProg.dir/src/main.cpp.o
TcpProg: CMakeFiles/TcpProg.dir/src/TcpClient.cpp.o
TcpProg: CMakeFiles/TcpProg.dir/src/TcpServer.cpp.o
TcpProg: CMakeFiles/TcpProg.dir/build.make
TcpProg: CMakeFiles/TcpProg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/michael/cppCode/ClientTCP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable TcpProg"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TcpProg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TcpProg.dir/build: TcpProg

.PHONY : CMakeFiles/TcpProg.dir/build

CMakeFiles/TcpProg.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TcpProg.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TcpProg.dir/clean

CMakeFiles/TcpProg.dir/depend:
	cd /home/michael/cppCode/ClientTCP/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michael/cppCode/ClientTCP /home/michael/cppCode/ClientTCP /home/michael/cppCode/ClientTCP/build /home/michael/cppCode/ClientTCP/build /home/michael/cppCode/ClientTCP/build/CMakeFiles/TcpProg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TcpProg.dir/depend
