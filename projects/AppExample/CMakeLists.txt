cmake_minimum_required(VERSION 3.21)

project(AppExample C CXX)

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)
file(GLOB_RECURSE ASSETS ./assets/*)

if(WINDOWS)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
elseif(MACOS)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        RESOURCE True
        MACOSX_BUNDLE_GUI_IDENTIFIER ll.dashandslash.${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
elseif(LINUX)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
elseif(EMSCRIPTEN)
    # set(CMAKE_CXX_FLAGS " \
    # --bind \
    # -s TOTAL_MEMORY=67108864 \
    # -s USE_SDL=3 \
    # -s ASSERTIONS=1 \
    # -s DISABLE_EXCEPTION_CATCHING=0 \
    # -s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"writeArrayToMemory\"]' \
    # ")
    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")

    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	message(STATUS ">>>>>>> EMSCRIPTEN")
	add_compile_definitions("__EMSCRIPTEN__")
    set(WASM_BUILD_FLAGS "\
    -s SINGLE_FILE=1 \
    -s EXPORT_ES6=0 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME=createModule \
    -s NO_EXIT_RUNTIME=1 \
    -s ALLOW_MEMORY_GROWTH \
    -s ENVIRONMENT=web,worker \
    -s INVOKE_RUN=0 \
    -s ASSERTIONS=1 \
    -s NO_DISABLE_EXCEPTION_CATCHING=1 \
    -s STACK_SIZE=655360 \
    -s INITIAL_MEMORY=52428800 \
    -s EXPORTED_RUNTIME_METHODS=ccall,cwrap  \
    -s EXPORTED_FUNCTIONS=${WASM_EXPORTED_FUNCTIONS} \
    -s USE_PTHREADS \
    -O3")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${WASM_BUILD_FLAGS}")

	target_link_options(${PROJECT_NAME} PRIVATE
		# "-sMODULARIZE=1"
		# "-sEXPORT_NAME='Main'"
		# "-sINVOKE_RUN=0"
		# "-sDISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0"
		# "-sEXPORTED_FUNCTIONS=['_main']"
		# "-sEXPORTED_RUNTIME_METHODS=['callMain', 'ccall', 'cwrap']"
		"-sFORCE_FILESYSTEM=1"
		# "-sUSE_SDL=3"
		# "-sALLOW_MEMORY_GROWTH=1" 
		"-sASYNCIFY"
		# "-O2"
        "-sWASM=1"
        # "-sUSE_PTHREADS=1"
        "-sGL_ENABLE_GET_PROC_ADDRESS"
        "-sTOTAL_STACK=5242880"
	)

    if (EMSCRIPTEN)
        add_compile_options("-pthread")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    endif()

    # target_compile_options(${PROJECT_NAME} PRIVATE "SHELL:-s USE_PTHREADS")
    # target_link_options(${PROJECT_NAME} PRIVATE "SHELL:-s USE_PTHREADS")
    # set_target_properties(${PROJECT_NAME}
    #     PROPERTIES
    #         LINK_FLAGS "${EMSCRIPTEN_PTHREADS_LINKER_FLAGS}"
    # )
    # set_target_properties(${PROJECT_NAME}
	# 		PROPERTIES LINK_FLAGS
	# 				   "-s PRECISE_F32=1 -s TOTAL_MEMORY=268435456 -s ENVIRONMENT=web --memory-init-file 1 --emrun"
	# 				   SUFFIX ".html"
	# 	)
	# add_custom_command(
	# 	TARGET ${PROJECT_NAME} POST_BUILD
	# 	COMMAND ${CMAKE_COMMAND} -E copy
	# 			${CMAKE_SOURCE_DIR}/index.html
	# 			${CMAKE_BINARY_DIR}/index.html
	# )
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    blackboard::app
    blackboard::gfx
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${blackboard_app_SOURCE_DIR}
)

if(WINDOWS)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${bgfx_SOURCE_DIR}/bx/include/compat/msvc
)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    "-D_CRT_SECURE_NO_WARNINGS"
    "-D__STDC_FORMAT_MACROS"
    "-DSDL_MAIN_HANDLED"
)

# Copy assets folder in the Resources folder of binaray 

if(NOT EMSCRIPTEN)
    add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}_copy_assets")

    if(MACOS)
        add_custom_target("${PROJECT_NAME}_copy_assets" ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/assets"
    )
    else()
        add_custom_target("${PROJECT_NAME}_copy_assets" ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/assets
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources/assets"
    )
    endif()

    copy_shaderc_binary("$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif(NOT EMSCRIPTEN)

# Create tree group to tidy the files inside IDEs

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES} ${ASSETS})
