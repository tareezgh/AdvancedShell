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

// void CalcGrade(char* file, char* name)
void main(int argc, char *argv[])
{
  int i = 0, Grade = 0, testFile, gradeFile, rbytes, wbytes, flag1 = 1, flag2 = 1, correctInt;
  char FileName[256] = {}, testFileName[256] = {}, gradeFileName[256] = {};
  char correct[256] = {}, GradeCh[256] = {};

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

    strcpy(gradeFileName, "Grade_");
    strcat(gradeFileName, argv[2]);
    strcat(gradeFileName, "_");
    strcat(gradeFileName, argv[1]);
    strcat(gradeFileName, txt_str);
    // gradeFileName = Grade_name_file.txt

    /// ******* OPEN FILES
    if ((testFile = open(testFileName, O_RDONLY, 0664)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    if ((gradeFile = open(gradeFileName, O_RDWR | O_APPEND, 0664)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    /// ********** START READ

    // Read num of questions
    while (flag1)
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
        flag1 = 0;
      }
    }
    numberOfQuestion = atoi(numberOfQuestionCh);
    q = malloc(numberOfQuestion * sizeof(struct Question)); // initiate global struct

    // Read number of correct answers
    while (flag2)
    {
      if ((rbytes = read(gradeFile, correct, 1)) == -1)
      {
        perror("read 1");
        exit(0);
      }

      if (correct[0] >= '0' && correct[0] <= '9')
      {
        correctInt = atoi(correct);
        flag2 = 0;
      }
    }
    Grade = 100 / numberOfQuestion * correctInt; // calculate grade
    sprintf(GradeCh, "%d", Grade);               // itoa

    // print to end of the file
    write(gradeFile, "\n", 1);
    write(gradeFile, "Final Grade: ", 13);
    if ((wbytes = write(gradeFile, GradeCh, strlen(GradeCh))) == -1)
    {
      perror("write");
      exit(0);
    }
    write(gradeFile, "/100", 4);
    // print
    printf("%s\n", argv[2]);
    printf("%s\n", argv[1]);
    printf("Final Grade: %d\n", Grade);

    close(testFile);
    close(gradeFile);

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
