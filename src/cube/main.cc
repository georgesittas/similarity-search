#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>

#include "config.h"
#include "dp_parser.h"
#include "data_point.h"
#include "common_utils.h"
#include "cube_nn_solver.h"

#define DELIMITER ' '
#define SAMPLE_SIZE 1000

int main(int argc, char* argv[]) {
  Configuration config(argc, argv);

  if (config.in_path.empty()) {
    std::cout << "Enter path to input file: ";
    std::getline(std::cin, config.in_path);
  }

  std::fstream input_file(config.in_path, std::ios::in);
  if (input_file.fail()) {
    std::cerr << "\nFatal error: couldn't open " << config.in_path << "\n";
    return 1;
  }

  std::cout << "Loading input file... " << std::flush;

  Stopwatch(BEGIN);
  std::vector<const DataPoint*> input_points = Parse(input_file, DELIMITER);

  std::cout << "Done! (" << Stopwatch(STOP) / 1000.0 << " sec)\n"
            << "Initializing data structures... " << std::flush;

  Stopwatch(BEGIN);
  std::vector<const DataPoint*> input_points_sample = RandomSample(
    SAMPLE_SIZE, input_points
  );

  int width = AvgDist(input_points_sample) / 5; // Tune-able

  CubeNnSolver solver(
    input_points, width, config.dimension, config.max_probes, config.max_points
  );

  std::cout << "Done! (" << Stopwatch(STOP) / 1000.0 << " sec)\n\n";

  if (config.qr_path.empty()) {
    std::cout << "Enter path to query file: ";
    std::getline(std::cin, config.qr_path);
  }

  if (config.out_path.empty()) {
    std::cout << "Enter path to output file: ";
    std::getline(std::cin, config.out_path);
  }

  std::fstream query_file(config.qr_path, std::ios::in);
  std::fstream output_file(config.out_path, std::ios::out);

  if (query_file.fail()) {
    std::cerr << "\nFatal error: couldn't open " << config.qr_path << "\n";
    return 2;
  } else if (output_file.fail()) {
    std::cerr << "\nFatal error: couldn't open " << config.out_path << "\n";
    return 3;
  }

  while (true) {
    double bf_duration, cube_duration;
    std::vector<std::pair<const DataPoint*, double> > exact_nns, approx_nns;

    int neighbors_missed = 0; // Total number of neighbors missed by the method

    // Metrics used to evaluate the algorithm's efficiency (over all queries in 'query_file')
    // Note: sum_true_dist and sum_approx_dist refer to 1-Nearest Neighbor distances
    double sum_true_dist = 0.0, avg_bf_duration = 0.0;
    double sum_approx_dist = 0.0, avg_cube_duration = 0.0;

    double avg_duration_rate = 0.0; // avg { cube duration / brute force duration }
    double avg_approx_factor = 0.0; // avg { approx 1-NN distance / true 1-NN distance }
    double max_approx_factor = 0.0; // max { approx 1-NN distance / true 1-NN distance }

    std::cout << "Loading query file... " << std::flush;

    Stopwatch(BEGIN);
    std::vector<const DataPoint*> query_points = Parse(query_file, DELIMITER);

    if (query_points.size() == 0) {
      std::cerr << "\nFatal error: empty query file\n";
      return 4;
    }

    std::cout << "Done! (" << Stopwatch(STOP) / 1000.0 << " sec)\n\nRunning... " << std::flush;
    for (const DataPoint* query_point : query_points) {
      exact_nns = solver.ExactKNns(query_point, config.num_neighbors, bf_duration);
      approx_nns = solver.ApproxKNns(query_point, config.num_neighbors, cube_duration);

      output_file << "Query: " << query_point->GetId();
      for (int i = 0, approx_count = approx_nns.size(); i < approx_count; i++) {
        output_file << "\n\nNearest neighbor-" << i + 1 << ": " << approx_nns[i].first->GetId()
                    <<   "\ndistanceHypercube: " << approx_nns[i].second
                    <<   "\ndistanceTrue: " << exact_nns[i].second;
      }

      neighbors_missed += config.num_neighbors - approx_nns.size();

      avg_bf_duration += bf_duration;
      avg_cube_duration += cube_duration;
      avg_duration_rate += cube_duration / bf_duration;

      if (approx_nns.size() != 0) {
        sum_true_dist += exact_nns[0].second;
        sum_approx_dist += approx_nns[0].second;

        avg_approx_factor += approx_nns[0].second / exact_nns[0].second;
        max_approx_factor = std::max(
          max_approx_factor, approx_nns[0].second / exact_nns[0].second
        );
      }

      output_file << "\n\ntHypercube: " << cube_duration
                  <<   "\ntTrue: " << bf_duration;

      std::vector<const DataPoint*> data_points_in_range =
        solver.ApproxRangeSearch(query_point, 0, config.search_radius, 20 * config.max_points);

      output_file << "\n\n" << config.search_radius << "-near neighbors:";
      for (const DataPoint* data_point : data_points_in_range) {
        output_file << "\n" << data_point->GetId();
      }

      output_file << "\n\n";
    }

    std::cout << "Done! Computed the following statistics (over all queries):\n\n"
              << "avg_bf_duration    : " << avg_bf_duration / query_points.size() << "ms\n"
              << "avg_cube_duration  : " << avg_cube_duration / query_points.size() << "ms\n"
              << "avg_duration_rate  : " << avg_duration_rate / query_points.size() << "\n"
              << "avg_approx_factor  : " << avg_approx_factor / query_points.size() << "\n"
              << "max_approx_factor  : " << max_approx_factor << "\n"
              << "sum_distances_rate : " << sum_true_dist / sum_approx_dist << "\n"
              << "\nThe method missed " << neighbors_missed << " neighbors in total\n";

    for (auto query : query_points) {
      delete query;
    }

    std::cout << "\nEnter path to new query file, or press enter to stop: ";
    std::getline(std::cin, config.qr_path);

    query_file.close();
    if (config.qr_path.empty()) {
      break;
    } else {
      query_file.open(config.qr_path, std::ios::in);
      if (query_file.fail()) {
        std::cerr << "\nFatal error: couldn't open " << config.qr_path << "\n";
        return 2;
      }
    }
  }

  std::cout << "\nTerminating...\n";

  for (auto data_point : input_points) {
    delete data_point;
  }

  input_file.close();
  output_file.close();

  return 0;
}
