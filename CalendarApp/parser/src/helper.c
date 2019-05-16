/*
* Name: Ryan Paul
* Id: 1017834
* Date: Tuesday Feb 5th
* Assignment 1
*/

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"
#include "Helper.h"

void dateTimeParser(DateTime **generalDateTime, char *dateAndTime);

char* addEventToCalendar(char* fileName, char* dtstartDate,char* dtstartTime, char* dtstampDate,char* dtstampTime, char* uid, char* summary)
{
  char *directory = malloc(300);
  char *dtstamp = calloc(1000,sizeof(char));
  char *dtstart = calloc(1000,sizeof(char));
  Event *event = malloc(sizeof(Event));
  DateTime *dtStamp = malloc(sizeof(DateTime));
  DateTime *dtStart = malloc(sizeof(DateTime));
  char *successfulCal = calloc(100,sizeof(char));

  strcpy(directory,"uploads/");
  strcat(directory,fileName);

  //makes the creation date time string
  strcpy(dtstamp,dtstampDate);
  strcat(dtstamp,"T");
  strcat(dtstamp,dtstampTime);

  //makes the start date time string
  strcpy(dtstart,dtstartDate);
  strcat(dtstart,"T");
  strcat(dtstart,dtstartTime);


  dateTimeParser(&dtStamp,dtstamp);
  strcpy(event->creationDateTime.date,dtStamp->date);
  strcpy(event->creationDateTime.time,dtStamp->time);

  if(dtStamp->UTC == 1)
  {
    event->creationDateTime.UTC = true;
  }
  else
  {
    event->creationDateTime.UTC = false;
  }

  printf("just freed dtStamp\n");
  free(dtStamp);

  dateTimeParser(&dtStart,dtstart);
  strcpy(event->startDateTime.date,dtStart->date);    /// Need to implement the utc
  strcpy(event->startDateTime.time,dtStart->time);

  if(dtStart->UTC == true)
  {
    event->startDateTime.UTC = true;
  }
  else
  {
    event->startDateTime.UTC = false;
  }

  printf("just freed dtStart\n");
  free(dtStart);

  printf("%s\n",uid);

  //printf("before concat");
  strcpy(event->UID,uid);

  event->properties =  initializeList(printProperty,deleteProperty,compareProperties);
  event->alarms = initializeList(printAlarm,deleteAlarm,compareAlarms);


  if(strlen(summary) == 0)
  {


  }
  else
  {
    Property *property = calloc(1,sizeof(Property)+1000);

    strcpy(property->propName,"SUMMARY");
    strcpy(property->propDescr,summary);
    insertBack(event->properties,property);

  }


  Calendar *cal = NULL;

  ICalErrorCode error = createCalendar(directory,&cal);

  printf("%s\n", directory);

  if(error != 0)
  {
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));

    //printf("this error while writing the calendar===\n");
    errorMessage = printError(error);
    //printf("error Message |%s|\n", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",fileName,errorMessage);
    free(errorMessage);

    return errorMessageReturned;
  }

  addEvent(cal,event);

  writeCalendar(directory, cal);

  sprintf(successfulCal,"{\"fileName\":\"%s\"}",fileName);
  return successfulCal;
}

char* GUIToCalendar(char* file, char* version, char* prodid, char* dtstartDate,char* dtstartTime, char* dtstampDate,char* dtstampTime, char* uid)
{

  char *dtstamp = calloc(1000,sizeof(char));
  char *dtstart = calloc(1000,sizeof(char));
  Calendar *cal = malloc(sizeof(Calendar));
  Event *event = malloc(sizeof(Event));
  DateTime *dtStamp = malloc(sizeof(DateTime));
  DateTime *dtStart = malloc(sizeof(DateTime));
  char *successfulCal = calloc(100,sizeof(char));


  char *directory = malloc(300);

  strcpy(directory,"./uploads/");
  strcat(directory,file);


  //makes the creation date time string
  strcpy(dtstamp,dtstampDate);
  strcat(dtstamp,"T");
  strcat(dtstamp,dtstampTime);

  //makes the start date time string
  strcpy(dtstart,dtstartDate);
  strcat(dtstart,"T");
  strcat(dtstart,dtstartTime);

  cal->version = atoi(version);
  strcpy(cal->prodID,prodid);


  dateTimeParser(&dtStamp,dtstamp);
  strcpy(event->creationDateTime.date,dtStamp->date);
  strcpy(event->creationDateTime.time,dtStamp->time);
  if(dtStamp->UTC == 1)
  {
    event->creationDateTime.UTC = true;
  }
  else
  {
    event->creationDateTime.UTC = false;
  }

  printf("just freed dtStamp\n");
  free(dtStamp);

  dateTimeParser(&dtStart,dtstart);
  strcpy(event->startDateTime.date,dtStart->date);    /// Need to implement the utc
  strcpy(event->startDateTime.time,dtStart->time);
  if(dtStart->UTC == true)
  {
    event->startDateTime.UTC = true;
  }
  else
  {
    event->startDateTime.UTC = false;
  }

  printf("just freed dtStart\n");
  free(dtStart);

  printf("%s\n",uid);

  //printf("before concat");
  strcpy(event->UID,uid);
  printf("before list init");
  cal->properties = initializeList(printProperty,deleteProperty,compareProperties);
  cal->events = initializeList(printEvent,deleteEvent,compareEvents);
  printf("before init event list");

  event->properties =  initializeList(printProperty,deleteProperty,compareProperties);
  event->alarms = initializeList(printAlarm,deleteAlarm,compareAlarms);
  printf("this is before add event\n");
  addEvent(cal, event);
  printf("this is after the add event\n");


  printf("reached before validate Calendar\n");
  ICalErrorCode errorValidate = validateCalendar(cal);

  printf("reached after validate Calendar\n");
  if(errorValidate != 0)
  {
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));

    printf("this error while validating the calendar===\n");
    errorMessage = printError(errorValidate);
    //printf("error Message |%s|\n", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",file,errorMessage);
    free(errorMessage);

    return errorMessageReturned;
  }

  ICalErrorCode errorCode = writeCalendar(directory, cal);

  if(errorCode != 0)
  {
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));

    printf("this error while writing the calendar===\n");
    errorMessage = printError(errorCode);
    //printf("error Message |%s|\n", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",file,errorMessage);
    free(errorMessage);

    return errorMessageReturned;
  }


  sprintf(successfulCal,"{\"fileName\":\"%s\"}",file);
  return successfulCal;
}


char* alarmToJSON(const Alarm *alarm)
{
  char *tempAlarm = calloc(100000,sizeof(char));

  if(alarm == NULL)
  {
    strcpy(tempAlarm,"{}");
    return tempAlarm;
  }

  int propCounter = 0;


  ListIterator iterAlarmProps = createIterator(alarm->properties);

	Property* elem;

	while((elem = nextElement(&iterAlarmProps)) != NULL)
  {
    propCounter++;
  }

  sprintf(tempAlarm,"{\"action\":\"%s\",\"trigger\":\"%s\",\"numProps\":%d}",alarm->action,alarm->trigger,propCounter);

  return tempAlarm;
}

char* eventPropertyToJSON(const Property *property)
{
  char *tempProperty = calloc(100000,sizeof(char));

  if(property == NULL)
  {
    strcpy(tempProperty,"{}");
    return tempProperty;
  }

  sprintf(tempProperty,"{\"propName\":\"%s\",\"propDescription\":\"%s\"}",property->propName, property->propDescr);

  return tempProperty;
}

char* eventPropertyListToJSON(char* fileName, char *eventNum)
{
  int numReference =  1;

  int eventNumber = atoi(eventNum);

  char *directory = malloc(300);

  strcpy(directory,"uploads/");
  strcat(directory,fileName);

  Calendar *cal = NULL;

  ICalErrorCode error = createCalendar(directory,&cal);

  if(error != 0)
  {
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));

    errorMessage = printError(error);
    //printf("error Message |%s|\n", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",fileName,errorMessage);
    return errorMessageReturned;
  }

  // const issue
  List *events = (List*)cal->events;

  ListIterator iterEvents = createIterator(events);

  Event *event = NULL;


  while((event = nextElement(&iterEvents)) != NULL)
  {
    if(numReference == eventNumber)
    {
      break;
    }
    numReference++;
  }


  char *tempPList = calloc(1000,sizeof(char));
  char *tempP;
  int checker = 0;
  int len = 1000;

  // const issue
  List *properties = (List*)event->properties;

  ListIterator iterProperty = createIterator(properties);

  Property* propertyOne;

  strcpy(tempPList,"[");

	while((propertyOne = nextElement(&iterProperty)) != NULL)
  {
    if(checker == 1)
    {
      strcat(tempPList,",");
    }
    tempP = eventPropertyToJSON(propertyOne);
    len = len+strlen(tempP)+10000;
    tempPList = realloc(tempPList,len);
    strcat(tempPList,tempP);
    free(tempP);
    checker = 1;
  }

  checker = 0;

  strcat(tempPList,"]");

  if(strcmp(tempPList,"[]") == 0)
  {
    char *errorMessageReturned = calloc(100,sizeof(char));

    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"zeroProperties\":\"true\"}",fileName);
    return errorMessageReturned;
  }

  return tempPList;

}

char* alarmListToJSON(char *fileName,char *eventNum)
{

  int numReference =  1;

  int eventNumber = atoi(eventNum);

  char *directory = malloc(300);

  strcpy(directory,"uploads/");
  strcat(directory,fileName);

  Calendar *cal = NULL;

  ICalErrorCode error = createCalendar(directory,&cal);

  if(error != 0)
  {
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));

    errorMessage = printError(error);
    //printf("error Message |%s|\n", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",fileName,errorMessage);
    return errorMessageReturned;
  }


  // const issue
  List *events = (List*)cal->events;

  ListIterator iterEvents = createIterator(events);

  Event *event = NULL;


  while((event = nextElement(&iterEvents)) != NULL)
  {
    if(numReference == eventNumber)
    {
      break;
    }
    numReference++;
  }




  char *tempAList = calloc(1000,sizeof(char));
  char *tempA;
  int checker = 0;
  int len = 1000;

  // const issue
  List *alarms = (List*)event->alarms;

  ListIterator iterAlarm = createIterator(alarms);

	Alarm* alarm;

  strcpy(tempAList,"[");

	while((alarm = nextElement(&iterAlarm)) != NULL)
  {
    if(checker == 1)
    {
      strcat(tempAList,",");
    }
    tempA = alarmToJSON(alarm);
    len = len+strlen(tempA)+10000;
    tempAList = realloc(tempAList,len);
    strcat(tempAList,tempA);
    free(tempA);
    checker = 1;
  }

  checker = 0;

  strcat(tempAList,"]");

  if(strcmp(tempAList,"[]") == 0)
  {
    char *errorMessageReturned = calloc(100,sizeof(char));

    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"zeroAlarm\":\"true\"}",fileName);
    return errorMessageReturned;
  }

  return tempAList;
}


char* eventObjectToJson(char *fileName)
{

  // char *errorMessage;
  //
  // char *errorMessageReturned = calloc(100,sizeof(char));

  char *directory = malloc(300);

  strcpy(directory,"uploads/");
  strcat(directory,fileName);

  Calendar *cal = NULL;

  ICalErrorCode error = createCalendar(directory,&cal);

  if(error != 0)
  {

    return "{}";
  }


  char *tempEList = calloc(1000,sizeof(char));
  char *tempE;
  int checker = 0;
  int len = 1000;

  if(cal->events == NULL)
  {
    strcpy(tempEList,"[]");
    return tempEList;
  }
  // const issue
  List *events = (List*)cal->events;

  ListIterator iterEvents = createIterator(events);

  Event* event;

  strcpy(tempEList,"[");

  len = len+strlen(tempEList)+1000;
  tempEList = realloc(tempEList,len);

  strcat(tempEList,"{\"fileName\":\"");
  strcat(tempEList,fileName);
  strcat(tempEList,"\"},");

  while((event = nextElement(&iterEvents)) != NULL)
  {
    if(checker == 1)
    {
      strcat(tempEList,",");
    }
    tempE = eventToJSON(event);
    len = len+strlen(tempE)+10000;
    tempEList = realloc(tempEList,len);
    strcat(tempEList,tempE);
    free(tempE);
    checker = 1;
  }

  checker = 0;

  strcat(tempEList,"]");

  //printf("%s\n", tempEList);


  return tempEList;
}

char* objectToJson(char *fileName)
{


  // add deleate calendar
  char *directory = malloc(300);

  strcpy(directory,"uploads/");
  strcat(directory,fileName);

  Calendar *cal = NULL;

  ICalErrorCode error = createCalendar(directory,&cal);

  //printf("error =  %d\n", error);

  if(error != 0)
  {
    //printf("reached error\n");
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));

    errorMessage = printError(error);
    //printf("error Message |%s|\n", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",fileName,errorMessage);
    free(errorMessage);

    return errorMessageReturned;
  }


  ICalErrorCode errorTwo = validateCalendar(cal);

  //printf("errorTwo = %d\n", errorTwo);

  if(errorTwo != 0)
  {
    //printf("reached errorTwo\n");
    char *errorMessage;

    char *errorMessageReturned = calloc(100,sizeof(char));
    errorMessage = printError(errorTwo);
    //printf("error Message |%s|", errorMessage);
    sprintf(errorMessageReturned,"{\"fileName\":\"%s\",\"errorMessage\":\"%s\"}",fileName,errorMessage);
    free(errorMessage);

    return errorMessageReturned;
  }



  char *tempCal = calloc(100000,sizeof(char));
  int calPropCount = 0;
  int calEventCount = 0;

  // if(cal == NULL)
  // {
  //   strcpy(tempCal,"{}");
  //   return tempCal;
  // }


  ListIterator iterCalProp = createIterator(cal->properties);

  Property* prop1;


  while((prop1 = nextElement(&iterCalProp)) != NULL)
  {
    calPropCount++;
  }

  calPropCount = calPropCount + 2;      // since version and prodid

  ListIterator iterCalEvent = createIterator(cal->events);

  Event* event1;      // before : property after: event


  while((event1 = nextElement(&iterCalEvent)) != NULL)
  {
    calEventCount++;
  }

  if(calEventCount == 0)
  {
    strcpy(tempCal,"{}");
    return tempCal;
  }

  sprintf(tempCal,"{\"fileName\":\"%s\",\"version\":%.0f,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}",fileName,cal->version,cal->prodID,calPropCount,calEventCount);
  // cal->Version always be 2 ??

  return tempCal;

  //return calendarToJSON(new);
}

/*
Opens the file and counts the amount of charaters in the file and then returns an int
*/
int fileSize(char* fileName)
{
    int counter = 0;
    char ch;

    FILE *file;

    file = fopen(fileName,"r");

    if(file != NULL)
    {
        while((ch = fgetc(file)) != EOF)
        {
            counter++;
        }
        //printf("Counter = %d\n", counter);
        fclose(file);
    }
    else
    {
      counter = 0;          //if Counter = 0 error file
    }
    return counter;
}

/*
Counts total amount of lines in the file (including folded lines) and returns an int
*/
int numOfLines(int size, char* array)
{
  int i;
  int line = 0;

  for(i = 0; i < size; i++)
  {
    if(array[i] == '\r' && array[i+1] == '\n')
    {
      line++;
      i = i+2;
    }

  }
  //printf("Number of lines in numOfLines function is %d\n", line);
  return line;
}


/*
Takes a content line from the 2D Array and splits it by a delimeter and stores the information in a structs
which has a name and a description
parameters - 1 Content Line from the 2D Array and the struct to store the infrormation
*/
void contentLine(char* contentLine, contentLineVales **line)
{
  int size = strlen(contentLine);
  int i;
  int l = 0;
  int r = 0;
  int left = 0;                    // indicates if left is full or empyt based on if it is equal to zero or one


  for(i = 0; i < size; i++)
  {
    if(left == 0)
    {

      (*line)->name[l] = (contentLine[i]);                         //turning each character to toupper before: (*line)->name[l] = toupper(contentLine[i]);
      (*line)->name[l+1] = '\0';
      l++;
    }
    if((contentLine[i+1] == ':' || contentLine[i+1] == ';') && left == 0)
    {
      left = 1;
      i = i+2;
    }
    if(left == 1)
    {
      //printf("test\n");
      (*line)->description[r] = (contentLine[i]);          // before: (*line)->description[r] = toupper(contentLine[i])
      (*line)->description[r+1] = '\0';
      r++;
    }
  }

  //printf("line name is %s line description is %s\n", (*line)->name, (*line)->description);

}


/*
Needs to be implemented, needs to check for if reaching the end of the fileName
*/
int sizeOfLineInBuffer(char* buffer, int fileCount)
{
  int i = 0;
  int counter = 0;

  for(i = 0; i < fileCount; i++)
  {
    if(buffer[i] == '\r' && buffer[i+1] == '\n')
    {
      if(buffer[i+2] == '\t' || buffer[i+2] == ' ')
      {
        i = i+3;
      }
      else
      {
        i = i+2;
        counter = 0;
      }
    }
    counter++;
  }

  //printf("==========================================================Counter = %d\n", counter);

  return 0;

}

void fileIntoString(char **pointerToString, char* fileName, int fileCount)
{
  int i = 0;
  char ch;            // ch is used to read a character from the file
  *pointerToString = calloc((fileCount+10), sizeof(char));

  FILE *ICSPointer;
  ICSPointer = fopen(fileName,"r");

  if(ICSPointer != NULL)
  {
      while((ch = fgetc(ICSPointer)) != EOF)
      {
          (*pointerToString)[i++] = ch;
      }
      fclose(ICSPointer);                               //Close File
  }
  else
  {
      //printf("Error Opening file\n");
  }
}


void dateTimeParser(DateTime **generalDateTime, char *dateAndTime)
{
  int i = 0;
  int size = 0;
  int l = 0;
  int r = 0;
  int left = 0;       // when left is zero, information is being stored in the date

  size  = strlen(dateAndTime);

  //printf("=========%c======\n\n\n\n\n\n\n\n",dateAndTime[size-1]);

  if(dateAndTime[size-1] == 'Z')
  {
    (*generalDateTime)->UTC = true;
  }
  else
  {
    (*generalDateTime)->UTC = false;
  }

  for(i = 0; i < size; i++)
  {
    if(left == 0)
    {
      (*generalDateTime)->date[l] = dateAndTime[i];
      l++;
    }
    if(dateAndTime[i+1] == 'T' && left == 0)        // before dateAndTime[i+1]
    {
      left = 1;
      i = i+2;
    }

    if(left == 1)
    {
      if(dateAndTime[i+1] == 'Z' || dateAndTime[i+1] == 'z')  // before dateAndTime[i+1]
      {
        (*generalDateTime)->time[r] = dateAndTime[i];
        break;
      }
      (*generalDateTime)->time[r] = dateAndTime[i];
      r++;
    }
  }

  (*generalDateTime)->date[9-1] = '\0';
  (*generalDateTime)->time[7-1] = '\0';

  //printf("\tdate=|%s| time=|%s|\n UTC = %d",(*generalDateTime)->date, (*generalDateTime)->time, (*generalDateTime)->UTC);
}



int parseCal(int totalLinesinFile, int index, char** contentLineCal,Calendar **obj)
{
  int i;
  for(i = index; i < totalLinesinFile; i++)
  {
    while(contentLineCal[i][0] == ';')
    {
      i++;
    }

    contentLineVales *lineInCal = calloc(1,sizeof(contentLineVales));
    contentLine(contentLineCal[i], &lineInCal);

    if(strcasecmp(lineInCal->name, "VERSION") == 0)
    {
      //printf("name = |%s| description = |%s|\n",line->name,line->description);
      (*obj)->version = atof(lineInCal->description);
      //printf("\tCalendar Version = |%.2f|\n",(*obj)->version);
    }

    else if(strcasecmp(lineInCal->name, "PRODID") == 0)
    {
      strcpy((*obj)->prodID, (lineInCal->description));
      //printf("\tCalendar PROID = |%s|\n",(*obj)->prodID);
    }
    else if(strcasecmp(lineInCal->name, "BEGIN") == 0 && strcasecmp(lineInCal->description, "VEVENT") == 0)
    {
      //printf("Parsing VEvent\n");
      Event *e = malloc(sizeof(Event));
      e->properties =  initializeList(printProperty,deleteProperty,compareProperties);
      e->alarms = initializeList(printAlarm,deleteAlarm,compareAlarms);
      i = eventParser(totalLinesinFile, i, contentLineCal, &e);
      insertBack((*obj)->events, e);
    }
    else if(strcasecmp(lineInCal->name, "END") == 0 && strcasecmp(lineInCal->description, "VCALENDAR") == 0)
    {
      free(lineInCal);
      return i;
    }
    else
    {
      if((strcasecmp(lineInCal->name, "END") != 0) && (strcasecmp(lineInCal->name, "BEGIN") != 0))
      {
        Property *pOfCalendar = malloc(sizeof(Property)+sizeof(char[strlen(lineInCal->description)])+1);
        strcpy(pOfCalendar->propName,lineInCal->name);
        strcpy(pOfCalendar->propDescr,lineInCal->description);
        //printf("\tVcalendar PROP Name = |%s| ",pOfCalendar->propName);
        //printf(" Vcalendar PROP Descr = |%s|\n",pOfCalendar->propDescr);
        insertBack((*obj)->properties,pOfCalendar);
        //free(pOfCalendar);
      }
    }

    free(lineInCal);
  }
  return i;
}



//----------------------------------




int eventParser(int totalLinesinFile, int index, char** contentLineEvent,Event **eventObj)
{
  int i;
  for(i = index; i < totalLinesinFile; i++)
  {

    while(contentLineEvent[i][0] == ';')
    {
      i++;
    }

    contentLineVales *lineInEvent = calloc(1,sizeof(contentLineVales));
    contentLine(contentLineEvent[i], &lineInEvent);

    if(strcasecmp(lineInEvent->name, "UID") == 0)
    {
      strcpy((*eventObj)->UID,lineInEvent->description);
      //printf("\t test test Event UID = |%s|\n", (*eventObj)->UID);
    }
    else if(strcasecmp(lineInEvent->name, "DTSTAMP") == 0)
    {
      DateTime *dtStamp = malloc(sizeof(DateTime));
      dateTimeParser(&dtStamp,lineInEvent->description);
      strcpy((*eventObj)->creationDateTime.date,dtStamp->date);
      strcpy((*eventObj)->creationDateTime.time,dtStamp->time);
      if(dtStamp->UTC == 1)
      {
        (*eventObj)->creationDateTime.UTC = true;
      }
      else
      {
        (*eventObj)->creationDateTime.UTC = false;
      }
      //printf("\tEvent DTSTAMP date=|%s| time=|%s|\n",((*eventObj)->creationDateTime).date, ((*eventObj)->creationDateTime).time);
      free(dtStamp);
    }
    else if(strcasecmp(lineInEvent->name, "DTSTART") == 0)
    {
      DateTime *dtStart = malloc(sizeof(DateTime));
      dateTimeParser(&dtStart,lineInEvent->description);
      strcpy((*eventObj)->startDateTime.date,dtStart->date);    /// Need to implement the utc
      strcpy((*eventObj)->startDateTime.time,dtStart->time);
      if(dtStart->UTC == true)
      {
        (*eventObj)->startDateTime.UTC = true;
      }
      else
      {
        (*eventObj)->startDateTime.UTC = false;
      }
      //printf("\tEvent DTSTART date=|%s| time=|%s|\n",((*eventObj)->startDateTime).date, ((*eventObj)->startDateTime).time);
      free(dtStart);
    }
    else if(strcasecmp(lineInEvent->name, "BEGIN") == 0 && strcasecmp(lineInEvent->description, "VALARM") == 0)
    {
      Alarm *a = malloc(sizeof(Alarm));
      a->properties = initializeList(printProperty,deleteProperty,compareProperties);
      i = alarmParser(totalLinesinFile, i, contentLineEvent, &a);
      insertBack((*eventObj)->alarms, a);
    }
    else if(strcasecmp(lineInEvent->name, "END") == 0 && strcasecmp(lineInEvent->description, "VEVENT") == 0)
    {
      free(lineInEvent);
      //printf("i value in event parser is %d\n", i);
      return i;
    }
    else
    {
      if((strcasecmp(lineInEvent->name, "END") != 0) && (strcasecmp(lineInEvent->name, "BEGIN") != 0))
      {
        Property *pOfEvent = malloc(sizeof(Property)+sizeof(char)*(strlen(lineInEvent->description)+1));
        strcpy(pOfEvent->propName,lineInEvent->name);
        strcpy(pOfEvent->propDescr,lineInEvent->description);
        //printf("\tEvent Properties Prop Name = |%s| ", pOfEvent->propName);
        //printf("Event Properties Prop Descr = |%s|\n", pOfEvent->propDescr);
        insertBack((*eventObj)->properties,pOfEvent);
        //free(pOfEvent);
      }
    }
    free(lineInEvent);
  }
  return i;
}



//-------------------------------------------------




int alarmParser(int totalLinesinFile, int index, char** contentLineAlarm,Alarm **alarmObj)
{
  int i;

  for(i = index; i < totalLinesinFile; i++)
  {

    while(contentLineAlarm[i][0] == ';')
    {
      i++;
    }

    contentLineVales *lineInAlarm = calloc(1,sizeof(contentLineVales));
    contentLine(contentLineAlarm[i], &lineInAlarm);

    if((strcasecmp(lineInAlarm->name, "TRIGGER") == 0))
    {
      (*alarmObj)->trigger = calloc((strlen(lineInAlarm->description)+1),sizeof(char));
      strcpy((*alarmObj)->trigger,lineInAlarm->description);
      //printf("\tAlarm trigger = |%s|\n", (*alarmObj)->trigger);
    }
    else if((strcasecmp(lineInAlarm->name, "ACTION") == 0))
    {
      strcpy((*alarmObj)->action,lineInAlarm->description);
      //printf("\tAlarm action = |%s|\n",(*alarmObj)->action);
    }
    else if((strcasecmp(lineInAlarm->name, "END") == 0) && (strcasecmp(lineInAlarm->description, "VALARM") == 0))
    {
      free(lineInAlarm);
      return i;
    }
    else
    {
      if((strcasecmp(lineInAlarm->name, "END") != 0) && (strcasecmp(lineInAlarm->name, "BEGIN") != 0))
      {
        Property *pOfAlarm = malloc(sizeof(Property)+sizeof(char[strlen(lineInAlarm->description)])+1);
        strcpy(pOfAlarm->propName,lineInAlarm->name);
        strcpy(pOfAlarm->propDescr,lineInAlarm->description);
        //printf("line name = |%s| line description is = |%s|\n",line->name, line->description);
        //printf("\tAlarm Properties Prop Name = |%s| ", pOfAlarm->propName);
        //printf("Alarm Properties Prop Descr = |%s|\n", pOfAlarm->propDescr);
        insertBack((*alarmObj)->properties,pOfAlarm);
        //free(pOfAlarm);
      }
    }

    free(lineInAlarm);
  }
  return i;
}



//-------------------------------------------------



ICalErrorCode invalidCalChecker(int numofLinesInFile, char** twoDArrayUnfoldedChecker)
{
  int i = 0;

  /* 0 indicates false 1 indicase true and if value is greate than 1, there is an duplicate */
  int version = 0;
  int number = 0;
  int prodID = 0;
  int beginVEvent = 0;
  int vEvent = 0;
  int scope = 0;
  int endVEvent = 0;
  int beginAlarm = 0;
  int endVAlarm = 0;
  int uid = 0;
  int dtStamp = 0;
  int dtStart = 0;
  int action = 0;
  int trigger = 0;
  int dateTimeLen = 0;

  if((strcasecmp(twoDArrayUnfoldedChecker[0],"BEGIN:VCALENDAR") == 0) && (strcasecmp(twoDArrayUnfoldedChecker[numofLinesInFile-1], "END:VCALENDAR") == 0))
  {
    //good
  }
  else
  {
    //printf("The start index is not begin;VCALENDAR and the end index is not end;vcalendar\n");
    return INV_CAL;
  }

  for(i = 0; i < numofLinesInFile; i++)
  {

    // Looks at comments and increments over them if nessassary
    while(twoDArrayUnfoldedChecker[i][0] == ';')
    {
      i++;
    }

    contentLineVales *lineChecker = calloc(1,sizeof(contentLineVales));
    contentLine(twoDArrayUnfoldedChecker[i], &lineChecker);

    if(strcasecmp(twoDArrayUnfoldedChecker[i], "BEGIN:VCALENDAR") == 0)
    {
      // Incremets scope to 1 to for vcal values
      scope++;
    }

    else if(strcasecmp(twoDArrayUnfoldedChecker[i], "BEGIN:VEVENT") == 0)
    {
      // Increments scope to 2 for vevent values
      // Indicates that there is an beign vevent which is used for the vcal validator
      beginVEvent = 1;
      vEvent++;
      scope++;
    }

    else if(strcasecmp(twoDArrayUnfoldedChecker[i], "BEGIN:VALARM") == 0)
    {
      // Increments scope to 3 for valarm values
      beginAlarm = 1;
      scope++;
    }

    else if(strcasecmp(twoDArrayUnfoldedChecker[i], "END:VCALENDAR") == 0)
    {
      // Scope gets decremented
      scope--;

      // there is not a event in the vcal, invalid v calendar
      if(beginVEvent == 0)
      {
        free(lineChecker);
        return INV_CAL;
      }

      //Checks if the beign vevent has an end vevent
      if(endVEvent == 0)
      {
        free(lineChecker);
        return INV_EVENT;
      }

      //checks if the vcalendar has a version
      if(version == 0)
      {
        free(lineChecker);
        return INV_CAL;
      }

      //checks if the vcalendar has a prodID
      if(prodID == 0)
      {
        free(lineChecker);
        return INV_CAL;
      }

      //checks if the vcalendar has a vEvent
      if(vEvent == 0)
      {
        free(lineChecker);
        return INV_CAL;
      }
    }

    else if(strcasecmp(twoDArrayUnfoldedChecker[i], "END:VEVENT") == 0)
    {
      // scope gets decremented
      // Indicates that there is an end vevent
      endVEvent++;
      scope--;

      // Indicates if an begin alarm has an end valarm
      if(beginAlarm == 1)
      {
        free(lineChecker);
        return INV_ALARM;
      }

      // Indicates if there is a uid in an event
      if(uid == 0)
      {
        free(lineChecker);
        return INV_EVENT;
      }

      // Indicates if there is a dtStamp is in the event
      if(dtStamp == 0)
      {
        free(lineChecker);
        return INV_EVENT;
      }

      // Indicates if there is a dtStart is in the event
      if(dtStart == 0)
      {
        free(lineChecker);
        return INV_EVENT;
      }
    }

    else if((beginAlarm == 1) && (strcasecmp(twoDArrayUnfoldedChecker[i], "END:VALARM") == 0))
    {
      // Indicates that there is an end VALARM
      // decrements the scope
      endVAlarm++;
      scope--;

      // Indicates if there is an action in the valarm
      if(action == 0)
      {
        free(lineChecker);
        return INV_ALARM;
      }

      // Indicates if there is an trigger in the valarm
      if(trigger == 0)
      {
        free(lineChecker);
        return INV_ALARM;
      }
      action = 0;
      trigger = 0;
      beginAlarm = -1;
    }

    // Indicates if the version is correct
    if((strcasecmp(lineChecker->name, "VERSION") == 0) && scope == 1)
    {
      version++;
      number = atof(lineChecker->description);
      if(number == 0.0)
      {
        free(lineChecker);
        return INV_VER;
      }
      if(version > 1)
      {
        free(lineChecker);
        return DUP_VER;
      }
    }

    // Indicates if the prodid is correct
    if((strcasecmp(lineChecker->name, "PRODID") == 0) && scope == 1)
    {
      prodID++;
      if(strcmp(lineChecker->description,"") == 0)
      {
        free(lineChecker);
        return INV_PRODID;
      }
      if(prodID > 1)
      {
        free(lineChecker);
        return DUP_PRODID;
      }
    }

    //--------------------------------------------- Event

    // Indicates if the uid is correct
    if(strcasecmp(lineChecker->name, "UID") == 0 && scope == 2)
    {
      uid++;
      if(strcmp(lineChecker->description, "") == 0)
      {
        free(lineChecker);
        return INV_EVENT;
      }
    }

    // Indicates if the dtstamp is correct
    if(strcasecmp(lineChecker->name, "DTSTAMP") == 0 && scope == 2)
    {
      dtStamp++;
      dateTimeLen = strlen(lineChecker->description);

      //printf("%s has length %d\n",lineChecker->description,dateTimeLen);

      if(strcmp(lineChecker->description, "") == 0)
      {
        free(lineChecker);
        return INV_DT;
      }
      if(dateTimeLen-1 < 14)
      {
        free(lineChecker);
        return INV_DT;
      }

      if(lineChecker->description[8] != 'T')
      {
        free(lineChecker);
        return INV_DT;
      }

      if(dateTimeLen-1 == 15)
      {
        if((lineChecker->description[dateTimeLen-1] == 'Z' || lineChecker->description[dateTimeLen-1] == ' '))
        {
        }
        else
        {
          free(lineChecker);
          return INV_DT;
        }
      }

    }

    // Indicates if the ststart is correct
    if(strcasecmp(lineChecker->name, "DTSTART") == 0 && scope == 2)
    {
      dtStart++;
      dateTimeLen = strlen(lineChecker->description);

      //printf("%s has length %d\n",lineChecker->description,dateTimeLen);

      if(strcmp(lineChecker->description, "") == 0)
      {
        free(lineChecker);
        return INV_DT;
      }
      if(dateTimeLen-1 < 14)
      {
        free(lineChecker);
        return INV_DT;
      }

      if(lineChecker->description[8] != 'T')
      {
        free(lineChecker);
        return INV_DT;
      }

      if(dateTimeLen-1 == 15)
      {
        if((lineChecker->description[dateTimeLen-1] == 'Z' || lineChecker->description[dateTimeLen-1] == ' '))
        {
        }
        else
        {
          free(lineChecker);
          return INV_DT;
        }
      }
    }

    //------------------------------------------------- Alarm

    // Indicates if the action is correct
    if(strcasecmp(lineChecker->name, "ACTION") == 0 && scope == 3)
    {
      action++;
      if(strcmp(lineChecker->description,"") == 0)
      {
        free(lineChecker);
        return INV_ALARM;
      }
    }

    // Indicates if the trigger is correct
    if(strcasecmp(lineChecker->name, "TRIGGER") == 0 && scope == 3)
    {
      trigger++;
      if(strcmp(lineChecker->description,"") == 0)
      {
        free(lineChecker);
        return INV_ALARM;
      }
    }

    if(strcasecmp(lineChecker->description, "") == 0 && scope == 1)
    {
      free(lineChecker);
      return INV_CAL;
    }

    if(strcasecmp(lineChecker->description, "") == 0 && scope == 2)
    {
      free(lineChecker);
      return INV_EVENT;
    }

    if(strcasecmp(lineChecker->description, "") == 0 && scope == 3)
    {
      free(lineChecker);
      return INV_ALARM;
    }

    if(strcasecmp(twoDArrayUnfoldedChecker[i], "") == 0 && scope == 1)
    {
      free(lineChecker);
      return INV_CAL;
    }

    if(strcasecmp(twoDArrayUnfoldedChecker[i], "") == 0 && scope == 2)
    {
      free(lineChecker);
      return INV_EVENT;
    }

    if(strcasecmp(twoDArrayUnfoldedChecker[i], "") == 0 && scope == 3)
    {
      free(lineChecker);
      return INV_ALARM;
    }

    free(lineChecker);
  }

  return OK;
}
