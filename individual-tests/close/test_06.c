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
   flags = declare_symbolic_flags();

   create_test_file(fname);
   __assume(exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags, 0);
   __assert(open_succeeds(fd));

   int cret = close(fd);
   __assert(close_succeeds(cret));

   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __assert(read_error(rret));

    return 0;
}