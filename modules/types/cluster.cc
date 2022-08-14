#include "cluster.h"

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

#include "data_point.h"
#include "common_utils.h"

// Returns the average distance from 'data_point' to all (different) points in 'cluster'.
static double AvgDistInCluster(const DataPoint* data_point, const Cluster& cluster) {
  int n_members = cluster.GetSize();
  const std::vector<const DataPoint*>* members = cluster.GetMembers();

  int contains_it = 0; // Is set to 1 if 'cluster' contains 'data_point'
  double avg_dist = 0.0;
  for (int i = 0; i < n_members; i++) {
    if (data_point->GetId() != (*members)[i]->GetId()) {
      avg_dist += Dist((*members)[i]->coordinates_, data_point->coordinates_);
    } else {
      contains_it = 1;
    }
  }

  // Careful: don't count dist from 'data_point' to itself, if 'cluster' contains it
  return n_members - contains_it != 0 ? avg_dist / (n_members - contains_it) : -1;
}

// Returns the index of the next closest cluster that 'data_point' (in cluster
// corresponding to 'cluster_idx' in 'clusters') could be assigned to.
static int NextClosestClusterIdx(int cluster_idx,
                                 const DataPoint* data_point,
                                 const std::vector<Cluster>& clusters) {
  double min_distance = -1;
  int next_best_cluster_idx = 0;

  for (int i = 0, k_clusters = clusters.size(); i < k_clusters; i++) {
    if (i != cluster_idx) {
      double curr_dist = Dist(clusters[i].coordinates_, data_point->coordinates_);

      if (curr_dist < min_distance || min_distance == -1) {
        min_distance = curr_dist;
        next_best_cluster_idx = i;
      }
    }
  }

  return next_best_cluster_idx;
}

std::pair<std::vector<double>, double>
ComputeSilhouettes(const std::vector<Cluster>& clusters) {
  int total_data_points = 0;
  int k_clusters = clusters.size();

  double avg_overall_silhouette = 0.0;
  std::vector<double> avg_silhouettes(k_clusters, 0.0);

  for (int i = 0; i < k_clusters; i++) {
    int n_members = clusters[i].GetSize();
    const std::vector<const DataPoint*>* members = clusters[i].GetMembers();

    if (n_members == 0) {
      std::cerr << "Error: silhouette computation failed for cluster " << i + 1 << "\n";
      continue;
    }

    for (int j = 0; j < n_members; j++) {
      double avg_dist = AvgDistInCluster((*members)[j], clusters[i]);
      double avg_dist_next_closest = AvgDistInCluster(
        (*members)[j], clusters[ NextClosestClusterIdx(i, (*members)[j], clusters) ]
      );

      if ((avg_dist == 0 && avg_dist_next_closest == 0) ||
           avg_dist < 0 || avg_dist_next_closest < 0) {
        std::cerr << "Error: silhouette computation failed for cluster " << i + 1 << "\n";
        continue;
      }

      avg_silhouettes[i] +=
        (avg_dist_next_closest - avg_dist) / std::max(avg_dist, avg_dist_next_closest);
    }

    avg_overall_silhouette += avg_silhouettes[i];

    total_data_points += n_members;
    avg_silhouettes[i] /= n_members;
  }

  avg_overall_silhouette /= total_data_points;
  return std::pair<std::vector<double>, double>(avg_silhouettes, avg_overall_silhouette);
}
