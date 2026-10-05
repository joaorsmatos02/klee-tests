/*
 * test_15.c - O_CREAT | O_EXCL | O_WRONLY on existing file fails (EEXIST)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_15.c
 * Run    : klee --posix-runtime --libc=uclibc test_15.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_CREAT | O_EXCL | O_WRONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags);

   __assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}