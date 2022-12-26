cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME blackboard_app)
set(CMAKE_CXX_STANDARD 20)
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	project (${PROJECT_NAME} C CXX)
elseif(APPLE)
	project (${PROJECT_NAME} C CXX OBJC)
endif()

include(cmake/fetch_external_dependencies.cmake)
	
add_subdirectory(blackboard_app)
add_subdirectory(blackboard_gfx)
