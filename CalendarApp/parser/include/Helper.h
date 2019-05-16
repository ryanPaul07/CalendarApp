
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"

typedef struct contentLine{
  char name[100];
  char description[100000];
} contentLineVales;

int fileSize(char* fileName);
int numOfLines(int size, char* array);
int sizeOfLineInBuffer(char* buffer, int fileCount);


void contentLine(char* contentLine, contentLineVales **line);

int parseCal(int totalLinesinFile, int index, char** contentLineCal,Calendar **obj);
int eventParser(int totalLinesinFile, int index, char** contentLineEvent,Event **eventObj);
int alarmParser(int totalLinesinFile, int index, char** contentLineAlarm,Alarm **alarmObj);

ICalErrorCode invalidCalChecker(int numofLinesInFile, char**twoDArrayUnfoldedChecker);
