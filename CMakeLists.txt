cmake_minimum_required(VERSION 3.18)

project(DearQOI
        LANGUAGES C CXX
        VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(SOURCES
    src/MainWindow.cpp
    src/main.cpp)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
        libs/spdlog/include
        ${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        OpenGL::OpenGL)