cmake_minimum_required(VERSION 3.10)

option(ARCHLINUX_BUILD "Defined if the project is building in archlinux" ON)

project(RubikGL VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)

configure_file(include/config.h.in include/config.h)

add_executable(RubikGL src/main.cc)

if (ARCHLINUX_BUILD)
  target_link_libraries(RubikGL PUBLIC SDL2::SDL2)
else()
  include_directories(${SDL2_INCLUDE_DIRS})
  target_link_libraries(RubikGL PUBLIC ${SDL2_LIBRARIES})
endif()

target_include_directories(RubikGL PUBLIC "${PROJECT_BINARY_DIR}/include")
