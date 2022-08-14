#ifndef MODULES_TYPES_LSH_HASH_H_
#define MODULES_TYPES_LSH_HASH_H_

#include <cmath>
#include <vector>
#include <cstdint>

#include "data_point.h"
#include "common_utils.h"

class LshHash {
 public:
  LshHash(int width, float shift, const std::vector<double>& rand_line_vector)
      : width_(width), shift_(shift), rand_line_vector_(rand_line_vector) {}

  // Returns The LSH hash value corresponding to 'data_point'.
  inline int64_t HashFunction(const DataPoint* data_point) const {
    return floor((Dot(data_point->coordinates_, rand_line_vector_) + shift_) / width_);
  }

 private:
  int width_;
  float shift_;
  std::vector<double> rand_line_vector_;
};

#endif // MODULES_TYPES_LSH_HASH_H_
