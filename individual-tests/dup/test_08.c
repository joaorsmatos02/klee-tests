/*
 * test_08.c - dup2(fd, fd) same fd returns fd (no-op)
 *
 * The code does *f2 = *f which is a self-copy. POSIX says dup2 to self
 * should return fd without closing. KLEE does this correctly by accident.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc
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

   int newfd = (int) __concretize(fd);
   int ret = dup2(fd, newfd);
   __sra_assert(dup2_returns(ret, newfd));

   cleanup_fd(fd);
   return 0;
}