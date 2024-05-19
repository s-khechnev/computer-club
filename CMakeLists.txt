cmake_minimum_required(VERSION 3.8)

project(computer-club LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
