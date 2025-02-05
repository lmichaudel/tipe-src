#ifndef LOADER_HPP
#define LOADER_HPP

#include <fstream>
#include <iostream>
#include <sstream>

#include "json.hpp"

using json = nlohmann::json;

inline std::vector<std::pair<double, double>>
parse_geo_json(const std::string& filename, int sample_size) {
  std::vector<std::pair<double, double>> v;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open file." << std::endl;
    return v;
  }

  json geojson;
  file >> geojson;

  int i = 0;
  for (const auto& feature : geojson["features"]) {
    i++;
    if (i > sample_size)
      break;

    json coordinates = feature["geometry"]["coordinates"];

    double lon = coordinates[0].get<double>();
    double lat = coordinates[1].get<double>();
    v.push_back({lon, lat});
  }
  return v;
}

inline SDL_Texture* load_texture(App* app, const std::string& filename) {
  SDL_Surface* surface = IMG_Load(filename.c_str());
  SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
  SDL_FreeSurface(surface);

  return texture;
}
#endif // LOADER_HPP
