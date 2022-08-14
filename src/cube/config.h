#ifndef SRC_CUBE_CONFIG_H_
#define SRC_CUBE_CONFIG_H_

#include <string>

struct Configuration {
  int dimension;
  int max_points;
  int max_probes;
  int num_neighbors;
  float search_radius;

  std::string in_path;  // Input file path name
  std::string qr_path;  // Query file path name
  std::string out_path; // Output file path name

  Configuration(int argc, char* argv[]);
};

#endif // SRC_CUBE_CONFIG_H_
