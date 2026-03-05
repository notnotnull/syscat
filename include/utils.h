#ifndef UTILS_H
#define UTILS_H

#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_exit 60

#define O_RDONLY 00
#define BUFFER_SIZE 128
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define NULL ((void *)0)
#define SUCCESS 0
#define FAILURE -1

unsigned long int len (const char *str);

void clear_buff (char *buffer);

int arg_check (int argc);

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
inline __attribute__ ((always_inline)) long
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
inline __attribute__ ((always_inline)) long
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
inline __attribute__ ((always_inline)) long
syscall3 (long n, long a1, long a2, long a3)
{
  long ret;
  asm ("syscall\n\t" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "memory", "rcx");
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
inline __attribute__ ((always_inline)) long
syscall4 (long n, long a1, long a2, long a3, long a4)
{
  long ret;
  register long r10 __asm__ ("r10") = a4;
  asm ("syscall\n\t" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "memory", "rcx");
  return ret;
}

#endif