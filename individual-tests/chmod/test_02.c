/*
 * test_02.c - chmod on non-existing file fails (ENOENT)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   char other[FNAME_SIZE];
   create_symbolic_file_name(fname);

   create_symbolic_file_name(other);
   create_test_file(other);
   __assume(not_exists(fname));

   int cret = chmod(fname, 0644);
   __assert(chmod_fails(cret));

   return 0;
}
