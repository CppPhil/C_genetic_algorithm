#include <stdlib.h>
#include <time.h>

#include "random.h"

void seedRandom(void)
{
  srand((unsigned int)time(NULL));
}

static int zeroOrOne(void)
{
  return rand() & 1;
}

char zeroOrOneCharacter(void)
{
  return zeroOrOne() + '0';
}

static double randomDouble(double min, double max)
{
  double scale = rand() / (double)RAND_MAX;
  return min + scale * (max - min);
}

double randomChance(void)
{
  return randomDouble(0.0, 1.0);
}
