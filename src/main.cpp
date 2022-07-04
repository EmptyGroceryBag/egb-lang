/*
Copyright 2021 Joey Meisner

This file is part of egb-lang.

egb-lang is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

egb-lang is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along
with egb-lang.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ast_function.h"
#include "ast_global_block.h"
#include "ast_node.h"
#include "cmake_config.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_os_ostream.h"
#include "parser.h"

using namespace llvm;

// Validate input file extension
int arg_validate_file_ext(const char* file_str, const std::string valid_ext) {
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

int main(int argc, char* argv[]) {
  std::cout << "egb-lang " << el_VERSION_MAJOR << "." << el_VERSION_MINOR
            << std::endl;

  if (argc < 2) {
    printf("\n");
    std::cout << "Usage: el FILE [FILE ...]" << std::endl;
    return 1;
  }

  // Parse cmdline arguments
  std::string input_file;
  std::string output_file;
  if (argc >= 2) {
    for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'o') {
        if (!argv[i+1]) {
          std::cout << "Error: File name expected after command line switch \'-o\'" << std::endl;
          return 1;
        }

        output_file = argv[i + 1];
        std::cout << "output_file = " << output_file << std::endl;
        continue;
      } else if (isalpha(argv[i][0]) && input_file.empty()) {
        input_file = argv[i];
        std::cout << "input_file = " << input_file << std::endl;
      }
    }
  }

  std::FILE* ifs;
  bool file_err = false;
  ifs = std::fopen(input_file.c_str(), "r+");
  if (!ifs) {
    std::cout << "Error: Could not open file \"" << input_file << "\"" << std::endl;
    file_err = true;
  }
  if (arg_validate_file_ext(input_file.c_str(), "el")) {
    std::cout << "\"" << input_file << "\"" ": Unrecognized file format" << std::endl;
    file_err = true;
  }
  
  if (file_err)
    return 1;

  std::cout << "Opened " << input_file << std::endl;

  char next_char;
  std::string buffer;
  while ((next_char = fgetc(ifs)) != EOF) buffer += next_char;

  std::fclose(ifs);

  std::cout << "Closed " << input_file << std::endl;

  const char* iterator = &buffer[0];
  Parser p(iterator);

  p.insertion_stack.push(&ASTGlobalBlock::get_global_block().global_scope);

  while (peek(p.iterator).token_type != static_cast<int>(Token::tok_eof)) {
    p.parse_top_level_expr();
  }
  std::cout << "parsed " << (*p.insertion_stack.top()).size()
            << " top level node(s)" << std::endl;

  if (!p.found_main) {
    std::cout << "Error: Could not find entry point \"main\"." << std::endl;
    p.error = true;
  }

  if (p.error) {
    std::cout << "Errors occurred. Exiting" << std::endl;
    return 1;
  }

  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);

  for (ASTNode* n : (*p.insertion_stack.top())) {
    if (!n) continue;

    ASTFunction* function = dynamic_cast<ASTFunction*>(n);
    if (function)
      function->code_gen(context, builder, llvm_module);
    else
      n->code_gen(context, builder);
  }

  //#define DEBUG_PRINT_IR
  //#ifdef DEBUG_PRINT_IR
  std::ostringstream term_output;
  raw_os_ostream output_stream(term_output);

  llvm_module.print(output_stream, nullptr);
  // Debug
  //std::cout << term_output.str() << std::endl;

  if (output_file.empty()) {
    output_file = "a.ll";
  }

  std::FILE* ofs = std::fopen(output_file.c_str(), "w+");
  if (!ofs) {
    std::cout << "Could not open " << output_file << " for writing" << std::endl;
    return errno;
  }
  
  int write_result = std::fwrite (
    term_output.str().data(),
    sizeof(term_output.str().data()[0]),
    term_output.str().size(),
    ofs
  );
  std::fclose(ofs);

  if (!write_result) {
    std::cout << "Error writing to " << output_file << std::endl;
    return errno;
  }
  //#endif

  return 0;
}
