#ifndef MODULES_NN_SOLVERS_LSH_LSH_NN_SOLVER_H_
#define MODULES_NN_SOLVERS_LSH_LSH_NN_SOLVER_H_

#include <vector>
#include <utility>

#include "nn_solver.h"
#include "data_point.h"
#include "hash_table.h"
#include "lsh_amplified_hash.h"

class LshNnSolver : public NnSolver {
 public:
  LshNnSolver(const std::vector<const DataPoint*>& input_points,
              double width,
              int table_size,
              int num_tables,
              int num_hashes);

  std::vector<std::pair<const DataPoint*, double> >
  ApproxKNns(const DataPoint* query_point, int k, double& elapsed) override;

  std::vector<const DataPoint*>
  ApproxRangeSearch(const DataPoint* query_point,
                    int min_r,
                    int max_r,
                    int limit = -1) override;

 private:
  double width_;
  int table_size_;
  int num_tables_;
  int num_hashes_;
  std::vector<HashTable<LshAmplifiedHash> > tables_;
};

#endif // MODULES_NN_SOLVERS_LSH_LSH_NN_SOLVER_H_
