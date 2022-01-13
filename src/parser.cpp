#include "parser.h"
#include <iostream>
#include <string>
#include <unordered_map>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
#include "ast_node.h"
#include "lexer.h"
#include "t_vals.h"
#include "tok_val_pair.h"

std::unordered_map<int, int> op_prec = {
    {'\\', 0}, {'+', 10}, {'-', 20}, {'*', 30}};

static TokValPair* token;

/*
int get_token_prec(Token token) {

}
*/

void Parser::UNIMPLEMENTED() {
  std::cout << "Not implemented yet" << std::endl;
}

ASTNode* Parser::parse_primary_expr() {
  ASTNode* lhs;

  switch (token->token_type) {  // primary expressions
    case static_cast<int>(Token::tok_integer):
      //@@@ check to see if nullptr
      lhs = new ASTInteger(token->token_value->int_num_val);
      return parse_binop_rhs(0, lhs);

    case static_cast<int>(Token::tok_floating_point):
      lhs = new ASTDouble(token->token_value->double_num_val);
      return parse_binop_rhs(0, lhs);

    case '(':
      return parse_paren_expr();

    default:
      //@@@
      UNIMPLEMENTED();
      return nullptr;
  }
}

ASTNode* Parser::parse_binop_rhs(int expr_prec, ASTNode* lhs) {
  for (;;) {
    // check to see if next token is a binary operator
    TokValPair* peeked_token = peek(iterator);
    if (peeked_token->token_type == ')') {
      iterator++;
    }
    char token_prec = op_prec[peeked_token->token_type];
    if (token_prec == 0) {
      token_prec = -1;
    }

    if (token_prec < expr_prec) {
      return lhs;
    }

    // if we got this far, we have a binary operator
    iterator++;
    token = get_token(iterator);
    ASTNode* rhs = parse_primary_expr();

    int next_prec = op_prec[token->token_type];
    if (next_prec == 0) {
      next_prec = -1;
    }
    if (token_prec < next_prec) {
      rhs = parse_binop_rhs(token_prec + 1, rhs);
    }

    lhs = new ASTBinExpr(peeked_token->token_type, lhs, rhs);
  }
}

ASTNode* Parser::parse_paren_expr() {
  token = get_token(iterator);  // eat '('
  ASTNode* n = parse_primary_expr();

  /*
  if(get_token(iterator)->token_type != ')'){
          //std::cerr << "Expected closing parenthesis" << std::endl;
          return nullptr;
  }
  */

  return n;
}

ASTNode* Parser::parse_top_level_expr() {
  token = get_token(iterator);

  ASTNode* lhs = nullptr;

  switch (token->token_type) {
    case static_cast<int>(Token::tok_def):
      UNIMPLEMENTED();
      return nullptr;

    case static_cast<int>(Token::tok_extern):
      UNIMPLEMENTED();
      return nullptr;

    case static_cast<int>(Token::tok_eof):
      return nullptr;

    default:                       // number or identifier
      lhs = parse_primary_expr();  // lhs
      return lhs;
  }
}
