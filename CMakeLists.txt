cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
project(TIPE LANGUAGES CXX)

add_compile_options(-Wall -pedantic)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS} /opt/homebrew/Cellar/sdl2_image/2.8.4/include)
include_directories(src/ gfx/)

add_library(gfx gfx/gfx.hpp gfx/gfx.cpp)
add_library(rtree
        src/rect.hpp
        src/rect.cpp
        src/rtree.hpp
        src/rtree.cpp
        src/split_algorithms/split_algorithms.hpp
        src/split_algorithms/exp.cpp)

add_executable(rtree-demo demo/main.hpp demo/main.cpp)
add_executable(app app/main.hpp app/main.cpp app/json.hpp
        app/loader.hpp)

target_link_libraries(gfx ${SDL2_LIBRARIES})
target_link_libraries(rtree-demo gfx rtree)
target_link_libraries(app gfx rtree /opt/homebrew/Cellar/sdl2_image/2.8.4/lib/libSDL2_image.dylib)

