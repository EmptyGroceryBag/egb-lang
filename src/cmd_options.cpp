#include <cmd_options.h>
#include <iostream>
#include <string>
#include <vector>

int CmdOptions::validate_file_ext(const std::string file_str, const std::string valid_ext) {
  std::string raw_ext;
  for (int i = file_str.size() - 1; i >= 0; i--) {
    if (file_str[i] == '.')
      break;
    raw_ext += file_str[i];
  }

  std::string ext;
  for (int i = 0, j = raw_ext.size() - 1; i < raw_ext.size(); i++, j--)
    ext += raw_ext[j];

  if ((ext == valid_ext) == false)
    return 1;

  return 0;
}

int CmdOptions::parse_option_list(const std::vector<CmdOptions::Option>* option_list) {
  if (!option_list)
    return 1;

  int output_file_count = 0;

  for (CmdOptions::Option o : *option_list) {
    switch (o.kind) {
    case CmdOptions::OptionKind::output_file:
      if (output_file_count > 1)
        break;
      output_file_name = o.arg;
      output_file_count++;
      break;
    case CmdOptions::OptionKind::input_file:
      input_file_name = o.arg;
      given_input_file = true;
      break;
    case CmdOptions::OptionKind::emit_ir:
      should_emit_ir = true;
    }
  }

  if (!given_input_file) {
    std::cout << "Error: No input files provided" << std::endl;
    std::cout << "Usage: el FILE [FILE ...]" << std::endl;
    return 1;
  }

  return 0;
}

std::vector<CmdOptions::Option>* CmdOptions::parse_options(const int argc, char** argv) {
  CmdOptions::OptionKind kind;

  std::vector<Option>* option_list = new std::vector<Option>();
  if (argc >= 2) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == 'o') {
          if (i + 1 >= argc || argv[i + 1][0] == '-') {
            std::cout << "Error: Invalid arguments for switch \"-o\"" << std::endl;
            std::cout << "Usage: \"el.exe FILE [FILE ...] [-o output_file_name]\"" << std::endl;
            return nullptr;
          }
          std::string arg = argv[++i];
          CmdOptions::Option option { CmdOptions::OptionKind::output_file, arg };
          option_list->push_back(option);
          continue;
        } else if (strcmp(argv[i] + 1, "emit-ir") == 0) {
          CmdOptions::Option option { CmdOptions::OptionKind::emit_ir };
          option_list->push_back(option);
          continue;
        }
      } else if (isalpha(argv[i][0])) {
        CmdOptions::Option option { CmdOptions::OptionKind::input_file, argv[i] };
        option_list->push_back(option);
      }
    }
  }

  return option_list;
}
