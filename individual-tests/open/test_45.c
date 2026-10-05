/*
 * test_45.c - Open with mode=0644 overwrites st_mode (even without O_CREAT)
 *
 * chmod sets 0777, open with mode=0644 clobbers it. stat shows 0644 not 0777.
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_45.c
 * Run    : klee --posix-runtime --libc=uclibc test_45.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDONLY;
   create_test_file(fname);
   __assume(exists(fname));
   int cret = chmod(fname, 0777);
   __assert(chmod_succeeds(cret));

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   //check if open clobbered permissions
   __assert(perms_are(fd, 0644));

   cleanup_fd(fd);

   return 0;
}