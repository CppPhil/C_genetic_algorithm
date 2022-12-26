#pragma once
#include <stdbool.h>
#include <stddef.h>

#include "fitnesses.h"

struct Population {
  char** chromosomes;
  size_t size;
  bool   isOk;
};

typedef struct Population Population;

Population populationCreateEmpty(void);

Population populationCreateRandom(
  size_t populationSize,
  size_t chromosomeLength);

Population populationCreateNext(
  Population population,
  Fitnesses  fitnesses,
  size_t     chromosomeLength,
  double     crossoverProbability,
  double     mutationProbability);

void populationDestroy(Population population);

bool populationAppend(
  Population* population,
  char*       chromosome,
  size_t      chromosomeLength);

char* populationPickHighestFitnessChromosome(
  Population population,
  Fitnesses  fitnesses);

typedef struct {
  char* first;
  char* second;
} SelectedChromosomes;

bool selectedChromosomesOk(SelectedChromosomes s);

SelectedChromosomes populationSelect(
  Population population,
  Fitnesses  fitnesses);
