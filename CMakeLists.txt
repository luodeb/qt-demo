cmake_minimum_required(VERSION 3.10)

project(Qt5SimpleDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(Qt5SimpleDemo main.cpp)

target_link_libraries(Qt5SimpleDemo Qt5::Widgets)
