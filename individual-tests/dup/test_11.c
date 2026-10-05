/*
 * test_11.c - dup'd fds share the file offset
 *
 * A dup'd fd refers to the same open file description, so writing through
 * one advances the offset of both. (KLEE copies the offset at dup time
 * instead: "XXX Incorrect, really we need another data structure for open
 * files".)
 *
 * Compile: clang -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I../../include test_11.c
 * Run    : klee --posix-runtime --libc=uclibc test_11.bc
 */
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;
   create_symbolic_file_name(fname);
   flags = O_RDWR;

   create_test_file(fname);
   __assume(exists(fname));

   int fd1 = open(fname, flags, 0644);
   __assert(open_succeeds(fd1));

   int fd2 = dup(fd1);
   __assert(dup_is_new(fd2, fd1));

   //both start at offset 0
   off_t pos1 = lseek(fd1, 0, SEEK_CUR);
   __assert(lseek_is(pos1, 0));
   off_t pos2 = lseek(fd2, 0, SEEK_CUR);
   __assert(lseek_is(pos2, 0));

   //write 5 bytes on fd1, moves fd1 offset to 5
   ssize_t wret = write(fd1, "hello", 5);
   __assert(write_all(wret, 5));
   off_t pos3 = lseek(fd1, 0, SEEK_CUR);
   __assert(lseek_is(pos3, 5));

   //the offset is shared, so fd2 is at 5 too
   off_t pos4 = lseek(fd2, 0, SEEK_CUR);
   __assert(lseek_is(pos4, 5));

   cleanup_fd(fd1);
   cleanup_fd(fd2);
   return 0;
}
