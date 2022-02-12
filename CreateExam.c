#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define txt_str ".txt"

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

// void CreateExam(char* file, char* sub, char* author)
void main(int argc, char *argv[])
{
  int fileName, num, i, j, wbytes;
  char buff[256];
  char chI[256] = {}, chJ[256] = {};
  char testFileName[256] = {};

  if (argc != 4)
  {
    //printf("Bad number of parameters\n");
    exit(1);
  }

  strcpy(testFileName, argv[1]);
  strcat(testFileName, txt_str);
  // testFileName = file.txt

  printf("Insert Number of Question:\n");
  scanf("%s", numberOfQuestionCh);
  numberOfQuestion = atoi(numberOfQuestionCh); // atoi for number of question

  q = malloc(numberOfQuestion * sizeof(struct Question)); // initiate global struct

  for (i = 0; i < numberOfQuestion; i++)
  {
    printf("Insert Question %d:\n", i + 1);
    scanf(" %[^\n]s", q[i].question); // without & and spaces
    printf("Insert 4 Answers:\n");
    for (j = 0; j < 4; j++)
    {
      printf(" %d. ", j + 1);
      scanf(" %[^\n]s", buff); // without & and spaces
      q[i].fourAnswers[j] = malloc(sizeof(strlen(buff)));
      strcpy(q[i].fourAnswers[j], buff);
    }
  }

  /// ******* OPEN FILES
  if ((fileName = open(testFileName, O_WRONLY | O_CREAT, 0664)) == -1) // creating the file
  {
    printf("%s Not Found!\n", argv[1]);
    exit(0);
  }

  // ******** Write to the file

  // write subject
  if ((wbytes = write(fileName, argv[2], strlen(argv[2]))) == -1)
  {
    perror("write");
    exit(0);
  }
  write(fileName, "\n", 1); // new line

  // write num of questions
  if ((wbytes = write(fileName, numberOfQuestionCh, strlen(numberOfQuestionCh))) == -1)
  {
    perror("write");
    exit(0);
  }
  write(fileName, "\n", 1); // new line

  // write the questions
  for (i = 0; i < numberOfQuestion; i++)
  {
    sprintf(chI, "%d", i + 1); // itoa(i,ch,10);
    if ((wbytes = write(fileName, "Question ", 9)) == -1)
    {
      perror("write");
      exit(0);
    }
    if ((wbytes = write(fileName, chI, strlen(chI))) == -1)
    {
      perror("write");
      exit(0);
    }

    write(fileName, ":", 1);
    write(fileName, "\n", 1); // new line
    write(fileName, "  ", 2);
    if ((wbytes = write(fileName, q[i].question, strlen(q[i].question))) == -1)
    {
      perror("write");
      exit(0);
    }
    write(fileName, "\n", 1); // new line

    // write the answers
    for (j = 0; j < 4; j++)
    {
      sprintf(chJ, "%d", j + 1); // itoa(j,ch,10);
      write(fileName, "    ", 4);

      if ((wbytes = write(fileName, chJ, strlen(chJ))) == -1)
      {
        perror("write");
        exit(0);
      }
      write(fileName, ". ", 2); // 1. 2. 3. 4.

      if ((wbytes = write(fileName, q[i].fourAnswers[j], strlen(q[i].fourAnswers[j]))) == -1)
      {
        perror("write");
        exit(0);
      }
      write(fileName, "\n", 1); // new line
    }                           // End for answers
  }                             // End for questions

  write(fileName, "\n", 1); // new line
  write(fileName, "Successfully", 12);
  write(fileName, "\n", 1); // new line

  // write the author name
  if ((wbytes = write(fileName, argv[3], strlen(argv[3]))) == -1)
  {
    perror("write");
    exit(0);
  }
  write(fileName, "\n", 1); // new line
  close(fileName);
  free(q);
}
