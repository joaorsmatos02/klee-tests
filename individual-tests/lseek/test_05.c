/*
 * test_05.c - SEEK_CUR backward from current position
 *
 * Seeks to 8, then seeks -3 backward (offset=5).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc
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
   __sra_assert(open_succeeds(fd));

   lseek(fd, 8, SEEK_SET);

   off_t pos = lseek(fd, -3, SEEK_CUR);
   __sra_assert(lseek_is(pos, 5));

   cleanup_fd(fd);
   return 0;
}