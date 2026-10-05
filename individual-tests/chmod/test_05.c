/*
 * test_05.c - chmod 0444 sets r--r--r-- (read only)
 *
 * perms_are() is unreliable, so the resulting bits are verified indirectly:
 * after chmod(0444) the owner must be able to open O_RDONLY but not O_WRONLY.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_05.c
 * Run    : klee --posix-runtime --libc=uclibc test_05.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   create_symbolic_file_name(fname);

   create_test_file(fname);
   __assume(exists(fname));

   int cret = chmod(fname, 0444);
   __sra_assert(chmod_succeeds(cret));

   int fd_r = open(fname, O_RDONLY);
   __sra_assert(open_succeeds(fd_r));
   cleanup_fd(fd_r);

   int fd_w = open(fname, O_WRONLY);
   __sra_assert(open_fails(fd_w));
   cleanup_fd(fd_w);

   return 0;
}
