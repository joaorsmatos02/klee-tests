/*
 * test_03.c - close invalid fd (-1) fails (EBADF)
 *
 * Pure close test - no open, no file setup.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);
   int cret = close(-1);
   __assert(close_fails(cret));

   return 0;
}