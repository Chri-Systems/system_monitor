cmake_minimum_required(VERSION 3.30)
project(system_monitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

qt_standard_project_setup()

qt_add_executable(system_monitor
        main.cpp
        ui/main_window.ui
        ui/process_list_item.ui
        processes/processes.cpp
        global.h
)


target_link_libraries(system_monitor PRIVATE
        Qt6::Widgets
        Qt6::Network
)

set_target_properties(system_monitor PROPERTIES
        AUTOUIC_SEARCH_PATHS "ui"
)