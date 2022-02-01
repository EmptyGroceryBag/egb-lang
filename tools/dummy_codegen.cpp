#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_os_ostream.h"

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
#include "parser.h"
#include "dummy_codegen.h"

using namespace llvm;

void dummy() {
	Parser parser;

  std::ostringstream test_output;
  raw_os_ostream output_stream(test_output);

  std::string buffer = "2+4";
  parser.iterator    = &buffer[0];
  ASTBinExpr* node   = dynamic_cast<ASTBinExpr*>(parser.parse_top_level_expr());

	std::unique_ptr<LLVMContext> context = std::make_unique<LLVMContext>();
	std::unique_ptr<Module> llvm_module = std::make_unique<Module>("main_mod", *context);
	std::unique_ptr<IRBuilder<>> builder = std::make_unique<IRBuilder<>>(*context);

	Function* entry_point = Function::Create (
		FunctionType::get(Type::getVoidTy(*context), false),
		GlobalValue::LinkageTypes::ExternalLinkage,
		"main",
		*llvm_module
	);

	BasicBlock* entry_point_block = BasicBlock::Create(*context, "entry", entry_point);
    builder->SetInsertPoint(entry_point_block);
	//Value* add = node->code_gen(*context, *builder);
	//add->print(output_stream);

	Value* lhs = ConstantInt::get(Type::getInt32Ty(*context), 2);
	Value* rhs = ConstantInt::get(Type::getInt32Ty(*context), 4);
    Value* add = builder->CreateAdd(lhs, rhs);
    Value* var = builder->CreateAlloca(Type::getInt32Ty(*context), add, "temp_add");

    builder->CreateRetVoid();
	//node->code_gen(context, builder);

	//entry_point->print(output_stream);
	llvm_module->print(output_stream, nullptr);
	std::cout << test_output.str() << std::endl;
}
