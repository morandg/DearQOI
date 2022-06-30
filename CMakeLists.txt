cmake_minimum_required(VERSION 3.18)

project(DearQOI
        LANGUAGES C CXX
        VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(DearIMGUI_SOURCES
        libs/imgui/imgui.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_demo.cpp
        libs/imgui/backends/imgui_impl_sdl.cpp
        libs/imgui/backends/imgui_impl_opengl2.cpp)

set(SOURCES
        src/QoiView.cpp
        src/MainWindow.cpp
        src/main.cpp)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
        libs/imgui/
        libs/spdlog/include
        libs/qoi
        ${SDL2_INCLUDE_DIRS})

add_executable(
        ${PROJECT_NAME}
        ${SOURCES}
        ${DearIMGUI_SOURCES})

target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        OpenGL::OpenGL)