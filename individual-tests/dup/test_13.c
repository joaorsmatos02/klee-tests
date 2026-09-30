/*
 * test_13.c - close dup'd fd, original still works
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_13.c
 * Run    : klee --posix-runtime --libc=uclibc test_13.bc
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

   int fd2 = dup(fd);
   __assert(dup_is_new(fd2, fd));

   int cret = close(fd2);
   __assert(close_succeeds(cret));

   char buf[5] = {0};
   ssize_t rret = read(fd, buf, 5);
   __assert(read_all(rret, 5));

   cleanup_fd(fd);
   return 0;
}