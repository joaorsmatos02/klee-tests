/*
 * test_03.c - O_RDWR on existing file succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_03.c
 * Run    : klee --posix-runtime --libc=uclibc test_03.bc
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

   int fd = open(fname, flags);

   __assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}