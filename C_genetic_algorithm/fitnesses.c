#include "fitnesses.h"
#include "memory.h"
#include "population.h"

static Fitnesses failure(void)
{
  return (Fitnesses){NULL, 0, 0.0, false};
}

Fitnesses fitnessesCreate(struct Population* population, fitness_t fitness)
{
  double* fitnesses = NEW_ARRAY(double, population->size);

  if (fitnesses == NULL) {
    return failure();
  }

  double sum = 0.0;

  for (size_t i = 0; i < population->size; ++i) {
    fitnesses[i] = fitness(population->chromosomes[i]);
    sum += fitnesses[i];
  }

  return (Fitnesses){fitnesses, population->size, sum, true};
}

void fitnessesDestroy(Fitnesses fitnesses)
{
  DELETE_ARRAY(fitnesses.data);
}
