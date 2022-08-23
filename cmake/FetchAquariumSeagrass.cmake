include(FetchContent)

FetchContent_Declare(
        aquarium-seagrass
        GIT_REPOSITORY https://github.com/pretore/aquarium-seagrass.git
        GIT_TAG v1.1.1
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(aquarium-seagrass)
