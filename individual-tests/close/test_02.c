/*
 * test_02.c - close valid O_RDWR fd succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDWR;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags, 0);
   __assert(open_succeeds(fd));

   int cret = close(fd);
   __assert(close_succeeds(cret));

   return 0;
}