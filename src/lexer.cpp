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
#include <string>
#include <utility>
#include <cstdio>

#include "lexer.h"

//@@@ use more meaningful struct for return type
std::pair<int, TVals*> get_token(std::string buffer, const char* iterator){
	TVals* vals = new TVals();
	vals->ident_str = "";
	vals->num_str = "";

	std::pair<int, TVals*> ret;
	ret.second = vals;

	// Whitespace
	while(isspace(*iterator)){
		iterator++; //eat
	}
	
	// Numbers
	if(isdigit(*iterator) || *iterator == '.' || *iterator == '-'){
		vals->num_str += *(iterator++);
		
		while(isdigit(*iterator) || *iterator == '.'){
			vals->num_str += *(iterator++);
		}

		/*
		Note: We are now doing our own integer parsing. Remember side
		effects
		*/
		try{
			if(string_to_int(vals->num_str, vals->int_num_val)){
				if(string_to_double(vals->num_str, vals->double_num_val)){
					//@@@ this is stupid
					int errno_ = 1;
					throw errno_; 
				}else{
					ret.first = static_cast<int>(Token::tok_floating_point);
					return ret;
				}
			}
			ret.first = static_cast<int>(Token::tok_integer);
			return ret;
		}catch(int e){ fprintf(stderr, "Error %d: Malformed double literal", e); }
	}

	// Identifiers
	if(isalpha(*iterator)){
		/*
		identifiers must begin with a letter and can end with any combinations of
		letters and numbers
		*/
		vals->ident_str += *(iterator++);

		while(isalnum(*iterator)){
			vals->ident_str += *(iterator++);
		}

		if(vals->ident_str == "def"){
			ret.first = static_cast<int>(Token::tok_def);
			return ret;
		}
		if(vals->ident_str == "extern"){
			ret.first = static_cast<int>(Token::tok_extern);
			return ret;
		}

		ret.first = static_cast<int>(Token::tok_identifier);
		return ret;
	}

	// Ignore comments - @@@ we probably don't need to return an undefined token
	if(*iterator == '#'){
		while(*(iterator++) != '\n' || *(iterator++) != '\0')
			;
		ret.first = static_cast<int>(Token::tok_undefined);
		return ret;
	}

	if (*iterator == '\0'){
		ret.first = static_cast<int>(Token::tok_eof);
		return ret;
	}
	
	ret.first = (*iterator);
	return ret;
}

int string_to_int(const std::string input_num, int& output_num){
	int decimal_point = input_num.find('.');

	if (decimal_point != -1) {
		return decimal_point;
	}

	output_num = stoi(input_num);
	return 0;
}

int string_to_double(const std::string input_num, double& output_num){
	int first_decimal_point = input_num.find('.');
	int duplicate_decimal_point = 0;

	if((duplicate_decimal_point = input_num.find('.', first_decimal_point+1)) != -1){
		return duplicate_decimal_point;
	}

	output_num = stod(input_num);
	return 0;
}
