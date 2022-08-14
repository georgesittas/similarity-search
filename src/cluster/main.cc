#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "config.h"
#include "cluster.h"
#include "clusterer.h"
#include "nn_solver.h"
#include "dp_parser.h"
#include "data_point.h"
#include "common_utils.h"
#include "lsh_nn_solver.h"
#include "cube_nn_solver.h"
#include "lloyd_clusterer.h"
#include "rassgn_clusterer.h"

#define DELIMITER ' '
#define SAMPLE_SIZE 1000

int main(int argc, char* argv[]) {
  Configuration config(argc, argv);

  if (!config.valid_config) {
    return 1; // Error msg is printed to std::cerr inside config's constructor
  }

  std::fstream input_file(config.in_path, std::ios::in);
  std::fstream output_file(config.out_path, std::ios::out);

  if (input_file.fail()) {
    std::cerr << "\nFatal error: couldn't open " << config.in_path << "\n";
    return 2;
  } else if (output_file.fail()) {
    std::cerr << "\nFatal error: couldn't open " << config.out_path << "\n";
    return 3;
  }

  std::cout << "Loading input file... " << std::flush;

  Stopwatch(BEGIN);
  std::vector<const DataPoint*> input_points = Parse(input_file, DELIMITER);

  std::cout << "Done! (" << Stopwatch(STOP) / 1000.0 << " sec)\n\n"
            << "Clustering vectors... " << std::flush;

  Stopwatch(BEGIN);
  NnSolver* nn_solver = nullptr;
  Clusterer* clusterer = nullptr;

  if (config.method == "Classic") {
    clusterer = new LloydClusterer(input_points, config.num_clusters);
  } else {
    std::vector<const DataPoint*> input_points_sample = RandomSample(
      SAMPLE_SIZE, input_points
    );

    int width = AvgDist(input_points_sample) / 5; // Tune-able

    if (config.method == "LSH") {
      nn_solver = new LshNnSolver(input_points,
                                  width,
                                  input_points.size() / 8, // Tune-able
                                  config.lsh_num_tables,
                                  config.lsh_num_hashes);
    } else {
      nn_solver = new CubeNnSolver(input_points,
                                   width,
                                   config.cube_dimension,
                                   config.cube_max_probes,
                                   config.cube_max_points);
    }

    clusterer = new RassgnClusterer(input_points, config.num_clusters, nn_solver);
  }

  std::vector<Cluster> clusters = clusterer->CreateClusters();
  double clustering_time = Stopwatch(STOP) / 1000.0;

  std::cout << "Done! (" << clustering_time << " sec)\n"
            << "Computing Silhouette scores... " << std::flush;

  Stopwatch(BEGIN);
  auto silhouette_stats = ComputeSilhouettes(clusters);

  std::cout << "Done! (" << Stopwatch(STOP) / 1000.0 << " sec)\n";

  output_file << "Algorithm: "
              << (config.method == "Classic" ? "Lloyds" : "Range Search " + config.method)
              << "\n";

  for (int i = 0; i < config.num_clusters; i++) {
    output_file << "CLUSTER-" << i + 1 << " {size: " << clusters[i].GetSize() << ", centroid: [";

    for (int j = 0, n_coordinates = clusters[i].coordinates_.size(); j < n_coordinates; j++) {
      output_file << clusters[i].coordinates_[j] << (j + 1 == n_coordinates ? "]}\n" : ", ");
    }
  }

  output_file << "Clustering time: " << clustering_time << "\n"
              << "Silhouette: [";

  for (double avg_cluster_silhouette : silhouette_stats.first) {
    output_file << avg_cluster_silhouette << ", ";
  }

  output_file << silhouette_stats.second << "]\n";

  if (config.complete_output) {
    for (int i = 0; i < config.num_clusters; i++) {
      output_file << "\nCLUSTER-" << i + 1 << " {centroid, ";

      const std::vector<const DataPoint*>* members = clusters[i].GetMembers();
      for (int j = 0, size = clusters[i].GetSize(); j < size; j++) {
        output_file << (*members)[j]->GetId() << (j + 1 == size ? "}\n" : ", ");
      }
    }
  }

  std::cout << "\nTerminating...\n";

  delete clusterer;
  if (nn_solver != nullptr) {
    delete nn_solver;
  }

  for (auto data_point : input_points) {
    delete data_point;
  }

  input_file.close();
  output_file.close();

  return 0;
}    
