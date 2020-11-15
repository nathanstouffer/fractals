cmake_minimum_required(VERSION 3.0)
project(fractals VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEPS_DIR "../deps")

set(CSCI441_DIR ${DEPS_DIR}/csci441)
include_directories(${CSCI441_DIR}/include)

set(BITMAP_DIR ${DEPS_DIR}/bitmap)
include_directories(${BITMAP_DIR}/include)

set(PROJ ${PROJECT_NAME})
add_executable(${PROJ} main.cpp)
target_link_libraries(${PROJ})
