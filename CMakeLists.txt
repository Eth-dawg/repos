cmake_minimum_required(VERSION 3.25)
project(CamelotPad VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CAMELOTPAD_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(camelotpad-core)
add_subdirectory(tools/injector)
