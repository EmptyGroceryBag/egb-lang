#include "ast_node.h"
#include "lexer.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#ifndef AST_BIN_EXP_H
#define AST_BIN_EXP_H

class ASTBinExpr : public ASTNode {
 public:
  ASTNode* lhs;
  ASTNode* rhs;
  int op;

  ASTBinExpr(int op, ASTNode* lhs, ASTNode* rhs);

  Value* code_gen(LLVMContext&, IRBuilder<>&);

  virtual std::string to_string(int);
  virtual std::string to_string();
};

#endif
