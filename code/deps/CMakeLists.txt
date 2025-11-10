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