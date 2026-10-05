/*
 * test_08.c - O_CREAT | O_RDWR creates new file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   char other[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_CREAT | O_RDWR;

   create_symbolic_file_name(other);
   create_test_file(other);
   __assume(not_exists(fname));

   int fd = open(fname, flags);

   __assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}