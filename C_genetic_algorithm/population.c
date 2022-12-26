#include "population.h"
#include "chromosome.h"
#include "memory.h"
#include "random.h"
#include "thresholds.h"

static Population failure(void)
{
  return (Population){NULL, 0, false};
}

Population populationCreateEmpty(void)
{
  return (Population){NULL, 0, true};
}

Population populationCreateRandom(
  size_t populationSize,
  size_t chromosomeLength)
{
  char** chromosomes = NEW_ARRAY(char*, populationSize);

  if (chromosomes == NULL) {
    return failure();
  }

  for (size_t i = 0; i < populationSize; ++i) {
    char* newChromosome = chromosomeCreateRandom(chromosomeLength);

    if (newChromosome == NULL) {
      for (size_t j = 0; j < i; ++j) {
        chromosomeDestroy(chromosomes[j]);
      }

      DELETE_ARRAY(chromosomes);
      return failure();
    }

    chromosomes[i] = newChromosome;
  }

  return (Population){chromosomes, populationSize, true};
}

Population populationCreateNext(
  Population population,
  Fitnesses  fitnesses,
  size_t     chromosomeLength,
  double     crossoverProbability,
  double     mutationProbability)
{
  Population nextPopulation = populationCreateEmpty();

  while (nextPopulation.size < population.size) {
    SelectedChromosomes selectedChromosomes
      = populationSelect(population, fitnesses);

    if (!selectedChromosomesOk(selectedChromosomes)) {
      populationDestroy(nextPopulation);
      return failure();
    }

    char* selected1
      = chromosomeDuplicate(selectedChromosomes.first, chromosomeLength);

    if (selected1 == NULL) {
      populationDestroy(nextPopulation);
      return failure();
    }

    char* selected2
      = chromosomeDuplicate(selectedChromosomes.second, chromosomeLength);

    if (selected2 == NULL) {
      chromosomeDestroy(selected1);
      populationDestroy(nextPopulation);
      return failure();
    }

    if (randomChance() < crossoverProbability) {
      if (!chromosomeCrossover(selected1, selected2, chromosomeLength)) {
        chromosomeDestroy(selected2);
        chromosomeDestroy(selected1);
        populationDestroy(nextPopulation);
        return failure();
      }
    }

    chromosomeMutate(selected1, chromosomeLength, mutationProbability);
    chromosomeMutate(selected2, chromosomeLength, mutationProbability);

    if (!populationAppend(&nextPopulation, selected1, chromosomeLength)) {
      chromosomeDestroy(selected2);
      populationDestroy(nextPopulation);
      return failure();
    }

    if (!populationAppend(&nextPopulation, selected2, chromosomeLength)) {
      populationDestroy(nextPopulation);
      return failure();
    }
  }

  return nextPopulation;
}

void populationDestroy(Population population)
{
  for (size_t i = 0; i < population.size; ++i) {
    chromosomeDestroy(population.chromosomes[i]);
  }

  DELETE_ARRAY(population.chromosomes);
}

bool populationAppend(
  Population* population,
  char*       chromosome,
  size_t      chromosomeLength)
{
  char** newChromosomes = (char**)realloc(
    population->chromosomes, (population->size + 1) * sizeof(char*));

  if (newChromosomes == NULL) {
    chromosomeDestroy(chromosome);
    return false;
  }

  newChromosomes[population->size] = chromosome;
  population->chromosomes          = newChromosomes;
  ++(population->size);
  return true;
}

char* populationPickHighestFitnessChromosome(
  Population population,
  Fitnesses  fitnesses)
{
  double maximumFitnessValue = fitnesses.data[0];
  size_t maximumFitnessIndex = 0;

  for (size_t i = 1; i < population.size; ++i) {
    if (fitnesses.data[i] > maximumFitnessValue) {
      maximumFitnessValue = fitnesses.data[i];
      maximumFitnessIndex = i;
    }
  }

  return population.chromosomes[maximumFitnessIndex];
}

static char* populationSelectChromosome(
  Population population,
  Thresholds thresholds)
{
  double randomValue = randomChance();

  for (size_t i = 0; i < population.size; ++i) {
    double threshold  = thresholds.data[i];
    char*  chromosome = population.chromosomes[i];

    if (randomValue < threshold) {
      return chromosome;
    }
  }

  return population.chromosomes[population.size - 1];
}

bool selectedChromosomesOk(SelectedChromosomes s)
{
  return s.first != NULL && s.second != NULL;
}

static SelectedChromosomes invalidSelectedChromosomes(void)
{
  return (SelectedChromosomes){NULL, NULL};
}

SelectedChromosomes populationSelect(Population population, Fitnesses fitnesses)
{
  Thresholds thresholds = thresholdsCreate(fitnesses);

  if (!thresholds.isOk) {
    return invalidSelectedChromosomes();
  }

  SelectedChromosomes result = {
    populationSelectChromosome(population, thresholds),
    populationSelectChromosome(population, thresholds)};
  thresholdsDestroy(thresholds);
  return result;
}
