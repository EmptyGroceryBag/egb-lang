#include <string>
#include <vector>

#include "ast_node.h"

#ifndef AST_GLOBAL_BLOCK_H
#define AST_GLOBAL_BLOCK_H

class ASTGlobalBlock {
 private:
  ASTGlobalBlock() = default;

 public:
  static ASTGlobalBlock& get_global_block() {
    static ASTGlobalBlock GLOBAL_BLOCK;
    return GLOBAL_BLOCK;
  }

  std::vector<ASTNode*> global_scope;

  ASTGlobalBlock(const ASTGlobalBlock& rhs) = delete;
  ASTGlobalBlock operator=(const ASTGlobalBlock& rhs) = delete;
};

#endif