/*
 * test_25.c - Open O_RDWR, close, reopen O_RDWR
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_25.c
 * Run    : klee --posix-runtime --libc=uclibc test_25.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDWR;
   create_test_file(fname);
   __assume(exists(fname));

   int fd1 = open(fname, flags);
   __assert(open_succeeds(fd1));
   int cret = close(fd1);
   __assert(close_succeeds(cret));

   int fd2 = open(fname, flags);
   __assert(open_succeeds(fd2));
   __assert(fd_is(fd1, fd2));

   cleanup_fd(fd2);
   return 0;
}