/*
Copyright 2021 Joey Meisner

This file is part of egb-lang.

egb-lang is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

egb-lang is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along
with egb-lang.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "lexer.h"
#include <cstdio>
#include <iostream>
#include <string>

#include "t_vals.h"
#include "tok_val_pair.h"

TokValPair* get_token(const char*& iterator) {
  //@@@ do I need heap allocation here?
  TVals* vals = new TVals();
  vals->ident_str = "";
  vals->num_str = "";

  //@@@ do I need heap allocation here?
  TokValPair* pair = new TokValPair;
  pair->token_value = vals;

  // Whitespace
  while (isspace(*iterator)) {
    iterator++;  // eat
  }

  // Numbers
  // @@@ Differentiate between unary (negate) and binary '-' (subtract) operator
  if (isdigit(*iterator) || *iterator == '.') {
    vals->num_str += *(iterator++);

    while (isdigit(*iterator) || *iterator == '.') {
      vals->num_str += *(iterator++);
    }

    if (string_to_int(vals->num_str, vals->int_num_val)) {
      if (string_to_double(vals->num_str, vals->double_num_val)) {
        pair->token_type = static_cast<int>(Token::tok_undefined);
      } else {
        pair->token_type = static_cast<int>(Token::tok_floating_point);
        return pair;
      }
    }
    pair->token_type = static_cast<int>(Token::tok_integer);
    return pair;
  }

  // Identifiers
  if (isalpha(*iterator) || *iterator == '_') {
    /*
    identifiers must begin with a letter and can end with any combinations of
    letters and numbers
    */
    vals->ident_str += *(iterator++);

    while (isalnum(*iterator) || *iterator == '_') {
      vals->ident_str += *(iterator++);
    }

    if (vals->ident_str == "def") {
      pair->token_type = static_cast<int>(Token::tok_def);
      return pair;
    }

    if (vals->ident_str == "extern") {
      pair->token_type = static_cast<int>(Token::tok_extern);
      return pair;
    }

    pair->token_type = static_cast<int>(Token::tok_identifier);
    return pair;
  }

  // Ignore comments - @@@ we probably don't need to return an undefined token
  // @@@ we can't parse comments that come directly before EOF
  if (*iterator == '#') {
    while (*iterator != '\n') {
      iterator++;
    }

    pair->token_type = static_cast<int>(Token::tok_undefined);
    return pair;
  }

  if (*iterator == '\0') {
    pair->token_type = static_cast<int>(Token::tok_eof);
    return pair;
  }

  pair->token_type = (*iterator);
  iterator++;
  return pair;
}

int string_to_int(const std::string input_num, long& output_num) {
  int decimal_point = input_num.find('.');

  if (decimal_point != -1) {
    return decimal_point;
  }

  output_num = stoi(input_num);
  return 0;
}

int string_to_double(const std::string input_num, double& output_num) {
  int first_decimal_point = input_num.find('.');
  int duplicate_decimal_point = 0;

  if ((duplicate_decimal_point =
           input_num.find('.', first_decimal_point + 1)) != -1) {
    return duplicate_decimal_point;
  }

  output_num = stod(input_num);
  return 0;
}

TokValPair* peek(const char* iterator) {
  const char* base_ptr = iterator;  // hopefully this makes a copy
  return get_token(base_ptr);
}
