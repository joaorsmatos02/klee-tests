/*
 * test_57.c - __file_open mode "a+" gives the open flags O_RDWR | O_CREAT | O_APPEND
 *
 * The flags glibc's fopen uses for "a+" (1090).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_57.c
 * Run    : klee --posix-runtime --libc=uclibc test_57.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);

   int fd = __file_open(fname, "a+");
   __sra_assert(open_succeeds(fd));
   __sra_assert(flags_equal(__file_flags(fd), O_RDWR | O_CREAT | O_APPEND));

   __file_close(fd);
   return 0;
}
