#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include <ast_node.h>

#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

using namespace llvm;

class ASTDouble : public ASTNode {
 public:
  double value;

  ASTDouble(double);

  Value* code_gen(LLVMContext&, IRBuilder<>&);

  // @@@ This is just to satiate the base class
  std::string to_string();
  std::string to_string(int n) { return ""; }
};

#endif
