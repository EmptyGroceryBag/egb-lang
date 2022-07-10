#include <cmd_options.h>
#include <iostream>
#include <string>
#include <vector>

int CmdOptions::arg_validate_file_ext(const char* file_str, const std::string valid_ext) {
  std::string raw_ext;
  for (int i = strlen(file_str) - 1; i >= 0; i--) {
    if (file_str[i] == '.')
      break;
    raw_ext += file_str[i];
  }

  std::string ext;
  for (int i = 0, j = strlen(raw_ext.c_str()) - 1; i < strlen(raw_ext.c_str()); i++, j--)
    ext += raw_ext[j];

  if ((ext == valid_ext) == false)
    return 1;

  return 0;
}

std::vector<CmdOptions::Option>* CmdOptions::parse_options(const int argc, char** argv) {
  CmdOptions::OptionKind kind;

  std::vector<Option>* option_list = new std::vector<Option>();
  if (argc >= 2) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == 'o') {
          CmdOptions::Option option{CmdOptions::OptionKind::output_file};
          option_list->push_back(option);
          continue;  
        } else if (strcmp(argv[i] + 1, "emit-ir") == 0) {
          CmdOptions::Option option{CmdOptions::OptionKind::emit_ir};
          option_list->push_back(option);
          continue;
        }
      }
    }
  }
  
  return option_list;
}