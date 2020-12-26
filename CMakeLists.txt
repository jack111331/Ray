cmake_minimum_required(VERSION 3.1.3)
project(Ray)

#set(CMAKE_VERBOSE_MAKEFILE ON)

set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/public/glew/include")
set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/public/glew/bin/Release/Win32")
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("Found GLEW")
endif()

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
add_subdirectory("./library/glfw-3.3.2")


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

add_subdirectory(public/yaml-cpp)
include_directories("public/yaml-cpp/include")

include_directories("./include")


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
        Whitted.cpp
        Dielectric.cpp
        BVH.cpp
        Photon.cpp
        AreaLight.cpp
        Lambertian.cpp
        Texture.cpp
        InstantRadiosity.cpp
        KDTree.cpp
        Pipeline.cpp
        GLUtil.cpp
        TriangleGroup.cpp
        PipelineFactory.cpp
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
        include/Whitted.h
        include/Dielectric.h
        include/BVH.h
        include/OBJ_Loader.h
        include/Photon.h
        include/AreaLight.h
        include/Lambertian.h
        include/Texture.h
        include/InstantRadiosity.h
        include/KDTree.h
        include/Pipeline.h
        include/GLUtil.h
        include/TriangleGroup.h
        include/PipelineFactory.h
        include/stb_image.h
        )

add_executable(Ray main.cpp ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(Ray GLEW::GLEW glfw OpenGL::GL yaml-cpp)