/*
 * test_10.c - Seek to negative absolute position fails (EINVAL)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc
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

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   off_t pos = lseek(fd, -1, SEEK_SET);
   __assert(lseek_fails(pos));

   cleanup_fd(fd);
   return 0;
}