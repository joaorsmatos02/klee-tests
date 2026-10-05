/*
 * test_03.c - dup on closed fd fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags, 0644);
   __sra_assert(open_succeeds(fd));

   int cret = close(fd);
   __sra_assert(close_succeeds(cret));

   int dret = dup(fd);
   __sra_assert(dup_fails(dret));

   return 0;
}