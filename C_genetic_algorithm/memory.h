#pragma once
#include <stddef.h>
#include <stdlib.h>

#define NEW_ARRAY(Type, ArraySize) (Type*)malloc((ArraySize) * sizeof(Type))
#define DELETE_ARRAY(Array) free(Array)
#define NEW(Type) NEW_ARRAY(Type, 1)
#define DELETE(Pointer) DELETE_ARRAY(Pointer)

char* stringCreate(size_t stringLength);

void stringDestroy(char* string);
