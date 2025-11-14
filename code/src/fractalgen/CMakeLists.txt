set(FRACTALGEN_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/fractalgen/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/fractalgen/generators/factory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/fractalgen/generators/generators.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/fractalgen/generators/factory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private/fractalgen/generators/generators.hpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${FRACTALGEN_FILES})

add_executable(fractalgen ${FRACTALGEN_FILES})

target_include_directories(fractalgen
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
)

# add small dependencies
target_link_libraries(fractalgen
    PRIVATE
    CLI11::CLI11
    stb
    stf
)