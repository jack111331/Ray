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

# Include any dependencies generated for this target.
include CMakeFiles/Ray.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Ray.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Ray.dir/flags.make

CMakeFiles/Ray.dir/main.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/main.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Ray.dir/main.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\main.cpp.obj -c D:\Code\Ray\main.cpp

CMakeFiles/Ray.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/main.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\main.cpp > CMakeFiles\Ray.dir\main.cpp.i

CMakeFiles/Ray.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/main.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\main.cpp -o CMakeFiles\Ray.dir\main.cpp.s

CMakeFiles/Ray.dir/Utility.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Utility.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Utility.cpp.obj: ../Utility.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Ray.dir/Utility.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Utility.cpp.obj -c D:\Code\Ray\Utility.cpp

CMakeFiles/Ray.dir/Utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Utility.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Utility.cpp > CMakeFiles\Ray.dir\Utility.cpp.i

CMakeFiles/Ray.dir/Utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Utility.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Utility.cpp -o CMakeFiles\Ray.dir\Utility.cpp.s

CMakeFiles/Ray.dir/Ray.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Ray.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Ray.cpp.obj: ../Ray.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Ray.dir/Ray.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Ray.cpp.obj -c D:\Code\Ray\Ray.cpp

CMakeFiles/Ray.dir/Ray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Ray.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Ray.cpp > CMakeFiles\Ray.dir\Ray.cpp.i

CMakeFiles/Ray.dir/Ray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Ray.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Ray.cpp -o CMakeFiles\Ray.dir\Ray.cpp.s

CMakeFiles/Ray.dir/Sphere.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Sphere.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Sphere.cpp.obj: ../Sphere.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Ray.dir/Sphere.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Sphere.cpp.obj -c D:\Code\Ray\Sphere.cpp

CMakeFiles/Ray.dir/Sphere.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Sphere.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Sphere.cpp > CMakeFiles\Ray.dir\Sphere.cpp.i

CMakeFiles/Ray.dir/Sphere.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Sphere.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Sphere.cpp -o CMakeFiles\Ray.dir\Sphere.cpp.s

CMakeFiles/Ray.dir/Hittable.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Hittable.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Hittable.cpp.obj: ../Hittable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Ray.dir/Hittable.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Hittable.cpp.obj -c D:\Code\Ray\Hittable.cpp

CMakeFiles/Ray.dir/Hittable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Hittable.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Hittable.cpp > CMakeFiles\Ray.dir\Hittable.cpp.i

CMakeFiles/Ray.dir/Hittable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Hittable.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Hittable.cpp -o CMakeFiles\Ray.dir\Hittable.cpp.s

CMakeFiles/Ray.dir/Camera.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Camera.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Camera.cpp.obj: ../Camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Ray.dir/Camera.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Camera.cpp.obj -c D:\Code\Ray\Camera.cpp

CMakeFiles/Ray.dir/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Camera.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Camera.cpp > CMakeFiles\Ray.dir\Camera.cpp.i

CMakeFiles/Ray.dir/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Camera.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Camera.cpp -o CMakeFiles\Ray.dir\Camera.cpp.s

CMakeFiles/Ray.dir/Scene.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Scene.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Scene.cpp.obj: ../Scene.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Ray.dir/Scene.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Scene.cpp.obj -c D:\Code\Ray\Scene.cpp

CMakeFiles/Ray.dir/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Scene.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Scene.cpp > CMakeFiles\Ray.dir\Scene.cpp.i

CMakeFiles/Ray.dir/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Scene.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Scene.cpp -o CMakeFiles\Ray.dir\Scene.cpp.s

CMakeFiles/Ray.dir/Config.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Config.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Config.cpp.obj: ../Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Ray.dir/Config.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Config.cpp.obj -c D:\Code\Ray\Config.cpp

CMakeFiles/Ray.dir/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Config.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Config.cpp > CMakeFiles\Ray.dir\Config.cpp.i

CMakeFiles/Ray.dir/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Config.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Config.cpp -o CMakeFiles\Ray.dir\Config.cpp.s

CMakeFiles/Ray.dir/Triangle.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Triangle.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Triangle.cpp.obj: ../Triangle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Ray.dir/Triangle.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Triangle.cpp.obj -c D:\Code\Ray\Triangle.cpp

CMakeFiles/Ray.dir/Triangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Triangle.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Triangle.cpp > CMakeFiles\Ray.dir\Triangle.cpp.i

CMakeFiles/Ray.dir/Triangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Triangle.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Triangle.cpp -o CMakeFiles\Ray.dir\Triangle.cpp.s

CMakeFiles/Ray.dir/Material.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Material.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Material.cpp.obj: ../Material.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Ray.dir/Material.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Material.cpp.obj -c D:\Code\Ray\Material.cpp

CMakeFiles/Ray.dir/Material.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Material.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Material.cpp > CMakeFiles\Ray.dir\Material.cpp.i

CMakeFiles/Ray.dir/Material.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Material.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Material.cpp -o CMakeFiles\Ray.dir\Material.cpp.s

CMakeFiles/Ray.dir/Light.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Light.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Light.cpp.obj: ../Light.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Ray.dir/Light.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Light.cpp.obj -c D:\Code\Ray\Light.cpp

CMakeFiles/Ray.dir/Light.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Light.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Light.cpp > CMakeFiles\Ray.dir\Light.cpp.i

CMakeFiles/Ray.dir/Light.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Light.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Light.cpp -o CMakeFiles\Ray.dir\Light.cpp.s

CMakeFiles/Ray.dir/HittableList.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/HittableList.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/HittableList.cpp.obj: ../HittableList.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Ray.dir/HittableList.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\HittableList.cpp.obj -c D:\Code\Ray\HittableList.cpp

CMakeFiles/Ray.dir/HittableList.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/HittableList.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\HittableList.cpp > CMakeFiles\Ray.dir\HittableList.cpp.i

CMakeFiles/Ray.dir/HittableList.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/HittableList.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\HittableList.cpp -o CMakeFiles\Ray.dir\HittableList.cpp.s

CMakeFiles/Ray.dir/Shader.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Shader.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Shader.cpp.obj: ../Shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Ray.dir/Shader.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Shader.cpp.obj -c D:\Code\Ray\Shader.cpp

CMakeFiles/Ray.dir/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Shader.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Shader.cpp > CMakeFiles\Ray.dir\Shader.cpp.i

CMakeFiles/Ray.dir/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Shader.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Shader.cpp -o CMakeFiles\Ray.dir\Shader.cpp.s

CMakeFiles/Ray.dir/IllumModelFactory.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/IllumModelFactory.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/IllumModelFactory.cpp.obj: ../IllumModelFactory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/Ray.dir/IllumModelFactory.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\IllumModelFactory.cpp.obj -c D:\Code\Ray\IllumModelFactory.cpp

CMakeFiles/Ray.dir/IllumModelFactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/IllumModelFactory.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\IllumModelFactory.cpp > CMakeFiles\Ray.dir\IllumModelFactory.cpp.i

CMakeFiles/Ray.dir/IllumModelFactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/IllumModelFactory.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\IllumModelFactory.cpp -o CMakeFiles\Ray.dir\IllumModelFactory.cpp.s

CMakeFiles/Ray.dir/Whitted.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Whitted.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Whitted.cpp.obj: ../Whitted.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/Ray.dir/Whitted.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Whitted.cpp.obj -c D:\Code\Ray\Whitted.cpp

CMakeFiles/Ray.dir/Whitted.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Whitted.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Whitted.cpp > CMakeFiles\Ray.dir\Whitted.cpp.i

CMakeFiles/Ray.dir/Whitted.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Whitted.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Whitted.cpp -o CMakeFiles\Ray.dir\Whitted.cpp.s

CMakeFiles/Ray.dir/Dielectric.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Dielectric.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Dielectric.cpp.obj: ../Dielectric.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/Ray.dir/Dielectric.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Dielectric.cpp.obj -c D:\Code\Ray\Dielectric.cpp

CMakeFiles/Ray.dir/Dielectric.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Dielectric.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Dielectric.cpp > CMakeFiles\Ray.dir\Dielectric.cpp.i

CMakeFiles/Ray.dir/Dielectric.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Dielectric.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Dielectric.cpp -o CMakeFiles\Ray.dir\Dielectric.cpp.s

CMakeFiles/Ray.dir/BVH.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/BVH.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/BVH.cpp.obj: ../BVH.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/Ray.dir/BVH.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\BVH.cpp.obj -c D:\Code\Ray\BVH.cpp

CMakeFiles/Ray.dir/BVH.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/BVH.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\BVH.cpp > CMakeFiles\Ray.dir\BVH.cpp.i

CMakeFiles/Ray.dir/BVH.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/BVH.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\BVH.cpp -o CMakeFiles\Ray.dir\BVH.cpp.s

CMakeFiles/Ray.dir/Photon.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Photon.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Photon.cpp.obj: ../Photon.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/Ray.dir/Photon.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Photon.cpp.obj -c D:\Code\Ray\Photon.cpp

CMakeFiles/Ray.dir/Photon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Photon.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Photon.cpp > CMakeFiles\Ray.dir\Photon.cpp.i

CMakeFiles/Ray.dir/Photon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Photon.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Photon.cpp -o CMakeFiles\Ray.dir\Photon.cpp.s

CMakeFiles/Ray.dir/AreaLight.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/AreaLight.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/AreaLight.cpp.obj: ../AreaLight.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object CMakeFiles/Ray.dir/AreaLight.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\AreaLight.cpp.obj -c D:\Code\Ray\AreaLight.cpp

CMakeFiles/Ray.dir/AreaLight.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/AreaLight.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\AreaLight.cpp > CMakeFiles\Ray.dir\AreaLight.cpp.i

CMakeFiles/Ray.dir/AreaLight.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/AreaLight.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\AreaLight.cpp -o CMakeFiles\Ray.dir\AreaLight.cpp.s

CMakeFiles/Ray.dir/Lambertian.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Lambertian.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Lambertian.cpp.obj: ../Lambertian.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building CXX object CMakeFiles/Ray.dir/Lambertian.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Lambertian.cpp.obj -c D:\Code\Ray\Lambertian.cpp

CMakeFiles/Ray.dir/Lambertian.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Lambertian.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Lambertian.cpp > CMakeFiles\Ray.dir\Lambertian.cpp.i

CMakeFiles/Ray.dir/Lambertian.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Lambertian.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Lambertian.cpp -o CMakeFiles\Ray.dir\Lambertian.cpp.s

CMakeFiles/Ray.dir/Texture.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Texture.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Texture.cpp.obj: ../Texture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Building CXX object CMakeFiles/Ray.dir/Texture.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Texture.cpp.obj -c D:\Code\Ray\Texture.cpp

CMakeFiles/Ray.dir/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Texture.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Texture.cpp > CMakeFiles\Ray.dir\Texture.cpp.i

CMakeFiles/Ray.dir/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Texture.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Texture.cpp -o CMakeFiles\Ray.dir\Texture.cpp.s

CMakeFiles/Ray.dir/InstantRadiosity.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/InstantRadiosity.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/InstantRadiosity.cpp.obj: ../InstantRadiosity.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_22) "Building CXX object CMakeFiles/Ray.dir/InstantRadiosity.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\InstantRadiosity.cpp.obj -c D:\Code\Ray\InstantRadiosity.cpp

CMakeFiles/Ray.dir/InstantRadiosity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/InstantRadiosity.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\InstantRadiosity.cpp > CMakeFiles\Ray.dir\InstantRadiosity.cpp.i

CMakeFiles/Ray.dir/InstantRadiosity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/InstantRadiosity.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\InstantRadiosity.cpp -o CMakeFiles\Ray.dir\InstantRadiosity.cpp.s

CMakeFiles/Ray.dir/KDTree.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/KDTree.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/KDTree.cpp.obj: ../KDTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_23) "Building CXX object CMakeFiles/Ray.dir/KDTree.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\KDTree.cpp.obj -c D:\Code\Ray\KDTree.cpp

CMakeFiles/Ray.dir/KDTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/KDTree.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\KDTree.cpp > CMakeFiles\Ray.dir\KDTree.cpp.i

CMakeFiles/Ray.dir/KDTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/KDTree.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\KDTree.cpp -o CMakeFiles\Ray.dir\KDTree.cpp.s

CMakeFiles/Ray.dir/Pipeline.cpp.obj: CMakeFiles/Ray.dir/flags.make
CMakeFiles/Ray.dir/Pipeline.cpp.obj: CMakeFiles/Ray.dir/includes_CXX.rsp
CMakeFiles/Ray.dir/Pipeline.cpp.obj: ../Pipeline.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_24) "Building CXX object CMakeFiles/Ray.dir/Pipeline.cpp.obj"
	D:\MinGW\bin\c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Ray.dir\Pipeline.cpp.obj -c D:\Code\Ray\Pipeline.cpp

CMakeFiles/Ray.dir/Pipeline.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ray.dir/Pipeline.cpp.i"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\Ray\Pipeline.cpp > CMakeFiles\Ray.dir\Pipeline.cpp.i

CMakeFiles/Ray.dir/Pipeline.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ray.dir/Pipeline.cpp.s"
	D:\MinGW\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\Ray\Pipeline.cpp -o CMakeFiles\Ray.dir\Pipeline.cpp.s

# Object files for target Ray
Ray_OBJECTS = \
"CMakeFiles/Ray.dir/main.cpp.obj" \
"CMakeFiles/Ray.dir/Utility.cpp.obj" \
"CMakeFiles/Ray.dir/Ray.cpp.obj" \
"CMakeFiles/Ray.dir/Sphere.cpp.obj" \
"CMakeFiles/Ray.dir/Hittable.cpp.obj" \
"CMakeFiles/Ray.dir/Camera.cpp.obj" \
"CMakeFiles/Ray.dir/Scene.cpp.obj" \
"CMakeFiles/Ray.dir/Config.cpp.obj" \
"CMakeFiles/Ray.dir/Triangle.cpp.obj" \
"CMakeFiles/Ray.dir/Material.cpp.obj" \
"CMakeFiles/Ray.dir/Light.cpp.obj" \
"CMakeFiles/Ray.dir/HittableList.cpp.obj" \
"CMakeFiles/Ray.dir/Shader.cpp.obj" \
"CMakeFiles/Ray.dir/IllumModelFactory.cpp.obj" \
"CMakeFiles/Ray.dir/Whitted.cpp.obj" \
"CMakeFiles/Ray.dir/Dielectric.cpp.obj" \
"CMakeFiles/Ray.dir/BVH.cpp.obj" \
"CMakeFiles/Ray.dir/Photon.cpp.obj" \
"CMakeFiles/Ray.dir/AreaLight.cpp.obj" \
"CMakeFiles/Ray.dir/Lambertian.cpp.obj" \
"CMakeFiles/Ray.dir/Texture.cpp.obj" \
"CMakeFiles/Ray.dir/InstantRadiosity.cpp.obj" \
"CMakeFiles/Ray.dir/KDTree.cpp.obj" \
"CMakeFiles/Ray.dir/Pipeline.cpp.obj"

# External object files for target Ray
Ray_EXTERNAL_OBJECTS =

Ray.exe: CMakeFiles/Ray.dir/main.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Utility.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Ray.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Sphere.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Hittable.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Camera.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Scene.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Config.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Triangle.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Material.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Light.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/HittableList.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Shader.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/IllumModelFactory.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Whitted.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Dielectric.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/BVH.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Photon.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/AreaLight.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Lambertian.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Texture.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/InstantRadiosity.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/KDTree.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/Pipeline.cpp.obj
Ray.exe: CMakeFiles/Ray.dir/build.make
Ray.exe: ../library/glew-2.2.0/lib/Release/Win32/glew32.lib
Ray.exe: ../library/glew32.lib
Ray.exe: library/glfw-3.3.2/src/libglfw3dll.a
Ray.exe: ../library/glew-2.2.0/lib/Release/Win32/glew32.lib
Ray.exe: CMakeFiles/Ray.dir/linklibs.rsp
Ray.exe: CMakeFiles/Ray.dir/objects1.rsp
Ray.exe: CMakeFiles/Ray.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Code\Ray\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_25) "Linking CXX executable Ray.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Ray.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Ray.dir/build: Ray.exe

.PHONY : CMakeFiles/Ray.dir/build

CMakeFiles/Ray.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Ray.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Ray.dir/clean

CMakeFiles/Ray.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Code\Ray D:\Code\Ray D:\Code\Ray\cmake-build-debug D:\Code\Ray\cmake-build-debug D:\Code\Ray\cmake-build-debug\CMakeFiles\Ray.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Ray.dir/depend

