#include <memory>

#include "t_vals.h"

#ifndef TOK_VAL_PAIR_H
#define TOK_VAL_PAIR_H

struct TokValPair {
  int token_type;
  TVals token_value;

  TokValPair() {
    token_type = 0;

    TVals init_token_value;
    token_value = init_token_value;
  }

  /*
  TokValPair operator=(TokValPair& rhs) {
    TokValPair new_pair;
    new_pair.token_type = rhs.token_type;
    new_pair.token_value = rhs.token_value;

    return new_pair;
  }
  */
};

#endif
