#include <gtest/gtest.h>
#include <cstdio>
#include "lexer.h"

TEST(TestComments, TestComment) {
	std::FILE* f_out = std::fopen("comment_test.txt", "w+");
	fprintf(f_out, "# this is a comment\n");
	fprintf(f_out, "#		this is a comment with tabs at the beginning of it\n");
	fprintf(f_out, "# this is a comment with tabs at the end		\n");
	fprintf(f_out, "# this is a comment		with tabs in the middle\n");

	fclose(f_out);

	std::FILE* f_in = std::fopen("comment_test.txt", "r");
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_undefined));
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_undefined));
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_undefined));
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_undefined));
	fclose(f_in);
}
