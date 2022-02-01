#include "ast_node.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

using namespace llvm;

class ASTDouble : public ASTNode {
 public:
  double value;

  ASTDouble(double);

  virtual Value* code_gen(LLVMContext&, IRBuilder<>&);

  virtual std::string to_string();
  //@@@ return nullptr?
  virtual std::string to_string(int) { return nullptr; };
};

#endif
