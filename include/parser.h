#include "ast_node.h"
#include "ast_variable.h"
#include "lexer.h"

#ifndef PARSER_H
#define PARSER_H

class Parser {
 public:
  const char* iterator;
  bool error = false;
  bool found_main = false;

  Parser(const char* iterator) : iterator(iterator) {
    // @@@Incomplete: An enum with different error types would be more helpful
    error = false;
    found_main = false;
  };

  ASTNode* parse_binop_rhs(int, ASTNode*);
  ASTNode* parse_primary_expr();
  ASTNode* parse_top_level_expr();
  ASTNode* parse_paren_expr();
  ASTNode* parse_variable_expr();
  ASTNode* parse_function_prototype(ASTVariable* prototype);

  void UNIMPLEMENTED();
};

#endif
