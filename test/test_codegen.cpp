#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_function.h"
#include "ast_integer.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"
#include "parser.h"

using namespace llvm;

static std::string buffer;
static LLVMContext context;
static Module llvm_module("main_mod", context);
static IRBuilder<> builder(context);

static std::ostringstream test_output;
static raw_os_ostream output_stream(test_output);

TEST(test_ir_codegen, test_unsigned_integer_value) {
  Parser parser("1234");
  ASTInteger* node = dynamic_cast<ASTInteger*>(parser.parse_primary_expr());
  ASSERT_TRUE(node);

  Value* codegen_integer = node->code_gen(context, builder);
  ASSERT_TRUE(codegen_integer);
  Type* check_integer_type = codegen_integer->getType();
  EXPECT_TRUE(check_integer_type->isIntegerTy(32));

  codegen_integer->print(output_stream);
  EXPECT_EQ(test_output.str(), "i32 1234");
  test_output.str("");
}

TEST(test_ir_codegen, test_unsigned_double_value) {
  Parser parser("1234.4572542");
  ASTDouble* node = dynamic_cast<ASTDouble*>(parser.parse_primary_expr());
  ASSERT_TRUE(node);

  Value* codegen_double = node->code_gen(context, builder);
  ASSERT_TRUE(codegen_double);
  Type* check_double_type = codegen_double->getType();
  EXPECT_TRUE(check_double_type->isFloatingPointTy());

  codegen_double->print(output_stream);
  EXPECT_EQ(test_output.str(), "double 0x409349D43A71EBD6");
  test_output.str("");
}

TEST(test_ir_codegen, test_bin_expr_value) {
  Parser parser("2+4");
  ASTBinExpr* node = dynamic_cast<ASTBinExpr*>(parser.parse_primary_expr());
  ASSERT_TRUE(node);

  Value* bin_expr_value = node->code_gen(context, builder);
  ASSERT_TRUE(bin_expr_value);

  bin_expr_value->print(output_stream);
  EXPECT_EQ(test_output.str(), "i32 6");
  test_output.str("");
}

/* @@@For when we do proper scoping
TEST(test_ir_codegen, test_generate_variable_declaration) {
  Parser parser("u64 x;");
  ASTVariable* decl_expr =
    dynamic_cast<ASTVariable*>(parser.parse_top_level_expr());
  ASSERT_TRUE(decl_expr);

  Value* check_function =
    decl_expr->code_gen(context, builder);

  check_function->print(output_stream);
  std::cout << test_output.str() << std::endl;
  EXPECT_EQ(test_output.str(), "");
  test_output.str("");
}
*/

TEST(test_ir_codegen, test_generate_function_dump) {
  Parser parser("u8 funcy() {}");
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
  ASSERT_TRUE(prototype_expr);

  Value* check_function =
      prototype_expr->code_gen(context, builder, llvm_module);

  check_function->print(output_stream);
  std::cout << test_output.str() << std::endl;
  EXPECT_EQ(test_output.str(), "define i8 @funcy() {\nentry:\n}\n");
  test_output.str("");
}

TEST(test_ir_codegen, test_generate_function_dump_single_parameter) {
  Parser parser("u16 doofus(u64 param) {}");
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
  ASSERT_TRUE(prototype_expr);

  Value* check_function =
      prototype_expr->code_gen(context, builder, llvm_module);

  check_function->print(output_stream);
  std::cout << test_output.str() << std::endl;
  EXPECT_EQ(test_output.str(),
            "define i16 @doofus(i64 \%param) {\nentry:\n}\n");
  test_output.str("");
}

TEST(test_ir_codegen, test_generate_function_dump_mulitple_parameters) {
  Parser parser("u64 monke(bool x1, u8 x2, u16 y1, u32 y2) {}");
  ASTFunction* prototype_expr =
      dynamic_cast<ASTFunction*>(parser.parse_top_level_expr());
  ASSERT_TRUE(prototype_expr);

  Value* check_function =
      prototype_expr->code_gen(context, builder, llvm_module);

  check_function->print(output_stream);
  std::cout << test_output.str() << std::endl;
  EXPECT_EQ(test_output.str(),
            "define i64 @monke(i1 \%x1, i8 \%x2, i16 \%y1, i32 \%y2) "
            "{\nentry:\n}\n");
  test_output.str("");
}
