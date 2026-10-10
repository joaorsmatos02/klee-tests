/*
 * test_03.c - dup2 with newfd past the fd limit fails (EBADF)
 *
 * The test limits itself to 32 descriptors, 0 to 31, so newfd=32 is out of
 * range.
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
   limit_fds(32);

   int fd = open(fname, flags, 0644);
   __sra_assert(open_succeeds(fd));

   int ret = dup2(fd, 32);
   __sra_assert(dup2_fails(ret));

   cleanup_fd(fd);
   return 0;
}
