cmake_minimum_required(VERSION 3.31)
project(SdlHelloImage)

set(CMAKE_C_STANDARD 11)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2_image)
set(SDL2_SHARED OFF)
set(SDL2_IMAGE_SHARED OFF)

# this is not how you add sdl2_image include directory, it works (for debug) but it wrong
include_directories(${CMAKE_PROJECT_NAME} SDL2::SDL2 ${SDL2_image_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} src/main.c)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2main SDL2 SDL2_image)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets)

# workaround, copy dlls manually here
if (WIN32)
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SDL2_image_BINARY_DIR}/SDL2_imaged.dll
        ${CMAKE_CURRENT_BINARY_DIR}/SDL2_imaged.dll)
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/cmake-build-debug/_deps/sdl2-build/SDL2d.dll
        ${CMAKE_CURRENT_BINARY_DIR}/SDL2d.dll)
endif ()
