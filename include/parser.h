#include "ast_node.h"
#include "lexer.h"

#ifndef PARSER_H
#define PARSER_H

ASTNode* parse_expr(std::pair<int, TVals*>);
ASTNode* parse_int_expr(TVals*);
ASTNode* parse_double_expr(TVals*);

#endif