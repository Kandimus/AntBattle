cmake_minimum_required(VERSION 3.5)

option(TESTS_UNIT "Enable unit tests" ON)

set(PROJECT server)

project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
