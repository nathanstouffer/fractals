set(MANDELVIEW_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${MANDELVIEW_FILES})

add_executable(mandelview ${MANDELVIEW_FILES})