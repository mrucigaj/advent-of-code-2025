cmake_minimum_required(VERSION 3.15)

project(day10_part2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Avoid building unnecessary HiGHS components and tests.
set(FAST_BUILD ON CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    highs
    GIT_REPOSITORY https://github.com/ERGO-Code/HiGHS.git
    GIT_TAG v1.15.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(highs)

add_executable(day10_part2 part2.cpp)
target_link_libraries(day10_part2 PRIVATE highs::highs)