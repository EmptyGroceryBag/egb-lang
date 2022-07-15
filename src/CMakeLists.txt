cmake_minimum_required(VERSION 3.14.5)
project(el VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MSVC_STATIC_CRT 1 CACHE BOOL "Statically link Microsoft CRT")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_definitions(NOMINMAX)
  add_link_options("/ignore:4099")
  if(MSVC_STATIC_CRT)
    set(CMAKE_CXX_FLAGS "/O2 /EHsc /Z7 /MT /wd4624 /wd4267 /wd4244 /wd4099 /wd4715 /wd4129")
  else()
    set(CMAKE_CXX_FLAGS "/O2 /EHsc /Z7 /wd4624 /wd4267 /wd4244 /wd4715 /wd4129")
  endif()
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("not debug")
  set(CMAKE_CXX_FLAGS "-O2 -Werror")
else()
  message("debug")
  set(CMAKE_CXX_FLAGS "-Werror")
  message(${CMAKE_CXX_FLAGS})
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  add_compile_definitions(CMAKE_MACOS)
  if(NOT LLVM_DIR)
    execute_process(
      # on homebrew, "llvm" points to 13 (or whatever the latest version is),
      # so we must append "@12" to the prefix in order to enforce LLVM 12.0.0
      COMMAND brew --prefix llvm
      OUTPUT_VARIABLE raw_prefix
      )
    string(STRIP ${raw_prefix} prefix)
    set(LLVM_DIR ${prefix}/lib/cmake/llvm/)
  endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
endif()

if(WIN32)
  message(STATUS "Winders!")
  add_compile_definitions(CMAKE_WIN32)
endif()

# LLVM
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

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(
  el 
  el_lib
  ${llvm_libs}
)

#set(BUILD_TEST 0 CACHE BOOL "Build test executable")

add_subdirectory(${PROJECT_SOURCE_DIR}/../test test)

set(BUILD_FUZZER OFF CACHE BOOL "Build fuzzer")
# Fuzz Testing
if(NOT WIN32 AND BUILD_FUZZER)
  message(STATUS "Building fuzzer")
  add_subdirectory(${PROJECT_SOURCE_DIR}/../fuzz fuzz)
endif()

# needs to come after set()'s
configure_file(${PROJECT_SOURCE_DIR}/../include/cmake_config.h.in 
  ${PROJECT_SOURCE_DIR}/../include/cmake_config.h)
    
target_include_directories(el PUBLIC ${PROJECT_SOURCE_DIR}/../include)
target_include_directories(el_lib PUBLIC ${PROJECT_SOURCE_DIR}/../include)
