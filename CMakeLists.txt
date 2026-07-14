cmake_minimum_required(VERSION 3.30)
project(system_monitor)

set(CMAKE_CXX_STANDARD 20)

add_executable(system_monitor WIN32
        main.cpp
        ui/ui.cpp
        ui/ui.h
        cpu/cpu_windows.cpp
        cpu/cpu.h
        ram/ram_windows.cpp
        ram/ram.h
        processes/processes_windows.cpp
        processes/processes.h
        cpu/cpu_linux.cpp
        ram/ram_linux.cpp
        processes/processes_linux.cpp
)

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(system_monitor PRIVATE
        raylib
        glfw
)