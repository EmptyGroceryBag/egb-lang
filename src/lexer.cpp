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

Token get_token(std::FILE* file){

	ident_str = "";
	num_str = "";

	last_char = fgetc(file);
	while(isspace(last_char)){
		last_char = fgetc(file);
	}
	
	if(isdigit(last_char) || last_char == '.'){
		num_str += last_char;

		while(isdigit(last_char) || last_char == '.'){
			last_char = fgetc(file);
			num_str += last_char;
		}

		/*
		Note: We are now doing our own integer parsing. Remember side
		effects
		*/
		std::cout << num_str << std::endl;
		if(string_to_int(num_str, int_num_val))
			// Later, we'll want to call string_to_double instead
			return Token::tok_undefined;

		return Token::tok_number;
	}

	if(isalpha(last_char)){
		/*
		identifiers must begin with a letter and can end with any combinations of
		letters and numbers
		*/
		ident_str += last_char;
		last_char = fgetc(file);

		while(isalnum(last_char)){
			ident_str += last_char;
			last_char = fgetc(file);
		}

		if(ident_str == "def"){
			return Token::tok_def;
		}
		if(ident_str == "extern"){
			return Token::tok_extern;
		}

		return Token::tok_identifier;
	}


	if(last_char == EOF)
		return Token::tok_eof;
	
	return Token::tok_undefined;
}

int string_to_int(std::string input_num, int& output_num){
	//check if there's a decimal point
	int num_value;
	int ret_value;

	try{
		if((ret_value = input_num.find('.')) != -1)
			throw ret_value;				
		}
	catch(...){ 
		std::cout 
			<< "string_to_int(): expected integer, but found floating point"
			<< "use string_to_double() instead"
			<< std::endl;
		return ret_value;
	}

	output_num = stoi(input_num);
	return 0;
}

//TODO
double string_to_double(std::string){
	return 0.0;
}
