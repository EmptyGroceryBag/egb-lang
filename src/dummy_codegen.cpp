#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_os_ostream.h>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
#include "parser.h"

#if (WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#if (UNIX)
#include <unistd.h>
#endif

using namespace llvm;

void dummy() {
  Parser parser;

  std::ostringstream test_output;
  raw_os_ostream output_stream(test_output);

  std::string buffer = "2+4";
  parser.iterator = &buffer[0];
  ASTBinExpr* node = dynamic_cast<ASTBinExpr*>(parser.parse_top_level_expr());

  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);

  Function* entry_point = Function::Create(
      FunctionType::get(Type::getVoidTy(context), false),
      GlobalValue::LinkageTypes::ExternalLinkage, "main", llvm_module);

  BasicBlock* entry_point_block =
      BasicBlock::Create(context, "entry", entry_point);
  builder.SetInsertPoint(entry_point_block);
  // Value* add = node->code_gen(context, *builder);
  // add->print(output_stream);

  // CreateAdd() will do constant folding
  // Value* lhs = ConstantInt::get(Type::getInt32Ty(context),
  // 2); Value* rhs =
  // ConstantInt::get(Type::getInt32Ty(context), 4);
  ConstantInt* lhs = builder.getInt32(2);
  ConstantInt* rhs = builder.getInt32(8);
  Value* add = builder.CreateAdd(lhs, rhs);
  Value* var = builder.CreateAlloca(Type::getInt32Ty(context), add, "x");

  // CreateBinOp() will also fold its operands. My guess is that CreateAdd()
  // calls CreateBinOp()
  Value* binop =
      builder.CreateBinOp(Instruction::BinaryOps::Add, lhs, rhs, "temp");
  Value* var1 = builder.CreateAlloca(Type::getInt32Ty(context), binop, "x");

  Value* test_load =
      builder.CreateLoad(Type::getInt32Ty(context), var1, "test_load");

  builder.CreateRetVoid();
  // node->code_gen(context, builder);

  int num_args = 2;
  std::vector<Type*> int32_params(num_args, Type::getInt32Ty(context));

  Function* add_func = Function::Create(
      FunctionType::get(Type::getInt32Ty(context), int32_params, false),
      GlobalValue::LinkageTypes::ExternalLinkage, "add", llvm_module);

  BasicBlock* add_func_block = BasicBlock::Create(context, "entry", add_func);
  builder.SetInsertPoint(add_func_block);

  std::vector<Value*> arg_values;
  for (int i = 0; i < num_args; i++) {
    arg_values.push_back(add_func->getArg(i));
  }

  builder.CreateRet(builder.CreateAdd(arg_values[0], arg_values[1]));

  // entry_point->print(output_stream);
  llvm_module.print(output_stream, nullptr);
  std::cout << test_output.str() << std::endl;
}
