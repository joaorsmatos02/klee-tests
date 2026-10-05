/*
 * test_12.c - Seek on closed fd fails (EBADF??)
 *
 * KLEE's POSIX runtime may not enforce closed fd check on lseek().
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_12.c
 * Run    : klee --posix-runtime --libc=uclibc test_12.bc
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

   close(fd);

   off_t pos = lseek(fd, 0, SEEK_SET);
   __assert(lseek_fails(pos));

   return 0;
}