#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <cmd_options.h>

TEST(test_cmd_options, test_output_file) {
  char* args[2] = { "el.exe", "-o" };
  int argc = 2;

  CmdOptions options;
  std::vector<CmdOptions::Option>* option_list = options.parse_options(argc, args);
  ASSERT_EQ(option_list->size(), 1);
  EXPECT_EQ(option_list->at(0).kind, CmdOptions::OptionKind::output_file);
}