#ifndef MODULES_NN_SOLVERS_LSH_LSH_AMPLIFIED_HASH_H_
#define MODULES_NN_SOLVERS_LSH_LSH_AMPLIFIED_HASH_H_

#include <vector>
#include <cstdint>

#include "lsh_hash.h"
#include "data_point.h"
#include "common_utils.h"

class LshAmplifiedHash {
 public:
  LshAmplifiedHash(int width, int data_point_dimension, int num_hashes);

  // Returns the lsh amplified hash value corresponding to 'data_point'.
  int64_t HashFunction(const DataPoint* data_point) const;

 private:
  std::vector<int> rand_vector_;
  std::vector<LshHash> lsh_hashes_;
};

#endif // MODULES_NN_SOLVERS_LSH_LSH_AMPLIFIED_HASH_H_
