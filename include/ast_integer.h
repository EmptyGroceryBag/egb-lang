#include "ast_node.h"

#ifndef AST_INTEGER_H
#define AST_INTEGER_H

class ASTInteger : public ASTNode{
	public:
		int int_val;

		ASTInteger(int);
};

#endif
