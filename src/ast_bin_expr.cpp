#include <iostream>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast_bin_expr.h"

ASTBinExpr::ASTBinExpr(int op, ASTNode* lhs, ASTNode* rhs)
    : op(op), lhs(lhs), rhs(rhs){};

Value* ASTBinExpr::code_gen(LLVMContext& context, IRBuilder<>& builder) {
  Value* lhs_value = lhs->code_gen(context, builder);
  Value* rhs_value = rhs->code_gen(context, builder);

  return builder.CreateAdd(lhs_value, rhs_value);
}

std::string ASTBinExpr::to_string(int indentation) {
#ifdef _MSC_VER
  std::string type_str = "ASTBinExpr:\n";
#else
  std::string type_str = "\e[0;35mASTBinExpr:\n\e[0;37m";
#endif

  std::string lhs_str;
  if (dynamic_cast<ASTBinExpr*>(lhs)) {
    lhs_str = "  |-lhs: " + lhs->to_string(indentation + 1) + "\n";
  } else {
    lhs_str = "  |-lhs: " + lhs->to_string() + "\n";
  }

  std::string rhs_str;
  if (dynamic_cast<ASTBinExpr*>(rhs)) {
    rhs_str = "  |-rhs: " + rhs->to_string(indentation + 1) + "\n";
  } else {
    rhs_str = "  |-rhs: " + rhs->to_string() + "\n";
  }

  std::string op_str = "  |-op: ";
  op_str += static_cast<char>(op);

  if (indentation) {
    for (int i = 0; i < indentation; i++) {
      type_str = "  " + type_str;
      lhs_str = "  " + lhs_str;
      rhs_str = "  " + rhs_str;
      op_str = "  " + op_str;
    }

    type_str = "\n" + type_str;
  }

  return type_str + lhs_str + rhs_str + op_str;
}

std::string ASTBinExpr::to_string() { return to_string(0); }
