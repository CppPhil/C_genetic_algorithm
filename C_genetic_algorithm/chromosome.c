#include <stdlib.h>
#include <string.h>

#include "chromosome.h"
#include "memory.h"
#include "random.h"

char* chromosomeCreateRandom(size_t chromosomeLength)
{
  char* chromosome = stringCreate(chromosomeLength);

  if (chromosome == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < chromosomeLength; ++i) {
    chromosome[i] = zeroOrOneCharacter();
  }

  return chromosome;
}

void chromosomeDestroy(char* chromosome)
{
  stringDestroy(chromosome);
}

char* chromosomeDuplicate(const char* chromosome, size_t chromosomeLength)
{
  char* duplicate = stringCreate(chromosomeLength);

  if (duplicate == NULL) {
    return NULL;
  }

  memcpy(duplicate, chromosome, chromosomeLength + 1);
  return duplicate;
}

static void mutateCharacter(char* character)
{
  switch (*character) {
  case '0':
    *character = '1';
    break;
  case '1':
    *character = '0';
    break;
  }
}

void chromosomeMutate(
  char*  chromosome,
  size_t chromosomeLength,
  double probability)
{
  for (size_t i = 0; i < chromosomeLength; ++i) {
    if (randomChance() < probability) {
      mutateCharacter(chromosome + i);
    }
  }
}

bool chromosomeCrossover(
  char*  chromosome1,
  char*  chromosome2,
  size_t chromosomeLength)
{
  size_t crossoverPoint       = rand() % chromosomeLength; // Index
  size_t remainderLength      = (chromosomeLength - crossoverPoint) - 1;
  char*  chromosome2Remainder = NEW_ARRAY(char, remainderLength);

  if (chromosome2Remainder == NULL) {
    return false;
  }

  memcpy(
    chromosome2Remainder, (chromosome2 + crossoverPoint) + 1, remainderLength);
  memcpy(
    (chromosome2 + crossoverPoint) + 1,
    (chromosome1 + crossoverPoint) + 1,
    remainderLength);
  memcpy(
    (chromosome1 + crossoverPoint) + 1, chromosome2Remainder, remainderLength);
  DELETE_ARRAY(chromosome2Remainder);
  return true;
}
