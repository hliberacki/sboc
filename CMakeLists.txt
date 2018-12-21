cmake_minimum_required(VERSION 3.1)

project(SBOC)
set(CMAKE_CXX_FLAGS "-std=c++2a -Xclang -fconcepts-ts")
add_executable(${PROJECT_NAME} "examples/main.cpp")
