#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_function.h"
#include "ast_integer.h"
#include "ast_node.h"
#include "ast_variable.h"
#include "data_type.h"
#include "lexer.h"
#include "parser.h"
#include "t_vals.h"
#include "tok_val_pair.h"

std::unordered_map<int, int> op_prec = {
    {'\\', 0}, {'+', 20}, {'-', 20}, {'*', 40}};

std::vector<std::string> function_symbol_table;
std::vector<std::string> variable_symbol_table;

static TokValPair* token;

int get_token_prec(const TokValPair* token) {
  if (op_prec[token->token_type] == 0) {
    return -1;
  }

  return op_prec[token->token_type];
}

void Parser::UNIMPLEMENTED() {
  std::cout << "Not implemented yet" << std::endl;
}

ASTNode* Parser::parse_primary_expr() {
  ASTNode* lhs;

  switch (token->token_type) {
    case static_cast<int>(Token::tok_integer):
      lhs = new ASTInteger(token->token_value->int_num_val);
      if (!lhs) return nullptr;
      return parse_binop_rhs(0, lhs);

    case static_cast<int>(Token::tok_floating_point):
      lhs = new ASTDouble(token->token_value->double_num_val);
      if (!lhs) return nullptr;
      return parse_binop_rhs(0, lhs);

    case '(':
      return parse_paren_expr();

    default:
      UNIMPLEMENTED();
      return nullptr;
  }
}

ASTNode* Parser::parse_binop_rhs(int expr_prec, ASTNode* lhs) {
  for (;;) {
    token = get_token(iterator);
    int token_prec = get_token_prec(token);

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (token_prec < expr_prec) return lhs;

    // Okay, we know this is a binop.
    int binop = token->token_type;
    token = get_token(iterator);  // eat binop

    // Parse the primary expression after the binary operator.
    auto rhs = parse_primary_expr();
    if (!rhs) return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int next_prec = get_token_prec(token);
    if (token_prec < next_prec) {
      rhs = parse_binop_rhs(token_prec + 1, rhs);
      if (!rhs) return nullptr;
    }

    // Merge LHS/RHS.
    lhs = new ASTBinExpr(binop, lhs, rhs);
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

// Function Prototype:
// [type_name] ident(); - single return type
// [type_name, ...] ident(); - multiple return types
//
// [type_name] ident(ident);
// [type_name, ...] ident(ident, ...);
//
// Function Signature:
// [type_name] ident(type_name ident { *body* }
// [type_name, ...] ident(type_name ident, ...) { *body* }

ASTNode* Parser::parse_function_prototype(ASTVariable* prototype) {
  token = get_token(iterator);
  std::vector<ASTNode*> params;

  while (token->token_type == static_cast<int>(Token::tok_identifier)) {
    if (std::find(DataTypes::type_strings.begin(),
                  DataTypes::type_strings.end(),
                  token->token_value->ident_str) !=
        std::end(DataTypes::type_strings)) {
      params.push_back(parse_variable_expr());
      if (token->token_type == ',') {
        iterator++;
        token = get_token(iterator);
      } else if (token->token_type == ')') {
        return new ASTFunction(prototype, params);
      }
    }
  }

  if (token->token_type == ')') {
    return new ASTFunction(prototype, params);
  }

  std::cout << "Error: Expected closing parenthesis for parameter list"
            << std::endl;
  return nullptr;
}

// Variable:
// typename ident = value;

ASTNode* Parser::parse_variable_expr() {
  std::string type_str = token->token_value->ident_str;
  bool sign = false;
  if (type_str[0] != 'u') {
    sign = true;
  }

  // Get bit width suffix. there's probably a better way to do this...
  // we could also map type strings to the DataType enum and switch DataType
  // values
  std::string raw_width_str;
  int i = type_str.size() - 1;
  while (isdigit(type_str[i])) {
    raw_width_str += type_str[i];
    i--;
  }

  std::string width_str = raw_width_str;

  for (int i = 0, j = width_str.size() - 1; i < width_str.size(); i++, j--) {
    width_str[i] = raw_width_str[j];
  }

  int width = stoi(width_str);

  ASTVariable::Attributes attributes{sign, width};

  // expecting identifier after type
  std::string name;
  token = get_token(iterator);
  if (token->token_type == static_cast<int>(Token::tok_identifier)) {
    name = token->token_value->ident_str;
  } else {
    std::cerr << "Error: Expected identifier" << std::endl;
  }

  token = get_token(iterator);

  // function prototype
  if (token->token_type == '(') {
    return parse_function_prototype(new ASTVariable(name, attributes));
  }

  // variable assignment, probably going to be an alloca
  if (token->token_type == '=') {
    // number literal
    ASTNode* value;
    token = get_token(iterator);
    switch (token->token_type) {
      // @@@ Eventually we'll want to detect and throw a warning for type
      // narrowing
      case static_cast<int>(Token::tok_integer):
        // @@@ This assignment might not be needed
        value = new ASTInteger(token->token_value->int_num_val);
        return new ASTVariable(name, attributes, value);

      // @@@ Research FP number precision
      case static_cast<int>(Token::tok_floating_point):
        value = new ASTInteger(token->token_value->int_num_val);
        return new ASTVariable(name, attributes, value);

      default:
        std::cout << "Error: Invalid number literal" << std::endl;
    }
  }

  return new ASTVariable(name, attributes);
}

ASTNode* Parser::parse_top_level_expr() {
  token = get_token(iterator);

  if (!token) return nullptr;

  ASTNode* lhs = nullptr;

  switch (token->token_type) {
    case static_cast<int>(Token::tok_def):
      UNIMPLEMENTED();
      return nullptr;

    case static_cast<int>(Token::tok_extern):
      UNIMPLEMENTED();
      return nullptr;

    // Type list for:
    // Function Prototype
    // Function Signature
    case '[':
      return parse_variable_expr();

    // Variable declaration
    // type_name ident;

    // Variable definition
    // type_name ident = number_literal;
    case static_cast<int>(Token::tok_identifier):
      if (std::find(DataTypes::type_strings.begin(),
                    DataTypes::type_strings.end(),
                    token->token_value->ident_str) !=
          std::end(DataTypes::type_strings))
        return parse_variable_expr();
      std::cout << "Error: Unknown identifier" << std::endl;

    case static_cast<int>(Token::tok_eof):
      return nullptr;

    default:                       // number or identifier
      lhs = parse_primary_expr();  // lhs
      return lhs;
  }
}
