/*
 * test_06.c - dup2 with newfd >= MAX_FDS fails (EBADF)
 *
 * MAX_FDS is 32 in KLEE. Passing newfd=32 is out of range.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc
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

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   int ret = dup2(fd, 32);
   __assert(dup2_fails(ret));

   cleanup_fd(fd);
   return 0;
}