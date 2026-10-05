/*
 * test_17.c - O_TRUNC | O_WRONLY on existing file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_17.c
 * Run    : klee --posix-runtime --libc=uclibc test_17.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_TRUNC | O_WRONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags);

   __assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}