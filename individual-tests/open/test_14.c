/*
 * test_14.c - Second fd is fd=4 (via dup)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_14.c
 * Run    : klee --posix-runtime --libc=uclibc test_14.bc
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

   int fd2 = dup(fd1);
   __assert(dup_is_new(fd2, fd1));

   __assert(fd_is(fd2, 4));

   int cret = close(fd2);
   __assert(close_succeeds(cret));
   cret = close(fd1);
   __assert(close_succeeds(cret));

   return 0;
}