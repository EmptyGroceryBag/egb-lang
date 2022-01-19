#include "ast_integer.h"
#include <string>

#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

ASTInteger::ASTInteger(int value) : value(value) {}

std::string ASTInteger::to_string() { return std::to_string(value); }

llvm::Value* ASTInteger::code_gen(llvm::LLVMContext& context) {
  return llvm::ConstantInt::get(context, llvm::APInt(32, value));
}
