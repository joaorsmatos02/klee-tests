/*
 * test_28.c - Open O_WRONLY, close, reopen O_RDONLY
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_28.c
 * Run    : klee --posix-runtime --libc=uclibc test_28.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);
   __assume(exists(fname));

   int fd1 = open(fname, O_WRONLY);
   __sra_assert(open_succeeds(fd1));
   int cret = close(fd1);
   __sra_assert(close_succeeds(cret));

   int fd2 = open(fname, O_RDONLY);
   __sra_assert(open_succeeds(fd2));

   cleanup_fd(fd2);
   return 0;
}