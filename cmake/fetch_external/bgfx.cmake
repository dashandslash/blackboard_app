cmake_minimum_required(VERSION 3.21)

include(FetchContent)

FetchContent_Declare(
    bgfx
    GIT_REPOSITORY "git@github.com:bkaradzic/bgfx.cmake.git"
    GIT_TAG v1.122.8572-455
    GIT_SHALLOW 1
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/bgfx
)

set( BGFX_BUILD_TOOLS ON CACHE INTERNAL "")
set( BGFX_BUILD_TOOLS_SHADER ON CACHE INTERNAL "")
set( BGFX_BUILD_EXAMPLES  OFF CACHE INTERNAL "" )
set( BGFX_CUSTOM_TARGETS  OFF CACHE INTERNAL "" )
FetchContent_MakeAvailable(bgfx)