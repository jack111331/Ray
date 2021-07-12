cmake_minimum_required(VERSION 3.1.3)
project(Ray)
set(CMAKE_CXX_STANDARD 11)

# Toggle GUI Support
set(GUI_SUPPORT FALSE)

#set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(OpenGL REQUIRED)

#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
#add_subdirectory("public/glfw")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/dependency")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/dependency/share/cmake/yaml-cpp")
find_package(glfw3 REQUIRED)



include_directories("public/glm")

add_subdirectory(public/yaml-cpp)
include_directories("public/yaml-cpp/include")
#find_package(yaml-cpp REQUIRED)

SET(wxWidgets_ROOT_DIR "${CMAKE_SOURCE_DIR}/public/wxWidgets/")
SET(wxWidgets_LIB_DIR "${CMAKE_SOURCE_DIR}/dependency/lib/vc_dll")
SET(wxWidgets_LIBRARIES "${CMAKE_SOURCE_DIR}/public/wxWidgets/lib")
SET(wxWidgets_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/public/wxWidgets/include/wx")
find_package(wxWidgets REQUIRED COMPONENTS net core base gl)
include(${wxWidgets_USE_FILE})

#set(IMGUI_SOURCE_LIST)
#include_directories("public/imgui")
#include_directories("public/imgui/backends")
#add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)
##include_directories("public/imgui/example/libs/gl3w")
#list(APPEND IMGUI_SOURCE_LIST
#        public/imgui/backends/imgui_impl_glfw.cpp
#        public/imgui/backends/imgui_impl_opengl3.cpp
#        public/imgui/imgui.cpp
#        public/imgui/imgui_draw.cpp
#        public/imgui/imgui_tables.cpp
#        public/imgui/imgui_widgets.cpp
#        )

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
include_directories("./include/TriMesh")
include_directories("./include/glad")


if (GUI_SUPPORT)
    add_compile_definitions(GUI_SUPPORT)
endif()

set(SOURCE_LIST)
set(HEADER_LIST)

list(APPEND SOURCE_LIST
        src/AmbientOcclusion.cpp
        src/AmbientOcclusionVolumeShadingPipeline.cpp
        src/AreaLight.cpp
        src/BLASNode.cpp
        src/BVH.cpp
        src/Camera.cpp
        src/Config.cpp
        src/DefferedShadingPipeline.cpp
        src/Dielectric.cpp
        src/GLUtil.cpp
        src/GeometryGroupObj.cpp
        src/GroupBVHTranslator.cpp
        src/GroupObj.cpp
        src/IllumModelFactory.cpp
        src/InstantRadiosity.cpp
        src/KDTree.cpp
        src/Lambertian.cpp
        src/Light.cpp
        src/Material.cpp
        src/ONB.cpp
        src/PathTrace.cpp
        src/Photon.cpp
        src/PhongShadingPipeline.cpp
        src/Pipeline.cpp
        src/PipelineFactory.cpp
        src/Ray.cpp
        src/Scene.cpp
        src/SelectorObj.cpp
        src/Shader.cpp
        src/ShaderProgram.cpp
        src/Sphere.cpp
        src/SSAODefferedShadingPipeline.cpp
        src/SuggestedContourPipeline.cpp
        src/Texture.cpp
        src/TransformObj.cpp
        src/Triangle.cpp
        src/TriangleGroup.cpp
        src/Utility.cpp
        src/Whitted.cpp
        src/Timer.cpp
        src/Context.cpp

        src/GLPanel.cpp
        src/MainFrame.cpp

        src/glad.c
#        ${IMGUI_SOURCE_LIST}
        )
list(APPEND HEADER_LIST
        include/AmbientOcclusion.h
        include/AmbientOcclusionVolumeShadingPipeline.h
        include/AreaLight.h
        include/BLASNode.h
        include/BVH.h
        include/Camera.h
        include/Config.h
        include/DefferedShadingPipeline.h
        include/Dielectric.h
        include/GLUtil.h
        include/GroupBVHTranslator.h
        include/GroupObj.h
        include/ObjectNode.h
        include/GeometryGroupObj.h
        include/IllumModel.h
        include/IllumModelFactory.h
        include/InstantRadiosity.h
        include/KDTree.h
        include/Lambertian.h
        include/Light.h
        include/Material.h
        include/ONB.h
        include/PathTrace.h
        include/PhongShadingPipeline.h
        include/Photon.h
        include/Pipeline.h
        include/PipelineFactory.h
        include/Ray.h
        include/Scene.h
        include/SelectorObj.h
        include/Shader.h
        include/ShaderProgram.h
        include/ShaderInclude.h
        include/Sphere.h
        include/SSAODefferedShadingPipeline.h
        include/SuggestedContourPipeline.h
        include/Texture.h
        include/TLASNode.h
        include/TransformObj.h
        include/Triangle.h
        include/TriangleGroup.h
        include/Whitted.h
        include/Timer.h
        include/Context.h

        include/Application.h
        include/MainFrame.h
        include/GLPanel.h

        include/OBJ_Loader.h
        include/stb_image.h
        )


#link_directories("${CMAKE_SOURCE_DIR}/lib")
# Add WIN32 to indicate cmake to build the windows GUI application
add_executable(Ray WIN32 src/main.cpp ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(Ray glfw OpenGL::GL yaml-cpp ${wxWidgets_LIBRARIES})