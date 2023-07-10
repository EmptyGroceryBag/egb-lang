cmake_minimum_required(VERSION 3.14.5)
project(el VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# MSVC on Windows
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_definitions(NOMINMAX)
  # Supress warnings LLVM warnings
  set(CMAKE_CXX_FLAGS "/O2 /EHsc /Z7 /wd4624 /wd4267 /wd4244 /wd4099 /wd4715 /wd4129")
# clang on Windows
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32)
  add_compile_definitions(NOMINMAX)
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "-Og -Werror")
else()
  set(CMAKE_CXX_FLAGS "-O2 -Werror")
  message(${CMAKE_CXX_FLAGS})
endif()

# LLVM configuration
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Main executable
add_executable(
  el
  main.cpp
)
target_include_directories(el PUBLIC ${PROJECT_SOURCE_DIR}/../include)

# Core functionality
add_library(
  el_lib

  lexer.cpp
  parser.cpp
  cmd_options.cpp
  ast_integer.cpp
  ast_double.cpp
  ast_bin_expr.cpp
  ast_variable.cpp
  ast_function.cpp

  ${PROJECT_SOURCE_DIR}/../include/ast_bin_expr.h
  ${PROJECT_SOURCE_DIR}/../include/ast_double.h
  ${PROJECT_SOURCE_DIR}/../include/ast_integer.h
  ${PROJECT_SOURCE_DIR}/../include/ast_node.h
  ${PROJECT_SOURCE_DIR}/../include/cmd_options.h
  ${PROJECT_SOURCE_DIR}/../include/lexer.h
  ${PROJECT_SOURCE_DIR}/../include/parser.h
  ${PROJECT_SOURCE_DIR}/../include/t_vals.h
  ${PROJECT_SOURCE_DIR}/../include/tok_val_pair.h
  ${PROJECT_SOURCE_DIR}/../include/ast_function.h
  ${PROJECT_SOURCE_DIR}/../include/ast_global_block.h
  ${PROJECT_SOURCE_DIR}/../include/ast_variable.h
  ${PROJECT_SOURCE_DIR}/../include/data_type.h
)
target_include_directories(el_lib PUBLIC ${PROJECT_SOURCE_DIR}/../include)

# Let el and el_lib use LLVM
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(
  el 
  el_lib
  ${llvm_libs}
)

# Google test
add_subdirectory(${PROJECT_SOURCE_DIR}/../test test)

# Fuzz Testing
set(BUILD_FUZZER OFF CACHE BOOL "Build fuzzer")
if(NOT WIN32 AND BUILD_FUZZER)
  message(STATUS "Building fuzzer")
  add_subdirectory(${PROJECT_SOURCE_DIR}/../fuzz fuzz)
endif()

configure_file(${PROJECT_SOURCE_DIR}/../include/cmake_config.h.in 
  ${PROJECT_SOURCE_DIR}/../include/cmake_config.h)
