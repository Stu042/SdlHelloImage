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


include_directories(SdlHelloImage ${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS})
include_directories(headers)

add_executable(SdlHelloImage src/main.c)

target_link_libraries(SdlHelloImage PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

add_custom_command(
        TARGET SdlHelloImage POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)