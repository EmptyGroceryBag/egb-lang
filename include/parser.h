#include <stack>
#include <vector>

#include "ast_global_block.h"
#include "ast_function.h"
#include "ast_node.h"
#include "ast_variable.h"
#include "lexer.h"

#ifndef PARSER_H
#define PARSER_H

class Parser {
 public:
  const char* iterator;
  bool error;
  bool found_main;

  std::stack<std::vector<ASTNode*>*> insertion_stack;

  Parser() { Parser(""); }

  Parser(const char* iterator) : iterator(iterator) {
    // Because the global scope vector is global, we must clear it
    // before attempting to use it again
    ASTGlobalBlock::get_global_block().global_scope.clear();
    insertion_stack.push(&ASTGlobalBlock::get_global_block().global_scope);

    // @@@Incomplete: An enum with different error types would be more helpful
    error = false;
    found_main = false;
  }

  ASTNode* parse_binop_rhs(int, ASTNode*);
  ASTNode* parse_primary_expr();
  ASTNode* parse_top_level_expr();
  ASTNode* parse_paren_expr();
  ASTNode* parse_variable_expr(bool);
  ASTNode* parse_function_prototype(ASTVariable*);
  ASTNode* report_error(std::string error);
  ASTVariable* check_for_redeclaration(ASTVariable*, bool);
  void parse_block_scope(ASTFunction*);

  void UNIMPLEMENTED();
};

#endif
