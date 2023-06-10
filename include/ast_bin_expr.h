// @@@Debug
#include <iostream>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "ast_node.h"
#include "lexer.h"

#ifndef AST_BIN_EXP_H
#define AST_BIN_EXP_H

class ASTBinExpr : public ASTNode {
 public:
  ASTNode* lhs;
  ASTNode* rhs;
  int op;

  ASTBinExpr(int op, ASTNode* lhs, ASTNode* rhs);

  ~ASTBinExpr() {
    // @@@Debug
    std::cout << "~ASTBinExpr()" << std::endl;
    delete lhs;
    delete rhs;
  }

  Value* code_gen(LLVMContext&, IRBuilder<>&);

  std::string to_string();
  std::string to_string(int);
};

#endif
