/*
 * test_09.c - close then reopen reuses fd number
 *
 * POSIX reuses the lowest available fd. Close and reopen should get same number.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd1 = open(fname, flags, 6444);
   __sra_assert(open_succeeds(fd1));

   int cret = close(fd1);
   __sra_assert(close_succeeds(cret));

   int fd2 = open(fname, flags, 6444);
   __sra_assert(open_succeeds(fd2));

   __sra_assert(fd_is(fd1, fd2));

   cleanup_fd(fd2);
   return 0;
}