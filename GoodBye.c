#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

// Function that exits the program, closes the shell, and returns to the original shell.
void main(int argc, char *argv[])
{
  if (argc != 1)
  {
    //printf("Bad number of parameters\n");
    exit(1);
  }

  exit(EXIT_SUCCESS);
}