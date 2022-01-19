#include <cstdio>
#include <gtest/gtest.h>
#include <string>

#include "lexer.h"

TEST(TestNumbers, TestPositiveInteger) {
  std::string test_string = "1234";
  const char* start       = &test_string[0];
  int token               = get_token(test_string, start)->token_type;
  EXPECT_EQ(token, static_cast<int>(Token::tok_integer));
}

TEST(TestNumbers, TestSignedInteger) {
  std::string test_string = "-1234";
  const char* start       = &test_string[0];
  int token               = get_token(test_string, start)->token_type;
  EXPECT_EQ(token, static_cast<int>(Token::tok_integer));
}

TEST(TestNumbers, TestUnsignedFloatingPoint) {
  std::string test_string = "12.34";
  const char* start       = &test_string[0];
  int token               = get_token(test_string, start)->token_type;
  EXPECT_EQ(token, static_cast<int>(Token::tok_floating_point));
}

TEST(TestNumbers, TestSignedFloatingPoint) {
  std::string test_string = "-12.34";
  const char* start       = &test_string[0];
  int token               = get_token(test_string, start)->token_type;
  EXPECT_EQ(token, static_cast<int>(Token::tok_floating_point));
}