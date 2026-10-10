/*
 * test_07.c - close dup'd fd, original still works
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc
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

   int fd2 = dup(fd);
   __sra_assert(dup_is_new(fd2, fd));

   int cret = close(fd2);
   __sra_assert(close_succeeds(cret));

   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __sra_assert(read_all(rret, 5));

   cleanup_fd(fd);
   return 0;
}