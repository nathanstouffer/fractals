set(FRACTAL_GEN_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${FRACTAL_GEN_FILES})

add_executable(fractal-gen ${FRACTAL_GEN_FILES})