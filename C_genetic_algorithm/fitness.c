#include <string.h>

#include "fitness.h"
#include "percentage.h"

char expectedChromosomeBuffer[1024 + 1];

void setExpectedChromosome(const char* expectedChromosome)
{
  strcpy(expectedChromosomeBuffer, expectedChromosome);
}

double fitness(const char* chromosome, ...)
{
  int   equalCharacters    = 0;
  int   totalCharacters    = 0;
  char* expectedChromosome = expectedChromosomeBuffer;

  for (; *chromosome != '\0' && *expectedChromosome != '\0';
       ++chromosome, ++expectedChromosome) {
    if (*chromosome == *expectedChromosome) {
      ++equalCharacters;
    }

    ++totalCharacters;
  }

  return percentageOf(equalCharacters, totalCharacters);
}
