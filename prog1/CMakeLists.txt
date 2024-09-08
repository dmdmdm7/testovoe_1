cmake_minimum_required(VERSION 3.10)

project(Program1)

set(CMAKE_CXX_STANDARD 11)

add_executable(Program1 src/main.cpp src/engine.cpp src/buffer.cpp)

target_include_directories(Program1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)