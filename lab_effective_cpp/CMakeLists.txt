cmake_minimum_required(VERSION 3.10)
project(PAOO_Effective_Cpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -Wall include -Wreorder: avertizeaza daca lista de initializare
# nu respecta ordinea declararii membrilor (Item 4)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(effective_cpp
    src/main.cpp
    src/item4_init.cpp
    src/item5_implicit.cpp
    src/item6_disallow.cpp
)

target_include_directories(effective_cpp PRIVATE include)