/*
 * test_02.c - write 5 bytes to O_RDWR file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_02.c
 * Run    : klee --posix-runtime --libc=uclibc test_02.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   create_test_file(fname);
   __assume(exists(fname));
   __assume(flags_equal(flags, O_RDWR));

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   ssize_t wret = write(fd, "hello", 5);
   __assert(write_all(wret, 5));

   cleanup_fd(fd);
   return 0;
}