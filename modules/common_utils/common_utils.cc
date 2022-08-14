#include "common_utils.h"

#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>
#include <unordered_set>

#include "data_point.h"

std::mt19937& RandGen() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return gen;
}

int64_t Mod(int64_t a, int64_t b) {
  int64_t m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }

  return m;
}

double Stopwatch(StopwatchState state) {
  static bool working = false;
  static std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

  if (state == BEGIN) {
    working = true;
    start = std::chrono::high_resolution_clock::now();
    return 0.0;
  }

  if (working) {
    working = false;
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - start;
    return fp_ms.count();
  }

  return -1; // Invalid call
}

double Dot(const std::vector<double>& a, const std::vector<double>& b) {
  double sum = 0;
  int n_dimensions = a.size();
  for (int i = 0; i < n_dimensions; i++) {
    sum += a[i] * b[i];
  }

  return sum;
}

static double ManhattanDist(const std::vector<double>& a, const std::vector<double>& b) {
  double res = 0.0;
  int n_dimensions = a.size();
  for (int i = 0; i < n_dimensions; i++) {
    res += std::abs(a[i] - b[i]);
  }

  return res;
}

static double EuclideanDist(const std::vector<double>& a, const std::vector<double>& b) {
  double diff, res = 0.0;
  int n_dimensions = a.size();
  for (int i = 0; i < n_dimensions; i++) {
    diff = a[i] - b[i];
    res += diff * diff;
  }

  return sqrt(res);
}

double Dist(const std::vector<double>& a, const std::vector<double>& b, DistType dist_type) {
  switch (dist_type) {
    case MANHATTAN:
      return ManhattanDist(a, b);
    case EUCLIDEAN:
    default:
      return EuclideanDist(a, b);
  }
}

double AvgDist(const std::vector<const DataPoint*>& data_points, DistType dist_type) {
  double avg = 0.0;
  int n_data_points = data_points.size();

  for (int i = 0; i < n_data_points - 1; i++) {
    for (int j = i + 1; j < n_data_points; j++) {
      avg += Dist(data_points[i]->coordinates_, data_points[j]->coordinates_, dist_type);
    }
  }

  // The number of distances calculated is n_data_points choose 2, so we divide 'avg' by that
  return 2.0 * avg / (n_data_points * (n_data_points - 1));
}

DataPoint AvgDataPoint(const std::vector<const DataPoint*>& data_points) {
  int n_data_points = data_points.size();
  int n_dimensions = data_points[0]->coordinates_.size();

  DataPoint avg_data_point(std::string(""), data_points[0]->coordinates_);
  std::vector<double>& avg_coordinates = avg_data_point.coordinates_;

  for (int i = 0; i < n_dimensions; avg_coordinates[i++] /= n_data_points) {
    for (int j = 1; j < n_data_points; j++) {
      avg_coordinates[i] += data_points[j]->coordinates_[i];
    }
  }

  return avg_data_point;
}

std::vector<const DataPoint*> RandomSample(int sample_size,
                                           const std::vector<const DataPoint*>& data_points) {
  std::unordered_set<const DataPoint*> sample;
  std::uniform_int_distribution<> uniform_int(0, data_points.size() - 1);

  int n_data_points = data_points.size();
  while (((int) sample.size()) < n_data_points && ((int) sample.size()) < sample_size) {
    sample.insert( data_points[ uniform_int(RandGen()) ] );
  }

  return std::vector<const DataPoint*>(sample.begin(), sample.end());
}
