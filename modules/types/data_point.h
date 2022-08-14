#ifndef MODULES_TYPES_DATA_POINT_H_
#define MODULES_TYPES_DATA_POINT_H_

#include <string>
#include <vector>

class DataPoint {
 public:
  std::vector<double> coordinates_;

  DataPoint() {}
  DataPoint(std::string id) : id_(id) {}
  DataPoint(std::string id, const std::vector<double>& coordinates)
    : coordinates_(coordinates), id_(id) {}

  inline const std::string& GetId() const { return id_; }

 private:
  std::string id_;
};

#endif // MODULES_TYPES_DATA_POINT_H_
