cmake_minimum_required(VERSION 3.21)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "git@github.com:g-truc/glm.git"
    GIT_TAG 0.9.8
    GIT_SHALLOW 1
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/glm
)
FetchContent_MakeAvailable(glm)