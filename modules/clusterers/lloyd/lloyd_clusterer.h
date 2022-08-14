#ifndef MODULES_CLUSTERERS_LLOYD_LLOYD_CLUSTERER_H_
#define MODULES_CLUSTERERS_LLOYD_LLOYD_CLUSTERER_H_

#include <vector>

#include "cluster.h"
#include "clusterer.h"
#include "data_point.h"

class LloydClusterer : public Clusterer {
 public:
  LloydClusterer(const std::vector<const DataPoint*>& input_points, int k_clusters)
      : Clusterer(input_points), k_clusters_(k_clusters) {}

  std::vector<Cluster> CreateClusters() override;

 private:
  int k_clusters_;
};

#endif // MODULES_CLUSTERERS_LLOYD_LLOYD_CLUSTERER_H_
