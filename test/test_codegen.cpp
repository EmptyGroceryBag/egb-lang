#include <gtest/gtest.h>
#include <ostream>
#include <string>

#include "ast_bin_expr.h"
#include "ast_double.h"
#include "ast_integer.h"
//@@@ debug
#include "llvm/ADT/APInt.h"
//@@@ end debug
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"
#include "parser.h"

static Parser parser;

TEST(test_codegen, test_unsigned_integer_value) {
  // llvm::raw_os_ostream output_stream(std::cout);

  std::string buffer = "1234";
  parser.iterator    = &buffer[0];
  ASTInteger* node   = dynamic_cast<ASTInteger*>(parser.parse_top_level_expr());
  ASSERT_TRUE(node);

  llvm::LLVMContext context;
  llvm::Value* codegen_integer = node->code_gen(context);
  ASSERT_TRUE(codegen_integer);
  llvm::Type* check_integer_type = codegen_integer->getType();
  EXPECT_TRUE(check_integer_type->isIntegerTy(32));
}

TEST(test_codegen, test_unsigned_double_value) {
  std::string buffer = "1234.4572542";
  parser.iterator    = &buffer[0];
  ASTDouble* node    = dynamic_cast<ASTDouble*>(parser.parse_top_level_expr());
  ASSERT_TRUE(node);

  llvm::LLVMContext context;
  llvm::Value* codegen_double = node->code_gen(context);
  ASSERT_TRUE(codegen_double);
  llvm::Type* check_double_type = codegen_double->getType();
  EXPECT_TRUE(check_double_type->isFloatingPointTy());
}

TEST(test_codegen, test_bin_expr_value) {
  llvm::raw_os_ostream output_stream(std::cout);

	std::string buffer = "2+4";
	parser.iterator    = &buffer[0];
	ASTBinExpr* node   = dynamic_cast<ASTBinExpr*>(parser.parse_top_level_expr());
	ASSERT_TRUE(node);
  std::cout << node->to_string(0) << std::endl;

	llvm::LLVMContext context;
	llvm::IRBuilder<> builder(context);

	llvm::Value* bin_expr_value = node->code_gen(context);
  ASSERT_TRUE(bin_expr_value);
  bin_expr_value->print(output_stream);
}
