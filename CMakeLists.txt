cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME blackboard_app)
set(CMAKE_CXX_STANDARD 20)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
elseif(UNIX AND APPLE AND NOT EMSCRIPTEN)
	set(MACOS TRUE)
elseif(WIN32)
	set(WINDOWS TRUE)
endif()

# set(CMAKE_CXX_FLAGS " \
# --bind \
# -s TOTAL_MEMORY=67108864 \
# -s USE_SDL=3 \
# -s ASSERTIONS=1 \
# -s DISABLE_EXCEPTION_CATCHING=0 \
# -s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"writeArrayToMemory\"]' \
# ")

if(WINDOWS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	project (${PROJECT_NAME} C CXX)
elseif(MACOS)
	project (${PROJECT_NAME} C CXX OBJC)
elseif(LINUX)
	project (${PROJECT_NAME} C CXX)
elseif(EMSCRIPTEN)
	project (${PROJECT_NAME} C CXX)
endif()

include(cmake/fetch_external_dependencies.cmake)
	
add_subdirectory(blackboard_app)
add_subdirectory(blackboard_gfx)
add_subdirectory(projects)
