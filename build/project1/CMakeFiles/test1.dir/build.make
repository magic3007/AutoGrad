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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/sucan/Desktop/Compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sucan/Desktop/Compiler/build

# Include any dependencies generated for this target.
include project1/CMakeFiles/test1.dir/depend.make

# Include the progress variables for this target.
include project1/CMakeFiles/test1.dir/progress.make

# Include the compile flags for this target's objects.
include project1/CMakeFiles/test1.dir/flags.make

project1/CMakeFiles/test1.dir/run.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/run.cc.o: ../project1/run.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object project1/CMakeFiles/test1.dir/run.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/run.cc.o -c /home/sucan/Desktop/Compiler/project1/run.cc

project1/CMakeFiles/test1.dir/run.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/run.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/run.cc > CMakeFiles/test1.dir/run.cc.i

project1/CMakeFiles/test1.dir/run.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/run.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/run.cc -o CMakeFiles/test1.dir/run.cc.s

project1/CMakeFiles/test1.dir/run.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/run.cc.o.requires

project1/CMakeFiles/test1.dir/run.cc.o.provides: project1/CMakeFiles/test1.dir/run.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/run.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/run.cc.o.provides

project1/CMakeFiles/test1.dir/run.cc.o.provides.build: project1/CMakeFiles/test1.dir/run.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o: ../project1/kernels/kernel_case1.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_case1.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_case1.cc

project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_case1.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_case1.cc > CMakeFiles/test1.dir/kernels/kernel_case1.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_case1.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_case1.cc -o CMakeFiles/test1.dir/kernels/kernel_case1.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o: ../project1/kernels/kernel_case10.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_case10.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_case10.cc

project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_case10.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_case10.cc > CMakeFiles/test1.dir/kernels/kernel_case10.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_case10.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_case10.cc -o CMakeFiles/test1.dir/kernels/kernel_case10.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o: ../project1/kernels/kernel_case4.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_case4.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_case4.cc

project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_case4.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_case4.cc > CMakeFiles/test1.dir/kernels/kernel_case4.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_case4.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_case4.cc -o CMakeFiles/test1.dir/kernels/kernel_case4.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o: ../project1/kernels/kernel_case5.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_case5.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_case5.cc

project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_case5.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_case5.cc > CMakeFiles/test1.dir/kernels/kernel_case5.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_case5.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_case5.cc -o CMakeFiles/test1.dir/kernels/kernel_case5.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o: ../project1/kernels/kernel_case6.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_case6.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_case6.cc

project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_case6.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_case6.cc > CMakeFiles/test1.dir/kernels/kernel_case6.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_case6.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_case6.cc -o CMakeFiles/test1.dir/kernels/kernel_case6.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o: ../project1/kernels/kernel_case7.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_case7.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_case7.cc

project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_case7.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_case7.cc > CMakeFiles/test1.dir/kernels/kernel_case7.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_case7.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_case7.cc -o CMakeFiles/test1.dir/kernels/kernel_case7.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o


project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o: project1/CMakeFiles/test1.dir/flags.make
project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o: ../project1/kernels/kernel_example.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test1.dir/kernels/kernel_example.cc.o -c /home/sucan/Desktop/Compiler/project1/kernels/kernel_example.cc

project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1.dir/kernels/kernel_example.cc.i"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sucan/Desktop/Compiler/project1/kernels/kernel_example.cc > CMakeFiles/test1.dir/kernels/kernel_example.cc.i

project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1.dir/kernels/kernel_example.cc.s"
	cd /home/sucan/Desktop/Compiler/build/project1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sucan/Desktop/Compiler/project1/kernels/kernel_example.cc -o CMakeFiles/test1.dir/kernels/kernel_example.cc.s

project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.requires:

.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.requires

project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.provides: project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.requires
	$(MAKE) -f project1/CMakeFiles/test1.dir/build.make project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.provides.build
.PHONY : project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.provides

project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.provides.build: project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o


# Object files for target test1
test1_OBJECTS = \
"CMakeFiles/test1.dir/run.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_case1.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_case10.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_case4.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_case5.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_case6.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_case7.cc.o" \
"CMakeFiles/test1.dir/kernels/kernel_example.cc.o"

# External object files for target test1
test1_EXTERNAL_OBJECTS =

project1/test1: project1/CMakeFiles/test1.dir/run.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o
project1/test1: project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o
project1/test1: project1/CMakeFiles/test1.dir/build.make
project1/test1: project1/CMakeFiles/test1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sucan/Desktop/Compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable test1"
	cd /home/sucan/Desktop/Compiler/build/project1 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
project1/CMakeFiles/test1.dir/build: project1/test1

.PHONY : project1/CMakeFiles/test1.dir/build

project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/run.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_case1.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_case10.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_case4.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_case5.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_case6.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_case7.cc.o.requires
project1/CMakeFiles/test1.dir/requires: project1/CMakeFiles/test1.dir/kernels/kernel_example.cc.o.requires

.PHONY : project1/CMakeFiles/test1.dir/requires

project1/CMakeFiles/test1.dir/clean:
	cd /home/sucan/Desktop/Compiler/build/project1 && $(CMAKE_COMMAND) -P CMakeFiles/test1.dir/cmake_clean.cmake
.PHONY : project1/CMakeFiles/test1.dir/clean

project1/CMakeFiles/test1.dir/depend:
	cd /home/sucan/Desktop/Compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sucan/Desktop/Compiler /home/sucan/Desktop/Compiler/project1 /home/sucan/Desktop/Compiler/build /home/sucan/Desktop/Compiler/build/project1 /home/sucan/Desktop/Compiler/build/project1/CMakeFiles/test1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : project1/CMakeFiles/test1.dir/depend

