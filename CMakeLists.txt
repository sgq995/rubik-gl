cmake_minimum_required(VERSION 3.10)

option(ARCHLINUX_BUILD "Defined if the project is building in archlinux" ON)

project(RubikGL VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

configure_file(include/config.h.in include/config.h)

file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cc")

add_executable(RubikGL ${SOURCES})

if (ARCHLINUX_BUILD)
  target_link_libraries(RubikGL PUBLIC OpenGL::GL)
  target_link_libraries(RubikGL PUBLIC GLEW::GLEW)
  target_link_libraries(RubikGL PUBLIC SDL2::SDL2)
else()
  include_directories(${SDL2_INCLUDE_DIRS})
  target_link_libraries(RubikGL PUBLIC ${SDL2_LIBRARIES})
endif()

target_include_directories(RubikGL PUBLIC "${PROJECT_BINARY_DIR}/include")
target_include_directories(RubikGL PUBLIC "${PROJECT_SOURCE_DIR}/include")
