cmake_minimum_required(VERSION 3.16)
project(MyRaylibApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_executable(MyRaylibApp
    src/main.cpp
)

target_link_libraries(MyRaylibApp PRIVATE raylib)

target_include_directories(MyRaylibApp PRIVATE lib/src)