#include "cla_parser.h"

#include <string>
#include <unordered_map>

ClaParser::ClaParser(int argc, char* argv[]) {
  valid_args_ = true;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      valid_args_ = false; // Current option doesn't start with '-'
      return;
    } else if (argv[i + 1] != nullptr && argv[i + 1][0] != '-') {
      i++;
      option_to_arg_[ std::string(argv[i - 1]) ] = std::string(argv[i]);
    } else {
      option_to_arg_[ std::string(argv[i]) ] = "no_argument";
    }
  }
}

std::string ClaParser::GetArgument(std::string option) const {
  std::unordered_map<std::string, std::string>::const_iterator entry;
  if ((entry = option_to_arg_.find(option)) != option_to_arg_.end()) {
    return entry->second;
  }

  static std::string empty_string("");
  return empty_string;
}
