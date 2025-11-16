set(FRACTALVIEW_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${FRACTALVIEW_FILES})

add_executable(fractalview ${FRACTALVIEW_FILES})