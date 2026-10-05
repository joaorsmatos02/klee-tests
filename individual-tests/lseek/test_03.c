/*
 * test_03.c - SEEK_SET to end of 10-byte file
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

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   //10 at the moment since our test files have 10 bytes size
   off_t pos = lseek(fd, 10, SEEK_SET);
   __assert(lseek_is(pos, 10));

   cleanup_fd(fd);
   return 0;
}