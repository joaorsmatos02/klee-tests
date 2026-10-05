/*
 * test_11.c - Seek on invalid fd (-1) fails (EBADF)
 *
 * Only pure lseek test - no open, no file setup.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);
   off_t pos = lseek(-1, 0, SEEK_SET);
   __sra_assert(lseek_fails(pos));

   return 0;
}