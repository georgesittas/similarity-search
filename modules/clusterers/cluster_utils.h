#ifndef MODULES_CLUSTERERS_CLUSTER_UTILS_H_
#define MODULES_CLUSTERERS_CLUSTER_UTILS_H_

#include <vector>

#include "cluster.h"
#include "data_point.h"

// Returns the minimum distance between two centroids in 'clusters'.
double MinDistBetweenCentroids(const std::vector<Cluster>& clusters);

// Returns the minimum distance between 'data_point' and a cluster in 'clusters'.
// Additionally, the corresponding cluster index is returned in 'nearest_cluster_idx'.
double MinDistFromPointToClusters(const DataPoint* data_point,
                                  const std::vector<Cluster>& clusters,
                                  int& nearest_cluster_idx);

// Initializes 'k_clusters' clusters from 'input_points' and returns a vector with them.
std::vector<Cluster> InitClusters(int k_clusters,
                                  const std::vector<const DataPoint*>& input_points);

#endif // MODULES_CLUSTERERS_CLUSTER_UTILS_H_
