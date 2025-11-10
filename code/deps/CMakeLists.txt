include(FetchContent)

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
    GIT_TAG f6aa6c33c4e3ba54f70a1270c7cb19bc4ea34ed8
)
FetchContent_MakeAvailable(stf)