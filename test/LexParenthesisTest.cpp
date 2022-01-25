#include <cstdio>
#include <gtest/gtest.h>
#include <string>

#include "lexer.h"

TEST(TestParenthesis, TestOpenParenthesis) {
  std::string test_string = "(";
  const char* start = &test_string[0];
  int token = get_token(test_string, start)->token_type;
  EXPECT_EQ(token, '(');
}

TEST(TestParenthesis, TestCloseParenthesis) {
  std::string test_string = ")";
  const char* start = &test_string[0];
  int token = get_token(test_string, start)->token_type;
  EXPECT_EQ(token, ')');
}