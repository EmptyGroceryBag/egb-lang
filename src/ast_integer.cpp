#include "ast_integer.h"
#include <string>

//@@@ don't need this anymore
//#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

ASTInteger::ASTInteger(int value) : value(value) {}

std::string ASTInteger::to_string() { return std::to_string(value); }

llvm::Value* ASTInteger::code_gen(llvm::LLVMContext& context,
                                  llvm::IRBuilder<>&) {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), value);
}
