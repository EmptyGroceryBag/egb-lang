#include <iostream>
#include <string>
#include "ast_number.h"
#include "lexer.h"

ASTNode* parse_expr(int token){
	switch(token){
	case static_cast<int>(Token::tok_integer):
		std::cout << "Parsed Integer Token" << std::endl;
		//TODO call parse_num_expr()
		return new ASTNumber(0);

	case static_cast<int>(Token::tok_floating_point):
		std::cout << "Parsed Double Token" << std::endl;
		//TODO call parse_num_expr()
		return new ASTNumber(0.0);

	default:
		std::cout << "Parsed Other Token" << std::endl;
		//@@@
		return nullptr;
	}
}
