/*
 * test_08.c - close then lseek fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags, 0);
   __sra_assert(open_succeeds(fd));

   int cret = close(fd);
   __sra_assert(close_succeeds(cret));

   off_t pos = lseek(fd, 0, SEEK_SET);
   __sra_assert(lseek_fails(pos));

   return 0;
}