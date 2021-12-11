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
#include <cstdio>

#include "lexer.h"

static std::string num_str = "";
static std::string ident_str;
static int int_num_val = 0;
static double double_num_val = 0.0;
static int last_char = 0;

//@@@
static int cursor = 0;

Token get_token(std::string input_str, char*& iterator){
	ident_str = "";
	num_str = "";

	// Numbers
	last_char = static_cast<char>(*(iterator++));
	while(isspace(last_char)){
		last_char = *(iterator++);
	}
	
	if(isdigit(last_char) || last_char == '.' || last_char == '-'){
		num_str += last_char;
		last_char = *iterator;

		while(isdigit(last_char) || last_char == '.'){
			last_char = *(iterator++);
			num_str += last_char;
		}

		/*
		Note: We are now doing our own integer parsing. Remember side
		effects
		*/
		if(string_to_int(num_str, int_num_val)){
			if(string_to_double(num_str, double_num_val)){
					return Token::tok_undefined;
			}else{
					return Token::tok_floating_point;
			}
		}

		return Token::tok_integer;
	}

	// Identifiers
	if(isalpha(last_char)){
		/*
		identifiers must begin with a letter and can end with any combinations of
		letters and numbers
		*/
		ident_str += last_char;
		last_char = *(iterator++);

		while(isalnum(last_char)){
			ident_str += last_char;
			last_char = *(iterator++);
		}

		if(ident_str == "def"){
			return Token::tok_def;
		}
		if(ident_str == "extern"){
			return Token::tok_extern;
		}

		return Token::tok_identifier;
	}

	// Ignore comments
	if(last_char == '#'){
		while((last_char = *(iterator++)) != '\n')
			;

		return Token::tok_undefined;
	}

	if(last_char == '\0')
		return Token::tok_eof;
	
	return Token::tok_undefined;
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
