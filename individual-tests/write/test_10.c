/*
 * test_10.c - O_APPEND write goes to end of file, verify offset
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_10.c
 * Run    : klee --posix-runtime --libc=uclibc test_10.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = declare_symbolic_flags();

   create_test_file(fname);
   __assume(exists(fname));
   __assume(flags_equal(flags, O_APPEND | O_WRONLY));

   int fd = open(fname, flags, 0644);
   __assert(open_succeeds(fd));

   off_t original_size = lseek(fd, 0, SEEK_END);
   printf("[info] original file size = %d\n", (int) __concretize(original_size));

   ssize_t wret = write(fd, "abc", 3);
   __assert(write_all(wret, 3));

   off_t current = lseek(fd, 0, SEEK_CUR);
   __assert(lseek_is(current, original_size + 3));

   cleanup_fd(fd);
   return 0;
}