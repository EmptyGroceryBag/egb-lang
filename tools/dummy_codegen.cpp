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

void dummy() {
	Parser parser;

  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);

  std::string buffer = "2+4";
  parser.iterator    = &buffer[0];
  ASTBinExpr* node   = dynamic_cast<ASTBinExpr*>(parser.parse_top_level_expr());

	std::unique_ptr<llvm::LLVMContext> context = std::make_unique<llvm::LLVMContext>();
	std::unique_ptr<llvm::Module> llvm_module = std::make_unique<llvm::Module>("main_mod", *context);
	std::unique_ptr<llvm::IRBuilder<>> builder = std::make_unique<llvm::IRBuilder<>>(*context);

	llvm::Function* entry_point = llvm::Function::Create (
		llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false),
		llvm::GlobalValue::LinkageTypes::ExternalLinkage,
		"main",
		*llvm_module
	);

	llvm::BasicBlock* entry_point_block = llvm::BasicBlock::Create(*context, "entry", entry_point);
    builder->SetInsertPoint(entry_point_block);
	//llvm::Value* add = node->code_gen(*context, *builder);
	//add->print(output_stream);

	llvm::Value* lhs = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 2);
	llvm::Value* rhs = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 4);
    llvm::Value* add = builder->CreateAdd(lhs, rhs);
    llvm::Value* var = builder->CreateAlloca(llvm::Type::getInt32Ty(*context), add, "temp_add");

    builder->CreateRetVoid();
	//node->code_gen(context, builder);

	//entry_point->print(output_stream);
	llvm_module->print(output_stream, nullptr);
	std::cout << test_output.str() << std::endl;
}
