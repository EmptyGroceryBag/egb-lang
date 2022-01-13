#include "ast_integer.h"
#include <string>

ASTInteger::ASTInteger(int value) : value(value) {}

std::string ASTInteger::to_string() { return std::to_string(value); }