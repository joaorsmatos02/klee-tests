/*
 * test_08.c - chmod 0666 then open O_RDWR succeeds
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_08.c
 * Run    : klee --posix-runtime --libc=uclibc test_08.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDWR;

   create_test_file(fname);
   __assume(exists(fname));

   int cret = chmod(fname, 0666);
   __sra_assert(chmod_succeeds(cret));

   int fd = open(fname, flags);
   __sra_assert(open_succeeds(fd));

   cleanup_fd(fd);
   return 0;
}
