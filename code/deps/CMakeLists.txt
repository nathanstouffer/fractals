include(FetchContent)

# declare and populate CLI11 library
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.6.1
)
FetchContent_MakeAvailable(CLI11)

# declare and populate stb library
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        f1c79c02822848a9bed4315b12c8c8f3761e1296
)
FetchContent_MakeAvailable(stb)

# stb doesn't define a target so we add our own
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# declare and populate stf library
FetchContent_Declare(
    stf
    GIT_REPOSITORY https://github.com/nathanstouffer/stf.git
    GIT_TAG fe5b51b948c7637d5cd8eeee92c0977df79524bd
)
FetchContent_MakeAvailable(stf)

# declare and populate glfw library
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        162896e5b9a40dc382c5c438cd12c90a5ff86ddd
)
FetchContent_MakeAvailable(glfw)

# declare and populate glm library
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        a583c59e1616a628b18195869767ea4d6faca5f4  
)
FetchContent_MakeAvailable(glm)

# csci441 doesn't define a target so we add our own
add_library(csci441 INTERFACE)
target_include_directories(csci441 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/csci441/include")
target_compile_definitions(csci441 INTERFACE M_PI=3.1415926535)