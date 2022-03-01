#include <memory>
#include "t_vals.h"

#ifndef TOK_VAL_PAIR_H
#define TOK_VAL_PAIR_H

struct TokValPair {
  int token_type;
  TVals token_value;

  TokValPair(int token_type,
             TVals token_value) :
  token_type(token_type),
  token_value(token_value) {}
};

#endif
