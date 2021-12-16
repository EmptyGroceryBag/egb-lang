#include <gtest/gtest.h>
#include <cstdio>
#include "lexer.h"

TEST(TestComments, TestComment) {
	std::string test_string = "# this is a comment\n";
	EXPECT_EQ(
		get_token(test_string, &test_string[0])->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
TEST(TestComments, TestCommentBeginningTab) {
	std::string test_string = "#		this is a comment with tabs at the beginning of it\n";
	EXPECT_EQ(
		get_token(test_string, &test_string[0])->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
TEST(TestComments, TestCommentEndingTab) {
	std::string test_string = "# this is a comment with tabs at the end		\n";
	EXPECT_EQ(
		get_token(test_string, &test_string[0])->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
TEST(TestComments, TestCommentMiddleTab) {
	std::string test_string = "# this is a comment		with tabs in the middle\n";
	EXPECT_EQ(
		get_token(test_string, &test_string[0])->token_type,
		static_cast<int>(Token::tok_undefined)
	);
}
