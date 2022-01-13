#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "lexer.h"

TEST(TestCharInString, TestChar) {
  std::string input = "test";

  for (int i = 0; i < input.size(); i++) {
    char next_char = next_char_in_string(input);
    EXPECT_EQ(next_char, input[i]);
  }
}
