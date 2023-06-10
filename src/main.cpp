/*
Copyright 2021 Joey Meisner

This file is part of egb-lang.

egb-lang is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

egb-lang is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along
with egb-lang.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// @@@ TODO: Don't include windows.h
#include <Windows.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_os_ostream.h>
#include <parser.h>

#include "ast_function.h"
#include "ast_global_block.h"
#include "ast_node.h"
#include "cmd_options.h"
#include "cmake_config.h"

using namespace llvm;

int spawn_clang(const char* input) {
  
#ifdef CMAKE_WIN32
  HANDLE stdin_read = NULL;
  HANDLE stdin_write = NULL;
  SECURITY_ATTRIBUTES sa;
  memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));
  sa.bInheritHandle = TRUE;
  if (!CreatePipe(&stdin_read, &stdin_write, &sa, 0)) {
    std::cout << "Error " << GetLastError() << ": failed to create pipe" << std::endl;
    return GetLastError();
  }

  HANDLE std_err = GetStdHandle(STD_ERROR_HANDLE);
  HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);

  STARTUPINFO startup_info;
  memset(&startup_info, 0, sizeof(STARTUPINFO));
  startup_info.cb = sizeof(STARTUPINFOEX);
  startup_info.hStdInput = stdin_read;
  startup_info.hStdError = std_err;
  startup_info.hStdOutput = std_out;
  startup_info.dwFlags |= STARTF_USESTDHANDLES;
  PROCESS_INFORMATION proc_info;
  memset(&proc_info, 0, sizeof(PROCESS_INFORMATION));

  // @@@ Too lazy to check for an error here
  SIZE_T attr_list_size;
  InitializeProcThreadAttributeList(NULL, 1, 0, &attr_list_size);

  STARTUPINFOEX startup_info_ex;
  memset(&startup_info_ex, 0, sizeof(STARTUPINFOEX));
  startup_info_ex.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(
    GetProcessHeap(),
    0,
    attr_list_size
  );

  startup_info_ex.StartupInfo = startup_info;

  // @@@ Too lazy to check for an error here
  InitializeProcThreadAttributeList(
    startup_info_ex.lpAttributeList,
    1,
    0,
    &attr_list_size
  );

  HANDLE handle_list[3];
  handle_list[0] = stdin_read;
  handle_list[1] = std_out;
  handle_list[2] = std_err;
  bool update_handle_list = UpdateProcThreadAttribute(
    startup_info_ex.lpAttributeList,
    0,
    PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
    handle_list,
    sizeof(handle_list),
    NULL,
    NULL
  );

  if (!update_handle_list) {
    std::cout << "Error " << GetLastError() << ": failed to update handle list" << std::endl;
    return GetLastError();
  }

  DWORD bytes_written = 0;
  int input_length = strlen(input);
  if (!WriteFile(stdin_write, input, input_length, &bytes_written, NULL)) {
    std::cout << "Error " << GetLastError() << ": WriteFile()" << std::endl;
    return GetLastError();
  }

  // CreateProcessW segfaults if lpApplicationName is NULL
  bool result = CreateProcessA(
    NULL,
    const_cast<char*>("C:\\llvm-release\\bin\\clang.exe -xir -"),
    NULL,
    NULL,
    TRUE,
    EXTENDED_STARTUPINFO_PRESENT,
    NULL,
    NULL,
    (LPSTARTUPINFOA)&startup_info_ex,
    &proc_info
  );

  if (!result) {
    std::cout << "Error " << GetLastError() << ": failed to start clang" << std::endl;
    return GetLastError();
  }

  CloseHandle(stdin_write);
  CloseHandle(stdin_read);
  WaitForSingleObject(proc_info.hProcess, INFINITE);

  CloseHandle(proc_info.hProcess);
  CloseHandle(proc_info.hThread);

  return 0;
#endif
}

int main(int argc, char** argv) {
  std::cout << "egb-lang " << el_VERSION_MAJOR << "." << el_VERSION_MINOR
            << std::endl;

  CmdOptions options;
  if (argc < 2) {
    printf("\n");
    std::cout << "Error: No input files provided" << std::endl;
    std::cout << "Usage: el FILE [FILE ...]" << std::endl;
    return 1;
  } else {
    std::vector<CmdOptions::Option>* option_list = options.parse_options(argc, argv);
    if (options.parse_option_list(option_list))
      return 1;
  }

  std::FILE* ifs;
  ifs = std::fopen(options.input_file_name .c_str(), "r+");
  if (!ifs) {
    std::cout << "Error: Could not open file \"" << options.input_file_name << "\"" << std::endl;
    return 1;
  }

  std::cout << "Opened " << options.input_file_name << std::endl;

  char next_char;
  std::string buffer;
  while ((next_char = fgetc(ifs)) != EOF) buffer += next_char;

  std::fclose(ifs);

  std::cout << "Closed " << options.input_file_name << std::endl;

  const char* iterator = &buffer[0];
  Parser p(iterator);

  p.insertion_stack.push(&ASTGlobalBlock::get_global_block().global_scope);

  while (peek(p.iterator).token_type != static_cast<int>(Token::tok_eof)) {
    p.parse_top_level_expr();
  }
  std::cout << "parsed " << (*p.insertion_stack.top()).size()
            << " top level node(s)" << std::endl;

  if (!p.found_main) {
    std::cout << "Error: Could not find entry point \"main\"." << std::endl;
    p.error = true;
  }

  if (p.error) {
    std::cout << "Errors occurred. Exiting" << std::endl;
    return 1;
  }

  LLVMContext context;
  Module llvm_module("main_mod", context);
  IRBuilder<> builder(context);

  for (ASTNode* n : (*p.insertion_stack.top())) {
    if (!n) continue;

    ASTFunction* function = dynamic_cast<ASTFunction*>(n);
    if (function)
      function->code_gen(context, builder, llvm_module);
    else
      n->code_gen(context, builder);
  }

  std::ostringstream ir_ostream;
  raw_os_ostream the_ir_ostream(ir_ostream);
  llvm_module.print(the_ir_ostream, nullptr);

  if (options.should_emit_ir) {
    if (options.output_file_name.empty())
      options.output_file_name = "a.ll";

    if (options.validate_file_ext(options.output_file_name, "ll")) {
      std::cout << "Error: Invalid file extension for LLVM IR file \"" << options.output_file_name << "\"" << std::endl;
      return 1;
    }

    FILE* ir_file = fopen(options.output_file_name.c_str(), "w+");
    if (!ir_file){
      std::cout << "Error: Could not open file \"" << options.output_file_name << "\"" << std::endl;
      return 1;
    }
    fputs(ir_ostream.str().c_str(), ir_file);
  } else {
    return spawn_clang(ir_ostream.str().c_str());
  }

}