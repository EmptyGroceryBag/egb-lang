#include <vector>

#ifndef AST_TYPES_H
#define AST_TYPES_H

namespace DataTypes {
  std::vector<std::string> type_strings = {
    "bool", 
    "uint8", 
    "uint16",
    "uint32",
    "uint64",

    // Signed integers
    "int8",
    "int16",
    "int32",
    "int64",

    "double"

    "void"
  };
};

#endif
