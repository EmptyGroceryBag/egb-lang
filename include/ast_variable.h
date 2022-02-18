#include <string>

#include "ast_node.h"
//#include "data_type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#ifndef AST_VARIABLE_H
#define AST_VARIABLE_H

using namespace llvm;

class ASTVariable : public ASTNode {
 public:
  struct Attributes {
    bool sign;
    int width;

    bool operator==(Attributes& rhs) {
      if (sign == rhs.sign && width == rhs.width) {
        return true;
      }
      return false;
    }
  } attributes;

  ASTVariable(std::string name, Attributes attributes, ASTNode* value);
  ASTVariable(std::string name, Attributes attributes);

  std::string name;
  ASTNode* value = nullptr;

  Value* code_gen(LLVMContext&, IRBuilder<>&);

  // @@@ This is just to satiate the base class
  std::string to_string() { return to_string(0); }
  std::string to_string(int n) { return ""; }
};

#endif
