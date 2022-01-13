#include "ast_node.h"

#ifndef AST_INTEGER_H
#define AST_INTEGER_H

class ASTInteger : public ASTNode {
 public:
  int value;

  ASTInteger(int);

  virtual std::string to_string();
  //@@@ return nullptr?
  virtual std::string to_string(int) { return nullptr; }
};

#endif