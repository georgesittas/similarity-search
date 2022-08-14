#ifndef MODULES_NN_SOLVERS_CUBE_CUBE_NN_SOLVER_H_
#define MODULES_NN_SOLVERS_CUBE_CUBE_NN_SOLVER_H_

#include <vector>
#include <utility>

#include "nn_solver.h"
#include "data_point.h"
#include "hash_table.h"
#include "cube_amplified_hash.h"

class CubeNnSolver : public NnSolver {
 public:
  CubeNnSolver(const std::vector<const DataPoint*>& input_points,
               double width,
               int dimension,
               int max_probes,
               int max_points);

  std::vector<std::pair<const DataPoint*, double> >
  ApproxKNns(const DataPoint* query_point, int k, double& elapsed) override;

  std::vector<const DataPoint*>
  ApproxRangeSearch(const DataPoint* query_point,
                    int min_r,
                    int max_r,
                    int limit = -1) override;

 private:
  double width_;
  int dimension_;
  int max_probes_;
  int max_points_;
  HashTable<CubeAmplifiedHash> table_;
};

#endif // MODULES_NN_SOLVERS_CUBE_CUBE_NN_SOLVER_H_
