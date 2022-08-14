#include "config.h"

#include <string>
#include <iomanip>
#include <iostream>

#include "cla_parser.h"

Configuration::Configuration(int argc, char* argv[])
    : num_hashes(4),
      num_tables(5),
      num_neighbors(1),
      search_radius(10000.0),
      in_path(""),
      qr_path(""),
      out_path("") {
  ClaParser cla_parser(argc, argv);
  std::cout << std::fixed << std::setprecision(3);

  if (!cla_parser.ValidArgs()) {
    std::cerr << "\nInvalid arguments, proceeding with default configuration..\n";
    return;
  }

  std::string arg = cla_parser.GetArgument(std::string("-k"));
  if (!arg.empty()) {
    num_hashes = std::stoi(arg);
  }

  arg = cla_parser.GetArgument(std::string("-L"));
  if (!arg.empty()) {
    num_tables = std::stoi(arg);
  }

  arg = cla_parser.GetArgument(std::string("-N"));
  if (!arg.empty()) {
    num_neighbors = std::stoi(arg);
  }

  arg = cla_parser.GetArgument(std::string("-R"));
  if (!arg.empty()) {
    search_radius = std::stof(arg);
  }

  // We don't mind storing the empty string, since paths are "" by default
  in_path = cla_parser.GetArgument(std::string("-i"));
  qr_path = cla_parser.GetArgument(std::string("-q"));
  out_path = cla_parser.GetArgument(std::string("-o"));
}
