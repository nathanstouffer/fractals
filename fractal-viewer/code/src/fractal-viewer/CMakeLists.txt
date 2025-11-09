set(FRACTAL_VIEWER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${FRACTAL_VIEWER_FILES})

add_executable(fractal-viewer ${FRACTAL_VIEWER_FILES})