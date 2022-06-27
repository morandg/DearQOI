cmake_minimum_required(VERSION 3.18)

project(DearQOI
        LANGUAGES C CXX
        VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(SOURCES
    src/main.cpp)

include_directories(libs/spdlog/include)

add_executable(${PROJECT_NAME} ${SOURCES})
