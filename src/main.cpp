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
#include <vector>

#include "ast_node.h"
#include "cmake_config.h"
#include "parser.h"

int main(int argc, char* argv[]) {
  std::cout << "egb-lang " << el_VERSION_MAJOR << "." << el_VERSION_MINOR
            << std::endl;

  std::FILE* ifs;
  std::string buffer;

  if (argc < 2) {
    printf("\n\n");
    std::cerr << "Error: No input files specified" << std::endl;
    std::cout << "Usage: el FILE [FILE ...]" << std::endl;
    return 1;
  }

  ifs = std::fopen(argv[1], "r");
  if (!ifs) return errno;

  std::cout << "Opened " << argv[1] << std::endl;

  char next_char;
  while ((next_char = fgetc(ifs)) != EOF) buffer += next_char;

  std::vector<ASTNode*> tree;
  ASTNode* node;

  const char* iterator = &buffer[0];
  Parser p(iterator);

  while ((node = p.parse_top_level_expr()) != nullptr) tree.push_back(node);

  //@@@ Do codegen somewhere here

  std::fclose(ifs);
  std::cout << "Closed " << argv[1] << std::endl;

  return 0;
}
