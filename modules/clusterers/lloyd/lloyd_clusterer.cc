#include "lloyd_clusterer.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "cluster.h"
#include "clusterer.h"
#include "data_point.h"
#include "common_utils.h"
#include "cluster_utils.h"

std::vector<Cluster> LloydClusterer::CreateClusters() {
  std::unordered_map<std::string, int> point_to_cluster_idx;
  std::vector<Cluster> clusters = InitClusters(k_clusters_, input_points_);

  while (true) {
    int n_new_assignments = 0;

    for (const DataPoint* data_point : input_points_) {
      int nearest_cluster_idx;
      MinDistFromPointToClusters(data_point, clusters, nearest_cluster_idx);
      clusters[nearest_cluster_idx].AddMember(data_point);

      auto entry = point_to_cluster_idx.find(data_point->GetId());
      if (entry != point_to_cluster_idx.end()) {
        if (entry->second != nearest_cluster_idx) {
          n_new_assignments++;
          point_to_cluster_idx[ data_point->GetId() ] = nearest_cluster_idx;
        }
      } else {
        n_new_assignments++;
        point_to_cluster_idx[ data_point->GetId() ] = nearest_cluster_idx;
      }
    }

    if (n_new_assignments == 0) {
      break; // Clustering has converged
    }

    // Compute new centroids with the mean vector method
    std::vector<Cluster> new_clusters;
    for (Cluster& cluster : clusters) {
      DataPoint new_centroid = AvgDataPoint(*cluster.GetMembers());
      new_clusters.push_back( Cluster(new_centroid) );
    }

    clusters = new_clusters;
  }

  return clusters;
}
