cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
project(TIPE)

#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#    set(ASAN_FLAGS "-fsanitize=address -g -O1 -fno-omit-frame-pointer")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
#    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${ASAN_FLAGS}")
#endif ()

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

        src/split_algorithms/split_algorithm.hpp
        src/split_algorithms/exp.cpp
)

target_link_libraries(TIPE ${SDL2_LIBRARIES})
