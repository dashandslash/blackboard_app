cmake_minimum_required(VERSION 3.21)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY "git@github.com:gabime/spdlog.git"
    GIT_TAG v1.10.0
    GIT_SHALLOW 1
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/spdlog
)
FetchContent_MakeAvailable(spdlog)