/*
 * test_09.c - O_CREAT | O_RDONLY creates new file
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   char other[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_CREAT | O_RDONLY;

   create_symbolic_file_name(other);
   create_test_file(other);
   __assume(not_exists(fname));

   int fd = open(fname, flags, 0644);

   __sra_assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}