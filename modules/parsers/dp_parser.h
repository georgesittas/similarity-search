#ifndef MODULES_PARSERS_DP_PARSER_H_
#define MODULES_PARSERS_DP_PARSER_H_

#include <vector>
#include <fstream>

#include "data_point.h"

// Parses 'file', which has 'delimiter'-separated values representing a data point
// (as modeled in data_point.h), and returns a vector with all of the data points.
std::vector<const DataPoint*> Parse(std::fstream& file, char delimiter);

#endif // MODULES_PARSERS_DP_PARSER_H_
