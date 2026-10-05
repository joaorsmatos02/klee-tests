/*
 * test_46.c - Open with mode=0000 leaves an existing file's st_mode unchanged
 *
 * chmod sets 0666; open without O_CREAT ignores its mode, so stat still
 * shows 0666. (KLEE overwrites it with 0000.)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_46.c
 * Run    : klee --posix-runtime --libc=uclibc test_46.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDONLY;
   create_test_file(fname);
   __assume(exists(fname));
   int cret = chmod(fname, 0666);
   __sra_assert(chmod_succeeds(cret));

   int fd = open(fname, flags, 0);
   __sra_assert(open_succeeds(fd));

   // open's mode only applies to a file it creates
   __sra_assert(perms_are(fd, 0666));

   cleanup_fd(fd);

   return 0;
}
