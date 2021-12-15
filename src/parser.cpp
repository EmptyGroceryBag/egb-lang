#include <iostream>
#include <string>

#include "parser.h"
#include "ast_node.h"
#include "ast_integer.h"
#include "ast_double.h"
#include "lexer.h"

ASTNode* parse_expr(std::pair<int, TVals*> token){
	switch(token.first){
	case static_cast<int>(Token::tok_integer):
		return parse_int_expr(token.second);

	case static_cast<int>(Token::tok_floating_point):
		return parse_double_expr(token.second);

	default:
		std::cout << "Parsed Other Token" << std::endl;
		//@@@
		return nullptr;
	}
}

ASTNode* parse_int_expr(TVals* vals){
	ASTNode *n = new ASTInteger(vals->int_num_val);
	//debug
	std::cout << "parser.cpp: vals->int_num_val = " << vals->int_num_val << std::endl;
	return n;
}

ASTNode* parse_double_expr(TVals* vals){
	ASTNode *n = new ASTDouble(vals->double_num_val);
	//debug
	std::cout << "parser.cpp: vals->double_num_val = " << vals->double_num_val << std::endl;
	return n;
}