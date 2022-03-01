#include "tok_val_pair.h"

#ifndef LEXER_H
#define LEXER_H

enum class Token {
  tok_eof = -1,
  tok_identifier = -2,
  tok_integer = -3,
  tok_floating_point = -4,
  tok_undefined = -999
};

TokValPair get_token(const char*&);
TokValPair peek(const char*);

int string_to_int(const std::string, long&);
int string_to_double(const std::string, double&);

#endif
