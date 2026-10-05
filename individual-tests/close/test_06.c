/*
 * test_06.c - close then read fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_06.c
 * Run    : klee --posix-runtime --libc=uclibc test_06.bc
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

   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __sra_assert(read_error(rret));

    return 0;
}