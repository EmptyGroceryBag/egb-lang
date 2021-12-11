#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include "lexer.h"

TEST(TestNumbers, TestPositiveInteger) {
	std::string test_string = "1234";
	char* start = &test_string[0];
  EXPECT_EQ(
			get_token(test_string, start),
			Token::tok_integer);
}

TEST(TestNumbers, TestNegativeInteger) {
	std::string test_string = "-1234";
	char* start = &test_string[0];
  EXPECT_EQ(
			get_token(test_string, start),
			Token::tok_integer);
}

TEST(TestNumbers, TestUnsignedFloatingPoint) {
	std::string test_string = "12.34";
	char* start = &test_string[0];
  EXPECT_EQ(
			get_token(test_string, start),
			Token::tok_floating_point);
}
