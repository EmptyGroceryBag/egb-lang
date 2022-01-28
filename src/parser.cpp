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

  switch (token->token_type) {
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

// function signature
// uint32 ident()

ASTNode* Parser::parse_function_prototype(ASTVariable* prototype) {
	token = get_token(iterator);
	std::vector<ASTNode*> params;
	while (token->token_type == static_cast<int>(Token::tok_uint32_type)) {
			params.push_back(parse_variable_expr(false));
			if (token->token_type == ',') {
				iterator++;
				token = get_token(iterator);
				continue;
			}
			break;
	}

	if (token->token_type == ')') {
		return new ASTFunction(prototype, params);
	}

	return nullptr;
}

ASTNode* Parser::parse_variable_expr(bool is_definition) {
  // get bit width suffix. there's probably a better way to do this...
  // we could also map type strings to the DataType enum and switch DataType
  // values
  bool sign = false;
  int width = 0;
  std::string type_str = token->token_value->ident_str;
  std::string raw_width_str;

  if (type_str[0] != 'u') {
    sign = true;
  }

  int i = type_str.size() - 1;
  while (isdigit(type_str[i])) {
    raw_width_str += type_str[i];
    i--;
  }

  std::string width_str = raw_width_str;

  for (int i = 0, j = width_str.size() - 1; i < width_str.size(); i++, j--) {
    width_str[i] = raw_width_str[j];
  }

  width = stoi(width_str);

  ASTVariable::Attributes attributes{sign, width};

  // expecting identifier after type
  std::string name;
  token = get_token(iterator);
  if (token->token_type == static_cast<int>(Token::tok_identifier)) {
    name = token->token_value->ident_str;
  }

	// function prototype
	token = peek(iterator);
	if (token->token_type == '(') {
		iterator++;
		return parse_function_prototype(new ASTVariable(name, attributes));
	}

  // number literal
	if (is_definition) {
		ASTInteger* value;
		token = get_token(iterator);
		switch (token->token_type) {
			case static_cast<int>(Token::tok_integer):
				value = new ASTInteger(token->token_value->int_num_val);
				return new ASTVariable(name, attributes, value);
		}
	}

  return new ASTVariable(name, attributes);
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

    case static_cast<int>(Token::tok_uint32_type):
      return parse_variable_expr(true);

    case static_cast<int>(Token::tok_eof):
      return nullptr;

    default:                       // number or identifier
      lhs = parse_primary_expr();  // lhs
      return lhs;
  }
}
