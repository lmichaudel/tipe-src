#ifndef MAIN_HPP
#define MAIN_HPP

#include "gfx/gfx.hpp"
#include "loader.hpp"
#include "rtree.hpp"

#include <iostream>
#include <chrono>
#include <vector>

#include <SDL2/SDL_image.h>

typedef std::chrono::high_resolution_clock Clock;

constexpr double MIN_LON = -125.0, MAX_LON = -66.0;
constexpr double MIN_LAT = 24.0, MAX_LAT = 50.0;

inline std::vector<std::pair<double, double>> readBinaryFile(const std::string& filename) {
  std::ifstream infile(filename, std::ios::binary);
  if (!infile) {
    std::cerr << "Error: Could not open input file." << std::endl;
    return {};
  }
  size_t size;
  infile.read(reinterpret_cast<char*>(&size), sizeof(size));
  std::vector<std::pair<double, double>> coordinates(size);
  infile.read(reinterpret_cast<char*>(coordinates.data()), size * sizeof(std::pair<double, double>));
  infile.close();
  return coordinates;
}

#endif // MAIN_HPP
