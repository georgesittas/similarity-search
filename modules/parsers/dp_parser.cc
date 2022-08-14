#include "dp_parser.h"

#include <vector>
#include <fstream>
#include <sstream>

#include "data_point.h"

std::vector<const DataPoint*> Parse(std::fstream& file, char delimiter) {
  std::vector<const DataPoint*> data_points;

  std::string line, token, id;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    } else if (line.back() == '\r') {
      line.pop_back();
    }

    std::stringstream ss(line);
    std::getline(ss, id, delimiter);

    DataPoint* new_data_point = new DataPoint(id);
    while (std::getline(ss, token, delimiter)) {
      new_data_point->coordinates_.push_back(std::stof(token));
    }

    data_points.push_back(new_data_point);
  }

  return data_points;
}
