#include "utils.h"

/**
 * @brief Calculates the length of a given string.
 *
 * @param str (const char *) The string for which the length is to be
 * calculated.
 * @return (unsigned long): Length of the string or 0 if the string is null.
 */
unsigned long int
len (const char *str)
{
  if (str == NULL)
    {
      return 0;
    }

  unsigned long length = 0;
  while (str[length] != '\0')
    {
      length++;
    }
  return length;
}

/**
 * @brief Reset the buffer.
 *
 * @param buffer (char *): address of the buffer.
 *
 */
void
clear_buff (char *buffer)
{
  for (int i = 0; i < BUFFER_SIZE; ++i)
    {
      buffer[i] = 0;
    }
}

int
arg_check (int argc)
{
  const char *error_message = "Usage: syscat <file>\n\0";
  if (argc < 2)
    {
      syscall3 (__NR_write, STDERR_FILENO, (long)(error_message), len (error_message));
      return FAILURE;
    }
  return SUCCESS;
}