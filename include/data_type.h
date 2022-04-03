#include <vector>

#ifndef AST_TYPES_H
#define AST_TYPES_H

namespace DataTypes {
  std::vector<std::string> type_strings = {
    "bool", "u8", "u16", "u32",
    "u64",
    // Signed integers
    "i8", "i16", "i32", "i64",
    "double",
    "void"
  };
};

#endif
