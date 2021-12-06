#include <gtest/gtest.h>
#include <cstdio>
#include "lexer.h"

TEST(TestNumbers, TestPositiveInteger) {
  EXPECT_EQ(
			static_cast<int>(get_token("1234")), 
			static_cast<int>(Token::tok_integer));
}

TEST(TestNumbers, TestNegativeInteger) {
  EXPECT_EQ(
			static_cast<int>(get_token("-1234")), 
			static_cast<int>(Token::tok_integer));
}

TEST(TestNumbers, TestUnsignedFloatingPoint) {
  EXPECT_EQ(
			static_cast<int>(get_token("12.34")),
			static_cast<int>(Token::tok_floating_point));
}
