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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ast_function.h"
#include "ast_node.h"
#include "ast_global_block.h"
#include "cmake_config.h"
#include "parser.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_os_ostream.h"

using namespace llvm;

int main(int argc, char* argv[]) {
  std::cout << "egb-lang " << el_VERSION_MAJOR << "." << el_VERSION_MINOR
            << std::endl;

  std::FILE* ifs;

  if (argc < 2) {
    printf("\n\n");
    std::cerr << "Error: No input files specified" << std::endl;
    std::cout << "Usage: el FILE [FILE ...]" << std::endl;
    return 1;
  }

  ifs = std::fopen(argv[1], "r");
  if (!ifs) {
    std::cout << "Error: Could not open file \"" << argv[1] << "\"" << std::endl;
    return errno;
  }

  std::cout << "Opened " << argv[1] << std::endl;

  char next_char;
  std::string buffer;
  while ((next_char = fgetc(ifs)) != EOF) buffer += next_char;

  std::fclose(ifs);
  std::cout << "Closed " << argv[1] << std::endl;

  const char* iterator = &buffer[0];
  Parser p(iterator);

  std::vector<ASTNode*>& global_scope = ASTGlobalBlock::get_global_block().global_scope;
  p.insertion_stack.push(global_scope);

  while (peek(p.iterator).token_type != static_cast<int>(Token::tok_eof)) {
    p.parse_top_level_expr();
  }
  std::cout << "parsed " << global_scope.size() << " top level node(s)" << std::endl;

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

  for (ASTNode* n : global_scope) {
    if (!n) continue;

    ASTFunction* entry_point = dynamic_cast<ASTFunction*>(n);
    if (entry_point) entry_point->code_gen(context, builder, llvm_module);
    else n->code_gen(context, builder);
  }

//#define DEBUG_PRINT_IR
//#ifdef DEBUG_PRINT_IR
    std::ostringstream test_output;
    raw_os_ostream output_stream(test_output);

    llvm_module.print(output_stream, nullptr);
    std::cout << test_output.str() << std::endl;
//#endif

  return 0;
}
