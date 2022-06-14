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
#include "cmake_config.h"
#include "parser.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_os_ostream.h"

using namespace llvm;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
  Parser p(reinterpret_cast<const char*>(Data));
  
  std::vector<ASTNode*> syntax_tree;
  //ASTNode* current_node = nullptr;
  while (peek(p.iterator).token_type != static_cast<int>(Token::tok_eof)) {
    syntax_tree.push_back(p.parse_top_level_expr());
  }
  std::cout << "parsed " << syntax_tree.size() << " node(s)" << std::endl;

  if (!p.found_main) {
    std::cout << "Error: Could not find entry point \"main\"." << std::endl;
    p.error = true;
  }

  if (p.error) {
    std::cout << "Errors occurred. Exiting" << std::endl;

    for (ASTNode* n : syntax_tree) {
      delete n;
    }

    syntax_tree.clear();

    return 1;
  }

  std::cout << "Generating code" << std::endl;
  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);

  for (ASTNode* n : syntax_tree) {
    if (!n) {
      break;
    }

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
