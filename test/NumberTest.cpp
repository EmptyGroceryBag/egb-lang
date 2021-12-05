#include <gtest/gtest.h>
#include <cstdio>
#include "lexer.h"

TEST(TestNumbers, TestPositiveInteger) {
	std::FILE* f_out = std::fopen("unsigned_integer_test.txt", "w+");
	fprintf(f_out, "1234");

	fclose(f_out);

	std::FILE* f_in = std::fopen("unsigned_integer_test.txt", "r");
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_integer));
}

TEST(TestNumbers, TestNegativeInteger) {
	std::FILE* f_out = std::fopen("signed_integer_test.txt", "w+");
	fprintf(f_out, "-1234");

	fclose(f_out);

	std::FILE* f_in = std::fopen("signed_integer_test.txt", "r");
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_undefined));
}

TEST(TestNumbers, TestUnsignedFloatingPoint) {
	std::FILE* f_out = std::fopen("unsigned_floating_point_test.txt", "w+");
	fprintf(f_out, "12.34");

	fclose(f_out);

	std::FILE* f_in = std::fopen("unsigned_floating_point_test.txt", "r");
	//@@@ eventually get_token should return Token::tok_double
  EXPECT_EQ(static_cast<int>(get_token(f_in)), static_cast<int>(Token::tok_undefined));
}
