#ifndef SRC_CLUSTER_CONFIG_H_
#define SRC_CLUSTER_CONFIG_H_

#include <string>

struct Configuration {
  int num_clusters;
  int lsh_num_hashes;
  int lsh_num_tables;
  int cube_max_points;
  int cube_dimension;
  int cube_max_probes;

  bool valid_config;
  bool complete_output;

  std::string in_path;  // Input file path name
  std::string out_path; // Output file path name
  std::string method;   // Method name (either "Classic", "LSH" or "Hypercube")

  Configuration(int argc, char* argv[]);
};

#endif // SRC_CLUSTER_CONFIG_H_
