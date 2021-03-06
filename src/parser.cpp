#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_function.h"
#include "ast_global_block.h"
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

TokValPair token;
TokValPair peeked_token;

std::vector<std::string>& types = DataTypes::type_strings;

int get_token_prec(const TokValPair token) {
  if (op_prec[token.token_type] == 0) {
    return -1;
  }

  return op_prec[token.token_type];
}

void Parser::UNIMPLEMENTED() {
  std::cout << "Not implemented yet" << std::endl;
}

ASTNode* Parser::report_error(std::string error_string) {
  std::cerr << "Error: " << error_string << std::endl;
  error = true;
  return nullptr;
}

ASTNode* Parser::parse_primary_expr() {
  token = get_token(iterator);  // The problem seems to be this...
  ASTNode* lhs;

  switch (token.token_type) {
    case static_cast<int>(Token::tok_integer):
      lhs = new ASTInteger(token.token_value.int_num_val);
      if (!lhs) return nullptr;
      return parse_binop_rhs(0, lhs);

    case static_cast<int>(Token::tok_floating_point):
      lhs = new ASTDouble(token.token_value.double_num_val);
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
    int binop = token.token_type;
    // token = get_token(iterator);  // eat binop

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

// @@@Junk: This function is basically useless
ASTNode* Parser::parse_paren_expr() {
  // token = get_token(iterator);
  ASTNode* n = parse_primary_expr();

  /*
  if(get_token(iterator)->token_type != ')'){
          //std::cerr << "Expected closing parenthesis" << std::endl;
          return nullptr;
  }
  */

  return n;
}

void Parser::parse_block_scope(ASTFunction* this_function) {
  get_token(iterator);
  insertion_stack.push(&this_function->scope);
  peeked_token = peek(iterator);

  while (peeked_token.token_type == static_cast<int>(Token::tok_identifier)) {
    // @@@TODO: Turn this into `parse_top_level_expr` later
    parse_variable_expr(false);
    peeked_token = peek(iterator);
  }

  // @@@Debug
  /*
  std::cout << "# of statements in " << this_function->prototype->name << "
  function block = "
            << (*insertion_stack.top()).size() << std::endl;
  */

  if (peeked_token.token_type == '}') {
    get_token(iterator);

    // @@@Debug
    /*
    for (ASTNode* n : *insertion_stack.top()) {
      ASTVariable* v = dynamic_cast<ASTVariable*>(n);
      if (v) {
        std::cout << v->name << std::endl;
      }
    }
    */

    insertion_stack.pop();
    (*insertion_stack.top()).push_back(this_function);
  } else {
    report_error("Expected closing brace to end function block");
  }
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
  ASTFunction* this_function = new ASTFunction(prototype);

  peeked_token = peek(iterator);
  std::vector<ASTNode*> params;  // We pass a copy of params, but not the scope

  while (peeked_token.token_type == static_cast<int>(Token::tok_identifier)) {
    if (std::find(types.begin(), types.end(),
                  peeked_token.token_value.ident_str) != types.end()) {
      params.push_back(parse_variable_expr(true));

      if (peek(iterator).token_type == ',') {
        get_token(iterator);
        peeked_token = peek(iterator);
      } else if (peek(iterator).token_type == ')') {
        this_function->params = params;
        break;
      } else {
        return report_error(
            "Expected closing parenthesis to close parameter list");
      }
    } else {
      return report_error("Expected type name to begin parameter list");
    }
  }

  if (peek(iterator).token_type != ')') {
    return report_error("Expected closing parenthesis to end parameter list");
  }

  get_token(iterator);
  if (peek(iterator).token_type == '{') {
    parse_block_scope(this_function);
    if (error)
      return nullptr;
    else
      return this_function;
  } else {
    return report_error("Expected opening brace to begin function block");
  }
}

ASTVariable* Parser::check_for_redeclaration(ASTVariable* this_variable,
                                             bool is_function_prototype) {
  for (ASTNode* node : *insertion_stack.top()) {
    // Debug
    std::cout << insertion_stack.top() << std::endl;
    ASTFunction* function = dynamic_cast<ASTFunction*>(node);
    ASTVariable* variable = dynamic_cast<ASTVariable*>(node);
    if (is_function_prototype) {
      if (function) {
        if (function->prototype->name == this_variable->name) return nullptr;
      }
    } else if (variable) {
      if (variable->name == this_variable->name) return nullptr;
    }
  }

  return this_variable;
}

// Variable:
// typename ident = value;  definition
// typename ident;          declaration

ASTNode* Parser::parse_variable_expr(bool comma_separated) {
  ASTVariable*
      this_variable;  // Won't be returned in the case of a syntax error.

  token = get_token(iterator);
  std::string type_str = token.token_value.ident_str;
  if (type_str.size() < 1) {
    return report_error("Error: Invalid type name");
  }

  bool sign = false;
  if (type_str[0] != 'u') {
    sign = true;
  }

  // Get bit width suffix. there's probably a better way to do this...
  // we could also map type strings to the DataType enum and switch DataType
  // values
  int width;
  if (type_str != "void" && type_str != "double" && type_str != "bool") {
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

    try {
      width = stoi(width_str);
    } catch (std::out_of_range e) {
      std::cout << e.what() << std::endl;
    }
  } else if (type_str == "bool") {
    width = 1;
  } else
    width = -1;

  ASTVariable::Attributes attributes{sign, width};

  // expecting identifier after type
  std::string name;
  peeked_token = peek(iterator);

  if (peeked_token.token_type == static_cast<int>(Token::tok_identifier)) {
    if (std::find(types.begin(), types.end(),
                  peeked_token.token_value.ident_str) != types.end()) {
      return report_error("Names of variables cannot be a type name");
    }

    token = get_token(iterator);
    name = token.token_value.ident_str;
  } else {
    return report_error("Expected identifier for variable name");
  }

  peeked_token = peek(iterator);

  // Function prototype
  if (peeked_token.token_type == '(') {
    get_token(iterator);
    if (name == "main") found_main = true;

    this_variable = check_for_redeclaration(
        new ASTVariable(name, attributes, insertion_stack.top()), true);

    if (!this_variable)
      report_error("Redeclaration of function \"" + name + "\"");

    ASTNode* this_function = parse_function_prototype(this_variable);

    if (error) return nullptr;

    return this_function;
  }

  // Variable assignment
  if (peeked_token.token_type == '=') {
    // Number literal
    ASTNode* value;
    get_token(iterator);
    token = get_token(iterator);
    switch (token.token_type) {
      // @@@Incomplete: Eventually we'll want to detect and throw a
      // warning for type narrowing
      case static_cast<int>(Token::tok_integer):
        // @@@ This assignment might not be needed
        value = new ASTInteger(token.token_value.int_num_val);
        this_variable = check_for_redeclaration(
            new ASTVariable(name, attributes, insertion_stack.top(), value),
            false);
        break;

      // @@@ Research FP number precision
      case static_cast<int>(Token::tok_floating_point):
        value = new ASTInteger(token.token_value.int_num_val);
        this_variable = check_for_redeclaration(
            new ASTVariable(name, attributes, insertion_stack.top(), value),
            false);
        break;

      default:
        return report_error("Invalid number literal");
    }
    if (peek(iterator).token_type == ';') {
      get_token(iterator);

      if (!this_variable)
        return report_error("Redeclaration of variable \"" + name + "\"");

      (*insertion_stack.top()).push_back(this_variable);
      return this_variable;
    } else {
      return report_error("Expected semicolon to end statement");
    }
  }

  // Variable declaration
  if (!comma_separated) {
    if (peeked_token.token_type == ';') {
      get_token(iterator);

      this_variable = check_for_redeclaration(
          new ASTVariable(name, attributes, insertion_stack.top()), false);
      if (!this_variable)
        return report_error("Redeclaration of variable \"" + name + "\"");

      (*insertion_stack.top()).push_back(this_variable);
      return this_variable;
    } else {
      return report_error("Expected semicolon to end statement");
    }
  }

  return new ASTVariable(name, attributes, insertion_stack.top());
}

ASTNode* Parser::parse_top_level_expr() {
  ASTNode* lhs = nullptr;

parse:
  peeked_token = peek(iterator);

  switch (peeked_token.token_type) {
    // Type list for:
    // Function Prototype
    // Function Signature
    case '[':
      return parse_variable_expr(false);

    // Variable declaration
    // type_name ident;

    // Variable definition
    // type_name ident = number_literal;
    case static_cast<int>(Token::tok_identifier):
      if (std::find(DataTypes::type_strings.begin(),
                    DataTypes::type_strings.end(),
                    peeked_token.token_value.ident_str) !=
          std::end(DataTypes::type_strings)) {
        ASTNode* variable = parse_variable_expr(false);
        return variable;
      }
      get_token(iterator);
      return report_error("Unknown identifier");

    // @@@I suspect this is dead code
    case static_cast<int>(Token::tok_eof):
      std::cout << "EOF reached" << std::endl;
      return nullptr;

    default:
      get_token(iterator);
      return report_error("Unexpected token");
  }
}
