cmake_minimum_required(VERSION 3.10)

project(qt-demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(qt-demo 
    main.cpp
    CrashDialog.cpp
    CrashDialog.h
    CrashDetailsDialog.cpp
    CrashDetailsDialog.h
)

target_link_libraries(qt-demo Qt5::Widgets)
