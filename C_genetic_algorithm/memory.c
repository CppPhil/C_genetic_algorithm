#include <string.h>

#include "memory.h"

char* stringCreate(size_t stringLength)
{
  char* string = NEW_ARRAY(char, stringLength + 1);

  if (string == NULL) {
    return NULL;
  }

  memset(string, 0, stringLength + 1);
  return string;
}

void stringDestroy(char* string)
{
  DELETE_ARRAY(string);
}
