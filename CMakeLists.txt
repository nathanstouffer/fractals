cmake_minimum_required(VERSION 3.24)

project(fractals LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# set property so Visual Studio generates filters corresponding to folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(fractal-gen)
add_subdirectory(fractal-viewer)