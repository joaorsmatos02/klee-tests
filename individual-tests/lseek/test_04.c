/*
 * test_04.c - SEEK_CUR forward from current position
 *
 * Reads 3 bytes (offset=3), then seeks +4 forward (offset=7).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_04.c
 * Run    : klee --posix-runtime --libc=uclibc test_04.bc
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

   char skip[3] = {0};
   ssize_t rret = read(fd, skip, 3);
   __sra_assert(read_all(rret, 3));

   off_t pos = lseek(fd, 4, SEEK_CUR);
   __sra_assert(lseek_is(pos, 7));

   cleanup_fd(fd);
   return 0;
}