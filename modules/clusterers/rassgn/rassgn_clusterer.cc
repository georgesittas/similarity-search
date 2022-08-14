#include "rassgn_clusterer.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "cluster.h"
#include "clusterer.h"
#include "nn_solver.h"
#include "data_point.h"
#include "common_utils.h"
#include "cluster_utils.h"

std::vector<Cluster> RassgnClusterer::CreateClusters() {
  std::unordered_map<std::string, int> point_to_cluster_idx;
  std::vector<Cluster> clusters = InitClusters(k_clusters_, input_points_);

  while (true) {
    int n_new_assignments = 0; // Used to determine whether the algorithm's converged
    bool found_at_least_one_assignment;

    double min_radius = 0.0;
    double max_radius = MinDistBetweenCentroids(clusters) / 2.0;

    do {
      // Assign data points using progressively expanding range searches around centroids
      // until we reach a point where there are no new assignments => continue with Lloyd's
      found_at_least_one_assignment = false;
      for (int i = 0; i < k_clusters_; i++) {
        std::vector<const DataPoint*> points_inside_range =
          nn_solver_->ApproxRangeSearch(&clusters[i], min_radius, max_radius);

        for (const DataPoint* data_point : points_inside_range) {
          auto entry = point_to_cluster_idx.find(data_point->GetId());

          if (entry == point_to_cluster_idx.end()) {
            n_new_assignments++;
            found_at_least_one_assignment = true;
            point_to_cluster_idx[ data_point->GetId() ] = i;
          } else {
            double dist1 = Dist(clusters[i].coordinates_, data_point->coordinates_);
            double dist2 = Dist(clusters[entry->second].coordinates_, data_point->coordinates_);

            // Only assign current data point to current cluster if it's closer to it
            if (dist1 < dist2) {
              n_new_assignments++;
              found_at_least_one_assignment = true;
              point_to_cluster_idx[ data_point->GetId() ] = i;
            }
          }
        }
      }

      min_radius = max_radius;
      max_radius *= 2;
    } while (found_at_least_one_assignment);

    // Assign each point to its corresponding cluster (Lloyd scheme for unassigned points)
    for (const DataPoint* data_point : input_points_) {
      auto entry = point_to_cluster_idx.find(data_point->GetId());

      if (entry == point_to_cluster_idx.end()) {
        n_new_assignments++;
        int nearest_cluster_idx;
        MinDistFromPointToClusters(data_point, clusters, nearest_cluster_idx);
        clusters[nearest_cluster_idx].AddMember(data_point);
        point_to_cluster_idx[ data_point->GetId() ] = nearest_cluster_idx;
      } else {
        clusters[entry->second].AddMember(data_point);
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
