/*
 * test_40.c - Open starts at offset 0
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_40.c
 * Run    : klee --posix-runtime --libc=uclibc test_40.bc
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

   int fd = open(fname, flags);
   __assert(open_succeeds(fd));

   off_t pos = lseek(fd, 0, SEEK_CUR);
   __assert(lseek_is(pos, 0));

   cleanup_fd(fd);
   return 0;
}