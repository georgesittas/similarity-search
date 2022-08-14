#include "lsh_amplified_hash.h"

#include <vector>
#include <random>
#include <climits>
#include <cstdint>

#include "lsh_hash.h"
#include "data_point.h"
#include "common_utils.h"

#define MODULUS 4294967291 // This is a prime number (2^32 - 5)

LshAmplifiedHash::LshAmplifiedHash(int width, int data_point_dimension, int num_hashes) {
  std::vector<double> rand_line_vector;

  std::normal_distribution<> standard_normal(0.0, 1.0);
  std::uniform_real_distribution<> uniform_real(0, width);
  std::uniform_int_distribution<> uniform_int(0, INT32_MAX);

  for (int i = 0; i < num_hashes; i++) {
    rand_vector_.push_back(uniform_int(RandGen()));

    for (int i = 0; i < data_point_dimension; i++) {
      rand_line_vector.push_back(standard_normal(RandGen()));
    }

    lsh_hashes_.push_back(LshHash(width, uniform_real(RandGen()), rand_line_vector));
  }
}

int64_t LshAmplifiedHash::HashFunction(const DataPoint* data_point) const {
  int64_t hashval = 0;

  // Uses the property: (a + b) mod M = ((a mod M) + (b mod M)) mod M
  for (int i = 0, num_hashes = lsh_hashes_.size(); i < num_hashes; i++) {
    hashval = Mod(
        Mod(hashval, MODULUS)
      + Mod(rand_vector_[i] * lsh_hashes_[i].HashFunction(data_point), MODULUS),
      MODULUS
    );
  }

  return hashval;
}
