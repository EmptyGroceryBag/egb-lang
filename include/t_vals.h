#include <string>

#ifndef T_VALS_H
#define T_VALS_H

struct TVals {
  std::string num_str;
  std::string ident_str;
  long int_num_val;
  double double_num_val;

  TVals() {
    num_str = "";
    ident_str = "";
    int_num_val = 0;
    double_num_val = 0.0;
  }
};
#endif
