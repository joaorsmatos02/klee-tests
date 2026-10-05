/*
 * test_15.c - O_TRUNC: SEEK_END verifies truncated size after write
 *
 * NOTE: O_TRUNC has known issues in KLEE - SEEK_END may report the original
 * buffer capacity (10) instead of the written size (4).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_15.c
 * Run    : klee --posix-runtime --libc=uclibc test_15.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDWR | O_TRUNC;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   ssize_t wret = write(fd, "test", 4);
   __assert(write_all(wret, 4));

   off_t pos1 = lseek(fd, 0, SEEK_SET);
   __assert(lseek_is(pos1, 0));
   off_t pos2 = lseek(fd, 0, SEEK_END);
   __assert(lseek_is(pos2, 4));

   cleanup_fd(fd);
   return 0;
}