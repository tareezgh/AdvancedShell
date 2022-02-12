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

// void CheckExam(char* file, char* name)
void main(int argc, char *argv[])
{
  int i, j = 0, k = 0, counter = 0, flag = 1;
  int testFile, solTestFile, answeredTestFile, gradeFile, rbytes, wbytes;
  char FileName[256] = {}, testFileName[256] = {}, solTestFileName[256] = {}, answeredTestFileName[256] = {}, gradeFileName[256] = {};
  char trueAnswer[256] = {}, charCounter[256] = {};

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

    strcpy(solTestFileName, sol_str);
    strcat(solTestFileName, argv[1]);
    strcat(solTestFileName, txt_str);
    // solTestFileName = Sol_file.txt

    strcpy(answeredTestFileName, argv[2]);
    strcat(answeredTestFileName, "_");
    strcat(answeredTestFileName, argv[1]);
    strcat(answeredTestFileName, txt_str);
    // answeredTestFileName = name_file.txt

    strcpy(gradeFileName, "Grade_");
    strcat(gradeFileName, argv[2]);
    strcat(gradeFileName, "_");
    strcat(gradeFileName, argv[1]);
    strcat(gradeFileName, txt_str);
    // gradeFileName = Grade_name_file.txt

    /// ******* OPEN FILES
    // open test file to read
    if ((testFile = open(testFileName, O_RDONLY, 0664)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    // open test SOL file to read
    if ((solTestFile = open(solTestFileName, O_RDONLY)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    // open test Answered Question file to read
    if ((answeredTestFile = open(answeredTestFileName, O_RDONLY)) == -1)
    {
      printf("%s Not Found!\n", argv[1]);
      exit(0);
    }

    // open grade file to write
    if ((gradeFile = open(gradeFileName, O_WRONLY | O_CREAT, 0664)) == -1)
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
    // read inserted Answer and save to q[k].insertAnswers
    // read from Sol_file.txt and compare with name_file.txt
    while (read(solTestFile, &trueAnswer[j], 1) && read(answeredTestFile, &q[k].insertAnswers, 1)) // until end of line
    {
      // printf("k = %d j = %d\n", k, j);
      if (trueAnswer[j] != '\n' && q[k].insertAnswers[j] != '\n')
      {
        // printf("trueAnswer is %c ***** insertedAnswer is %s\n", trueAnswer[j], q[k].insertAnswers);
        if (k < numberOfQuestion)
        {
          if (strcmp(&trueAnswer[j], q[k].insertAnswers) == 0) // right
            counter++;
          // wrong
          j++;
          k++;
        }

        // printf("counter is %d\n", counter);
      }
      j++;
    }

    // write to file gradeFileName
    sprintf(charCounter, "%d", counter); // itoa(i,ch,10);
    if ((wbytes = write(gradeFile, charCounter, strlen(charCounter))) == -1)
    {
      perror("write");
      exit(0);
    }
    write(gradeFile, "/", 1);
    if ((wbytes = write(gradeFile, numberOfQuestionCh, strlen(numberOfQuestionCh))) == -1)
    {
      perror("write");
      exit(0);
    }

    close(testFile);
    close(solTestFile);
    close(answeredTestFile);
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
