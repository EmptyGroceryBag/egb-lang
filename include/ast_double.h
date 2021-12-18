#include "ast_node.h"

#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

class ASTDouble : public ASTNode{
	public:
		double value;

		ASTDouble(double);

		virtual std::string to_string();
		//@@@ return nullptr?
		virtual std::string to_string(int) { return nullptr; };
};

#endif