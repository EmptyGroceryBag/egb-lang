#ifndef CMD_OPTIONS_H
#define CMD_OPTIONS_H

#include <iostream>
#include <string>
#include <vector>

struct CmdOptions {

  enum OptionKind {
    output_file,
    emit_ir
  };

  struct Option {
    OptionKind kind;
    std::vector<std::string> option_args;
  };

  int arg_validate_file_ext(const char*, const std::string);
  std::vector<Option>* parse_options(const int, char**);
};

#endif