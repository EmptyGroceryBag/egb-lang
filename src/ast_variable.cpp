#include "ast_variable.h"

#include "ast_node.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_os_ostream.h"

#include <vector>

using namespace llvm;

ASTVariable::ASTVariable(std::string name, Attributes attributes,
                         std::vector<ASTNode*>* enclosing_scope, ASTNode* value)
    : name(name), attributes(attributes), enclosing_scope(enclosing_scope), value(value) {}

ASTVariable::ASTVariable(std::string name, Attributes attributes, std::vector<ASTNode*>* enclosing_scope)
    : name(name), attributes(attributes), enclosing_scope(enclosing_scope) {}

Value* ASTVariable::code_gen(LLVMContext& context, IRBuilder<>& builder) {
  if (!value) {
    return builder.CreateAlloca(Type::getIntNTy(context, attributes.width), nullptr, name);
  }
  return builder.CreateAlloca(Type::getIntNTy(context, attributes.width),
                              value->code_gen(context, builder), 
                              name);
}
