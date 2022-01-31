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
  ASTVariable* prototype;
  std::vector<ASTNode*> params;

  ASTFunction(ASTVariable* prototype, std::vector<ASTNode*> params);

  virtual Value* code_gen(LLVMContext&, IRBuilder<>&);
  Value* code_gen(LLVMContext&, IRBuilder<>&, Module&);

  virtual std::string to_string();
  //@@@ return nullptr?
  virtual std::string to_string(int);
};

#endif
