#ifndef MODULES_COMMON_UTILS_COMMON_UTILS_H_
#define MODULES_COMMON_UTILS_COMMON_UTILS_H_

#include <vector>
#include <random>
#include <cstdint>

#include "data_point.h"

typedef enum { STOP, BEGIN } StopwatchState;
typedef enum { MANHATTAN=1, EUCLIDEAN=2 } DistType;

// Returns a (mersenne twister) random number generator.
std::mt19937& RandGen();

// Returns the remainder 0 <= r < 'b' of the euclidean division 'a' / 'b'.
int64_t Mod(int64_t a, int64_t b);

// If 'state' is set to BEGIN, a stopwatch starts working and 0 is returned. Otherwise, it
// returns the time elapsed since the last call with 'state' set to BEGIN (in milliseconds).
// If stopwatch(STOP) is called without having first started the stopwatch, -1 is returned.
double Stopwatch(StopwatchState state);

// Returns the dot product of vectors 'a' and 'b'.
double Dot(const std::vector<double>& a, const std::vector<double>& b);

// Returns the 'dist_type' distance from 'a' to 'b'.
double Dist(const std::vector<double>& a,
            const std::vector<double>& b,
            DistType dist_type=EUCLIDEAN);

// Returns the average 'dist_type' distance between two data points in 'data_points'.
double AvgDist(const std::vector<const DataPoint*>& data_points, DistType dist_type=EUCLIDEAN);

// Returns the average data point corresponding to 'data_points'.
DataPoint AvgDataPoint(const std::vector<const DataPoint*>& data_points);

// Returns a random sample consisting of at most 'sample_size' elements in 'data_points'.
std::vector<const DataPoint*> RandomSample(int sample_size,
                                           const std::vector<const DataPoint*>& data_points);

#endif // MODULES_COMMON_UTILS_COMMON_UTILS_H_
