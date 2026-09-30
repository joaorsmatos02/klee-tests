/*
 * test_55.c - __file_open mode "r+" gives the open flags O_RDWR
 *
 * The flags glibc's fopen uses for "r+" (2).
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_55.c
 * Run    : klee --posix-runtime --libc=uclibc test_55.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);

   int fd = __file_open(fname, "r+");
   __assert(open_succeeds(fd));
   __assert(flags_equal(__file_flags(fd), O_RDWR));

   __file_close(fd);
   return 0;
}
