# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files\JetBrains\CLion 2019.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files\JetBrains\CLion 2019.2.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Code\Ray

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Code\Ray\cmake-build-debug

# Utility rule file for uninstall.

# Include the progress variables for this target.
include library/glfw-3.3.2/CMakeFiles/uninstall.dir/progress.make

library/glfw-3.3.2/CMakeFiles/uninstall:
	cd /d D:\Code\Ray\cmake-build-debug\library\glfw-3.3.2 && "D:\Program Files\JetBrains\CLion 2019.2.4\bin\cmake\win\bin\cmake.exe" -P D:/Code/Ray/cmake-build-debug/library/glfw-3.3.2/cmake_uninstall.cmake

uninstall: library/glfw-3.3.2/CMakeFiles/uninstall
uninstall: library/glfw-3.3.2/CMakeFiles/uninstall.dir/build.make

.PHONY : uninstall

# Rule to build all files generated by this target.
library/glfw-3.3.2/CMakeFiles/uninstall.dir/build: uninstall

.PHONY : library/glfw-3.3.2/CMakeFiles/uninstall.dir/build

library/glfw-3.3.2/CMakeFiles/uninstall.dir/clean:
	cd /d D:\Code\Ray\cmake-build-debug\library\glfw-3.3.2 && $(CMAKE_COMMAND) -P CMakeFiles\uninstall.dir\cmake_clean.cmake
.PHONY : library/glfw-3.3.2/CMakeFiles/uninstall.dir/clean

library/glfw-3.3.2/CMakeFiles/uninstall.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Code\Ray D:\Code\Ray\library\glfw-3.3.2 D:\Code\Ray\cmake-build-debug D:\Code\Ray\cmake-build-debug\library\glfw-3.3.2 D:\Code\Ray\cmake-build-debug\library\glfw-3.3.2\CMakeFiles\uninstall.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : library/glfw-3.3.2/CMakeFiles/uninstall.dir/depend

