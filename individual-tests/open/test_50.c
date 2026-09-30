/*
 * test_50.c - Close fd=3, reopen gets fd=3 (recycling)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_50.c
 * Run    : klee --posix-runtime --libc=uclibc test_50.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   create_test_file(fname);
   __assume(exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd1 = open(fname, flags);
   __assert(open_succeeds(fd1));
   __assert(fd_is(fd1, 3));

   int ret = close(fd1);
   __assert(close_succeeds(ret));

   int fd2 = open(fname, flags);
   __assert(open_succeeds(fd2));
   __assert(fd_is(fd2, 3));

   cleanup_fd(fd2);
   return 0;
}
