message("fuzzy")

add_executable(
  fuzzer
  ${PROJECT_SOURCE_DIR}/lexer.cpp
  ${PROJECT_SOURCE_DIR}/fuzz_main.cpp
  ${PROJECT_SOURCE_DIR}/parser.cpp
  ${PROJECT_SOURCE_DIR}/ast_integer.cpp
  ${PROJECT_SOURCE_DIR}/ast_double.cpp
  ${PROJECT_SOURCE_DIR}/ast_bin_expr.cpp
  ${PROJECT_SOURCE_DIR}/ast_variable.cpp
  ${PROJECT_SOURCE_DIR}/ast_function.cpp

  ${PROJECT_SOURCE_DIR}/../include/ast_bin_expr.h
  ${PROJECT_SOURCE_DIR}/../include/ast_double.h
  ${PROJECT_SOURCE_DIR}/../include/ast_integer.h
  ${PROJECT_SOURCE_DIR}/../include/ast_node.h
  ${PROJECT_SOURCE_DIR}/../include/lexer.h
  ${PROJECT_SOURCE_DIR}/../include/parser.h
  ${PROJECT_SOURCE_DIR}/../include/t_vals.h
  ${PROJECT_SOURCE_DIR}/../include/tok_val_pair.h
  ${PROJECT_SOURCE_DIR}/../include/ast_function.h
  ${PROJECT_SOURCE_DIR}/../include/ast_global_block.h
  ${PROJECT_SOURCE_DIR}/../include/ast_variable.h
  ${PROJECT_SOURCE_DIR}/../include/data_type.h
)

set(CMAKE_CXX_FLAGS "-g -Og -fsanitize=fuzzer")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

llvm_map_components_to_libnames(llvm_libs support core irreader fuzzmutate)
target_link_libraries(fuzzer ${llvm_libs})
target_include_directories(fuzzer PUBLIC ${PROJECT_SOURCE_DIR}/../include)

