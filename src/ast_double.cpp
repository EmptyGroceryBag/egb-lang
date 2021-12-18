#include <string>
#include "ast_double.h"

ASTDouble::ASTDouble(double value) : value(value) {}

std::string ASTDouble::to_string() {
	return std::to_string(value);
}