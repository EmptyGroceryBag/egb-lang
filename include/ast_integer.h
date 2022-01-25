#include "ast_node.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#ifndef AST_INTEGER_H
#define AST_INTEGER_H

class ASTInteger : public ASTNode {
 public:
  int value;

  ASTInteger(int);

  virtual llvm::Value* code_gen(llvm::LLVMContext&, llvm::IRBuilder<>&);

  virtual std::string to_string();
  //@@@ return nullptr?
  virtual std::string to_string(int) { return nullptr; }
};

#endif
