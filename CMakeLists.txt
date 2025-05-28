include(FetchContent)

FetchContent_Declare(
        hashmap
        GIT_REPOSITORY https://github.com/rodrigoo-r/hashmap
        GIT_TAG        master
)

FetchContent_Declare(
        vector
        GIT_REPOSITORY https://github.com/rodrigoo-r/vector
        GIT_TAG        master
)

FetchContent_MakeAvailable(hashmap)
FetchContent_MakeAvailable(vector)

cmake_minimum_required(VERSION 3.30)
project(cli C)

set(CMAKE_C_STANDARD 11)

add_library(cli STATIC cli.c
        value/value.h
        type/type.h
        app/app.h)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/hashmap-src)
target_link_libraries(cli PRIVATE hashmap)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/vector-src)
target_link_libraries(cli PRIVATE vector)