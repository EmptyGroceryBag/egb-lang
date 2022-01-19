#include "ast_double.h"
#include <string>

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

ASTDouble::ASTDouble(double value) : value(value) {}

std::string ASTDouble::to_string() { return std::to_string(value); }

llvm::Value* ASTDouble::code_gen(llvm::LLVMContext& context) {
  return llvm::ConstantFP::get(context, llvm::APFloat(value));
}
