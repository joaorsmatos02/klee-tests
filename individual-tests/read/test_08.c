/*
 * test_08.c - read more bytes than file has (partial read / EOF / sym files has 10 we read 20)
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

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   char buf[20] = {0};
   ssize_t rret = read(fd, buf, 20);
   __assert(read_at_most(rret));

   cleanup_fd(fd);
   return 0;
}