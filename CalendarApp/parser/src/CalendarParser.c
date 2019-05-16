/*
* Name: Ryan Paul
* Id: 1017834
* Date: Tuesday Feb 5th
* Assignment 1
*/

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"
#include "Helper.h"

void fileIntoString(char **pointerToString, char* fileName, int fileCount);
void dateTimeParser(DateTime **generalDateTime, char *dateAndTime);

/** Function to create a Calendar object based on the contents of an iCalendar file.
 *@pre File name cannot be an empty string or NULL.  File name must have the .ics extension.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid calendar has been created, its address was stored in the variable obj, and OK was returned
		or
		An error occurred, the calendar was not created, all temporary memory was freed, obj was set to NULL, and the
		appropriate error code was returned
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param fileName - a string containing the name of the iCalendar file
 *@param a double pointer to a Calendar struct that needs to be allocated
**/

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
    int i = 0;
    int j = 0;
    int fileCount = 0;      // Used to count the num of character in a iCalendar file
    int lineInFile = 0;   //Used to count the number of lines in a iCalendar file
    char *buffer = NULL;        // buffer is a string representation of the iCalendar file
    int endIndex = 0;
    int startIndex = 0;
    int stringSize = 0; //Size of a string in a 2D Array
    int lengthOfFileName = 0;
    int lineInFileError = 0;




    fileCount = fileSize(fileName);   // Num of chas in the File

    if(fileCount == 0)
    {
      return INV_FILE;
    }

    //printf("File count is %d\n", fileCount);

    fileIntoString(&buffer, fileName, fileCount);

    int aa = 0;
    int calendarValidator = 0;

    for(aa = 0; aa < fileCount; aa++)
    {
      if(buffer[aa] == '\r' && buffer[aa+1] == '\n')
      {
        calendarValidator = 1;
        break;
      }
    }


    if(calendarValidator == 0)
    {
      free(buffer);
      return INV_FILE;
    }


    // Chekcs if it is a .ics file gives error if fileName.foo

    lengthOfFileName = strlen(fileName);

    for(i = 0; i < lengthOfFileName; i++)
    {
      if(fileName[i] == '.')
      {
        if((fileName[i+1] != 'i') || (fileName[i+2] != 'c') || (fileName[i+3] != 's'))
        {
          return INV_FILE;
        }
      }
    }


    lineInFile = numOfLines(fileCount, buffer);
    lineInFileError = lineInFile + 10;   //more lines are malloced if there are erors in the file
    char** twoDArray = calloc(lineInFileError,sizeof(char*));   ///Malloc num of line in file before line unfolding before: lineInFile after: 14


    for(i = 0; i < fileCount; i++)
    {
        if(buffer[i] == '\r' && buffer[i+1] == '\n')
        {
          endIndex = i-1;
          stringSize = ((endIndex-startIndex)+1); //Size of string to be calloced is more by 1 just to be safe.
          twoDArray[j] = calloc(stringSize+1,sizeof(char));       // before: twoDArray[j] = calloc(stringSize,sizeof(char*)); after: twoDArray[j] = calloc(stringSize+1,sizeof(char));
          strncpy(twoDArray[j],buffer+startIndex,((endIndex-startIndex)+1));
          startIndex = i+2;
          i = i+1;      //Need to add 1 rather than 2 as the for loop increments the i counter by 1 more
          //printf("|%s|\n", twoDArray[j]);
          twoDArray[j][stringSize] = '\0';
          j++;
        }
    }


    //--------------------- Unfolded  file ---------------------


    int linesInFileUnfolded = 0;
    int totalLinesinFile = 0;
    int k;
    int l;
    int a = 0;
    char* tempString;
    int size = 0;
    int tempValid = 2;    // 2 INDICATES THAT IT IS NOT A UNFOLDED LINE 1 INDICATES ITS A FOLDED LINE

    for(k = 0; k < lineInFile; k++)
    {
      if(twoDArray[k][0] == '\t' || twoDArray[k][0] == ' ')
      {
        linesInFileUnfolded++;
      }
    }

    totalLinesinFile = lineInFile - linesInFileUnfolded;

    //printf("total Lines in file is %d unfolded is %d\n",lineInFile, totalLinesinFile);

    char** twoDArrayUnfolded = malloc(sizeof(char*)*totalLinesinFile);

    for(l = 0; l < lineInFile; l++)
    {
      //printf("value of a  = %d\n", a);
      size = strlen(twoDArray[l]);
      tempString = calloc(size+5,sizeof(char));
      strcpy(tempString, twoDArray[l]);
      //printf("Temp string is = |%s|\n", tempString);

      if((l+1 <= lineInFile-1) && (twoDArray[l+1][0] == '\t' || twoDArray[l+1][0] == ' '))
      {
        while((l+1 <= lineInFile-1) && (twoDArray[l+1][0] == '\t' || twoDArray[l+1][0] == ' '))
        {
          //printf("Reached ------\n");
          if(l+1 <= lineInFile-1)
          {
            //printf("twoDArray[l+1] = |%s|\n", twoDArray[l+1]);
            size = strlen(twoDArray[l+1]);
            tempString = realloc(tempString, strlen(tempString)+size);
            strncat(tempString,twoDArray[l+1]+1,size);
            //printf("temp String concat is = |%s|\n",tempString);
            tempValid = 1;
            l++;
          }
        }
      }
      if((tempValid == 1 || tempValid == 2))
      {
        if(tempValid == 1)
        {
          //printf("Reached 2\n");
          twoDArrayUnfolded[a] = calloc(strlen(tempString)+1,sizeof(char));
          strcpy(twoDArrayUnfolded[a], tempString);
          //printf("|%s|\n",twoDArrayUnfolded[a]);
          a++;
          tempValid = 2;
        }
        else
        {
          //printf("Reached 1\n");
          twoDArrayUnfolded[a] = calloc(strlen(twoDArray[l])+1, sizeof(char));
          strcpy(twoDArrayUnfolded[a], tempString);
          //printf("twoDArray[l] = |%s|\n", twoDArray[l]);
          //printf("|%s|\n",twoDArrayUnfolded[a]);
          a++;
          tempValid = 2;
        }
      }

      //printf("l = %d lineInFile = %d a = %d\n", l, lineInFile, a);

      free(tempString);
      tempString = NULL;
    }


    //------------------------- Validating Ical -------------------------

    int validatorCheck = 0;

    validatorCheck = invalidCalChecker(totalLinesinFile, twoDArrayUnfolded);

    //printf("Error code is %d\n", validatorCheck);

    if(validatorCheck == 0)
    {
      //Do nothing
    }
    else if(validatorCheck == 1)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_FILE;
    }
    else if(validatorCheck == 2)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_CAL;
    }
    else if(validatorCheck == 3)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_VER;
    }
    else if(validatorCheck == 4)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return DUP_VER;
    }
    else if(validatorCheck == 5)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_PRODID;
    }
    else if(validatorCheck == 6)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return DUP_PRODID;
    }
    else if(validatorCheck == 7)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_EVENT;
    }
    else if(validatorCheck == 8)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_DT;
    }
    else if(validatorCheck == 9)
    {
      free(buffer);
      for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
      {
        free(twoDArray[i]);
      }
      free(twoDArray);

     for(int q = 0; q < totalLinesinFile; q++)
      {
        free(twoDArrayUnfolded[q]);
      }
      free(twoDArrayUnfolded);

      return INV_ALARM;
    }


    *obj = malloc(sizeof(Calendar));            // The Calendar Object

    /*if((*obj) == NULL)
    {
      return OTHER_ERROR;
    }*/

    (*obj)->properties = initializeList(printProperty,deleteProperty,compareProperties);
    (*obj)->events = initializeList(printEvent,deleteEvent,compareEvents);


    //------------------------- Parsed Ical -------------------------

    for(i = 0; i < totalLinesinFile; i++)
    {
      contentLineVales *line = calloc(1,sizeof(contentLineVales));
      contentLine(twoDArrayUnfolded[i], &line);

      if((strcasecmp(line->name, "BEGIN") == 0) && (strcasecmp(line->description, "VCALENDAR") == 0))
      {
        //printf("Parsing Calendar\n");
        i = parseCal(totalLinesinFile, i, twoDArrayUnfolded, obj);
        //printf("i value in main is %d\n", i);
      }

      free(line);
    }


    free(buffer);
    for(i = 0; i < lineInFileError; i++)     // before: lineInFile after: numOfUnfoldedLines
    {
      free(twoDArray[i]);
    }
    free(twoDArray);

   for(int q = 0; q < totalLinesinFile; q++)
    {
      free(twoDArrayUnfolded[q]);
    }
    free(twoDArrayUnfolded);

    return OK;
}


/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/

void deleteCalendar(Calendar* obj)
{
  freeList(obj->properties);
  freeList(obj->events);
  free(obj);
}


/** Function to create a string representation of a Calendar object.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a string representing the Calndar contents has been created
 *@return a string contaning a humanly readable representation of a Calendar object
 *@param obj - a pointer to a Calendar struct
**/
char* printCalendar(const Calendar* obj)
{
  char *printString;
  char *printCalProperties;
  char *printCalEvents;
  char tempVal[100];
  int len = 0;

  if(obj == NULL)
  {
    return NULL;
  }

  len = strlen(obj->prodID)+10000;

  printString = malloc(sizeof(char)*len);

  sprintf(printString,"BEGIN:VCALENDAR");

  //Calendar Properties

  printCalProperties = toString(obj->properties);     //to string adds a new line to the values

  len = len+strlen(printCalProperties);

  printString = realloc(printString,len);

  strcat(printString,printCalProperties);
  free(printCalProperties);

  strcat(printString,"\r\n");


  sprintf(tempVal,"VERSION:%.2f\r\nPRODID:%s",obj->version,obj->prodID);
  strcat(printString,tempVal);

  //Calendar Events

  printCalEvents = toString(obj->events);

  len = len+(strlen(printCalEvents));

  printString = realloc(printString,len);

  strcat(printString,printCalEvents);
  free(printCalEvents);

  strcat(printString,"\r\n");

  // End VCALENDAR

  len = len+200;

  printString = realloc(printString,len);

  strcat(printString,"END:VCALENDAR\r\n");

  return printString;
}


/** Function to "convert" the ICalErrorCode into a humanly redabale string.
 *@return a string contaning a humanly readable representation of the error code by indexing into
          the descr array using rhe error code enum value as an index
 *@param err - an error code
**/
char* printError(ICalErrorCode err)
{
  char* errorString = malloc(sizeof(char)*20);

  if(err == 0)
  {
    strcpy(errorString,"OK");
    return errorString;
  }
  else if(err == 1)
  {
    strcpy(errorString,"INV_FILE");
    return errorString;
  }
  else if(err == 2)
  {
    strcpy(errorString,"INV_CAL");
    return errorString;
  }
  else if(err == 3)
  {
    strcpy(errorString,"INV_VER");
    return errorString;
  }
  else if(err == 4)
  {
    strcpy(errorString,"DUP_VER");
    return errorString;
  }
  else if(err == 5)
  {
    strcpy(errorString,"INV_PRODID");
    return errorString;
  }
  else if(err == 6)
  {
    strcpy(errorString,"DUP_PRODID");
    return errorString;
  }
  else if(err == 7)
  {
    strcpy(errorString,"INV_EVENT");
    return errorString;
  }
  else if(err == 8)
  {
    strcpy(errorString,"INV_DT");
    return errorString;
  }
  else if(err == 9)
  {
    strcpy(errorString,"INV_ALARM");
    return errorString;
  }
  else if(err == 10)
  {
    strcpy(errorString,"WRITE_ERROR");
    return errorString;
  }
  else
  {
    strcpy(errorString,"OTHER_ERROR");
    return errorString;
  }
}


/** Function to writing a Calendar object into a file in iCalendar format.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a file representing the
        Calendar contents in iCalendar format has been created
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
{
  if(obj ==  NULL)
  {
    return WRITE_ERROR;
  }

  // char *ics;
  // ics = strrchr(fileName,'.');
  //
  // if(ics == NULL)
  // {
  //   return WRITE_ERROR;
  // }
  // else if((strcmp(ics,".ics")) != 0)
  // {
  //   return WRITE_ERROR;
  // }

  char*  buffer;
  buffer = printCalendar(obj);

  FILE *fp;

  fp = fopen(fileName,"w");

  if(fp == NULL)
  {
    return WRITE_ERROR;
  }

  fprintf(fp,"%s",buffer);

  fclose(fp);

  free(buffer);
  return OK;
}


/** Function to validating an existing a Calendar object
 *@pre Calendar object exists and is not null
 *@post Calendar has not been modified in any way
 *@return the error code indicating success or the error encountered when validating the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode validateCalendar(const Calendar* obj)
{
  if(obj == NULL)
  {
    return INV_CAL;
  }

  // Cal Properties

  int calScale = 0;
  int method = 0;
  //int numOfEventsCounter = 0;

  // Calendar Validator

  if(obj->version == 0)
  {
    return INV_CAL;
  }

  //printf("Strlen of prodid = %lu", strlen(obj->prodID));


  if(strcmp(obj->prodID,"") == 0 || strlen(obj->prodID) > 1000)
  {
    return INV_CAL;
  }

  if(obj->properties == NULL)
  {
    return INV_CAL;
  }

  ListIterator iterCalProp = createIterator(obj->properties);



	Property* elem;

	while((elem = nextElement(&iterCalProp)) != NULL)
  {
    if(strlen(elem->propName) > 200 || elem->propDescr == NULL)
    {
      return INV_CAL;
    }

    if(strcasecmp(elem->propName,"METHOD") == 0)
    {
      method++;
    }
    else if(strcasecmp(elem->propName,"CALSCALE") == 0)
    {
      if(strcmp(elem->propDescr,"") == 0)
      {
        return INV_CAL;
      }
      calScale++;
    }
    else
    {
      //printf("This is an invad property |%s:%s|\n",elem->propName,elem->propDescr);
      return INV_CAL;
    }
  }

  if(method > 1)
  {
    //printf("Method not found\n");
    return INV_CAL;
  }

  //printf("value of calScale should be 1 (%d)\n", calScale);

  if(calScale > 1)
  {
    //printf("CALSCALE occures multiple times\n");
    return INV_CAL;
  }


  // checks if there are events in the obj ----------------------------------------

  // ListIterator iterCalEventsTemp = createIterator(obj->events);
  //
	// Event* eventTemp;
  //
	// while((eventTemp = nextElement(&iterCalEventsTemp)) != NULL)
  // {
  //   numOfEventsCounter++;
  // }
  //
  // if(numOfEventsCounter == 0)
  // {
  //   printf("no Events\n");
  //   return INV_CAL;
  // }



  // ---------------------------------------------------- Event Validator

  int classCount = 0;
  int descrCount = 0;
  int geoCount = 0;
  int locationCount = 0;
  int prorityCount = 0;
  int statusCount = 0;
  int summaryCount = 0;
  int transpCount = 0;
  int organizerCounter = 0;
  int recurrenceID = 0;
  int urlCounter = 0;
  int lastModified = 0;
  int createdProp = 0;
  int sequenceProp = 0;
  int durationCount = 0;
  int durationTrue = 0;
  int dtEndCount = 0;
  int dtEndTrue = 0;
  int priorityCount = 0;

  if(obj->events == NULL)
  {
    return INV_CAL;     // should this be inv_cal or event =============================== ??????????
  }

  ListIterator iterEvents = createIterator(obj->events);



  Event* elemEvent;

  while((elemEvent = nextElement(&iterEvents)) != NULL)
  {

    classCount = 0;
    descrCount = 0;
    geoCount = 0;
    locationCount = 0;
    prorityCount = 0;
    statusCount = 0;
    summaryCount = 0;
    transpCount = 0;
    organizerCounter = 0;
    recurrenceID = 0;
    urlCounter = 0;
    lastModified = 0;
    createdProp = 0;
    sequenceProp = 0;
    durationCount = 0;
    durationTrue = 0;
    dtEndCount = 0;
    dtEndTrue = 0;
    priorityCount = 0;


    if((strcmp(elemEvent->UID, "")) == 0 || strlen(elemEvent->UID) > 1000)
    {
      return INV_EVENT;
    }

    if((strcmp(elemEvent->creationDateTime.time, "")) == 0 || (strcmp(elemEvent->creationDateTime.date, "") == 0))    // strcmp to check if they are empty
    {
      return INV_EVENT;
      //dtStampCount++;
    }

    //printf("lenght of date creation = |%s|, time = |%s|\n", elemEvent->creationDateTime.date, elemEvent->creationDateTime.time);

    if(strlen(elemEvent->creationDateTime.date) != 8 || strlen(elemEvent->creationDateTime.time) != 6)
    {
      //printf("too short !!!!lenght of date creation = %s, time = %s\n", elemEvent->creationDateTime.date, elemEvent->creationDateTime.time);
      return INV_EVENT;
    }
// if(elemEvent->startDateTime.time == NULL || elemEvent->startDateTime.date == NULL)

    if((strcmp(elemEvent->startDateTime.time, "")) == 0 || (strcmp(elemEvent->startDateTime.date, "") == 0))           // strcmp to check if they are empty
    {
      return INV_EVENT;
      //dtStartCount++;
    }

    //printf("lenght of date start = |%s|, time = |%s|\n",elemEvent->startDateTime.date, elemEvent->startDateTime.time);

    if(strlen(elemEvent->startDateTime.date) != 8 || strlen(elemEvent->startDateTime.time) != 6)
    {
      //printf("too short !!!! lenght of date start = %s, time = %s\n",elemEvent->startDateTime.date, elemEvent->startDateTime.time);
      return INV_EVENT;
    }

    if(elemEvent->properties == NULL)
    {
      return INV_EVENT;     // should this be inv_cal or event =============================== ??????????
    }


    ListIterator iterEventProp = createIterator(elemEvent->properties);

  	Property* elemTwo;

  	while((elemTwo = nextElement(&iterEventProp)) != NULL)
    {
      if(strlen(elemTwo->propName) > 200 || elemTwo->propDescr == NULL)
      {
        return INV_EVENT;
      }

      if(strcasecmp(elemTwo->propName,"ATTACH") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr);   // can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"CATEGORIES") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr);   // can occure multipe times
      }
      else if(strcasecmp(elemTwo->propName,"CLASS") == 0)
      {
        // Can only occure once
        classCount++;
        if(classCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"COMMENT") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr); // can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"DESCRIPTION") == 0)
      {
        // Can only occure once
        descrCount++;
        if(descrCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"GEO") == 0)
      {
        // Can only occure once
        geoCount++;
        if(geoCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"LOCATION") == 0)
      {
        // Can only occure once
        locationCount++;
        if(locationCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"PRIORITY") == 0)
      {
        // Can only occure once
        priorityCount++;
        if(prorityCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"RESOURCES") == 0)
      {
        // can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"STATUS") == 0)
      {
        // Can only occure once
        statusCount++;
        if(statusCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"SUMMARY") == 0)
      {
        // Can only occure once
        summaryCount++;
        if(summaryCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"TRANSP") == 0)
      {
        // Can only occure once
        transpCount++;
        if(transpCount == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"ATTENDEE") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr); // can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"CONTACT") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr); // can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"ORGANIZER") == 0)
      {
        // Can only occure once
        organizerCounter++;
        if(organizerCounter == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"RECURRENCE-ID") == 0)
      {
        //Can only occure once
        recurrenceID++;
        if(recurrenceID == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"RELATED-TO") == 0)
      {
        // can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"URL") == 0)
      {
        // Can occure only once
        urlCounter++;
        if(urlCounter == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"LAST-MODIFIED") == 0)
      {
        // Can only occure once
        lastModified++;
        if(lastModified == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"CREATED") == 0)
      {
        // cAN ONLY OCCURE once
        createdProp++;
        if(createdProp == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"SEQUENCE") == 0)
      {
        sequenceProp++;
        if(sequenceProp == 2)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"EXDATE") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr); // Can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"RDATE") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr); // Can occure multiple times
      }
      else if(strcasecmp(elemTwo->propName,"RRULE") == 0)
      {
        //printf("%s:%s\n",elemTwo->propName,elemTwo->propDescr);     // can occure more than once
      }
      else if(strcasecmp(elemTwo->propName,"DTEND") == 0)
      {
        dtEndCount++;
        dtEndTrue = 1;

        if(durationTrue == 1)
        {
          return INV_EVENT;
        }
      }
      else if(strcasecmp(elemTwo->propName,"DURATION") == 0)
      {
        durationCount++;
        durationTrue = 1;

        if(dtEndTrue == 1)
        {
          return INV_EVENT;
        }
      }
      else
      {
        return INV_EVENT;
      }
    }

    // if(durationCount == 0 && dtEndCount == 0)
    // {
    //   return INV_EVENT;
    // }

    int alarmDurationCount = 0;
    int alarmRepetCount = 0;
    int alarmAttachCount = 0;
    int alarmTrigCount = 0;

    if(elemEvent->alarms == NULL)
    {
      return INV_EVENT;
    }


    ListIterator iterAlarm = createIterator(elemEvent->alarms);

    Alarm* elemAlarm;

    while((elemAlarm = nextElement(&iterAlarm)) != NULL)
    {
      alarmDurationCount = 0;
      alarmRepetCount = 0;
      alarmAttachCount = 0;

      if((strcmp(elemAlarm->action,"")) == 0 || strlen(elemAlarm->action) > 200)
      {
        return INV_ALARM;
      }

      if(elemAlarm->trigger == NULL)
      {
        return INV_ALARM;
      }

      if(elemAlarm->properties == NULL)
      {
        return INV_ALARM;
      }

      ListIterator iterAlarmProperty = createIterator(elemAlarm->properties);

      Property* elemAlarmProperty;

      while((elemAlarmProperty = nextElement(&iterAlarmProperty)) != NULL)
      {
        if(strlen(elemAlarmProperty->propName) > 200 || elemAlarmProperty->propDescr == NULL)
        {
          return INV_ALARM;
        }

        if(strcasecmp(elemAlarmProperty->propName,"ATTACH") == 0)
        {
          alarmAttachCount++;
          if(alarmAttachCount == 2)
          {
            return INV_ALARM;
          }
        }
        else if(strcasecmp(elemAlarmProperty->propName,"DURATION") == 0)
        {
          alarmDurationCount++;
          if(alarmDurationCount == 2)
          {
            return INV_ALARM;
          }
        }
        else if(strcasecmp(elemAlarmProperty->propName,"REPEAT") == 0)
        {
          alarmRepetCount++;
          if(alarmRepetCount == 2)
          {
            return INV_ALARM;
          }
        }
        else
        {
          //printf("ALARM PROP: %s\n", elemAlarmProperty->propName);
          return INV_ALARM;
        }
      }

      if (alarmTrigCount > 0){
        return INV_ALARM;
      }

      if(alarmRepetCount == 1 && alarmDurationCount == 0)
      {
        return INV_ALARM;
      }
      else if(alarmRepetCount == 0 && alarmDurationCount == 1)
      {
        return INV_ALARM;
      }

    }

  }


  return OK;
}

void deleteEvent(void* toBeDeleted)
{
  Event *tempEvent = (Event*)toBeDeleted;

  if(tempEvent == NULL)
  {
    //ERROR
  }
  freeList(tempEvent->properties);
  freeList(tempEvent->alarms);
  free(tempEvent);
}

int compareEvents(const void* first, const void* second)
{
  Event *firstEvent;
  Event *secondEvent;

  if(first == NULL || second == NULL)
  {
    return 0;
  }

  firstEvent = (Event*)first;
  secondEvent = (Event*)second;

  return strcmp((char*)firstEvent->UID, (char*)secondEvent->UID);
}

char* printEvent(void* toBePrinted)
{
  if(toBePrinted == NULL)
  {
    return NULL;
  }

  Event *tempEventVal;
  char *printEventValue;
  char *printEventProperties;
  char *printEventAlarms;
  char *printEventCDate;
  char *printEventSDate;
  char tempVal[200];
  int len = 0;

  tempEventVal = (Event*)toBePrinted;

  len = strlen(tempEventVal->UID)+1000;

  printEventValue = malloc(sizeof(char)*len);
  sprintf(printEventValue,"BEGIN:VEVENT\r\nUID:%s",tempEventVal->UID);


  //Event Properties

  printEventProperties = toString(tempEventVal->properties);

  len = len+strlen(printEventProperties);

  printEventValue = realloc(printEventValue,len);

  strcat(printEventValue, printEventProperties);
  free(printEventProperties);

  strcat(printEventValue,"\r\n");


  //Event creation Dates

  printEventSDate = printDate(&(tempEventVal->creationDateTime));

  len = len+strlen(printEventSDate)+100;

  printEventValue = realloc(printEventValue,len);

  strcat(printEventValue,"DTSTAMP:");
  strcat(printEventValue,printEventSDate);
  free(printEventSDate);

  strcat(printEventValue,"\r\n");

  //Event start Datetiem


  printEventCDate =  printDate(&(tempEventVal->startDateTime));

  len = len+strlen(printEventCDate)+100;

  printEventValue = realloc(printEventValue,len);

  strcat(printEventValue,"DTSTART:");
  strcat(printEventValue,printEventCDate);
  free(printEventCDate);


  strcat(printEventValue,"\r\n");

  sprintf(tempVal,"UID:%s",tempEventVal->UID);
  strcat(printEventValue,tempVal);

  //Event Alarms

  printEventAlarms = toString(tempEventVal->alarms);

  len = len+strlen(printEventAlarms);

  printEventValue = realloc(printEventValue,len);

  strcat(printEventValue, printEventAlarms);
  free(printEventAlarms);

  strcat(printEventValue,"\r\n");


  //End event

  len = len+20;

  printEventValue = realloc(printEventValue,len);

  strcat(printEventValue, "END:VEVENT");

  return printEventValue;
}


//***********************


void deleteAlarm(void* toBeDeleted)
{
  Alarm *tempAlarm = (Alarm*)toBeDeleted;

  if(tempAlarm == NULL)
  {
    //ERROR
  }
  freeList(tempAlarm->properties);
  free(tempAlarm->trigger);
  free(tempAlarm);
}

int compareAlarms(const void* first, const void* second)
{
  Alarm *firstAlarm;
  Alarm *secondAlarm;

  if (first == NULL || second == NULL){
		return 0;
	}

  firstAlarm = (Alarm*)first;
  secondAlarm = (Alarm*)second;

  return (strcmp((char*)firstAlarm->action, (char*)secondAlarm->action)) && (strcmp((char*)firstAlarm->trigger, (char*)secondAlarm->trigger));
}

char* printAlarm(void* toBePrinted)
{
  if(toBePrinted == NULL)
  {
    return NULL;
  }
  Alarm *tempAlarm;
  char *printAlarmValues;
  char *printAlarmProperties;
  int len = 0;

  tempAlarm = (Alarm*)toBePrinted;

  len = strlen(tempAlarm->action)+strlen(tempAlarm->trigger);

  printAlarmValues = malloc(sizeof(char)*len+100);

  sprintf(printAlarmValues,"BEGIN:VALARM\r\nACTION:%s\r\nTRIGGER:%s",tempAlarm->action,tempAlarm->trigger);


  // Alarm properties

  printAlarmProperties = toString(tempAlarm->properties);

  len = len+strlen(printAlarmProperties)+10000;

  printAlarmValues = realloc(printAlarmValues,len);

  strcat(printAlarmValues,printAlarmProperties);
  free(printAlarmProperties);

  strcat(printAlarmValues,"\r\n");

  len = len+20;

  printAlarmValues = realloc(printAlarmValues,len);

  strcat(printAlarmValues,"END:VALARM");

  return printAlarmValues;
}


//***********************


void deleteProperty(void* toBeDeleted)
{
  Property *tempProperty = (Property*)toBeDeleted;

  if(tempProperty == NULL)
  {
    //ERROR
  }
  free(tempProperty);
}

int compareProperties(const void* first, const void* second)
{
  Property *firstProperty;
  Property *secondProperty;

  if (first == NULL || second == NULL){
		return 0;
	}

  firstProperty = (Property*)first;
  secondProperty = (Property*)second;

  return strcmp((char*)firstProperty->propName, (char*)secondProperty->propName);
}

char* printProperty(void* toBePrinted)
{
  if(toBePrinted == NULL)
  {
    return NULL;
  }

  Property *tempProp;
  char *printPropertyValue;
  int len = 0;

  tempProp = (Property*)toBePrinted;

  len = strlen(tempProp->propName)+strlen(tempProp->propDescr)+100;

  printPropertyValue = malloc(sizeof(char)*len);

  sprintf(printPropertyValue,"%s:%s",tempProp->propName,tempProp->propDescr);


  return printPropertyValue;
}


//***********************



void deleteDate(void* toBeDeleted)
{
  DateTime* tempDate = (DateTime*)toBeDeleted;

  if(tempDate == NULL)
  {
    //ERROR
  }
  free(tempDate);
}

int compareDates(const void* first, const void* second)
{
  DateTime* firstDate;
	DateTime* secondDate;

	if (first == NULL || second == NULL){
		return 0;
	}

	firstDate = (DateTime*)first;
	secondDate = (DateTime*)second;

	return strcmp((char*)firstDate->date, (char*)secondDate->date);
}

char* printDate(void* toBePrinted)
{
  if(toBePrinted == NULL)
  {
    return NULL;
  }

  DateTime *tempDate;
  char *printDateTime;
  int len = 0;

  tempDate = (DateTime*)toBePrinted;

  len = strlen(tempDate->date)+strlen(tempDate->time)+100;

  printDateTime = malloc(sizeof(char)*len);

  if(tempDate->UTC == true)
  {
    sprintf(printDateTime,"%sT%sZ",tempDate->date,tempDate->time);
  }
  else
  {
    sprintf(printDateTime,"%sT%s",tempDate->date,tempDate->time);
  }

  return printDateTime;
}


char* dtToJSON(DateTime prop)
{
  char *tempDateTime = calloc(100,sizeof(char));

  if(prop.UTC == true)
  {
    sprintf(tempDateTime,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":true}",prop.date,prop.time);
  }
  else
  {
    sprintf(tempDateTime,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":false}",prop.date,prop.time);
  }

  // printf("=====\"\"=====\n");
  // strcpy(tempDateTime,"{\"date\":");
  // strcat(tempDateTime,prop->date);
  // strcat()
  // {"date":"date val","time":"time val","isUTC":utcVal}

  return tempDateTime;
}

char* eventToJSON(const Event* event)
{
  char *tempEvent = calloc(100000,sizeof(char));

  if(event == NULL)
  {
    strcpy(tempEvent,"{}");
    return tempEvent;
  }

  char *tempDateTime;
  int propCounter = 0;
  int alarmCounter = 0;
  char tempSummary[1000000] = "";

  tempDateTime = dtToJSON(event->startDateTime);    // json of startDateTime

  ListIterator iterEventProps = createIterator(event->properties);

	Property* elem;

	while((elem = nextElement(&iterEventProps)) != NULL)
  {
    if(strcasecmp(elem->propName,"SUMMARY") == 0)
    {
      strcpy(tempSummary,elem->propDescr);
    }

    propCounter++;
  }

  ListIterator iterEventAlarm = createIterator(event->alarms);

	Alarm* alarm;

	while((alarm = nextElement(&iterEventAlarm)) != NULL)
  {
    alarmCounter++;
  }

  propCounter = propCounter + 3; // Since there are 3 required properties, for the vevent to be valid

  if(tempSummary == NULL)
  {
    sprintf(tempEvent,"{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"\"}",tempDateTime,propCounter,alarmCounter);
  }
  else
  {
    sprintf(tempEvent,"{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"%s\"}",tempDateTime,propCounter,alarmCounter,tempSummary);
  }

	//printf("********* %s\n", tempEvent);

  free(tempDateTime);
  return tempEvent;
}

char* eventListToJSON(const List* eventList)
{
  char *tempEList = calloc(1000,sizeof(char));
  char *tempE;
  int checker = 0;
  int len = 1000;

  if(eventList == NULL)
  {
    strcpy(tempEList,"[]");
    return tempEList;
  }
  // const issue
  List *events = (List*)eventList;

  ListIterator iterEvents = createIterator(events);

	Event* event;

  strcpy(tempEList,"[");

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




  return tempEList;
}

char* calendarToJSON(const Calendar* cal)
{
  char *tempCal = calloc(100000,sizeof(char));
  int calPropCount = 0;
  int calEventCount = 0;

  if(cal == NULL)
  {
    strcpy(tempCal,"{}");
    return tempCal;
  }


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

  sprintf(tempCal,"{\"version\":%.0f,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}",cal->version,cal->prodID,calPropCount,calEventCount);
  // cal->Version always be 2 ??

  return tempCal;
}

Calendar* JSONtoCalendar(const char* str)
{
  if(str == NULL)
  {
    return NULL;
  }

  Calendar *tempCal = calloc(1,sizeof(Calendar));
  int length = strlen(str);
  int i = 0;
  int j = 0;
  int startIndex = 0;
  int endIndex = 0;
  float tempVers = 0.0;
  int prodidIndex = 0;
  int prodValue = 0;
  char *tempVersion = calloc(10,sizeof(char));
  char *tempProdid = calloc(10000,sizeof(char));

  strncpy(tempVersion,str+2,7);
  //printf("%s\n",tempVersion);

  tempCal->properties =  initializeList(printProperty,deleteProperty,compareProperties);
  tempCal->events = initializeList(printEvent,deleteEvent,compareEvents);

  if(strcmp(tempVersion,"version") == 0)
  {
    tempVers = atof(&(str[11]));
    //printf("%.0f\n", tempVers);
    tempCal->version = tempVers;
  }
  else
  {
    free(tempCal->properties);
    free(tempCal->events);
    free(tempCal);
    free(tempVersion);
    free(tempProdid);
    return NULL;
  }


  for(i = 0; i < length; i++)
  {
    if(str[i] == 'p')
    {
      //printf("i = %d\n", i);
      startIndex = i;
      while(str[i] != '"')
      {
        endIndex++;
        i++;
      }
      break;
    }
  }
  //printf("startindex = %d\n", startIndex);
  //printf("endIndex = %d\n", endIndex);
  strncpy(tempProdid,str+startIndex,endIndex);
  //printf("%s\n",tempProdid);
  if(strcmp(tempProdid,"prodID") == 0)
  {
    prodidIndex = startIndex+9;
    //printf("prodidIndex == |%c|\n",str[prodidIndex]);

    j = prodidIndex;
    while(str[j] != '\"')
    {
      prodValue++;
      j++;
    }

    strncpy(tempCal->prodID,str+prodidIndex,prodValue);
    //printf("prodid val = |%s|\n", tempCal->prodID);
  }
  else
  {
    free(tempCal->properties);
    free(tempCal->events);
    free(tempCal);
    free(tempVersion);
    free(tempProdid);
    return NULL;
  }


  //{"version":2,"prodID":"-//hacksw/handcal//NONSGML v1.0//EN"}
  free(tempProdid);
  free(tempVersion);
  return tempCal;
}

Event* JSONtoEvent(const char* str)
{
  if(str == NULL)
  {
    return NULL;
  }

  Event *e1 = calloc(1,sizeof(Event));


  int i = 8;    // since value will alway start at 8
  int lengthCounter = 0;
  char *tempUID = calloc(10000,sizeof(char));


  while(str[i] != '\"')
  {
    lengthCounter++;
    i++;
  }

  strncpy(tempUID,str+8,lengthCounter);

  strcpy(e1->UID,tempUID);
  e1->properties =  initializeList(printProperty,deleteProperty,compareProperties);
  e1->alarms = initializeList(printAlarm,deleteAlarm,compareAlarms);

  free(tempUID);

  return e1;
}


void addEvent(Calendar* cal, Event* toBeAdded)
{
  if(cal == NULL || toBeAdded == NULL)
  {
    // Do nothing !!!!
  }
  else
  {
    //printf("Reached!\n");
    insertBack(cal->events, toBeAdded);
  }
}
