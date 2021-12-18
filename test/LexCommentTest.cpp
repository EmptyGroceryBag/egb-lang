#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include "lexer.h"

TEST(TestComments, TestComment) {
	std::string test_string = "# this is a comment\n";
	const char* start = &test_string[0];
	EXPECT_EQ(
		get_token(test_string, start)->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
TEST(TestComments, TestCommentBeginningTab) {
	std::string test_string = "#		this is a comment with tabs at the beginning of it\n";
	const char* start = &test_string[0];
	EXPECT_EQ(
		get_token(test_string, start)->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
TEST(TestComments, TestCommentEndingTab) {
	std::string test_string = "# this is a comment with tabs at the end		\n";
	const char* start = &test_string[0];
	EXPECT_EQ(
		get_token(test_string, start)->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
TEST(TestComments, TestCommentMiddleTab) {
	std::string test_string = "# this is a comment		with tabs in the middle\n";
	const char* start = &test_string[0];
	EXPECT_EQ(
		get_token(test_string, start)->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
