#include <iostream>
#include <string>

#include "parser.h"
#include "ast_node.h"
#include "ast_integer.h"
#include "ast_double.h"
#include "t_vals.h"
#include "tok_val_pair.h"
#include "lexer.h"

ASTNode* parse_expr(TokValPair* token){
	switch(token->token_type){
	case static_cast<int>(Token::tok_integer):
		return parse_int_expr(token->token_value);

	case static_cast<int>(Token::tok_floating_point):
		return parse_double_expr(token->token_value);

	default:
		std::cout << "Parsed Other Token" << std::endl;
		//@@@
		return nullptr;
	}
}

ASTNode* parse_int_expr(TVals* vals){
	//debug
	std::cout << "parser.cpp: vals->int_num_val = " << vals->int_num_val << std::endl;
	return new ASTInteger(vals->int_num_val);
}

ASTNode* parse_double_expr(TVals* vals){
	//debug
	std::cout << "parser.cpp: vals->double_num_val = " << vals->double_num_val << std::endl;
	return new ASTDouble(vals->double_num_val);
}