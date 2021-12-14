#include <iostream>
#include "ast_node.h"

#ifndef AST_NUMBER_H
#define AST_NUMBER_H

class ASTNumber : public ASTNode{
	public:
		double double_val;
		int int_val;

		ASTNumber(int);
		ASTNumber(double);
};

#endif
