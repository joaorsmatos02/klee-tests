/*
 * test_04.c - close invalid fd (99) fails (EBADF)
 *
 * fd way out of range (KLEE max is 32 due to size of the fd array).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);
   int cret = close(99);
   __sra_assert(close_fails(cret));

   return 0;
}