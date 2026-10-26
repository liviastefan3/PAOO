cmake_minimum_required(VERSION 3.10)
project(TemaPOO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(tema_poo
    src/main.cpp
    src/DynamicArray.cpp
)