/*
 * test_12.c - O_RDONLY on a write-only permissions file fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file_with_mode(fname, 0222);
   __assume(exists(fname));

   int fd = open(fname, flags);

   __sra_assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}
