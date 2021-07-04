mkdir dependency
cd public
REM It may need you with system priviledge
REM Note that I temporary use codeblocks MinGW generator

REM Buil YAML-cpp and install it
REM FIXME
cd yaml-cpp
mkdir build
cd build
REM Generate building YAML-cpp necessary files and build it as dynamic shared library
cmake -DYAML_BUILD_SHARED_LIBS=ON ../
cmake --build . --config Release
cmake --install . --prefix ../../../dependency
cd ..
rm -rf build


REM Switch to previous directory
cd ..

REM TODO Consider switch GLEW to GLAD...



REM Buil GLFW and install it
cd glfw
mkdir build
cd build
REM Generate building GLFW necessary files and build it as dynamic shared library
cmake -DBUILD_SHARED_LIBS=ON ../
cmake --build . --config Release
cmake --install . --prefix ../../../dependency
rm -rf build

cd ../../../