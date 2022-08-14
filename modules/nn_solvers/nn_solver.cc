#include "nn_solver.h"

#include <queue>
#include <vector>
#include <utility>
#include <algorithm>

#include "data_point.h"
#include "common_utils.h"

std::vector<std::pair<const DataPoint*, double> >
NnSolver::ExactKNns(const DataPoint* query_point, int k, double& elapsed) {
  auto less_than = [](const std::pair<const DataPoint*, double>& a,
                      const std::pair<const DataPoint*, double>& b) {
    return a.second < b.second;
  };

  // Maintain this heap to store the best k neighbors found
  std::priority_queue<
    std::pair<const DataPoint*, double>,
    std::vector<std::pair<const DataPoint*, double> >,
    decltype(less_than)
  > exact_nns_pq(less_than);

  Stopwatch(BEGIN);

  for (const DataPoint* data_point : input_points_) {
    double curr_dist = Dist(data_point->coordinates_, query_point->coordinates_);
    std::pair<const DataPoint*, double> curr_pair(data_point, curr_dist);

    if (((int) exact_nns_pq.size()) < k) {
      exact_nns_pq.push(curr_pair);
    } else if (less_than(curr_pair, exact_nns_pq.top())) {
      exact_nns_pq.pop();
      exact_nns_pq.push(curr_pair);
    }
  }

  elapsed = Stopwatch(STOP);

  std::vector<std::pair<const DataPoint*, double> > exact_nns_vec;
  while (!exact_nns_pq.empty()) {
    exact_nns_vec.push_back(exact_nns_pq.top());
    exact_nns_pq.pop();
  }

  std::reverse(exact_nns_vec.begin(), exact_nns_vec.end());
  return exact_nns_vec; // The first element is the nearest neighbor of all k
}
