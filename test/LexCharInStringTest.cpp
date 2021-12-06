#include <gtest/gtest.h>
#include <string>
#include <cstdio>
#include <iostream>
#include "lexer.h"

TEST(TestCharInString, TestChar) {
	std::string input = "test";

	for(int i = 0; i < input.size(); i++){
		char next_char = get_char_in_string(input);
		EXPECT_EQ(next_char, input[i]);
	}
}
