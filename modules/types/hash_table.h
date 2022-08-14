#ifndef MODULES_TYPES_HASH_TABLE_H_
#define MODULES_TYPES_HASH_TABLE_H_

#include <vector>
#include <utility>
#include <cstdint>

#include "data_point.h"
#include "common_utils.h"

template<class HashWrapper>
class HashTable {
 public:
  HashTable(int table_size, HashWrapper hash_wrapper)
      : table_size_(table_size), hash_wrapper_(hash_wrapper) {
    for (int i = 0; i < table_size_; i++) {
      std::vector<std::pair<const DataPoint*, int64_t> > empty_bucket;
      table_.push_back(empty_bucket);
    }
  }

  inline void Insert(const DataPoint* data_point) {
    int64_t hash_val = GetHashVal(data_point);
    table_.at(Mod(hash_val, table_size_))
          .push_back(std::pair<const DataPoint*, int64_t>(data_point, hash_val));
  }

  // Returns a pointer to the bucket where 'data_point' would be inserted.
  inline const std::vector<std::pair<const DataPoint*, int64_t> >*
  GetBucket(const DataPoint* data_point) {
    return &table_.at(Mod(GetHashVal(data_point), table_size_));
  }

  // Returns a pointer to the bucket with index 'bucket_idx'.
  inline const std::vector<std::pair<const DataPoint*, int64_t> >*
  GetBucket(int bucket_idx) {
    return &table_.at(bucket_idx);
  }

  inline int64_t GetHashVal(const DataPoint* data_point) {
    return hash_wrapper_.HashFunction(data_point);
  }

 private:
  int table_size_;
  HashWrapper hash_wrapper_; // Must contain: int64_t HashFunction(const DataPoint*)
  std::vector<std::vector<std::pair<const DataPoint*, int64_t> > > table_;
};

#endif // MODULES_TYPES_HASH_TABLE_H_
