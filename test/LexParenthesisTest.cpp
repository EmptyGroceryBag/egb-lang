#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include "lexer.h"

TEST(TestParenthesis, TestOpenParenthesis) {
	std::string test_string = "(";
	char* start = &test_string[0];
	int token = get_token(test_string, start).first;
	EXPECT_EQ(
		token,
		'('
	);
}

TEST(TestParenthesis, TestCloseParenthesis) {
	std::string test_string = ")";
	char* start = &test_string[0];
	int token = get_token(test_string, start).first;
	EXPECT_EQ(
		token,
		')'
	);
}