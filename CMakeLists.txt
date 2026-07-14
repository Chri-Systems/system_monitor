cmake_minimum_required(VERSION 3.30)
project(system_monitor)

set(CMAKE_CXX_STANDARD 20)

add_executable(system_monitor
        main.cpp
        ui/ui.cpp
        ui/ui.h
)

if(WIN32)
    set_property(TARGET system_monitor PROPERTY WIN32_EXECUTABLE TRUE)
    target_sources(system_monitor PRIVATE
            cpu/cpu_windows.cpp
            cpu/cpu_windows.h
            ram/ram_windows.cpp
            ram/ram_windows.h
            processes/processes_windows.cpp
            processes/processes_windows.h
    )
else()
    target_sources(system_monitor PRIVATE
            cpu/cpu_linux.cpp
            cpu/cpu_linux.h
            ram/ram_linux.cpp
            ram/ram_linux.h
            processes/processes_linux.cpp
            processes/processes_linux.h
    )
endif()

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(system_monitor PRIVATE
        raylib
        glfw
)