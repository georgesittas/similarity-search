#ifndef SRC_LSH_CONFIG_H_
#define SRC_LSH_CONFIG_H_

#include <string>

struct Configuration {
  int num_hashes;
  int num_tables;
  int num_neighbors;
  double search_radius;

  std::string in_path;  // Input file path name
  std::string qr_path;  // Query file path name
  std::string out_path; // Output file path name

  Configuration(int argc, char* argv[]);
};

#endif // SRC_LSH_CONFIG_H_
