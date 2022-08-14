#include "cube_amplified_hash.h"

#include <vector>
#include <string>
#include <random>
#include <cstdint>
#include <unordered_map>

#include "common_utils.h"

#define MODULUS 4294967291 // This is a prime number (2^32 - 5)

CubeAmplifiedHash::CubeAmplifiedHash(int width, int data_point_dimension, int num_hashes) {
  std::vector<double> rand_line_vector;

  std::normal_distribution<> standard_normal(0.0, 1.0);
  std::uniform_real_distribution<> uniform_real(0, width);

  for (int i = 0; i < num_hashes; i++) {
    for (int i = 0; i < data_point_dimension; i++) {
      rand_line_vector.push_back(standard_normal(RandGen()));
    }

    lsh_hashes_.push_back(LshHash(width, uniform_real(RandGen()), rand_line_vector));
    hash_value_maps_.push_back(std::unordered_map<int64_t, int>());
  }
}

int64_t CubeAmplifiedHash::HashFunction(const DataPoint* data_point) {
  std::string concat_hashes = "";
  std::uniform_int_distribution<> uniform_int(0,1);

  // The {0,1} labeling is local for each different LSH hash function
  for (int i = 0, num_hashes = lsh_hashes_.size(); i < num_hashes; i++) {
    int64_t hash_value = lsh_hashes_[i].HashFunction(data_point);

    if (hash_value_maps_[i].find(hash_value) == hash_value_maps_[i].end()) {
      hash_value_maps_[i][hash_value] = uniform_int( RandGen() );
    }

    concat_hashes += std::to_string(hash_value_maps_[i][hash_value]);
  }

  // long long type is guaranteed to have a size of at least 64 bits
  return std::stoll(concat_hashes, 0, 2);
}
