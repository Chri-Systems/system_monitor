cmake_minimum_required(VERSION 3.30)
project(system_monitor)

set(CMAKE_CXX_STANDARD 20)

add_executable(system_monitor main.cpp
        cpu/cpu.cpp
        cpu/cpu.h
        ram/ram.cpp
        ram/ram.h
        process/process.cpp
        process/process.h
)

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(system_monitor PRIVATE
        raylib
        glfw
)