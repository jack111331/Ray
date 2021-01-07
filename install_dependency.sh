git submodule update
mkdir -p dependency
cd public
# It may need you with system priviledge
# Note that I temporary use codeblocks MinGW generator

# Buil YAML-cpp and install it
cd yaml-cpp
mkdir -p build
cd build
# Generate building YAML-cpp necessary files and build it as dynamic shared library
cmake -G "CodeBlocks - MinGW Makefiles" -DYAML_BUILD_SHARED_LIBS=ON -DCMAKE_CXX_STANDARD=11 ../
cmake --build .
cmake --install . --prefix ../../../dependency
cd ..
rm -rf build


# Switch to previous directory
cd ..

# TODO Consider switch GLEW to GLAD...

# Buil GLFW and install it
cd glfw
mkdir -p build
cd build
# Generate building GLFW necessary files and build it as dynamic shared library
cmake -G "CodeBlocks - MinGW Makefiles" -DBUILD_SHARED_LIBS=ON ../
cmake --build .
cmake --install . --prefix ../../../dependency
cd ..
rm -rf build