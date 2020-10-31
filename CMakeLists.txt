cmake_minimum_required(VERSION 3.16)
project(Ray)

include_directories("./include")

#set(CMAKE_VERBOSE_MAKEFILE ON)

# - Setup Library
set(CMAKE_PREFIX_PATH "./library/glew-2.2.0")
set(CMAKE_LIBRARY_PATH "./library/glew-2.2.0/lib/Release/Win32/")
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
add_subdirectory("./library/glfw-3.3.2")

find_package(OpenGL REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#set(CMAKE_MODULE_PATH {CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
#message("${CMAKE_MODULE_PATH}")
#find_package(assimp REQUIRED)
#find_library(assimp NAMES assimp)
#message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
#if (assimp_FOUND)
#    message(STATUS "Found ASSIMP")
#    set(ASSIMP_LIBRARY "assimp")
#    add_library(${ASSIMP_LIBRARY} SHARED IMPORTED)
#    set_target_properties(${ASSIMP_LIBRARY} PROPERTIES IMPORTED_LOCATION "${ASSIMP_LIBRARY_DIRS}/libassimp.so")
#endif(assimp_FOUND)

#find_library(ASSIMP_LIBRARY assimp HINTS D:/Code/Ray/library/assimp_lib)
#message("${ASSIMP_LIBRARY}")
#add_subdirectory(library/assimp-5.0.1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_LIST)
set(HEADER_LIST)

list(APPEND SOURCE_LIST
        Utility.cpp
        Ray.cpp
        Sphere.cpp
        Hittable.cpp
        Camera.cpp
        Scene.cpp
        Config.cpp
        Triangle.cpp
        Material.cpp
        Light.cpp
        HittableList.cpp
        Shader.cpp
        IllumModelFactory.cpp
        Phong.cpp
        Whitted.cpp
        Dielectric.cpp
        BVH.cpp
        Photon.cpp
        AreaLight.cpp
        Lambertian.cpp
        )
list(APPEND HEADER_LIST
        include/Ray.h
        include/Hittable.h
        include/Sphere.h
        include/Camera.h
        include/Scene.h
        include/Config.h
        include/Triangle.h
        include/Material.h
        include/Light.h
        include/Hittable.h
        include/Shader.h
        include/IllumModel.h
        include/IllumModelFactory.h
        include/Phong.h
        include/Whitted.h
        include/Dielectric.h
        include/BVH.h
        include/OBJ_Loader.h
        include/Photon.h
        include/AreaLight.h
        include/Lambertian.h
        )

add_executable(Ray main.cpp ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(Ray ${CMAKE_SOURCE_DIR}/library/glew32.lib glfw OpenGL::GL Threads::Threads)