#ifndef LEXER_H
#define LEXER_H

struct TVals {
	std::string num_str;
	std::string ident_str;
	int int_num_val;
	double double_num_val;
	//int last_char;
};

enum class Token {
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,
	tok_identifier = -4,
	tok_integer = -5,
	tok_floating_point = -6,
	tok_generic = -7,
	tok_undefined = -999
};

std::pair<int, TVals*> get_token(std::string, const char*);

int string_to_int(const std::string, int&);
int string_to_double(const std::string, double&);

#endif
