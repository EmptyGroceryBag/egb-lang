#include <iostream>

#include <gtest/gtest.h>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "ast_integer.h"
#include "ast_double.h"

// @@@ Only a dumb test! Change later!!!
/*
Right now these tests check to see if the correct node type is
returned, as well as the token's value.
*/
TEST(TestNumberExpression, TestInteger){
	std::string buffer = "1234";
	char *iterator = &buffer[0];
	std::pair<int, TVals*> t = get_token(buffer, iterator);

	//should return an ASTInteger
	ASTNode* n = parse_expr(t);

	ASTInteger* check = dynamic_cast<ASTInteger*>(n);
	if(check){
		ASSERT_TRUE(check);
		ASSERT_EQ(check->int_val, 1234);
	}else{
		std::cerr << "ParseExpressionTest.cpp: Attempted to dereference nullptr" << std::endl;
	}
}

TEST(TestNumberExpression, TestFloatingPoint) {
	std::string buffer = "12.34";
	char* iterator = &buffer[0];
	std::pair<int, TVals*> t = get_token(buffer, iterator);

	//should return an ASTDouble
	ASTNode* n = parse_expr(t);

	ASTDouble* check = dynamic_cast<ASTDouble*>(n);
	if (check) {
		ASSERT_TRUE(check);
		ASSERT_EQ(check->double_val, 12.34);
	}
	else {
		std::cerr << "ParseExpressionTest.cpp: Attempted to dereference nullptr" << std::endl;
	}
}