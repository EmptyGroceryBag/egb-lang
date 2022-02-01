#include "ast_double.h"
#include <string>

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

ASTDouble::ASTDouble(double value) : value(value) {}

std::string ASTDouble::to_string() { return std::to_string(value); }

Value* ASTDouble::code_gen(LLVMContext& context,
                                 IRBuilder<>&) {
  return ConstantFP::get(context, APFloat(value));
}
