#include "ast_variable.h"

#include "ast_node.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

ASTVariable::ASTVariable(std::string name, Attributes attributes,
                         ASTNode* value)
    : name(name), attributes(attributes), value(value) {}

ASTVariable::ASTVariable(std::string name, Attributes attributes)
    : name(name), attributes(attributes) {}

Value* ASTVariable::code_gen(LLVMContext& context, IRBuilder<>& builder) {
  if (!value) {
    return value->code_gen(context, builder);
  }
  return builder.CreateAlloca(Type::getInt32Ty(context),
                              value->code_gen(context, builder), name);
}

//@@@
std::string ASTVariable::to_string(int indentation) { return nullptr; }
std::string ASTVariable::to_string() { return nullptr; }
