/*
 * test_04.c - O_RDONLY on non-existing file fails
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   char other[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   create_symbolic_file_name(other);
   create_test_file(other);
   __assume(not_exists(fname));
   __assume(flags_equal(flags, O_RDONLY));

   int fd = open(fname, flags);

   __assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}