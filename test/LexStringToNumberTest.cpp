#include <gtest/gtest.h>
#include <string>
#include "lexer.h"

TEST(TestStringToInteger, TestUnsignedInteger) {
	int output = 0;
	std::string input = "1234";
	string_to_int(input, output);
	EXPECT_EQ(output, 1234);
}

//TODO: Write other integer tests

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
	EXPECT_EQ(output, .5);
}

TEST(TestStringToDouble, TestMalformedUnsignedDouble) {
	double output = 0.0;
	std::string input = "1234.0.1";
	int index = string_to_double(input, output); // index of first duplicate decimal point
	EXPECT_EQ(output, 0.0); // we're expecting the output to be unchanged
	EXPECT_EQ(index, 6);

	input = "1.1.1.1.1.1";
	EXPECT_EQ(output, 0.0);
	EXPECT_EQ(string_to_double(input, output), 3);
}

TEST(TestStringToDouble, TestMalformedSignedDouble) {
	double output = 0.0;
	std::string input = "-1234.0.1";

	int index = string_to_double(input, output);
	EXPECT_EQ(output, 0.0);
	EXPECT_EQ(index, 7);

	input = "1.1.1.1.1.1";
	index = string_to_double(input, output);
	EXPECT_EQ(output, 0.0);
	EXPECT_EQ(index, 3);
}