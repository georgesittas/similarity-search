#ifndef MODULES_CLUSTERERS_CLUSTERER_H_
#define MODULES_CLUSTERERS_CLUSTERER_H_

#include <vector>

#include "cluster.h"
#include "data_point.h"

class Clusterer {
 public:
  Clusterer(const std::vector<const DataPoint*>& input_points)
      : input_points_(input_points) {}

  virtual ~Clusterer() = default;

  // Returns a vector of clusters computed for the given input points.
  virtual std::vector<Cluster> CreateClusters() = 0;

 protected:
  // This will be used as a dataset for the above method to work with
  const std::vector<const DataPoint*>& input_points_;
};

#endif // MODULES_CLUSTERERS_CLUSTERER_H_
