#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
#include "ast_variable.h"
#include "ast_function.h"
#include "parser.h"
//@@@
#include "data_type.h"
#include "lexer.h"

#define DIAGRAM_DEBUG 0

template <class T>
T* check_node_type(std::string buffer) {
  //@@@ maybe we should make these members static?
  Parser parser;

  parser.iterator = &buffer[0];
  return dynamic_cast<T*>(parser.parse_top_level_expr());
}

template <class T, typename N>
void check_node_type_and_value(std::string buffer, N value) {
  Parser parser;

  parser.iterator = &buffer[0];
  T* node = dynamic_cast<T*>(parser.parse_top_level_expr());
  ASSERT_TRUE(node);
  EXPECT_EQ(node->value, value);
}

TEST(test_parse_expression, test_unsigned_integer) {
  check_node_type_and_value<ASTInteger>("1234", 1234);
}

TEST(test_parse_expression, test_unsigned_floating_point) {
  check_node_type_and_value<ASTDouble>("12.34", 12.34);
}

TEST(test_parse_expression, test_bin_expr_addition_two_operands) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+4");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->value, 2);

  ASTInteger* rhs = dynamic_cast<ASTInteger*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->value, 4);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_multiplication_two_operands) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2*4");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '*');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->value, 2);

  ASTInteger* rhs = dynamic_cast<ASTInteger*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->value, 4);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_addition_three_operands) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+4+5");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->op, '+');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  ASSERT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  ASSERT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

//@@@ this does not parse with the correct precedence
// intended precedence: (2*4)+5
TEST(test_parse_expression, test_bin_expr_left_multiplication_three_operands) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2*4+5");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '*');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->op, '+');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  ASSERT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  ASSERT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

// this already parses with the correct precedence 2+(4*5)
TEST(test_parse_expression, test_bin_expr_right_multiplication_three_operands) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+4*5");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->op, '*');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  ASSERT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  ASSERT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_three_operands_left_parens) {
  // this should parse the same as no parenthesis
  ASTBinExpr* check = check_node_type<ASTBinExpr>("(2+4)+5");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  ASTBinExpr* lhs = dynamic_cast<ASTBinExpr*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->op, '+');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(lhs->lhs);
  ASSERT_TRUE(nested_lhs);
  ASSERT_EQ(nested_lhs->value, 2);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(lhs->rhs);
  ASSERT_TRUE(nested_rhs);
  ASSERT_EQ(nested_rhs->value, 4);

  ASTInteger* rhs = dynamic_cast<ASTInteger*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_three_operands_right_parens) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+(4+5)");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  ASSERT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  ASSERT_EQ(rhs->op, '+');
  std::cout << check->to_string(0) << std::endl;
  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  ASSERT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  ASSERT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

//@@@ too lazy to fill the following two tests out properly
TEST(test_parse_expression, test_bin_expr_nested) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+4+5+6");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_nested_with_parens) {
  // should parse the same as "2+4+5+6"
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+(4+5)+6");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string(0) << std::endl;
#endif
}

TEST(test_parse_expression, test_parse_variable) {
  //@@@ temporary

  std::string lex_buffer = "uint32";
  const char* iterator = &lex_buffer[0];
  TokValPair* variable_token = get_token(iterator);

  ASSERT_TRUE(variable_token);
  EXPECT_EQ(variable_token->token_type,
            static_cast<int>(Token::tok_uint32_type));
  EXPECT_EQ(variable_token->token_value->ident_str, "uint32");

  Parser parser;
  std::string parse_buffer = "uint32 name_name 8";
  parser.iterator = &parse_buffer[0];
  ASTVariable* variable_expr =
      dynamic_cast<ASTVariable*>(parser.parse_top_level_expr());

  ASSERT_TRUE(variable_expr);
  EXPECT_EQ(variable_expr->name, "name_name");
  ASTVariable::Attributes test_attributes{false, 32};
  bool check_attributes = (test_attributes == variable_expr->attributes);
  EXPECT_TRUE(check_attributes);
}

//@@@ the following tests require DRYing
TEST(test_parse_expression, test_parse_function_prototype) {
  Parser parser;

  std::string buffer = "uint32 name_name()";
  parser.iterator = &buffer[0];
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
	ASSERT_TRUE(prototype_expr);
	EXPECT_EQ(prototype_expr->params.size(), 0);

	ASSERT_TRUE(prototype_expr->prototype);
	EXPECT_EQ(prototype_expr->prototype->name, "name_name");
	EXPECT_FALSE(prototype_expr->prototype->attributes.sign);
	EXPECT_EQ(prototype_expr->prototype->attributes.width, 32);
}

TEST(test_parse_expression, test_parse_function_prototype_single_parameter) {
  Parser parser;

  std::string buffer = "uint32 name_name(uint32 another_name)";
  parser.iterator = &buffer[0];
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
	ASSERT_TRUE(prototype_expr);
	EXPECT_EQ(prototype_expr->params.size(), 1);

	ASSERT_TRUE(prototype_expr->prototype);
	EXPECT_EQ(prototype_expr->prototype->name, "name_name");
	EXPECT_FALSE(prototype_expr->prototype->attributes.sign);
	EXPECT_EQ(prototype_expr->prototype->attributes.width, 32);
	
	ASTVariable* param = dynamic_cast<ASTVariable*>(prototype_expr->params.at(0));
	ASTVariable::Attributes test_attributes{false, 32};
	ASSERT_TRUE(param);
	bool check_attributes = (test_attributes == param->attributes);
	EXPECT_TRUE(check_attributes);
	EXPECT_EQ(param->name, "another_name");
	EXPECT_FALSE(param->value);
}

TEST(test_parse_expression, test_parse_function_prototype_two_parameters) {
  Parser parser;

  std::string buffer = "uint32 name_name(uint32 x1, uint32 x2)";
  parser.iterator = &buffer[0];
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
	ASSERT_TRUE(prototype_expr);
	EXPECT_EQ(prototype_expr->params.size(), 2);

	ASSERT_TRUE(prototype_expr->prototype);
	EXPECT_EQ(prototype_expr->prototype->name, "name_name");
	EXPECT_FALSE(prototype_expr->prototype->attributes.sign);
	EXPECT_EQ(prototype_expr->prototype->attributes.width, 32);

	int i = 0;

	ASTVariable* param1 = dynamic_cast<ASTVariable*>(prototype_expr->params.at(i));
	ASTVariable::Attributes test_attributes{false, 32};
	ASSERT_TRUE(param1);
	bool check_first_param_attributes = (test_attributes == param1->attributes);
	EXPECT_TRUE(check_first_param_attributes);
	EXPECT_EQ(param1->name, "x1");
	EXPECT_FALSE(param1->value);

	i++;

	ASTVariable* param2 = dynamic_cast<ASTVariable*>(prototype_expr->params.at(i));
	ASSERT_TRUE(param2);
	bool check_second_param_attributes = (test_attributes == param2->attributes);
	EXPECT_TRUE(check_second_param_attributes);
	EXPECT_EQ(param2->name, "x2");
	EXPECT_FALSE(param2->value);
}
