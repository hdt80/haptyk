# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_SOURCE_DIR = /home/special-k/Documents/school/JP/SDKv2/gattlib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/special-k/Documents/school/JP/SDKv2/gattlib

# Include any dependencies generated for this target.
include examples/read_write/CMakeFiles/read_write.dir/depend.make

# Include the progress variables for this target.
include examples/read_write/CMakeFiles/read_write.dir/progress.make

# Include the compile flags for this target's objects.
include examples/read_write/CMakeFiles/read_write.dir/flags.make

examples/read_write/CMakeFiles/read_write.dir/read_write.c.o: examples/read_write/CMakeFiles/read_write.dir/flags.make
examples/read_write/CMakeFiles/read_write.dir/read_write.c.o: examples/read_write/read_write.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/special-k/Documents/school/JP/SDKv2/gattlib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/read_write/CMakeFiles/read_write.dir/read_write.c.o"
	cd /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/read_write.dir/read_write.c.o   -c /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write/read_write.c

examples/read_write/CMakeFiles/read_write.dir/read_write.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/read_write.dir/read_write.c.i"
	cd /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write/read_write.c > CMakeFiles/read_write.dir/read_write.c.i

examples/read_write/CMakeFiles/read_write.dir/read_write.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/read_write.dir/read_write.c.s"
	cd /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write/read_write.c -o CMakeFiles/read_write.dir/read_write.c.s

# Object files for target read_write
read_write_OBJECTS = \
"CMakeFiles/read_write.dir/read_write.c.o"

# External object files for target read_write
read_write_EXTERNAL_OBJECTS =

examples/read_write/read_write: examples/read_write/CMakeFiles/read_write.dir/read_write.c.o
examples/read_write/read_write: examples/read_write/CMakeFiles/read_write.dir/build.make
examples/read_write/read_write: dbus/libgattlib.so
examples/read_write/read_write: examples/read_write/CMakeFiles/read_write.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/special-k/Documents/school/JP/SDKv2/gattlib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable read_write"
	cd /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/read_write.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/read_write/CMakeFiles/read_write.dir/build: examples/read_write/read_write

.PHONY : examples/read_write/CMakeFiles/read_write.dir/build

examples/read_write/CMakeFiles/read_write.dir/clean:
	cd /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write && $(CMAKE_COMMAND) -P CMakeFiles/read_write.dir/cmake_clean.cmake
.PHONY : examples/read_write/CMakeFiles/read_write.dir/clean

examples/read_write/CMakeFiles/read_write.dir/depend:
	cd /home/special-k/Documents/school/JP/SDKv2/gattlib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/special-k/Documents/school/JP/SDKv2/gattlib /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write /home/special-k/Documents/school/JP/SDKv2/gattlib /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write /home/special-k/Documents/school/JP/SDKv2/gattlib/examples/read_write/CMakeFiles/read_write.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/read_write/CMakeFiles/read_write.dir/depend

