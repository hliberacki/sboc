cmake_minimum_required(VERSION 3.1)

project(SBOC)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Xclang -fconcepts-ts")

add_executable(Serializer "examples/main.cpp")
target_include_directories(Serializer PRIVATE ${PROJECT_SOURCE_DIR}/include
                                      PRIVATE ${PROJECT_SOURCE_DIR}/include/experimental)

add_executable(Errors "examples/errors.cpp")
target_include_directories(Errors PRIVATE ${PROJECT_SOURCE_DIR}/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/include/experimental)

add_executable(Basics "examples/basic.cpp")
target_include_directories(Basics PRIVATE ${PROJECT_SOURCE_DIR}/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/include/experimental)
