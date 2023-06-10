#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "ast_node.h"

#ifndef AST_INTEGER_H
#define AST_INTEGER_H

using namespace llvm;

class ASTInteger : public ASTNode {
 public:
  int value;

  ASTInteger(int);

  Value* code_gen(LLVMContext&, IRBuilder<>&);

  // @@@ This is just to satiate the base class
  std::string to_string();
  std::string to_string(int n) { return ""; }
};
#endif
