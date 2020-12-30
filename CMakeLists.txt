cmake_minimum_required(VERSION 3.1.3)
project(Ray)

#set(CMAKE_VERBOSE_MAKEFILE ON)

set(GLEW_VERBOSE TRUE)

set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/public/glew/include")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "public/glew")
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("Found GLEW")
endif()

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
add_subdirectory("public/glfw")

include_directories("public/glm")

add_subdirectory(public/yaml-cpp)
include_directories("public/yaml-cpp/include")

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

include_directories("./include")

# TODO install dependency to dir

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_LIST)
set(HEADER_LIST)

list(APPEND SOURCE_LIST
        src/Utility.cpp
        src/Ray.cpp
        src/Sphere.cpp
        src/Hittable.cpp
        src/Camera.cpp
        src/Scene.cpp
        src/Config.cpp
        src/Triangle.cpp
        src/Material.cpp
        src/Light.cpp
        src/HittableList.cpp
        src/Shader.cpp
        src/IllumModelFactory.cpp
        src/Whitted.cpp
        src/Dielectric.cpp
        src/BVH.cpp
        src/Photon.cpp
        src/AreaLight.cpp
        src/Lambertian.cpp
        src/Texture.cpp
        src/InstantRadiosity.cpp
        src/KDTree.cpp
        src/Pipeline.cpp
        src/GLUtil.cpp
        src/TriangleGroup.cpp
        src/PipelineFactory.cpp
        )
list(APPEND HEADER_LIST
        include/AreaLight.h
        include/BVH.h
        include/Camera.h
        include/Config.h
        include/Dielectric.h
        include/GLUtil.h
        include/Hittable.h
        include/HittableList.h
        include/IllumModel.h
        include/IllumModelFactory.h
        include/InstantRadiosity.h
        include/KDTree.h
        include/Lambertian.h
        include/Light.h
        include/Material.h
        include/Photon.h
        include/Pipeline.h
        include/PipelineFactory.h
        include/Ray.h
        include/Scene.h
        include/Shader.h
        include/Sphere.h
        include/Texture.h
        include/Triangle.h
        include/TriangleGroup.h
        include/Whitted.h

        include/OBJ_Loader.h
        include/stb_image.h
        )

add_executable(Ray src/main.cpp ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(Ray GLEW::GLEW glfw OpenGL::GL yaml-cpp)