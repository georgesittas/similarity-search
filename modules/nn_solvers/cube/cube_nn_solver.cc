#include "cube_nn_solver.h"

#include <queue>
#include <vector>
#include <bitset>
#include <utility>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

#include "nn_solver.h"
#include "data_point.h"
#include "hash_table.h"
#include "common_utils.h"
#include "cube_amplified_hash.h"

// Generates and returns all 'n_bits'-bit numbers with increasing hamming distance from 'num'.
static int64_t GetNextBucketIndex(bool& stop, int64_t num=0, int n_bits=0, bool reset=false) {
  // The following variables are set when 'reset' is true and they maintain internal state
  static int64_t seed;
  static int bit_count;
  static int target_hamming_distance;
  static std::vector<int64_t> nums_in_distance;

  if (reset) {
    seed = num;
    bit_count = n_bits;
    nums_in_distance.clear();
    target_hamming_distance = 1;
    return seed;
  }

  if (nums_in_distance.size() == 0) {
    if (++target_hamming_distance > bit_count) {
      stop = true; // Exceeded max hamming distance for current bit count
      return 0;
    }

    // Find and store all numbers with 'target_hamming_distance' distance from 'seed'
    for (int64_t i = 0, limit = 1 << bit_count; i < limit; i++) {
      if (((int) std::bitset<64>(i ^ seed).count()) == target_hamming_distance) {
        nums_in_distance.push_back(i);
      }
    }
  }

  int64_t retval = nums_in_distance.back();
  nums_in_distance.pop_back();
  return retval;
}

CubeNnSolver::CubeNnSolver(const std::vector<const DataPoint*>& input_points,
                           double width,
                           int dimension,
                           int max_probes,
                           int max_points)
    : NnSolver(input_points),
      width_(width),
      dimension_(dimension),
      max_probes_(max_probes),
      max_points_(max_points),
      table_(1 << dimension_,
             CubeAmplifiedHash(width_, input_points_[0]->coordinates_.size(), dimension_)) {
  for (int j = 0, input_size = input_points_.size(); j < input_size; j++) {
    table_.Insert(input_points_[j]);
  }
}

std::vector<std::pair<const DataPoint*, double> >
CubeNnSolver::ApproxKNns(const DataPoint* query_point, int k, double& elapsed) {
  auto less_than = [](const std::pair<const DataPoint*, double>& a,
                      const std::pair<const DataPoint*, double>& b) {
    return a.second < b.second;
  };

  // Maintain this heap to store the best k neighbors found
  std::priority_queue <
    std::pair<const DataPoint*, double>,
    std::vector<std::pair<const DataPoint*, double> >,
    decltype(less_than)
  > approx_nns_pq(less_than);

  Stopwatch(BEGIN);

  bool stop = false;

  int64_t query_bucket_idx = table_.GetHashVal(query_point);
  int64_t next_bucket_idx = GetNextBucketIndex(stop, query_bucket_idx, dimension_, true);

  for (int current_points = 0, n_probes = 0; n_probes++ < max_probes_ && !stop; ) {
    if (current_points == max_points_) {
      break;
    }

    const std::vector<std::pair<const DataPoint*, int64_t> >*
      bucket = table_.GetBucket(next_bucket_idx);

    for (const std::pair<const DataPoint*, int64_t>& pair : *bucket) {
      double curr_dist = Dist(pair.first->coordinates_, query_point->coordinates_);
      std::pair<const DataPoint*, double> curr_pair(pair.first, curr_dist);

      if (((int) approx_nns_pq.size()) < k) {
        approx_nns_pq.push(curr_pair);
      } else if (less_than(curr_pair, approx_nns_pq.top())) {
        approx_nns_pq.pop();
        approx_nns_pq.push(curr_pair);
      }

      if (++current_points == max_points_) {
        break;
      }
    }

    next_bucket_idx = GetNextBucketIndex(stop);
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
CubeNnSolver::ApproxRangeSearch(const DataPoint* query_point,
                                int min_r,
                                int max_r,
                                int limit) {
  std::vector<const DataPoint*> points_inside_range;

  bool stop = false;

  int64_t query_bucket_idx = table_.GetHashVal(query_point);
  int64_t next_bucket_idx = GetNextBucketIndex(stop, query_bucket_idx, dimension_, true);

  for (int current_points = 0, n_probes = 0; n_probes++ < max_probes_ && !stop; ) {
    const std::vector<std::pair<const DataPoint*, int64_t> >*
        bucket = table_.GetBucket(next_bucket_idx);

    for (const std::pair<const DataPoint*, int64_t>& pair : *bucket) {
      double curr_dist = Dist(pair.first->coordinates_, query_point->coordinates_);
      if (curr_dist >= min_r && curr_dist < max_r) {

        points_inside_range.push_back(pair.first);
        if (limit != -1 && ((int) points_inside_range.size()) >= limit) {
          return points_inside_range;
        }
      }

      if (++current_points == max_points_) {
        return points_inside_range;
      }
    }

    next_bucket_idx = GetNextBucketIndex(stop);
  }

  return points_inside_range;
}
