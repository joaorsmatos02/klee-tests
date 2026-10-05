/*
 * test_04.c - dup2 to specific target fd succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc
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

   int fd2 = dup2(fd, 10);
   __sra_assert(dup2_returns(fd2, 10));

   cleanup_fd(fd);
   cleanup_fd(fd2);
   return 0;
}