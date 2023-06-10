#include <string>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "ast_integer.h"

ASTInteger::ASTInteger(int value) : value(value) {}

std::string ASTInteger::to_string() { return std::to_string(value); }

Value* ASTInteger::code_gen(LLVMContext& context, IRBuilder<>&) {
  return ConstantInt::get(Type::getInt32Ty(context), value);
}
