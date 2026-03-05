#include "utils.h"

/*
 * Startup routine
 * https://stackoverflow.com/questions/50260855/how-get-arguments-value-using-inline-assembly-in-c-without-glibc
 */
#ifdef __x86_64__
asm (".global _start\n"
     "_start:\n"
     "   xorl %ebp,%ebp\n"    // mark outermost stack frame
     "   movq 0(%rsp),%rdi\n" // get argc
     "   lea 8(%rsp),%rsi\n"  // the arguments are pushed just below, so argv =
                              // %rbp + 8
     "   call main\n"         // call our main
     "   movq %rax,%rdi\n"    // take the main return code and use it as first
                              // argument for...
     "   movl $60,%eax\n"     // ... the exit syscall
     "   syscall\n"
     "   int3\n"); // just in case
#endif

int
main (int argc, char *argv[])
{
  int status = 0;
  int fd = 0;
  unsigned long int read_size = 0;
  char buffer[BUFFER_SIZE] = { 0 };
  const char *file_error_message = "Error opening file\n\0";

  status = arg_check (argc);
  if (status < 0)
    {
      return FAILURE;
    }

  // open file
  fd = syscall2 (__NR_open, (long)(argv[1]), O_RDONLY);
  if (fd < 0)
    {
      syscall3 (__NR_write, STDERR_FILENO, (long)(file_error_message), len (file_error_message));
      syscall1 (__NR_exit, 1);
    }

  // read file
  while ((read_size = syscall3 (__NR_read, fd, (long)buffer, BUFFER_SIZE)) > 0)
    {
      // write file
      syscall3 (__NR_write, STDOUT_FILENO, (long)buffer, read_size);
      clear_buff (buffer);
    }

  // close file
  syscall1 (__NR_close, fd);
  return SUCCESS;
}
