cmake_minimum_required(VERSION 3.10)
project(EffectiveCppItems13_14 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(items
    src/main.cpp
    src/item13.hpp
    src/item14.hpp
)

target_compile_options(items PRIVATE -Wall -Wextra)