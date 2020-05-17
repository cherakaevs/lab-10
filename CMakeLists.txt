cmake_minimum_required(VERSION 3.5)

project(Cherakaev CXX)

set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/W4 /D_CRT_SECURE_NO_WARNINGS)
endif ()

add_subdirectory(libutils)
add_subdirectory(lab-6-sorting)
add_subdirectory(lab-8-data-structures)
add_subdirectory(lab-10-graph)
