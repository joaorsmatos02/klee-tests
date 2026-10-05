/*
 * test_07.c - close then write fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_WRONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags, 0);
   __sra_assert(open_succeeds(fd));

   int cret = close(fd);
   __sra_assert(close_succeeds(cret));

   ssize_t wret = write(fd, "hello", 5);
   __sra_assert(write_error(wret));

   return 0;
}