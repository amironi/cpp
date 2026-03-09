cmake_minimum_required(VERSION 3.7)
project(play)
set(CMAKE_CXX_STANDARD 17)
set(BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

file(GLOB SOURCES "src/*.cpp")
include_directories(".")
add_executable(main ${SOURCES})
