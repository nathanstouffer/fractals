set(MANDELVIEW_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp"
)

# add directory structure to IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${MANDELVIEW_FILES})

file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets" SYMBOLIC)

add_executable(mandelview ${MANDELVIEW_FILES} "${DEPS_DIR}/glad/src/glad.c")

target_include_directories(mandelview
    PRIVATE
    "${DEPS_DIR}/glad/include"
)

target_link_libraries(mandelview
    stf
    csci441
    glm
    glfw
)