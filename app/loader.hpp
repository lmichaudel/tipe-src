#ifndef LOADER_HPP
#define LOADER_HPP

#include <fstream>
#include <iostream>
#include <sstream>

#include "json.hpp"

using json = nlohmann::json;

inline std::vector<std::pair<double, double>>
parse_geo_json(const std::string& filename) {
  std::vector<std::pair<double, double>> v;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open file." << std::endl;
    return v;
  }

  json geojson;
  file >> geojson;

  for (const auto& feature : geojson["features"]) {
    json coordinates = feature["geometry"]["coordinates"];

    double lon = coordinates[0].get<double>();
    double lat = coordinates[1].get<double>();
    v.push_back({lon, lat});
  }
  return v;
}

inline std::vector<std::pair<double, double>>
parse_csv(const std::string& filename) {
  std::vector<std::pair<double, double>> v;
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Error: Cannot open file." << std::endl;
    return v;
  }

  std::string line;
  bool headerSkipped = false;

  while (getline(file, line)) {
    if (!headerSkipped) { // Skip the first line if it contains headers
      headerSkipped = true;
      continue;
    }

    std::stringstream ss(line);
    std::string latStr, lonStr;

    if (getline(ss, latStr, ';') && getline(ss, lonStr, ';')) {
      try {
        double lat = stod(latStr);
        double lon = stod(lonStr);
        v.push_back(std::pair(lat, lon));
      } catch (const std::exception& e) {
        std::cerr << "Skipping invalid line: " << line << std::endl;
      }
    }
  }

  file.close();
  return v;
}
#endif // LOADER_HPP
