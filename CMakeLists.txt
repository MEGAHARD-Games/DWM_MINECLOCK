cmake_minimum_required(VERSION 4.2)
project(DWM_MINECLOCK)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# FetchContent downloads and configures dependencies
include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG "main"
        EXCLUDE_FROM_ALL
    )
FetchContent_MakeAvailable(SDL3)

# Copy assets
## Adapted from https://github.com/Moros1138/pge-template-project/blob/fb5c4bf23ececd2cc9eee880fd8a2e7fa05c2ece/CMakeLists.txt#L271-L287
set(ASSETS_DIR "assets")
set(SOURCE_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR})
set(ASSETS_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ASSETS_DIR})

file(GLOB_RECURSE src_assets_files
        RELATIVE ${SOURCE_ASSETS_DIR}/
        "${SOURCE_ASSETS_DIR}/*.*" "${SOURCE_ASSETS_DIR}/*")
foreach (fn ${src_assets_files})
    set(out_f ${ASSETS_OUTPUT_DIR}/${fn})
    add_custom_command(
            OUTPUT ${out_f}
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_ASSETS_DIR}/${fn} ${out_f}
            MAIN_DEPENDENCY ${SOURCE_ASSETS_DIR}/${fn}
    )
    list(APPEND out_assets_files ${out_f})
endforeach ()

add_custom_target(copy_assets DEPENDS ${out_assets_files})

add_executable(${CMAKE_PROJECT_NAME}
    WIN32
        main.cpp
        main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3)

add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)