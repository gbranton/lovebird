cmake_minimum_required(VERSION 4.4.3)

project(lovebird)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

# Allow designated initializers for base classes (see P2287)
add_compile_options(
    $<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>:-Wno-c99-designator>
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

include_directories(src)

add_subdirectory(src)
