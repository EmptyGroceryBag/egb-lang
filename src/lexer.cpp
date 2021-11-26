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

enum Token{
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,
	tok_identifier = -4,
	tok_number = -5
};

static std::string ident_str;
static double num_val;

int get_token(std::FILE* file){
	ident_str = "";
	static int last_char = ' ';

	while(isspace(last_char)){
		last_char = fgetc(file);
		//debug
		//std::cout << (char)last_char << std::endl;
	}

	if(isalpha(last_char)){
		/*
		identifiers must begin with a letter and can end with any combinations of
		letters and numbers
		*/
		ident_str += last_char;
		last_char = fgetc(file);
		//debug
		//std::cout << (char)last_char << std::endl;

		while(isalnum(last_char)){
			ident_str += last_char;
			last_char = fgetc(file);
			//debug
			//std::cout << (char)last_char << std::endl;
		}

		if(ident_str == "def"){
			return tok_def;
		}
		if(ident_str == "extern"){
			return tok_extern;
		}

		return tok_identifier;
	}

	if(last_char == -1)
		return tok_eof;

	return 0;
}
