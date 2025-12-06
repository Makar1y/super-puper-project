#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module1.h"

int countBlocks(char* filename_json){
  FILE *myfile = fopen(filename_json, "r");
  if(!myfile)
    return -1;

  fseek(myfile, 0, SEEK_END);
  long size = ftell(myfile);
  rewind(myfile);

  char* text = malloc(size+1);
  fread(text, 1, size, myfile);
  text[size] = '\0';
  fclose(myfile);

  int count = 0;
  int inString = 0;

  for (int i = 0; i < size; ++i) {
    if (text[i] == '"'){
      int backslashes = 0;
      for (int j = i - 1; j >= 0 && text[j] == '\\'; j--) {
        backslashes++;
      }
      if (backslashes % 2 == 0) {
        inString = !inString;
      }
    }
    if(!inString && text[i] == '}' && text[i + 1] == ',') {
      count++;
    }
  }
  if (count > 0) count++;
  free(text);
  return count;
}

void copyHTMLBlocks(char* template_html, char* output_html, char* filename_json){
  FILE *myfile = fopen(template_html, "r");
  if(!myfile)
    return;
  
  fseek(myfile, 0, SEEK_END);
  long size = ftell(myfile);
  rewind(myfile);

  char* text = malloc(size+1);
  fread(text, 1, size, myfile);
  text[size] = '\0';
  fclose(myfile);

  int blocksCount = countBlocks(filename_json);

  char* newHTML = calloc(size * blocksCount + size, 1);

  int i;
  for (i = 0; i < size - 1; ++i) {
    if (text[i] == '{' && text[i+1] == '?') {
      i+=2;
      break;
    }
    *(newHTML + i) = text[i];
  }
  newHTML[i-1] = '\0';
  char* copySegmend = malloc(size);
  int segmentSize = 0;
  for (;i < size - 1; ++i) {
    if (text[i] == '?' && text[i + 1] == '}') {
      break;
    }
    *(copySegmend + segmentSize) = text[i];
    segmentSize++;
  }
  copySegmend[segmentSize + 1] = '\0';
  segmentSize += 1;


  for (int j = 0; j < blocksCount; j++){
    strcat(newHTML, copySegmend);
  }
  i += 2;
  int add = strlen(newHTML)  - i;
  for (; i < size; ++i) {
    *(newHTML + (i + add)) = text[i];
  }
  myfile = fopen(output_html, "w");
  fprintf(myfile, "%s\n", newHTML);
  fclose(myfile);
  free(newHTML);
  free(copySegmend);
};


void copyHTMLThatAreArrays(char* template_html, char* output_html, char* filename_json){
  FILE *myfile = fopen(template_html, "r");
  if(!myfile)
    return;
  
  fseek(myfile, 0, SEEK_END);
  long size = ftell(myfile);
  rewind(myfile);

  char* text = malloc(size+1);
  fread(text, 1, size, myfile);
  text[size] = '\0';
  fclose(myfile);

  char* newHTML = calloc(size * 2, 1);

  int i;
  int newHTMLLen = 0;
  char arrays[30][100];
  char elements[30][100];
  int elementCount = 0;
  int elementOccuranceCount[30];
  int arraysCount = 0;
  int arraysOccuranceCount[30];
  for (i = 0; i < size;  ++i) {
    if (text[i] == '{' && text[i+1] == '%') {
      newHTML[newHTMLLen] = '\0';
      i++;
      while (text[i] != '{'){
        i++;
      };
      i++;
      char* array = malloc(1000);
      int arraySize = 0;
      while (text[i] != '}') {
        *(array + arraySize) = text[i];
        arraySize++;
        i++;
      }
      i++;
      *(array + arraySize) = '\0';
      int occurance = 1;
      int found = 0;
      for (int j = 0; j < arraysCount; j++) {
        if (!strcmp(array, arrays[j])){
          arraysOccuranceCount[j]++;
          occurance = arraysOccuranceCount[j];
          found = 1;
          break;
        }
      }
      
      if (!found) {
        strcpy(arrays[arraysCount], array);
        arraysOccuranceCount[arraysCount] = 1;
        arraysCount++;
      }
      char *copySegmend1 = malloc(size), *copySegmend2 = malloc(size);
      int segmentSize1 = 0, segmentSize2 = 0;

      while (text[i] != '{' || text[i+1] != '{'){
        *(copySegmend1 + segmentSize1) = text[i];
        segmentSize1++;
        i++;
      }
      i+=4;
      copySegmend1[segmentSize1] = '\0';
      while (text[i] != '%' || text[i+1] != '}') {
        *(copySegmend2 + segmentSize2) = text[i];
        segmentSize2++;
        i++;
      }
      copySegmend2[segmentSize2] = '\0';
      FILE* json = fopen(filename_json, "r");
      fseek(json, 0, SEEK_END);
      long size2 = ftell(json);
      rewind(json);

      char* text2 = malloc(size2+1);
      fread(text2, 1, size2, json);
      text2[size2] = '\0';
      fclose(json);
      char* tmp = text2;
      int count = 0;
      while ((tmp = strstr(tmp, array)) != NULL) {
        count++;
        if (count == occurance) {
          break;
        }
        tmp += strlen(array);
      }
      int j = 0;
      while (*(tmp + j) != '[') {
        j++;
      };
      int doubleQuoteCount = 0;
      while (*(tmp + j) != ']') {
        if (*(tmp + j) == '"') {
          doubleQuoteCount++;
        }
        j++;
      }
      int copyCount = doubleQuoteCount/2;
      j = 0;
      while (*(tmp + j) != '[') {
        j++;
      }
      
      char arrayElements[30][100];
      for (int k = 0; k < copyCount; k++) {
        int elementSize = 0;
        while (*(tmp + j) != '"') {
          j++;
          
        }
        j++;
        while (*(tmp + j) != '"'){
          arrayElements[k][elementSize] = *(tmp + j);
          elementSize++;
          j++;
        }
        j++;
        arrayElements[k][elementSize] = '\0';
      }
     

      for (int k = 0; k < copyCount; k++) {
        strcpy(newHTML + newHTMLLen, copySegmend1);
        newHTMLLen += strlen(copySegmend1);
        strcpy(newHTML + newHTMLLen, arrayElements[k]);
        newHTMLLen += strlen(arrayElements[k]);
        strcpy(newHTML + newHTMLLen, copySegmend2);
        newHTMLLen += strlen(copySegmend2);
      }
      i++;
      free(text2);
      free(copySegmend1);
      free(copySegmend2);
      free(array);
      continue;
    }

    if (text[i] == '{' && text[i+1] == '{') {
      newHTML[newHTMLLen] = '\0';
      i++;
      while (text[i] != '{'){
        i++;
      };
      i++;
      char* element = malloc(1000);
      element[0] = '"';
      int elementSize = 1;
      int underscore = 0;
      while (text[i] != '}') {
        if(text[i] == '_' && underscore == 0){
          underscore = 1;
          i++;
          continue;
        }
        *(element + elementSize) = text[i];
        elementSize++;
        i++;
      }
      i++;
      *(element + elementSize) = '"';
      *(element + elementSize + 1) = '\0';
      int occurance = 1;
      int found = 0;
      for (int j = 0; j < elementCount; j++) {
        if (!strcmp(element, elements[j])){
          if(!underscore)
            elementOccuranceCount[j]++;
          occurance = elementOccuranceCount[j];
          found = 1;
          break;
        }
      }
      
      if (!found) {
        strcpy(elements[elementCount], element);
        elementOccuranceCount[elementCount] = 1;
        elementCount++;
      }
      char* copySegmend = malloc(size);
      int segmentSize = 0;

      
      FILE* json = fopen(filename_json, "r");
      fseek(json, 0, SEEK_END);
      long size2 = ftell(json);
      rewind(json);

      char* text2 = malloc(size2+1);
      fread(text2, 1, size2, json);
      text2[size2] = '\0';
      fclose(json);
      char* tmp = text2;
      int count = 0;
      while ((tmp = strstr(tmp, element)) != NULL) {
        count++;
        if (count == occurance) {
          break;
        }
        tmp += strlen(element);
      }
      
      int j = 0;
      while (*(tmp + j) != ' ') {
        j++;
      };
      j++;
      int quotecount = 0;
      if(*(tmp + j) == '"'){
        j++;
        while (tmp[j] != '\0') {
            if (tmp[j] == '"') {
                j++;
                break;
            }
            
              
            copySegmend[segmentSize] = tmp[j];
            segmentSize++;
            j++;
        }
      } else {
        while (tmp[j] != '\0') {
        char c = tmp[j];

        if ((c >= '0' && c <= '9') ||
            c == '-' || c == '+' ||
            c == '.' ||
            c == 'e' || c == 'E')
        {
          copySegmend[segmentSize] = c;
          segmentSize++;
          j++;
        } else {
            break;
        }
        }
      }

      copySegmend[segmentSize] = '\0';
      strcpy(newHTML + newHTMLLen, copySegmend);
      newHTMLLen += strlen(copySegmend);
      free(text2);
      free(copySegmend);
      free(element);
      continue;
    }


    if(text[i] == '\n') //stupid windows
      size--;
    newHTML[newHTMLLen] = text[i];
    newHTMLLen++;
  }
  myfile = fopen(output_html, "w");
  fprintf(myfile, "%s\n", newHTML);
  fclose(myfile);
  free(newHTML);
}
