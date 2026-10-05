/*
 * test_19.c - O_APPEND | O_WRONLY on existing file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_19.c
 * Run    : klee --posix-runtime --libc=uclibc test_19.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_APPEND | O_WRONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags);

   __assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}