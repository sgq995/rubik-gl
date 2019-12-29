cmake_minimum_required(VERSION 3.10)

project(RubikGL VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

configure_file(include/config.h.in include/config.h)

add_executable(RubikGL src/main.cc)

target_include_directories(RubikGL PUBLIC
                           "${PROJECT_BINARY_DIR}/include"
                           )

target_link_libraries(RubikGL ${SDL2_LIBRARIES})
