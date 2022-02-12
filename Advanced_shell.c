// Created by Tareez Ghandour
// run in terminal "chmod +x Advanced_shell.sh" then "./Advanced_shell.sh"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TOK_BUFSIZE 256
#define txt_str ".txt"
#define sol_str "Sol_"
#define Create_func "CreateExam"
#define Sol_func "SolExam"
#define Start_func "StartExam"
#define Check_func "CheckExam"
#define Calc_func "CalcGrade"
#define bye_func "GoodBye"

void display_prompt();
char *read_cmd();
char **split_cmd(char *line);
int cmd_execute(char **args);
int cmd_launch(char **args);
int cmd_cd(char **args);
int cmd_help(char **args);
int cmd_exit(char **args);
int prog(char **args);
void GoodBye();

char *builtin_str[] = {"cd", "help", "exit"};
int (*builtin_func[])(char **) = {&cmd_cd, &cmd_help, &cmd_exit};

int main(int argc, char *argv[])
{
  char *CMD;
  char **args;
  int run = 1;

  // get input and split words into tokens ,use builtin cmd (cd, hel,exit)
  do
  {
    display_prompt();
    CMD = read_cmd();
    args = split_cmd(CMD);
    run = prog(args);
    free(CMD);
    free(args);

  } while (run); // Exit shell if the user enter Goodbye function

  return EXIT_SUCCESS;
}

int prog(char **args)
{
  char path[256] = {"/home/braude/"};  // Change the path
  pid_t pid;
  int flag = 0;

  if (args[0] == NULL)
  {
    return 1;
  } //An empty command was entered.

  if (!strcmp(args[0], "CreateExam"))
    strcat(path, Create_func);

  else if (!strcmp(args[0], "SolExam"))
    strcat(path, Sol_func);

  else if (!strcmp(args[0], "StartExam"))
    strcat(path, Start_func);

  else if (!strcmp(args[0], "CheckExam"))
    strcat(path, Check_func);

  else if (!strcmp(args[0], "CalcGrade"))
    strcat(path, Calc_func);

  else if (!strcmp(args[0], "GoodBye"))
    GoodBye();

  else
    return cmd_execute(args);

  pid = fork();
  if (pid == 0)
  { // Child process

    if (execv(path, args) == -1)
      perror("Not Supported\n");
    exit(EXIT_FAILURE);
  }
  else if (pid != 0)
    wait(NULL);

  return 1;
}

char *read_cmd(void)
{
  int bufsize = 1024;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer)
  {
    perror("Allocation Memory");
    exit(EXIT_FAILURE);
  }
  while (1)
  {
    c = getchar(); // Read a character

    if (c == EOF || c == '\n') // If we hit EOF, replace it with a null character and return.
    {
      buffer[position] = '\0';
      return buffer;
    }
    else
      buffer[position] = c;
    position++;

    if (position >= bufsize) // If we have exceeded the buffer, reallocate.
    {
      bufsize += 1024;
      buffer = realloc(buffer, bufsize);
      if (!buffer)
      {
        perror("Allocation Memory");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **split_cmd(char *CMD)
{
  int bufsize = TOK_BUFSIZE, index = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens)
  {
    perror("Allocation Memory");
    exit(EXIT_FAILURE);
  }
  token = strtok(CMD, " "); //return pointers to CMD, and place \0 at the end of each token

  while (token != NULL)
  {
    tokens[index++] = token; //store each pointer in an array

    if (index >= bufsize) //reallocate the array of pointers if necessary
    {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens)
      {
        perror("Allocation Memory");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, " "); //at which point we null-terminate the list of tokens
  }
  tokens[index] = NULL;
  return tokens;
}

int cmd_execute(char **args)
{
  int i;
  for (i = 0; i < (sizeof(builtin_str) / sizeof(char *)); i++)
  {
    if (strcmp(args[0], builtin_str[i]) == 0)
      return (*builtin_func[i])(args);
  }
  return cmd_launch(args);
}

int cmd_launch(char **args)
{ //function to execute other linux cmd
  pid_t pid;

  pid = fork();
  if (pid == 0)
  { // Child process
    if (execvp(args[0], args) == -1)
      printf("Not Supported\n");
    exit(EXIT_FAILURE);
  }
  else if (pid != 0)
    wait(NULL);

  return 1;
}

void display_prompt() // Done
{
  printf("AdvShell> ");
  printf("\n1) CreateExam  2) StartExam  3) SolExam  4) CheckExam  5) CalcGrade  6) GoodBye\n ");
}

//function for cd , help ,exit
int cmd_cd(char **args)
{
  if (args[1] == NULL)
    perror("expected argument to \"cd\"\n");
  else if (chdir(args[1]) != 0)
    perror("File doesn't exist");

  return 1;
}

int cmd_help(char **args)
{
  int i;
  for (i = 0; i < (sizeof(builtin_str) / sizeof(char *)); i++)
    printf("  %s\n", builtin_str[i]);

  return 1;
}

//end function cd help exit
int cmd_exit(char **args)
{
  return 0;
}

void GoodBye()
{
  exit(EXIT_SUCCESS);
}

// The End