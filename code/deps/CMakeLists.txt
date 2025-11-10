include(FetchContent)

# declare and populate stf library
FetchContent_Declare(
    stf
    GIT_REPOSITORY https://github.com/nathanstouffer/stf.git
    GIT_TAG f6aa6c33c4e3ba54f70a1270c7cb19bc4ea34ed8
)
FetchContent_MakeAvailable(stf)