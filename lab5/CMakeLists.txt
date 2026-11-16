cmake_minimum_required(VERSION 3.10)
project(EffectiveCppItems10_11_12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(items
    src/main.cpp
    src/item10.hpp
    src/item11.hpp
    src/item12.hpp
)

# Avertismente utile: te ajuta gcc sa gasesti greseli
target_compile_options(items PRIVATE -Wall -Wextra)