#ifndef CMD_OPTIONS_H
#define CMD_OPTIONS_H

#include <iostream>
#include <string>
#include <vector>

struct CmdOptions {

  std::string input_file_name;
  std::string output_file_name;
  bool should_emit_ir = false;

  bool given_input_file = false;

  enum OptionKind {
    output_file,
    input_file,
    emit_ir
  };

  struct Option {
    OptionKind kind;
    std::string arg;
  };

  int validate_file_ext(const std::string, const std::string);
  int parse_option_list(const std::vector<CmdOptions::Option>*);
  std::vector<Option>* parse_options(const int, char**);
};

#endif
