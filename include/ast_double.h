#include "ast_node.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

class ASTDouble : public ASTNode {
 public:
  double value;

  ASTDouble(double);

  virtual llvm::Value* code_gen(llvm::LLVMContext&, llvm::IRBuilder<>&);

  virtual std::string to_string();
  //@@@ return nullptr?
  virtual std::string to_string(int) { return nullptr; };
};

#endif
