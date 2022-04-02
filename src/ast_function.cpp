#include <string>

#include "ast_function.h"
#include "ast_node.h"
#include "ast_variable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"

using namespace llvm;

ASTFunction::ASTFunction(ASTVariable* prototype, std::vector<ASTNode*> params)
    : prototype(prototype), params(params) {}

ASTFunction::ASTFunction(ASTVariable* prototype)
    : prototype(prototype) {}

Value* ASTFunction::code_gen(LLVMContext& context, IRBuilder<>& builder) {
  return nullptr;
}

// @@@TODO: Detect if the function has a body. If so, generate a llvm::BasicBlock,
// and visit each node in p.insertion_stack.top(). To do this, we need to find a
// good way to pass the Parser object to each node. Maybe we should make the Parser
// a singleton class. Alternatively, we can make the insertion_stack a global in
// main.cpp

Value* ASTFunction::code_gen(LLVMContext& context, IRBuilder<>& builder,
                             Module& llvm_module) {
  std::vector<Type*> generated_params;
  if (params.size() != 0) {
    for (int i = 0; i < params.size(); i++) {
      ASTVariable* param = dynamic_cast<ASTVariable*>(params[i]);
      generated_params.push_back(Type::getIntNTy(context, param->attributes.width));
    }
  }

  Function* entry_point = Function::Create(
      FunctionType::get(Type::getIntNTy(context, prototype->attributes.width), generated_params, false),
      GlobalValue::LinkageTypes::ExternalLinkage, prototype->name, llvm_module);

  std::vector<Value*> args;
  for (int i = 0; i < params.size(); i++) {
    args.push_back(entry_point->getArg(i));
    args.at(i)->setName(dynamic_cast<ASTVariable*>(params.at(i))->name);
  }

  BasicBlock* entry_point_block =
      BasicBlock::Create(context, "entry", entry_point);
  builder.SetInsertPoint(entry_point_block);

  return entry_point;
}
