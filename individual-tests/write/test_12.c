/*
 * test_12.c - O_CREAT | O_WRONLY write to new file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   char other[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_CREAT | O_WRONLY;

   create_symbolic_file_name(other);
   create_test_file(other);
   __assume(not_exists(fname));

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   ssize_t wret = write(fd, "new!", 4);
   __assert(write_all(wret, 4));

   cleanup_fd(fd);
   return 0;
}