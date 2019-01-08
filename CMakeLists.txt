cmake_minimum_required(VERSION 3.1)

project(SBOC)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Xclang -fconcepts-ts")

add_executable(Serializer "examples/main.cpp")
target_include_directories(Serializer PUBLIC ${PROJECT_SOURCE_DIR}/include
                                      PUBLIC ${PROJECT_SOURCE_DIR}/include/experimental)
