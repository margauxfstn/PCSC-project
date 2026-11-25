cmake_minimum_required(VERSION 3.18)
project(PCSCProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# --- Eigen (header-only) ------------------------------------------------------
# Use an installed Eigen if there is one, otherwise download version 3.4.0
# as in the course. SOURCE_SUBDIR points to a folder that does not exist so
# that Eigen's own CMake project (tests, docs...) is not configured: only the
# headers are needed.
find_package(Eigen3 3.3 QUIET NO_MODULE)
if(NOT TARGET Eigen3::Eigen)
    include(FetchContent)
    FetchContent_Declare(eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG        3.4.0
        GIT_SHALLOW    TRUE
        SOURCE_SUBDIR  do-not-configure)
    FetchContent_MakeAvailable(eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    target_include_directories(Eigen3::Eigen SYSTEM INTERFACE ${eigen_SOURCE_DIR})
endif()

# --- Library: all the numerical code ------------------------------------------
add_library(rootfinding
    src/Function.cpp
    src/Result.cpp
    src/IterativeRootFinder.cpp
    src/Bisection.cpp
    src/AnalyticJacobian.cpp
    src/FiniteDifferenceJacobian.cpp
    src/FrozenJacobian.cpp
    src/ChordSlope.cpp
    src/Newton.cpp
    src/FixedPoint.cpp
    src/AitkenFixedPoint.cpp
    src/Config.cpp
    src/FunctionLibrary.cpp
    src/SolverFactory.cpp
    src/ResultWriter.cpp)
target_include_directories(rootfinding PUBLIC include)
target_link_libraries(rootfinding PUBLIC Eigen3::Eigen)
target_compile_options(rootfinding PRIVATE -Wall -Wextra -Wpedantic)

# --- Central executable --------------------------------------------------------
add_executable(solver src/main.cpp)
target_link_libraries(solver PRIVATE rootfinding)

# --- Tests ---------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)

# --- Documentation: "cmake --build build --target doc" (needs Doxygen) ---------
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating the Doxygen documentation in docs/html")
endif()
