/*
 * test_44.c - chmod(0222) then open O_RDONLY fails (EACCES)
 *
 * A file without read permission cannot be opened for reading. (KLEE lets
 * it: its has_permission never checks read permission for O_RDONLY.)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_44.c
 * Run    : klee --posix-runtime --libc=uclibc test_44.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);
   flags = O_RDONLY;
   create_test_file(fname);
   __assume(exists(fname));
   int cret = chmod(fname, 0222);
   __sra_assert(chmod_succeeds(cret));

   int fd = open(fname, flags);
   __sra_assert(open_fails(fd));

   cleanup_fd(fd);
   return 0;
}
