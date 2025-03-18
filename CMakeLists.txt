cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD 17)
project(TIPE LANGUAGES CXX)

add_compile_options(-Wall -pedantic -fsanitize=address)
add_link_options(-fsanitize=address)

### FIND SDL2
find_package(SDL2 REQUIRED)

### INCLUDE DIRECTORIES
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_PATH}/include rtree/ ext/)

### BUILD RTREE
add_library(rtree
        rtree/rect.hpp
        rtree/rect.cpp
        rtree/rtree.hpp
        rtree/rtree.cpp
        rtree/split_algorithms/growable_rect.hpp
        rtree/split_algorithms/split_algorithms.hpp
        rtree/split_algorithms/split_algorithms.cpp
)

### BUILD GFX
add_library(gfx ext/gfx/gfx.hpp ext/gfx/gfx.cpp)
target_link_libraries(gfx ${SDL2_LIBRARIES})

### BUILD TESTS
add_executable(test test/main.hpp test/main.cpp)
target_link_libraries(test gfx rtree)

### BUILD DATABASE_COMPILE
add_executable(db_compile app/db_compile.cpp)
### BUILD APP
add_executable(app app/main.hpp app/main.cpp
        app/benchmarks/benchmarks.cpp
        app/benchmarks/benchmarks.hpp)
target_link_libraries(app gfx rtree)
