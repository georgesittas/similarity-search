#ifndef MODULES_TYPES_CLUSTER_H_
#define MODULES_TYPES_CLUSTER_H_

#include <vector>
#include <utility>

#include "data_point.h"

class Cluster : public DataPoint {
 public:
  Cluster(const DataPoint& data_point)
      : DataPoint(data_point.GetId(), data_point.coordinates_) {}

  inline int GetSize() const { return members_.size(); }
  inline const std::vector<const DataPoint*>* GetMembers() const { return &members_; }
  inline void AddMember(const DataPoint* data_point) { members_.push_back(data_point); }

 private:
  std::vector<const DataPoint*> members_;
};

// Returns a pair that contains a vector with the average Silhouette scores per cluster in
// 'clusters' and the average overall Silhouette score for all points in 'clusters'.
std::pair<std::vector<double>, double>
ComputeSilhouettes(const std::vector<Cluster>& clusters);

#endif // MODULES_TYPES_CLUSTER_H_
