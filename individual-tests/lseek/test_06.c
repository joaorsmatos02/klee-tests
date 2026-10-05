/*
 * test_06.c - SEEK_CUR with 0 (get current position without moving)
 *
 * Seeks to 7, then lseek(0, SEEK_CUR) should return 7.
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

   int fd = open(fname, flags, 0644);
   __sra_assert(open_succeeds(fd));

   lseek(fd, 7, SEEK_SET);

   off_t pos = lseek(fd, 0, SEEK_CUR);
   __sra_assert(lseek_is(pos, 7));

   cleanup_fd(fd);
   return 0;
}