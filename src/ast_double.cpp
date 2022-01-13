#include "ast_double.h"
#include <string>

ASTDouble::ASTDouble(double value) : value(value) {}

std::string ASTDouble::to_string() { return std::to_string(value); }