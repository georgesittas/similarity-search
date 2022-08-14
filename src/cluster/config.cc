#include "config.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "cla_parser.h"

Configuration::Configuration(int argc, char* argv[])
    : lsh_num_hashes(4),
      lsh_num_tables(3),
      cube_max_points(10),
      cube_dimension(3),
      cube_max_probes(2),
      valid_config(true) {
  ClaParser cla_parser(argc, argv);
  std::cout << std::fixed << std::setprecision(3);

  if (!cla_parser.ValidArgs()) {
    std::cerr << "\nFatal error: invalid command line arguments\n";
    valid_config = false;
    return;
  }

  in_path = cla_parser.GetArgument(std::string("-i"));
  out_path = cla_parser.GetArgument(std::string("-o"));
  method = cla_parser.GetArgument(std::string("-m"));

  std::string cf_path = cla_parser.GetArgument(std::string("-c"));

  if (in_path.empty() || cf_path.empty() || out_path.empty() || method.empty() || 
      (method != "Classic" && method != "LSH" && method != "Hypercube")) {
    std::cerr << "\nFatal error: invalid command line arguments\n";
    valid_config = false;
    return;
  }

  complete_output = !cla_parser.GetArgument(std::string("-complete")).empty();

  std::fstream config_file(cf_path, std::ios::in);
  if (config_file.fail()) {
    std::cerr << "\nFatal error: couldn't open " << cf_path << "\n";
    valid_config = false;
    return;
  }

  int n_options = 0;
  std::string line;
  std::unordered_map<std::string, int> option_to_arg;
  std::unordered_map<std::string, int>::iterator entry;

  // Read config file & map options (eg. "number_of_clusters") to their values
  while (std::getline(config_file, line)) {
    if (line.empty()) {
      continue;
    } else if (line.back() == '\r') {
      line.pop_back();
    }

    std::string option, argument;
    std::stringstream ss(line);

    std::getline(ss, option, ':');
    std::getline(ss, argument);

    if (option_to_arg.find(option) != option_to_arg.end()) {
      std::cerr << "\nFatal error: ambiguous configuration file\n";
      valid_config = false;
      return;
    }

    option_to_arg[option] = std::stoi(argument);
  }

  entry = option_to_arg.find(std::string("number_of_clusters"));
  if (entry != option_to_arg.end()) {
    n_options++;
    num_clusters = entry->second;
  } else {
    std::cerr << "\nFatal error: number of clusters missing from configuration file\n";
    valid_config = false;
    return;
  }

  entry = option_to_arg.find(std::string("number_of_vector_hash_tables"));
  if (entry != option_to_arg.end()) {
    n_options++;
    lsh_num_tables = entry->second;
  }

  entry = option_to_arg.find(std::string("number_of_vector_hash_functions"));
  if (entry != option_to_arg.end()) {
    n_options++;
    lsh_num_hashes = entry->second;
  }

  entry = option_to_arg.find(std::string("max_number_M_hypercube"));
  if (entry != option_to_arg.end()) {
    n_options++;
    cube_max_points = entry->second;
  }

  entry = option_to_arg.find(std::string("number_of_hypercube_dimensions"));
  if (entry != option_to_arg.end()) {
    n_options++;
    cube_dimension = entry->second;
  }

  entry = option_to_arg.find(std::string("number_of_probes"));
  if (entry != option_to_arg.end()) {
    n_options++;
    cube_max_probes = entry->second;
  }

  if (n_options != ((int) option_to_arg.size())) {
    std::cerr << "\nFatal error: invalid configuration file\n";
    valid_config = false;
    return;
  }

  config_file.close();
}
