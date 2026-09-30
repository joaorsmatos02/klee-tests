/*
 * test_09.c - Seek past EOF (allowed on regular files)
 *
 * 10-byte file, lseek(100, SEEK_SET) should return 100.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_09.c
 * Run    : klee --posix-runtime --libc=uclibc test_09.bc
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

   off_t pos = lseek(fd, 100, SEEK_SET);
   __assert(lseek_is(pos, 100));

   cleanup_fd(fd);
   return 0;
}