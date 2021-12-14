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

#include <iostream>
#include <cstdio>
#include <vector>

#include "cmake_config.h"
#include "lexer.h"
#include "ast_number.h"
#include "ast_node.h"
#include "parser.h"

int main(int argc, char* argv[]){
	std::cout << "egb-lang " 
		<< el_VERSION_MAJOR << "." 
		<< el_VERSION_MINOR << std::endl;

	std::FILE* ifs;
	std::string buffer;

	if(argc < 2){
		printf("\n\n");
		std::cerr << "Error: No input files specified" << std::endl;
		std::cout << "Usage: el FILE [FILE ...]" << std::endl;
		return 1;
	}

	try{
		ifs = std::fopen(argv[1], "r");
		if (!ifs){
			throw errno;
		}
	}catch (...){
		std::cerr << "Error " << errno;
		return errno;
	}

	std::cout << "Opened " << argv[1] << std::endl;

	char next_char;
	while((next_char = fgetc(ifs)) != EOF){
		buffer += next_char;
	}

	char* iterator = &buffer[0];
	Token token;
	std::vector<ASTNode*> nodes;

	while((token = get_token(buffer, iterator)) != Token::tok_eof){
		nodes.push_back(parse_expr(static_cast<int>(token)));
	}

	std::fclose(ifs);
	std::cout << "Closed " << argv[1] << std::endl;

	return 0;
}
