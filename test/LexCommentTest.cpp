#include <gtest/gtest.h>
#include <cstdio>
#include "lexer.h"

TEST(TestComments, TestComment) {
  EXPECT_EQ(
			static_cast<int>(get_token("# this is a comment\n")), 
			static_cast<int>(Token::tok_undefined));
  EXPECT_EQ(
			static_cast<int>(get_token("#		this is a comment with tabs at the beginning of it\n")), 
			static_cast<int>(Token::tok_undefined));
  EXPECT_EQ(
			static_cast<int>(get_token("# this is a comment with tabs at the end		\n")), 
			static_cast<int>(Token::tok_undefined));
  EXPECT_EQ(
			static_cast<int>(get_token("# this is a comment		with tabs in the middle\n")), 
			static_cast<int>(Token::tok_undefined));
}
