#include <string>

#include "ast_function.h"
#include "ast_variable.h"
#include "ast_node.h"
#include "data_type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"

using namespace llvm;

ASTFunction::ASTFunction(ASTVariable* prototype, std::vector<ASTNode*> params) : 
	prototype(prototype), params(params) {}

Value* ASTFunction::code_gen(LLVMContext& context, IRBuilder<>& builder) {
	return nullptr;
}

Value* ASTFunction::code_gen(LLVMContext& context, IRBuilder<>& builder, Module& llvm_module) {

  std::vector<Type*> generated_params;
	for(int i = 0; i < params.size(); i++){
		generated_params.push_back(Type::getInt32Ty(context));
	}

	Function* entry_point = Function::Create(
		FunctionType::get(Type::getInt32Ty(context),generated_params, false),
		GlobalValue::LinkageTypes::ExternalLinkage, prototype->name, llvm_module);

	std::vector<Value*> args;
	for(int i = 0; i < params.size(); i++){
		args.push_back(entry_point->getArg(i));
		args.at(i)->setName(dynamic_cast<ASTVariable*>(params.at(i))->name);
	}

  BasicBlock* entry_point_block =
		BasicBlock::Create(context, "entry", entry_point);
		builder.SetInsertPoint(entry_point_block);

	return entry_point;
}

std::string ASTFunction::to_string(int indentation) { return nullptr; }
std::string ASTFunction::to_string() { return nullptr; }
