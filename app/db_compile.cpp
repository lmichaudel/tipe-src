#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <random>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

void writeBinaryFile(const string& filename,
                     const vector<pair<double, double>>& coordinates) {
  ofstream outfile(filename, ios::binary);
  if (!outfile) {
    cerr << "Error: Could not open output file." << endl;
    return;
  }
  size_t size = coordinates.size();
  outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
  outfile.write(reinterpret_cast<const char*>(coordinates.data()),
                size * sizeof(pair<double, double>));
  outfile.close();
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <input.geojson> <output.bin>" << endl;
    return 1;
  }

  ifstream infile(argv[1]);
  if (!infile) {
    cerr << "Error: Could not open input file." << endl;
    return 1;
  }

  int c = 0;
  // json geojson;
  // infile >> geojson;
  // infile.close();

  vector<pair<double, double>> coordinates;

  // if (geojson.contains("features") && geojson["features"].is_array()) {
  //     for (const auto& feature : geojson["features"]) {
  //         if (feature.contains("geometry") &&
  //         feature["geometry"].contains("coordinates")) {
  //             auto geom = feature["geometry"];
  //             if (geom["type"] == "Point") {
  //                 coordinates.emplace_back(geom["coordinates"][1],
  //                 geom["coordinates"][0]);
  //             } else if (geom["type"] == "MultiPoint" || geom["type"] ==
  //             "LineString") {
  //                 for (const auto& point : geom["coordinates"]) {
  //                     coordinates.emplace_back(point[1], point[0]);
  //                 }
  //             }
  //         }
  //     }
  // }

  string line;
  while (getline(infile, line)) {
    json feature = json::parse(line);
    if (feature.contains("geometry") &&
        feature["geometry"].contains("coordinates")) {
      auto geom = feature["geometry"];
      if (geom["type"] == "Point") {
        coordinates.emplace_back(geom["coordinates"][1],
                                 geom["coordinates"][0]);
        c++;
      }
    }
  }

  // Shuffle the data because its sorted by location, if you use a small sample
  // you only get one area.

  auto rng = std::default_random_engine{};
  std::shuffle(std::begin(coordinates), std::end(coordinates), rng);

  writeBinaryFile(argv[2], coordinates);
  cout << "Binary file generated successfully: " << argv[2] << ", detected "
       << c << " entries." << endl;
  return 0;
}
