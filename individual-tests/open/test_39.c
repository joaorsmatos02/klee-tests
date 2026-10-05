/*
 * test_39.c - O_RDWR sets both, read and write succeed
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_39.c
 * Run    : klee --posix-runtime --libc=uclibc test_39.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDWR;
   create_test_file(fname);
   __assume(exists(fname));

   int fd = open(fname, flags);
   __sra_assert(open_succeeds(fd));

   lseek(fd, 0, SEEK_SET);
   ssize_t wret = write(fd, "hello", 5);
   __sra_assert(write_all(wret, 5));

   lseek(fd, 0, SEEK_SET);
   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __sra_assert(read_all(rret, 5));
   __sra_assert(buffers_match("hello", buf, 5));

   cleanup_fd(fd);
   return 0;
}