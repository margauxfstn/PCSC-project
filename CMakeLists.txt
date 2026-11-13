cmake_minimum_required(VERSION 3.10)
project(PCSCProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rootfinding src/RootFinder.cpp src/Bisection.cpp)
target_include_directories(rootfinding PUBLIC include)

add_executable(solver src/main.cpp)
target_link_libraries(solver PRIVATE rootfinding)

enable_testing()
add_subdirectory(tests)