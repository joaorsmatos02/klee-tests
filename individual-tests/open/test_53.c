/*
 * test_53.c - __file_open mode "w" gives the open flags O_WRONLY | O_CREAT | O_TRUNC
 *
 * The flags glibc's fopen uses for "w" (577).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_53.c
 * Run    : klee --posix-runtime --libc=uclibc test_53.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);

   int fd = __file_open(fname, "w");
   __sra_assert(open_succeeds(fd));
   __sra_assert(flags_equal(__file_flags(fd), O_WRONLY | O_CREAT | O_TRUNC));

   __file_close(fd);
   return 0;
}
