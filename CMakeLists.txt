cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
project(TIPE LANGUAGES CXX)

add_compile_options(-Wall -pedantic)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(src/)

add_executable(TIPE
        demo/gfx.cpp
        demo/gfx.hpp

        demo/main.hpp
        demo/main.cpp

        src/rect.hpp
        src/rect.cpp

        src/rtree.hpp
        src/rtree.cpp

        src/split_algorithms/split_algorithms.hpp
        src/split_algorithms/exp.cpp
)

target_link_libraries(TIPE ${SDL2_LIBRARIES})
