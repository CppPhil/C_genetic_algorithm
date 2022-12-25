#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chromosome.h"
#include "constants.h"
#include "population.h"

char* runN(
  fitness_t fitness,
  int       chromosomeLength,
  double    crossoverProbability,
  double    mutationProbability,
  int       iterations)
{
  Population population
    = populationCreateRandom(DEFAULT_POPULATION_SIZE, (size_t)chromosomeLength);

  if (!population.isOk) {
    return NULL;
  }

  Fitnesses fitnesses = fitnessesCreate(&population, fitness);

  if (!fitnesses.isOk) {
    populationDestroy(population);
    return NULL;
  }

  for (int i = 0; i < iterations; ++i) {
    Population nextPopulation = populationCreateNext(
      population,
      fitnesses,
      (size_t)chromosomeLength,
      crossoverProbability,
      mutationProbability);

    fitnessesDestroy(fitnesses);
    populationDestroy(population);

    if (!nextPopulation.isOk) {
      return NULL;
    }

    population = nextPopulation;
    fitnesses  = fitnessesCreate(&population, fitness);

    if (!fitnesses.isOk) {
      populationDestroy(population);
      return NULL;
    }
  }

  char* maximumFitnessChromosomeOriginal
    = populationPickHighestFitnessChromosome(population, fitnesses);
  char* maximumFitnessChromosome = chromosomeDuplicate(
    maximumFitnessChromosomeOriginal, (size_t)chromosomeLength);

  fitnessesDestroy(fitnesses);
  populationDestroy(population);

  if (maximumFitnessChromosome == NULL) {
    return NULL;
  }

  return maximumFitnessChromosome;
}

char* run(
  fitness_t fitness,
  int       chromosomeLength,
  double    crossoverProbability,
  double    mutationProbability)
{
  return runN(
    fitness,
    chromosomeLength,
    crossoverProbability,
    mutationProbability,
    DEFAULT_ITERATION_COUNT);
}

enum { CL = 35 };

int WdbkQqzHOHv19()
{
  return rand() & 1;
}

int main(void)
{
  char goal[1 + CL] = {0};

  for (size_t i = 0; i < CL; ++i) {
    goal[i] = '0' + WdbkQqzHOHv19();
  }

  setExpectedChromosome(goal);
  char* actualChromosome = run(&fitness, CL, 0.6, 0.002);
  int   strcmpResult     = strcmp(actualChromosome, goal);

  if (strcmpResult == 0) {
    printf("The strings are equal.\n");
  }
  else {
    fprintf(
      stderr,
      "ERROR!\n"
      "  Expected: %s\n"
      "  Actual  : %s\n",
      goal,
      actualChromosome);
    return -1;
  }

  chromosomeDestroy(actualChromosome);
  return 0;
}
