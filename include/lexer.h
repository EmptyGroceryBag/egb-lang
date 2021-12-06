#ifndef LEXER_H
#define LEXER_H

enum class Token{
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,
	tok_identifier = -4,
	tok_integer = -5,
	tok_floating_point = -6,
	tok_undefined = -999
};

Token get_token(std::string);

int string_to_int(std::string, int&);
int string_to_double(std::string, double&);
char get_char_in_string(std::string);

#endif
