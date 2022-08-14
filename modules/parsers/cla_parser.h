#ifndef MODULES_PARSERS_CLA_PARSER_H_
#define MODULES_PARSERS_CLA_PARSER_H_

#include <string>
#include <unordered_map>

class ClaParser {
 public:
  ClaParser(int argc, char* argv[]);

  // Returns true if the provided arguments are correct, false otherwise.
  inline bool ValidArgs() const { return valid_args_; }

  // Returns the argument corresponding to 'option' (or "", if option was not provided).
  std::string GetArgument(std::string option) const;

 private:
  bool valid_args_;
  std::unordered_map<std::string, std::string> option_to_arg_;
};

#endif // MODULES_PARSERS_CLA_PARSER_H_
