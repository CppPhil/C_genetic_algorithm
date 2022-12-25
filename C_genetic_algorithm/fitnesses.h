#pragma once
#include "fitness.h"

struct Population;

typedef struct {
  double* data;
  size_t  size;
  double  sum;
  bool    isOk;
} Fitnesses;

Fitnesses fitnessesCreate(struct Population* population, fitness_t fitness);

void fitnessesDestroy(Fitnesses fitnesses);
