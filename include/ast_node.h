#include <string>

#ifndef AST_NODE_H
#define AST_NODE_H

class ASTNode {
 public:
  virtual ~ASTNode() = default;

  virtual std::string to_string(int indentation) = 0;
  virtual std::string to_string() = 0;
};

#endif
