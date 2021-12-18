#include "ast_node.h"
#include "lexer.h"

#ifndef AST_BIN_EXP_H
#define AST_BIN_EXP_H

class ASTBinExpr : public ASTNode {
public:
	ASTNode* lhs;
	ASTNode* rhs;
	int op;

	ASTBinExpr(int op, ASTNode* lhs, ASTNode* rhs);

	virtual std::string to_string(int);
	virtual std::string to_string();
};

#endif
