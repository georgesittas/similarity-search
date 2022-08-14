#ifndef MODULES_NN_SOLVERS_CUBE_CUBE_AMPLIFIED_HASH_H_
#define MODULES_NN_SOLVERS_CUBE_CUBE_AMPLIFIED_HASH_H_

#include <vector>
#include <cstdint>
#include <unordered_map>

#include "lsh_hash.h"

class CubeAmplifiedHash {
 public:
  CubeAmplifiedHash(int width, int data_point_dimension, int num_hashes);

  // Returns The hash value corresponding to 'data_point'.
  int64_t HashFunction(const DataPoint* data_point);

 private:
  std::vector<LshHash> lsh_hashes_;
  std::vector<std::unordered_map<int64_t, int> > hash_value_maps_;
};

#endif // MODULES_NN_SOLVERS_CUBE_CUBE_AMPLIFIED_HASH_H_
