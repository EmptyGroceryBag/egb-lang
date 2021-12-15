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

/*
 * Lexer algorithm:
 * - Get next character, discard if whitespace
 * - If char is number, enter numeric loop
 * - If char is letter, enter identifier loop
 * - If char is #, keep getting characters until end of line
 *   - At the next line, call get_token() and return
 * - If char is EOF, return token_EOF
 * - If char is not one of these, simply return its ASCII value
 */

#include <iostream>
#include <string>
//#include <memory>
#include <utility>
#include <cstdio>

#include "lexer.h"

std::pair<int, TVals*> get_token(std::string input_str, char*& iterator){
	TVals* vals = new TVals();
	//@@@
	//static std::unique_ptr<TVals> vals = std::make_unique<TVals>();
	vals->ident_str = "";
	vals->num_str = "";

	std::pair<int, TVals*> ret;
	ret.second = vals;

	// Numbers
	vals->last_char = *(iterator++);
	while(isspace(vals->last_char)){
		vals->last_char = *(iterator++);
	}
	
	if(isdigit(vals->last_char) || vals->last_char == '.' || vals->last_char == '-'){
		vals->num_str += vals->last_char;
		vals->last_char = *iterator;

		while(isdigit(vals->last_char) || vals->last_char == '.'){
			vals->last_char = *(iterator++);
			vals->num_str += vals->last_char;
		}

		/*
		Note: We are now doing our own integer parsing. Remember side
		effects
		*/
		try{
			if(string_to_int(vals->num_str, vals->int_num_val)){
				if(string_to_double(vals->num_str, vals->double_num_val)){
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
	if(isalpha(vals->last_char)){
		/*
		identifiers must begin with a letter and can end with any combinations of
		letters and numbers
		*/
		vals->ident_str += vals->last_char;
		vals->last_char = *(iterator++);

		while(isalnum(vals->last_char)){
			vals->ident_str += vals->last_char;
			vals->last_char = *(iterator++);
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

	// Ignore comments
	if(vals->last_char == '#'){
		while((vals->last_char = *(iterator++)) != '\n')
			;
		ret.first = static_cast<int>(Token::tok_undefined);
		return ret;
	}

	if (vals->last_char == '\0'){
		ret.first = static_cast<int>(Token::tok_eof);
		return ret;
	}
	
	ret.first = static_cast<int>(vals->last_char);
	return ret;
}

int string_to_int(std::string input_num, int& output_num){
	int num_value;
	int ret_value;

	//check if there's a decimal point
	try{
		if((ret_value = input_num.find('.')) != -1)
			throw ret_value;				
		}
	catch(...){ 
		return ret_value;
	}

	output_num = stoi(input_num);
	return 0;
}

int string_to_double(std::string input_num, double& output_num){
	int num_value;
	int ret_value;
	int found_decimal_point = input_num.find('.');

	//check for duplicate decimal points
	try{
		for(int i = found_decimal_point + 1; i < input_num.size(); i++){
			if(input_num[i] == '.'){
				throw (ret_value = i);
			}
		}
	}
	catch(...){ 
		return ret_value;
	}

	output_num = stod(input_num);
	return 0;
}
