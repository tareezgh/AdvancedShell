#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define txt_str ".txt"
#define sol_str "Sol_"
int numberOfQuestion;
char numberOfQuestionCh[256];

typedef struct Question
{
  char question[256];
  char *fourAnswers[5];
  char trueAnswer[256];
  char insertAnswers[256]; // array for save answers by user
} Question;

struct Question *q; // global array

int checkIfFileExists(const char *filename);

// void SolExam(char* file)
void main(int argc, char *argv[])
{
  int testFile, solTestFile, i, flag = 1, k = 0, rbytes, wbytes;
  char FileName[256] = {}, testFileName[256] = {}, solTestFileName[256] = {};

  if (argc != 2)
  {
    //printf("Bad number of parameters\n");
    exit(1);
  }

  strcpy(FileName, argv[1]);
  if (checkIfFileExists(FileName))
  {
    // ******** Files Names
    strcat(testFileName, argv[1]);
    strcat(testFileName, txt_str);
    // testFileName = file.txt

    strcpy(solTestFileName, sol_str);
    strcat(solTestFileName, argv[1]);
    strcat(solTestFileName, txt_str);
    // solTestFileName = Sol_file.txt

    /// ******* OPEN FILES

    if ((testFile = open(testFileName, O_RDONLY, 0664)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    if ((solTestFile = open(solTestFileName, O_WRONLY | O_CREAT, 0664)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    /// ********** START READ

    while (flag)
    {
      if ((rbytes = read(testFile, numberOfQuestionCh, 1)) == -1)
      {
        perror("read 1");
        exit(0);
      }
      if (numberOfQuestionCh[0] == '\n')
      {
        if ((rbytes = read(testFile, numberOfQuestionCh, 1)) == -1)
        {
          perror("read 2");
          exit(0);
        }
        flag = 0;
      }
    }

    numberOfQuestion = atoi(numberOfQuestionCh);
    q = malloc(numberOfQuestion * sizeof(struct Question)); // initiate global struct

    for (i = 0; i < numberOfQuestion; i++)
    {
      printf("Insert True Sol for Question %d:\n", i + 1);
      scanf("%s", q[i].trueAnswer);
    }

    for (i = 0; i < numberOfQuestion; i++)
    {
      if ((wbytes = write(solTestFile, q[i].trueAnswer, strlen(q[i].trueAnswer))) == -1)
      {
        perror("write");
        exit(0);
      }
      write(solTestFile, "\n", 1); // new line
    }

    close(testFile);
    close(solTestFile);
    free(q);
  }
  else
    printf("%s Not Found!\n", argv[1]);
}

int checkIfFileExists(const char *filename)
{
  char file[256] = {};
  struct stat buffer;
  strcpy(file, filename);
  strcat(file, txt_str);
  int exist = stat(file, &buffer);
  if (exist == 0)
    return 1;
  else
    return 0;
}