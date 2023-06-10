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

#include <cstdio>
#include <iostream>
#include <string>

#include "lexer.h"
#include "t_vals.h"
#include "tok_val_pair.h"

TokValPair get_token(const char*& iterator) {
  TVals vals;
  TokValPair pair;

next_token:
  // Whitespace
  while (isspace(*iterator)) {
    iterator++;  // eat
  }

  // Numbers
  // @@@ Differentiate between unary (negate) and binary '-' (subtract) operator
  if (isdigit(*iterator) || *iterator == '.') {
    pair.token_value.num_str += *(iterator++);

    while (isdigit(*iterator) || *iterator == '.')
      pair.token_value.num_str += *(iterator++);

    if (string_to_int(pair.token_value.num_str, pair.token_value.int_num_val)) {
      if (string_to_double(pair.token_value.num_str,
                           pair.token_value.double_num_val)) {
        pair.token_value.ident_str = pair.token_value.num_str;
        pair.token_type = static_cast<int>(Token::tok_identifier);
        return pair;
      } else {
        pair.token_type = static_cast<int>(Token::tok_floating_point);
        return pair;
      }
    }
    pair.token_type = static_cast<int>(Token::tok_integer);
    return pair;
  }

  // Identifiers
  if (isalpha(*iterator) || *iterator == '_') {
    /*
    identifiers must begin with a letter and can end with any combinations of
    letters and numbers
    */
    pair.token_value.ident_str += *(iterator++);

    while (isalnum(*iterator) || *iterator == '_')
      pair.token_value.ident_str += *(iterator++);

    pair.token_type = static_cast<int>(Token::tok_identifier);
    return pair;
  }

  // Ignore comments
  // @@@Cleanup: New formatting rules for single-line loops and conditionals
  if (*iterator == '#') {
    while (*iterator != '\n' && *iterator != '\0') iterator++;
    if (*iterator == '\n') {
      iterator++;
      goto next_token;
    }
  }

  if (*iterator == '\0') {
    pair.token_type = static_cast<int>(Token::tok_eof);
    return pair;
  }

  pair.token_type = (*iterator);
  iterator++;
  return pair;
}

int string_to_int(const std::string input_num, long& output_num) {
  int decimal_point = input_num.find('.');

  if (decimal_point != -1) return decimal_point;

  try {
    output_num = stoi(input_num);
  } catch (std::out_of_range e) {
    std::cout << e.what() << std::endl;
  }
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

TokValPair peek(const char* iterator) { return get_token(iterator); }
