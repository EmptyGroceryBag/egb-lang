#include <gtest/gtest.h>
#include <string>
#include <cstdio>
#include "lexer.h"

TEST(TestStringToDouble, TestUnsignedDouble) {
	double output = 0.0;
	std::string input = "1234.0";
	string_to_double(input, output);
	EXPECT_EQ(output, 1234.0);
}

TEST(TestStringToDouble, TestUnsignedDoubleNoIntegral) {
	double output = 0.0;
	std::string input = ".5";
	string_to_double(input, output);
	EXPECT_EQ(output, 0.5);
}

TEST(TestStringToDouble, TestMalformedUnsignedDouble) {
	double output = 0.0;
	std::string input = "1234.0.1";

	int val = string_to_double(input, output);
	EXPECT_EQ(output, 0.0);
	EXPECT_EQ(val, 6);

	input = "1.1.1.1.1.1";
	string_to_double(input, output); 
	EXPECT_EQ(output, 0.0);
}
