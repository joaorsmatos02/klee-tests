/*
 * test_07.c - read from invalid fd (-1) fails (EBADF)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_07.c
 * Run    : klee --posix-runtime --libc=uclibc test_07.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];

   create_symbolic_file_name(fname);
   create_test_file(fname);
   char buf[5] = {0};
   ssize_t rret = read(-1, buf, 5);
   __sra_assert(read_error(rret));

   return 0;
}