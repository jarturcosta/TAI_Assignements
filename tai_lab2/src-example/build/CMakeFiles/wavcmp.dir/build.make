# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build

# Include any dependencies generated for this target.
include CMakeFiles/wavcmp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wavcmp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wavcmp.dir/flags.make

CMakeFiles/wavcmp.dir/wavcmp.cpp.o: CMakeFiles/wavcmp.dir/flags.make
CMakeFiles/wavcmp.dir/wavcmp.cpp.o: ../wavcmp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wavcmp.dir/wavcmp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wavcmp.dir/wavcmp.cpp.o -c /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/wavcmp.cpp

CMakeFiles/wavcmp.dir/wavcmp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wavcmp.dir/wavcmp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/wavcmp.cpp > CMakeFiles/wavcmp.dir/wavcmp.cpp.i

CMakeFiles/wavcmp.dir/wavcmp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wavcmp.dir/wavcmp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/wavcmp.cpp -o CMakeFiles/wavcmp.dir/wavcmp.cpp.s

# Object files for target wavcmp
wavcmp_OBJECTS = \
"CMakeFiles/wavcmp.dir/wavcmp.cpp.o"

# External object files for target wavcmp
wavcmp_EXTERNAL_OBJECTS =

/home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/bin-example/wavcmp: CMakeFiles/wavcmp.dir/wavcmp.cpp.o
/home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/bin-example/wavcmp: CMakeFiles/wavcmp.dir/build.make
/home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/bin-example/wavcmp: CMakeFiles/wavcmp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/bin-example/wavcmp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wavcmp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wavcmp.dir/build: /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/bin-example/wavcmp

.PHONY : CMakeFiles/wavcmp.dir/build

CMakeFiles/wavcmp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wavcmp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wavcmp.dir/clean

CMakeFiles/wavcmp.dir/depend:
	cd /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build /home/jarturcosta/Documents/Mestrado/TAI/tai_a1/tai_lab2/src-example/build/CMakeFiles/wavcmp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wavcmp.dir/depend

