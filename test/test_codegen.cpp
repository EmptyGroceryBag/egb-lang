#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
#include "ast_function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"
#include "parser.h"

static Parser parser;

TEST(test_ir_codegen, test_unsigned_integer_value) {
  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);

  std::string buffer = "1234";
  parser.iterator = &buffer[0];
  ASTInteger* node = dynamic_cast<ASTInteger*>(parser.parse_top_level_expr());
  ASSERT_TRUE(node);

  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);

  llvm::Value* codegen_integer = node->code_gen(context, builder);
  ASSERT_TRUE(codegen_integer);
  llvm::Type* check_integer_type = codegen_integer->getType();
  EXPECT_TRUE(check_integer_type->isIntegerTy(32));
  codegen_integer->print(output_stream);
  EXPECT_EQ(test_output.str(), "i32 1234");
}

TEST(test_ir_codegen, test_unsigned_double_value) {
  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);

  std::string buffer = "1234.4572542";
  parser.iterator = &buffer[0];
  ASTDouble* node = dynamic_cast<ASTDouble*>(parser.parse_top_level_expr());
  ASSERT_TRUE(node);

  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);

  llvm::Value* codegen_double = node->code_gen(context, builder);
  ASSERT_TRUE(codegen_double);
  llvm::Type* check_double_type = codegen_double->getType();
  EXPECT_TRUE(check_double_type->isFloatingPointTy());

  codegen_double->print(output_stream);
  EXPECT_EQ(test_output.str(), "double 0x409349D43A71EBD6");
}

TEST(test_ir_codegen, test_bin_expr_value) {
  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);

  std::string buffer = "2+4";
  parser.iterator = &buffer[0];
  ASTBinExpr* node = dynamic_cast<ASTBinExpr*>(parser.parse_top_level_expr());
  ASSERT_TRUE(node);

  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);

  llvm::Value* bin_expr_value = node->code_gen(context, builder);
  ASSERT_TRUE(bin_expr_value);

  bin_expr_value->print(output_stream);
  EXPECT_EQ(test_output.str(), "i32 6");
}

TEST(test_ir_codegen, test_function_dump) {
  std::string buffer = "uint32 name_name()";
  parser.iterator = &buffer[0];
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
	ASSERT_TRUE(prototype_expr);

  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);
	Value* check_function = prototype_expr->code_gen(context, builder, llvm_module);

  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);
	check_function->print(output_stream);
	std::cout << test_output.str() << std::endl;
	//EXPECT_EQ(test_output.str(), "declare i32 @name_name()\n");
}

TEST(test_ir_codegen, test_function_dump_single_parameter) {
  std::string buffer = "uint32 name_name(uint32 param)";
  parser.iterator = &buffer[0];
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
	ASSERT_TRUE(prototype_expr);

  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);
	Value* check_function = prototype_expr->code_gen(context, builder, llvm_module);

  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);
	check_function->print(output_stream);
	std::cout << test_output.str() << std::endl;	
	//EXPECT_EQ(test_output.str(), "declare i32 @name_name()\n");
}

TEST(test_ir_codegen, test_function_dump_mulitple_parameters) {
  std::string buffer = "uint32 name_name(uint32 x1, uint32 x2, uint32 y1, uint32 y2)";
  parser.iterator = &buffer[0];
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
	ASSERT_TRUE(prototype_expr);

  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);
	Value* check_function = prototype_expr->code_gen(context, builder, llvm_module);

  std::ostringstream test_output;
  llvm::raw_os_ostream output_stream(test_output);
	check_function->print(output_stream);
	std::cout << test_output.str() << std::endl;	
	//EXPECT_EQ(test_output.str(), "declare i32 @name_name()\n");
}

/*
void dummy();
TEST(TEST, TESTY){
	dummy();
}
*/
