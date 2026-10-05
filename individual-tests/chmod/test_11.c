/*
 * test_11.c - chmod 0000 then open O_RDONLY fails (EACCES)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDONLY;

   create_test_file(fname);
   __assume(exists(fname));

   int cret = chmod(fname, 0000);
   __assert(chmod_succeeds(cret));

   int fd = open(fname, flags);
   __assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}
