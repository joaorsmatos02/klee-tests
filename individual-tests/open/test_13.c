/*
 * test_13.c - First open() returns fd=3
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags);

   __sra_assert(open_succeeds(fd));
   __sra_assert(fd_is(fd, 3));

   int cret = close(fd);
   __sra_assert(close_succeeds(cret));

   return 0;
}