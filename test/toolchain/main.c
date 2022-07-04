#include <stdio.h>
#include <stdint.h>

/*
  Compile IR file to object file:
    llc -filetype=obj dummy.ir -o dummy.o

  Compile C++ file to object file:
    clang -c main.c -o main.o

  Link C++ and LLVM IR object files:
    clang dummy.o main.o -o main.exe
*/

extern uint32_t dummy();

int main() {
  printf("%d", dummy()); // Should print '0'
  
  return 0;
}