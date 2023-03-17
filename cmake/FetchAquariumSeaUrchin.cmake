include(FetchContent)

FetchContent_Declare(
        aquarium-sea-urchin
        GIT_REPOSITORY https://github.com/pretore/aquarium-sea-urchin.git
        GIT_TAG v1.4.0
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(aquarium-sea-urchin)