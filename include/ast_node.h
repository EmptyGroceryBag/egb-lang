#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#ifndef AST_NODE_H
#define AST_NODE_H

using namespace llvm;

class ASTNode {
 public:
  virtual ~ASTNode() = default;

  virtual llvm::Value* code_gen(LLVMContext&, IRBuilder<>&) = 0;

  virtual std::string to_string(int indentation) = 0;
  virtual std::string to_string() = 0;
};

#endif
