cmake_minimum_required(VERSION 3.21)

include(FetchContent)

FetchContent_Declare(SDL3
    GIT_REPOSITORY "git@github.com:libsdl-org/SDL.git"
    GIT_TAG main
    GIT_SHALLOW 1
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/SDL
)

set(SDL_SHARED OFF CACHE BOOL "SDL_SHARED")
set(SDL_STATIC ON CACHE BOOL "SDL_STATIC")
FetchContent_MakeAvailable(SDL3)

if(LINUX)
    target_compile_definitions(SDL3-static PUBLIC
        SDL_VIDEO_DRIVER_X11=1
        NOMINMAX
    )
endif()