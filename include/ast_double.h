#include "ast_node.h"

#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

class ASTDouble : public ASTNode{
	public:
		double double_val;

		ASTDouble(double);
};

#endif