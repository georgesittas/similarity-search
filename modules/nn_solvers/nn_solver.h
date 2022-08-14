#ifndef MODULES_NN_SOLVERS_NN_SOLVER_H_
#define MODULES_NN_SOLVERS_NN_SOLVER_H_

#include <vector>
#include <utility>

#include "data_point.h"

class NnSolver {
 public:
  NnSolver(const std::vector<const DataPoint*>& input_points)
      : input_points_(input_points) {}

  virtual ~NnSolver() = default;

  // Returns a vector of 'k' pointers to exact nearest neighboring data points to
  // 'query_point', as well as their distances from it. The time elapsed to find
  // them (in millisec) is also returned in 'elapsed'.
  std::vector<std::pair<const DataPoint*, double> >
  ExactKNns(const DataPoint* query_point, int k, double& elapsed);

  // Returns a vector of (up to) 'k' pointers to approximate nearest neighboring data
  // points to 'query_point', as well as their distances from it. The time elapsed to
  // find them (in millisec) is also returned in 'elapsed'.
  virtual std::vector<std::pair<const DataPoint*, double> >
  ApproxKNns(const DataPoint* query_point, int k, double& elapsed) = 0;

  // Returns a vector of (at most 'limit', if 'limit' != -1) pointers to data points
  // in range ['min_r', 'max_r') from 'query_point'.
  virtual std::vector<const DataPoint*>
  ApproxRangeSearch(const DataPoint* query_point, int min_r, int max_r, int limit = -1) = 0;

 protected:
  // This will be used as a dataset for the above methods to work with
  const std::vector<const DataPoint*>& input_points_;
};

#endif // MODULES_NN_SOLVERS_NN_SOLVER_H_
