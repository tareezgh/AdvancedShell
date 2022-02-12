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

// void StartExam(char* file, char* name)
void main(int argc, char *argv[])
{
  int i, j, k = 0, flag = 1, numQ, testFile, answeredTestFile, rbytes, wbytes;
  char numA[256], buff[1024];
  char FileName[256] = {}, testFileName[256] = {}, answeredTestFileName[256] = {};

  if (argc != 3)
  {
    //printf("Bad number of parameters\n");
    exit(1);
  }

  strcpy(FileName, argv[1]);
  if (checkIfFileExists(FileName))
  {
    // ******** Files Names
    strcpy(testFileName, argv[1]);
    strcat(testFileName, txt_str);
    // testFileName = file.txt

    strcpy(answeredTestFileName, argv[2]);
    strcat(answeredTestFileName, "_");
    strcat(answeredTestFileName, argv[1]);
    strcat(answeredTestFileName, txt_str);
    // answeredTestFileName = name_file.txt

    /// ******* OPEN FILES
    if ((testFile = open(testFileName, O_RDONLY)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    if ((answeredTestFile = open(answeredTestFileName, O_WRONLY | O_CREAT, 0664)) == -1) //creating the file
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    /// ********** START READ

    // Read num of questions
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

    lseek(testFile, 0, SEEK_SET); // return to the start of the file
    if ((rbytes = read(testFile, buff, 1024)) == -1)
    {
      perror("read");
      exit(0);
    }
    printf("%s\n", buff); // print test's content

    /// ********** insert answers

    printf("Insert number of Question and sol:\n");
    for (i = 0; i < numberOfQuestion; i++)
    {
      scanf("%d %s", &numQ, numA);
      strcpy(q[numQ - 1].insertAnswers, numA); // q[numQ].insertAnswers = numA;
    }

    // Write the answers to the file by order
    for (i = 0; i < numberOfQuestion; i++)
    {
      if ((wbytes = write(answeredTestFile, q[i].insertAnswers, strlen(q[i].insertAnswers))) == -1)
      {
        perror("write");
        exit(0);
      }
      write(answeredTestFile, "\n", 1); // new line
    }

    close(testFile);
    close(answeredTestFile);

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
