/*
 * test_54.c - __file_open mode "a" gives the open flags O_WRONLY | O_CREAT | O_APPEND
 *
 * The flags glibc's fopen uses for "a" (1089).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_54.c
 * Run    : klee --posix-runtime --libc=uclibc test_54.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);

   int fd = __file_open(fname, "a");
   __assert(open_succeeds(fd));
   __assert(flags_equal(__file_flags(fd), O_WRONLY | O_CREAT | O_APPEND));

   __file_close(fd);
   return 0;
}
