/*
 * test_05.c - double close fails on second call (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags, 0);
   __sra_assert(open_succeeds(fd));

   int cret1 = close(fd);
   __sra_assert(close_succeeds(cret1));

   int cret2 = close(fd);
   __sra_assert(close_fails(cret2));

   return 0;
}