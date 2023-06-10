#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "cmd_options.h"

TEST(test_cmd_options, test_output_file) {
  const int argc = 3;
  const char* args[argc] = { "el.exe", "-o", "test.ll" };

  CmdOptions options;
  std::vector<CmdOptions::Option>* option_list = options.parse_options(argc, const_cast<char**>(args));
  ASSERT_TRUE(option_list);
  EXPECT_EQ(option_list->size(), 1);
  EXPECT_EQ(option_list->at(0).kind, CmdOptions::OptionKind::output_file);
  EXPECT_EQ(option_list->at(0).arg, "test.ll");

  options.parse_option_list(option_list);
}

TEST(test_cmd_options, test_output_file_no_arg) {
  const int argc = 2;
  const char* args[argc] = { "el.exe", "-o" };

  CmdOptions options;
  std::vector<CmdOptions::Option>* option_list = options.parse_options(argc, const_cast<char**>(args));
  ASSERT_FALSE(option_list);
}

/*
TEST(test_cmd_options, test_emit_ir) {
  char* args[2] = { "el.exe", "-emit-ir" };
  int argc = 2;

  CmdOptions options;
  std::vector<CmdOptions::Option>* option_list = options.parse_options(argc, args);
  ASSERT_EQ(option_list->size(), 1);
  EXPECT_EQ(option_list->at(0).kind, CmdOptions::OptionKind::emit_ir);
}

TEST(test_cmd_options, test_duplicate_options) {
  char* args[2] = { "el.exe", "-emit-ir -emit-ir" };
  int argc = 2;

  CmdOptions options;
  std::vector<CmdOptions::Option>* option_list = options.parse_options(argc, args);
  ASSERT_EQ(option_list->size(), 2);
  EXPECT_EQ(option_list->at(0).kind, CmdOptions::OptionKind::emit_ir);
  EXPECT_EQ(option_list->at(1).kind, CmdOptions::OptionKind::emit_ir);
}
*/
