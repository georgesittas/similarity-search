#ifndef MODULES_CLUSTERERS_RASSGN_RASSGN_CLUSTERER_H_
#define MODULES_CLUSTERERS_RASSGN_RASSGN_CLUSTERER_H_

#include <vector>

#include "cluster.h"
#include "clusterer.h"
#include "nn_solver.h"
#include "data_point.h"

class RassgnClusterer : public Clusterer {
 public:
  RassgnClusterer(const std::vector<const DataPoint*>& input_points,
                  int k_clusters,
                  NnSolver* nn_solver)
      : Clusterer(input_points),
        k_clusters_(k_clusters),
        nn_solver_(nn_solver) {}

  std::vector<Cluster> CreateClusters() override;

 private:
  int k_clusters_;
  NnSolver* nn_solver_;
};

#endif // MODULES_CLUSTERERS_RASSGN_RASSGN_CLUSTERER_H_
