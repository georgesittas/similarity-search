#include "lsh_nn_solver.h"

#include <queue>
#include <vector>
#include <utility>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

#include "nn_solver.h"
#include "data_point.h"
#include "hash_table.h"
#include "common_utils.h"
#include "lsh_amplified_hash.h"

LshNnSolver::LshNnSolver(const std::vector<const DataPoint*>& input_points,
                         double width,
                         int table_size,
                         int num_tables,
                         int num_hashes)
    : NnSolver(input_points),
      width_(width),
      table_size_(table_size),
      num_tables_(num_tables),
      num_hashes_(num_hashes) {
  int input_size = input_points_.size();
  for (int i = 0; i < num_tables_; i++) {
    tables_.push_back(
      HashTable<LshAmplifiedHash>(
        table_size_,
        LshAmplifiedHash(width_, input_points_[0]->coordinates_.size(), num_hashes_)
      )
    );

    for (int j = 0; j < input_size; j++) {
      tables_[i].Insert(input_points_[j]);
    }
  }
}

std::vector<std::pair<const DataPoint*, double> >
LshNnSolver::ApproxKNns(const DataPoint* query_point, int k, double& elapsed) {
  auto less_than = [](const std::pair<const DataPoint*, double>& a,
                      const std::pair<const DataPoint*, double>& b) {
    return a.second < b.second;
  };

  // Maintain this heap to store the best k neighbors found in all tables
  std::priority_queue<
    std::pair<const DataPoint*, double>,
    std::vector<std::pair<const DataPoint*, double> >,
    decltype(less_than)
  > approx_nns_pq(less_than);

  // Make sure to not double-include a neighbor in the best-k (if it exists in >1 tables)
  std::unordered_set<std::string> included_data_points;

  Stopwatch(BEGIN);

  for (int i = 0; i < num_tables_; i++) {
    int64_t query_hash_val = tables_[i].GetHashVal(query_point);
    const std::vector<std::pair<const DataPoint*, int64_t> >*
      bucket = tables_[i].GetBucket(query_point);

    for (const std::pair<const DataPoint*, int64_t>& pair : *bucket) {

      // We check only those points in the bucket with the same amplified LSH hash value
      if (pair.second == query_hash_val) {
        double curr_dist = Dist(pair.first->coordinates_, query_point->coordinates_);
        std::pair<const DataPoint*, double> curr_pair(pair.first, curr_dist);

        if (included_data_points.find(pair.first->GetId()) == included_data_points.end()) {
          if (((int) approx_nns_pq.size()) < k) {
            approx_nns_pq.push(curr_pair);
            included_data_points.insert(pair.first->GetId());
          } else if (less_than(curr_pair, approx_nns_pq.top())) {
            approx_nns_pq.pop();
            approx_nns_pq.push(curr_pair);
            included_data_points.insert(pair.first->GetId());
          }
        }
      }
    }
  }

  elapsed = Stopwatch(STOP);

  std::vector<std::pair<const DataPoint*, double> > approx_nns_vec;
  while (!approx_nns_pq.empty()) {
    approx_nns_vec.push_back(approx_nns_pq.top());
    approx_nns_pq.pop();
  }

  std::reverse(approx_nns_vec.begin(), approx_nns_vec.end());
  return approx_nns_vec; // The first element is the nearest neighbor of all k
}

std::vector<const DataPoint*>
LshNnSolver::ApproxRangeSearch(const DataPoint* query_point,
                               int min_r,
                               int max_r,
                               int limit) {
  std::vector<const DataPoint*> points_inside_range;
  std::unordered_set<std::string> included_data_points;

  for (int i = 0; i < num_tables_; i++) {
    const std::vector<std::pair<const DataPoint*, int64_t> >*
      bucket = tables_[i].GetBucket(query_point);

    for (const std::pair<const DataPoint*, int64_t>& pair : *bucket) {
      double curr_dist = Dist(pair.first->coordinates_, query_point->coordinates_);

      if (curr_dist >= min_r && curr_dist < max_r &&
          included_data_points.find(pair.first->GetId()) == included_data_points.end()) {
        points_inside_range.push_back(pair.first);
        included_data_points.insert(pair.first->GetId());

        if (limit != -1 && ((int) points_inside_range.size()) >= limit) {
          return points_inside_range;
        }
      }
    }
  }

  return points_inside_range;
}
