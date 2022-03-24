// @@@Debug
#include <iostream>
#include <string>
#include <vector>

#include "ast_node.h"
#include "ast_variable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#ifndef AST_FUNCTION_H
#define AST_FUNCTION_H

using namespace llvm;

class ASTFunction : public ASTNode {
 public:
  ASTVariable* prototype = nullptr;
  std::vector<ASTNode*> params;
  std::vector<ASTNode*> scope;

  ASTFunction(ASTVariable* prototype, std::vector<ASTNode*> params);
  ASTFunction(ASTVariable* prototype);

  ~ASTFunction() {
    // @@@Debug
    std::cout << "~ASTFunction()" << std::endl;
    for (ASTNode* n : params) {
      delete n;
    }
  }

  Value* code_gen(LLVMContext&, IRBuilder<>&);
  Value* code_gen(LLVMContext&, IRBuilder<>&, Module&);

  // @@@ This is just to satiate the base class
  std::string to_string() { return to_string(0); }
  std::string to_string(int n) { return ""; }
};

#endif
