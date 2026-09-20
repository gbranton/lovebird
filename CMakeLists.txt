cmake_minimum_required(VERSION 4.4.3)

project(lovebird)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(src)

add_subdirectory(src)
