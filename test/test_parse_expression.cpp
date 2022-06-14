#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_function.h"
#include "ast_global_block.h"
#include "ast_integer.h"
#include "ast_variable.h"
#include "lexer.h"
#include "parser.h"

#define DIAGRAM_DEBUG 1

template <class T>
T* check_node_type(std::string buffer) {
  Parser parser(&buffer[0]);
  T* node = dynamic_cast<T*>(parser.parse_top_level_expr());
  return node;
}

template <class T>
T* check_primary_expr(std::string buffer) {
  Parser parser(&buffer[0]);
  T* node = dynamic_cast<T*>(parser.parse_primary_expr());
  return node;
}

template <class T, typename N>
void check_primary_expr_value(std::string buffer, N value) {
  Parser parser(&buffer[0]);
  T* node = dynamic_cast<T*>(parser.parse_primary_expr());
  ASSERT_TRUE(node);
  EXPECT_EQ(node->value, value);
}

TEST(test_parse_expression, test_unsigned_integer) {
  check_primary_expr_value<ASTInteger>("1234", 1234);
}

TEST(test_parse_expression, test_unsigned_floating_point) {
  check_primary_expr_value<ASTDouble>("12.34", 12.34);
}

TEST(test_parse_expression, test_bin_expr_addition_two_operands) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2+4");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->value, 2);

  ASTInteger* rhs = dynamic_cast<ASTInteger*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->value, 4);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_multiplication_two_operands) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2*4");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '*');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->value, 2);

  ASTInteger* rhs = dynamic_cast<ASTInteger*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->value, 4);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_addition_three_operands) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2+4+5");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->op, '+');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  EXPECT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  EXPECT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_left_multiplication_three_operands) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2*4+5");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '*');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->op, '+');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  EXPECT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  EXPECT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

// this already parses with the correct precedence 2+(4*5)
TEST(test_parse_expression, test_bin_expr_right_multiplication_three_operands) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2+4*5");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->op, '*');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  EXPECT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  EXPECT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_three_operands_left_parens) {
  // this should parse the same as no parenthesis
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("(2+4)+5");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

  ASTBinExpr* lhs = dynamic_cast<ASTBinExpr*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->op, '+');

  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(lhs->lhs);
  ASSERT_TRUE(nested_lhs);
  EXPECT_EQ(nested_lhs->value, 2);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(lhs->rhs);
  ASSERT_TRUE(nested_rhs);
  EXPECT_EQ(nested_rhs->value, 4);

  ASTInteger* rhs = dynamic_cast<ASTInteger*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_three_operands_right_parens) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2+(4+5)");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

  ASTInteger* lhs = dynamic_cast<ASTInteger*>(check->lhs);
  ASSERT_TRUE(lhs);
  EXPECT_EQ(lhs->value, 2);

  ASTBinExpr* rhs = dynamic_cast<ASTBinExpr*>(check->rhs);
  ASSERT_TRUE(rhs);
  EXPECT_EQ(rhs->op, '+');
  ASTInteger* nested_lhs = dynamic_cast<ASTInteger*>(rhs->lhs);
  ASSERT_TRUE(nested_lhs);
  EXPECT_EQ(nested_lhs->value, 4);

  ASTInteger* nested_rhs = dynamic_cast<ASTInteger*>(rhs->rhs);
  ASSERT_TRUE(nested_rhs);
  EXPECT_EQ(nested_rhs->value, 5);

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

// @@@ too lazy to fill the following two tests out properly
TEST(test_parse_expression, test_bin_expr_nested) {
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2+4+5+6");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_bin_expr_nested_with_parens) {
  // should parse the same as "2+4+5+6"
  ASTBinExpr* check = check_primary_expr<ASTBinExpr>("2+(4+5)+6");
  ASSERT_TRUE(check);
  EXPECT_EQ(check->op, '+');

#if (DIAGRAM_DEBUG)
  std::cout << check->to_string() << std::endl;
#endif
}

TEST(test_parse_expression, test_parse_variable_declaration) {
  Parser parser("u32 x;");
  ASTVariable* variable_expr =
      dynamic_cast<ASTVariable*>(parser.parse_top_level_expr());

  ASSERT_TRUE(variable_expr);
  EXPECT_EQ(variable_expr->name, "x");
}

TEST(test_parse_expression, test_parse_variable_assignment) {
  Parser parser("u32 x = 8;");
  ASTVariable* variable_expr =
      dynamic_cast<ASTVariable*>(parser.parse_top_level_expr());

  ASSERT_TRUE(variable_expr);
  EXPECT_EQ(variable_expr->name, "x");
  ASTVariable::Attributes test_attributes{false, 32};
  EXPECT_TRUE(test_attributes == variable_expr->attributes);
}

ASTFunction* check_function_prototype(std::string buffer) {
  Parser parser(&buffer[0]);

  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
  return prototype_expr;
}

TEST(test_parse_expression, test_parse_function_prototype) {
  ASTFunction* prototype_expr = check_function_prototype("u32 funcy() {}");
  ASSERT_TRUE(prototype_expr);

  EXPECT_EQ(prototype_expr->params.size(), 0);
  ASSERT_TRUE(prototype_expr->prototype);
  EXPECT_EQ(prototype_expr->prototype->name, "funcy");
  EXPECT_FALSE(prototype_expr->prototype->attributes.sign);
  EXPECT_EQ(prototype_expr->prototype->attributes.width, 32);
}

TEST(test_parse_expression, test_parse_function_prototype_single_parameter) {
  ASTFunction* prototype_expr =
      check_function_prototype("u32 doofus(u32 y) {}");
  ASSERT_TRUE(prototype_expr);

  EXPECT_EQ(prototype_expr->params.size(), 1);
  ASSERT_TRUE(prototype_expr->prototype);
  EXPECT_EQ(prototype_expr->prototype->name, "doofus");
  ASTVariable::Attributes prototype_attributes{false, 32};
  EXPECT_TRUE(prototype_attributes == prototype_expr->prototype->attributes);

  ASTVariable* param = dynamic_cast<ASTVariable*>(prototype_expr->params.at(0));
  ASSERT_TRUE(param);
  EXPECT_EQ(param->name, "y");
  ASTVariable::Attributes test_attributes{false, 32};
  EXPECT_TRUE(test_attributes == param->attributes);
  EXPECT_FALSE(param->value);
}

TEST(test_parse_expression, test_parse_function_prototype_two_parameters) {
  ASTFunction* prototype_expr =
      check_function_prototype("u32 sum(u32 x1, u32 x2) {}");
  ASSERT_TRUE(prototype_expr);

  ASSERT_TRUE(prototype_expr->prototype);
  EXPECT_EQ(prototype_expr->params.size(), 2);
  EXPECT_EQ(prototype_expr->prototype->name, "sum");
  ASTVariable::Attributes prototype_attributes{false, 32};
  EXPECT_TRUE(prototype_attributes == prototype_expr->prototype->attributes);

  int i = 0;
  for (ASTNode* param : prototype_expr->params) {
    ASTVariable* param_expr = dynamic_cast<ASTVariable*>(param);
    ASSERT_TRUE(param_expr);

    ASTVariable::Attributes test_attributes{false, 32};
    EXPECT_TRUE(test_attributes == param_expr->attributes);

    std::string param_name = "x";
    param_name += std::to_string(i + 1);
    EXPECT_EQ(param_expr->name, param_name);

    EXPECT_FALSE(param_expr->value);

    i++;
  }
}