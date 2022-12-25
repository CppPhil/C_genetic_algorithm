#pragma once
#include <stddef.h>
#include <stdbool.h>

char* chromosomeCreateRandom(size_t chromosomeLength);

void chromosomeDestroy(char* chromosome);

char* chromosomeDuplicate(const char* chromosome, size_t chromosomeLength);

void chromosomeMutate(
  char*  chromosome,
  size_t chromosomeLength,
  double probability);

bool chromosomeCrossover(
  char*  chromosome1,
  char*  chromosome2,
  size_t chromosomeLength);
