set(GENERATOR_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/generator/fractals.hpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${GENERATOR_FILES})

add_executable(generator ${GENERATOR_FILES})

target_include_directories(generator
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
)