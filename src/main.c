#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_exit 60

#define O_RDONLY 00
#define BUFFER_SIZE 128
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

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

/**
 * GCC Attributes, Modifiers, Quantifiers Used:
 *
 * always_inline - forces inlining of functions when not optimizing.
 * inline - direct GCC to make calls to this function faster.
 */

/**
 * @brief Execute a syscall with 1 argument.
 *
 * @param n - syscall number.
 * @param a1 - argument 1.
 *
 * @return long - syscall's return value
 */
static inline __attribute__ ((always_inline)) long
syscall1 (long n, long a1)
{
  long ret;
  asm ("syscall\n\t" : "=a"(ret) : "a"(n), "D"(a1) : "memory", "rcx");
  return ret;
}

/**
 * @brief Execute a syscall with 2 arguments.
 *
 * @param n - syscall number.
 * @param a1 - argument 1.
 * @param a2 - argument 2.
 *
 * @return long - syscall's return value
 */
static inline __attribute__ ((always_inline)) long
syscall2 (long n, long a1, long a2)
{
  long ret;
  asm ("syscall\n\t" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2) : "memory", "rcx");
  return ret;
}

/**
 * @brief Execute a syscall with 3 arguments.
 *
 * @param n - syscall number.
 * @param a1 - argument 1.
 * @param a2 - argument 2.
 * @param a3 - argument 3.
 *
 * @return long - syscall's return value
 */
static inline __attribute__ ((always_inline)) long
syscall3 (long n, long a1, long a2, long a3)
{
  long ret;
  asm ("syscall\n\t"
       : "=a"(ret)
       : "a"(n), "D"(a1), "S"(a2), "d"(a3)
       : "memory", "rcx");
  return ret;
}

/**
 * @brief Execute a syscall with 4 arguments.
 *
 * @param n - syscall number.
 * @param a1 - argument 1.
 * @param a2 - argument 2.
 * @param a3 - argument 3.
 * @param a3 - argument 4.
 *
 * @return long - syscall's return value
 */
static inline __attribute__ ((always_inline)) long
syscall4 (long n, long a1, long a2, long a3, long a4)
{
  long ret;
  register long r10 __asm__ ("r10") = a4;
  asm ("syscall\n\t"
       : "=a"(ret)
       : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10)
       : "memory", "rcx");
  return ret;
}

/**
 * @brief Reset the buffer.
 *
 * @param buffer (char *): address of the buffer.
 *
 */
static inline __attribute__ ((always_inline)) void
clear_buff (char *buffer)
{
  for (int i = 0; i < BUFFER_SIZE; ++i)
    {
      buffer[i] = 0;
    }
}

int
main (int argc, char *argv[])
{
  (void)argc;
  // open file
  int fd = 0;
  fd = syscall2 (__NR_open, (long)(argv[1]), O_RDONLY);
  if (fd < 0)
    {
      syscall3 (__NR_write, STDERR_FILENO, (long)"Error opening file\n", 19);
      syscall1 (__NR_exit, 1);
    }

  char buffer[BUFFER_SIZE] = { 0 };
  unsigned long int read_size = 0;

  // read file
  while ((read_size = syscall3(__NR_read, fd, (long)buffer, BUFFER_SIZE)) > 0)
    {
      // write file
      syscall3 (__NR_write, STDOUT_FILENO, (long)(buffer), read_size);
      clear_buff (buffer);
    }

  // close file
  syscall1 (__NR_close, fd);
  return 0;
}