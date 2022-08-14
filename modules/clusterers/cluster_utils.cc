#include "cluster_utils.h"

#include <vector>
#include <random>
#include <utility>

#include "cluster.h"
#include "data_point.h"
#include "common_utils.h"

double MinDistBetweenCentroids(const std::vector<Cluster>& clusters) {
  double min_dist = -1;

  for (int i = 0, n_centroids = clusters.size(); i < n_centroids - 1; i++) {
    for (int j = i + 1; j < n_centroids; j++) {
      double dist = Dist(clusters[i].coordinates_, clusters[j].coordinates_);
      if (dist < min_dist || min_dist == -1) {
        min_dist = dist;
      }
    }
  }

  return min_dist;
}

double MinDistFromPointToClusters(const DataPoint* data_point,
                                  const std::vector<Cluster>& clusters,
                                  int& nearest_cluster_idx) {
  double min_dist = -1;
  for (int i = 0, k_clusters = clusters.size(); i < k_clusters; i++) {
    double curr_dist = Dist(data_point->coordinates_, clusters[i].coordinates_);

    if (curr_dist < min_dist || min_dist == -1) {
      min_dist = curr_dist;
      nearest_cluster_idx = i;
    }
  }

  return min_dist;
}

static bool IsCluster(const DataPoint* data_point, const std::vector<Cluster>& clusters) {
  for (const Cluster& cluster : clusters) {
    if (cluster.GetId() == data_point->GetId()) {
      return true;
    }
  }

  return false;
}

std::vector<Cluster> InitClusters(int k_clusters,
                                  const std::vector<const DataPoint*>& input_points) {
  std::uniform_real_distribution<> uniform_real(0, 1);
  std::uniform_int_distribution<> uniform_int(0, input_points.size() - 1);

  // K-Means++ clustering initialization scheme
  int random_idx = uniform_int(RandGen()); // Pick first cluster uniformly at random
  std::vector<Cluster> clusters = { Cluster(*input_points[random_idx]) };

  while (((int) clusters.size()) != k_clusters) {
    std::vector<double> min_dists;
    std::vector<double> probabilities;

    double max_min_dist = -1; // Used to normalize min dists

    // Compute minimum distance from each data point to a cluster in 'clusters'
    for (const DataPoint* data_point : input_points) {
      if (!IsCluster(data_point, clusters)) {
        int nearest_cluster_idx; // This is not going to be used here
        double curr_min_dist = MinDistFromPointToClusters(
          data_point, clusters, nearest_cluster_idx
        );

        min_dists.push_back(curr_min_dist);
        if (curr_min_dist > max_min_dist) {
          max_min_dist = curr_min_dist;
        }
      }
    }

    // Normalize minimum distances & compute the sum of their squares
    double dist_squared_sum = 0;
    for (double& min_dist : min_dists) {
      min_dist /= max_min_dist;
      dist_squared_sum += min_dist * min_dist;
    }

    // Compute the probability for choosing a data point as the next cluster
    int idx = 0;
    for (const DataPoint* data_point : input_points) {
      if (!IsCluster(data_point, clusters)) {
        probabilities.push_back((min_dists[idx] * min_dists[idx]) / dist_squared_sum);
        idx++;
      }
    }

    // Choose a random data point as next cluster, based on its probability
    idx = 0;
    double cumulative_prob = 0.0, random_prob = uniform_real(RandGen());
    for (const DataPoint* data_point : input_points) {
      if (!IsCluster(data_point, clusters)) {
        cumulative_prob += probabilities[idx++];
        if (random_prob <= cumulative_prob) {
          clusters.push_back( Cluster(*data_point) );
          break;
        }
      }
    }

    min_dists.clear();
    probabilities.clear();
  }

  return clusters;
}
