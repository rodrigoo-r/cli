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

FetchContent_Declare(
        atoi
        GIT_REPOSITORY https://github.com/rodrigoo-r/atoi
        GIT_TAG        master
)

FetchContent_Declare(
        string_builder
        GIT_REPOSITORY https://github.com/rodrigoo-r/string_builder
        GIT_TAG        master
)

FetchContent_Declare(
        str_pad
        GIT_REPOSITORY https://github.com/rodrigoo-r/str_pad
        GIT_TAG        master
)

FetchContent_MakeAvailable(hashmap)
FetchContent_MakeAvailable(vector)
FetchContent_MakeAvailable(atoi)
FetchContent_MakeAvailable(string_builder)
FetchContent_MakeAvailable(str_pad)

cmake_minimum_required(VERSION 3.12)
project(cli C)

set(CMAKE_C_STANDARD 11)

add_library(cli STATIC cli.c
        value/value.h
        type/type.h
        app/app.h
        help/generator.h
        shared/str_cmp_fn.h)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/hashmap-src)
target_link_libraries(cli PRIVATE hashmap)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/vector-src)
target_link_libraries(cli PRIVATE vector)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/atoi-src)
target_link_libraries(cli PRIVATE atoi)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/stdbool-src)
target_link_libraries(cli PRIVATE string_builder)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/string_builder-src)
target_link_libraries(cli PRIVATE str_pad)
target_include_directories(cli PRIVATE ${CMAKE_BINARY_DIR}/_deps/str_pad-src)