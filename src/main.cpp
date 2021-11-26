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
#include <fstream>

#include "cmake_config.h"
#include "parser.h"

int main(int argc, char* argv[]){
	std::cout << "egb-lang " 
		<< el_VERSION_MAJOR << "." 
		<< el_VERSION_MINOR << std::endl;

	get_token();

	std::ifstream ifs;

	if(argc < 2){
		std::cerr << "Error: No input files specified" << std::endl;
		return 1;
	}

	std::ios_base::iostate exception_mask = ifs.exceptions() | std::ios::failbit;
	ifs.exceptions(exception_mask);

	try{
		ifs.open(argv[1], std::ios_base::in);
	}catch (std::ios_base::failure& e){
		std::cerr << "Could not open file " << argv[1] << std::endl
			<< e.what() << std::endl << e.code().value();
		return e.code().value();
	}

	std::cout << "Opened " << argv[1] << std::endl;
	ifs.close();
	std::cout << "Closed " << argv[1] << std::endl;

	return 0;
}
