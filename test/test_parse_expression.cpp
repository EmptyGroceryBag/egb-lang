#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
#include "parser.h"

//@@@ maybe we should make these members static?
Parser p;

template <class T>
T* check_node_type(std::string buffer) {
  p.iterator = &buffer[0];
  return dynamic_cast<T*>(p.parse_top_level_expr());
}

template <class T, typename N>
void check_node_type_and_value(std::string buffer, N value) {
  p.iterator = &buffer[0];
  T* node = dynamic_cast<T*>(p.parse_top_level_expr());
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

  std::cout << check->to_string(0) << std::endl;
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

  std::cout << check->to_string(0) << std::endl;
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

  std::cout << check->to_string(0) << std::endl;
}

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

  std::cout << check->to_string(0) << std::endl;
}

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

  std::cout << check->to_string(0) << std::endl;
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

  std::cout << check->to_string(0) << std::endl;
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

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  ASSERT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  ASSERT_EQ(nested_rhs->value, 5);

  std::cout << check->to_string(0) << std::endl;
}

//@@@ too lazy to fill the following two tests out properly
TEST(test_parse_expression, test_bin_expr_nested) {
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+4+5+6");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  std::cout << check->to_string(0) << std::endl;
}

TEST(test_parse_expression, test_bin_expr_nested_with_parens) {
  // should parse the same as "2+4+5+6"
  ASTBinExpr* check = check_node_type<ASTBinExpr>("2+(4+5)+6");
  ASSERT_TRUE(check);
  ASSERT_EQ(check->op, '+');

  std::cout << check->to_string(0) << std::endl;
}
