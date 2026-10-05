/*
 * test_05.c - read 1 byte (minimal read)
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
   __assert(open_succeeds(fd));

   char buf[1] = {0};
   ssize_t rret = read(fd, buf, 1);
   __assert(read_all(rret, 1));

   cleanup_fd(fd);
   return 0;
}