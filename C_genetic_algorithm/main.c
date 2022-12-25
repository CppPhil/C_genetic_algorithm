#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef double   fitness_t(const char*, ...);
extern fitness_t fitness;

static int zeroOrOne(void)
{
  return rand() & 1;
}

static char zeroOrOneCharacter(void)
{
  return zeroOrOne() + '0';
}

char* generate(size_t length)
{
  char* chromosome = malloc((length + 1) * sizeof(char));

  if (chromosome == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < length; ++i) {
    chromosome[i] = zeroOrOneCharacter();
  }

  chromosome[length] = '\0';
  return chromosome;
}

double randomDouble(double min, double max) /* -> 0.0 to 1.0 */
{
  double scale = rand() / (double)RAND_MAX;
  return min + scale * (max - min);
}

double sum(double* fitnesses, int size)
{
  double result = 0.0;

  for (int i = 0; i < size; ++i) {
    result += fitnesses[i];
  }

  return result;
}

double* thresholdsCreate(double* fitnesses, int size, double sumOfFitness)
{
  double* thresholds = malloc(size * sizeof(double));

  if (thresholds == NULL) {
    return NULL;
  }

  double previousProbability = 0.0;

  for (int i = 0; i < size; ++i) {
    double fitness = fitnesses[i];
    previousProbability += (fitness / sumOfFitness);
    thresholds[i] = previousProbability;
  }

  thresholds[size - 1] = 1.0;
  return thresholds;
}

void thresholdsDestroy(double* thresholds)
{
  free(thresholds);
}

typedef struct {
  char* first;
  char* second;
} SelectedChromosomes;

SelectedChromosomes invalid(void)
{
  return (SelectedChromosomes){NULL, NULL};
}

bool isInvalid(SelectedChromosomes s)
{
  return s.first == NULL && s.second == NULL;
}

char* selectChromosome(char* population, double* thresholds, int size)
{
  double randomValue = randomDouble(0.0, 1.0);

  for (int i = 0; i < size; ++i) {
    double threshold  = thresholds[i];
    char*  chromosome = population[i];

    if (randomValue < threshold) {
      return chromosome;
    }
  }

  return population[size - 1];
}

SelectedChromosomes select(int size, char* population, double* fitnesses)
{
  double  sumOfFitness = sum(fitnesses, size);
  double* thresholds   = thresholdsCreate(fitnesses, size, sumOfFitness);

  if (thresholds == NULL) {
    return invalid();
  }

  SelectedChromosomes result = {
    selectChromosome(population, thresholds, size),
    selectChromosome(population, thresholds, size)};
  thresholdsDestroy(thresholds);
  return result;
}

void mutateCharacter(char* character)
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

char* mutate(size_t n, char* s, double p)
{
  for (size_t i = 0; i < n; ++i) {
    double randomValue = randomDouble(0.0, 1.0);

    if (randomValue < p) {
      mutateCharacter(s + i);
    }
  }

  return s;
}

bool crossover(size_t n, char* s1, char* s2)
{
  size_t crossoverPoint  = rand() % n;
  size_t remainderLength = (n - crossoverPoint) - 1;
  char*  s2Remainder     = malloc(remainderLength);

  if (s2Remainder == NULL) {
    return false;
  }

  memcpy(s2Remainder, (s2 + crossoverPoint) + 1, remainderLength);
  memcpy((s2 + crossoverPoint) + 1, (s1 + crossoverPoint) + 1, remainderLength);
  memcpy((s1 + crossoverPoint) + 1, s2Remainder, remainderLength);
  free(s2Remainder);
  return true;
}

char** populationCreate(size_t populationSize, int chromosomeLength)
{
  char** population = malloc(populationSize * sizeof(char*));

  if (population == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < populationSize; ++i) {
    char* newChromosome = generate(chromosomeLength);

    if (newChromosome == NULL) {
      for (size_t j = 0; j < i; ++j) {
        free(population[j]);
      }

      return NULL;
    }

    population[i] = newChromosome;
  }

  return population;
}

void populationDestroy(char** population, size_t populationSize)
{
  for (size_t i = 0; i < populationSize; ++i) {
    free(population[i]);
  }

  free(population);
}

double*
fitnessesCreate(char** population, size_t populationSize, fitness_t fitness)
{
  double* fitnesses = malloc(populationSize * sizeof(double));

  if (fitnesses == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < populationSize; ++i) {
    fitnesses[i] = fitness(population[i]);
  }

  return fitnesses;
}

void fitnessesDestroy(double* fitnesses)
{
  free(fitnesses);
}

char* copyChromosome(const char* chromosome, int length)
{
  char* copy = malloc(length + 1);

  if (copy == NULL) {
    return NULL;
  }

  memcpy(copy, chromosome, length + 1);
  return copy;
}

#define POPULATION_SIZE 100

// TODO: HERE: This function really just creates a new population once it's
// done.
char* runN(
  fitness_t fitness,
  int       chromosomeLength,
  double    crossoverProbability,
  double    mutationProbability,
  int       iterations)
{
  // TODO: This whole jazz needs to become its own function eventually or
  // something you need to do the 'evolation' 'iterations' times or something.
  char** originalPopulation
    = populationCreate(POPULATION_SIZE, chromosomeLength);

  if (originalPopulation == NULL) {
    return NULL;
  }

  double* fitnesses
    = fitnessesCreate(originalPopulation, POPULATION_SIZE, fitness);

  if (fitnesses == NULL) {
    populationDestroy(originalPopulation, POPULATION_SIZE);
    return NULL;
  }

  SelectedChromosomes selected
    = select(POPULATION_SIZE, originalPopulation, fitnesses);
  char* selected1 = copyChromosome(selected.first, chromosomeLength);

  if (selected1 == NULL) {
    fitnessesDestroy(fitnesses);
    populationDestroy(originalPopulation, POPULATION_SIZE);
    return NULL;
  }

  char* selected2 = copyChromosome(selected.second, chromosomeLength);

  if (selected2 == NULL) {
    free(selected1);
    fitnessesDestroy(fitnesses);
    populationDestroy(originalPopulation, POPULATION_SIZE);
    return NULL;
  }

  if (randomDouble(0.0, 1.0) < crossoverProbability) {
    if (!crossover((size_t)chromosomeLength, selected1, selected2)) {
      free(selected2);
      free(selected1);
      fitnessesDestroy(fitnesses);
      populationDestroy(originalPopulation, POPULATION_SIZE);
      return NULL;
    }
  }

  mutate((size_t)chromosomeLength, selected1, mutationProbability);
  mutate((size_t)chromosomeLength, selected2, mutationProbability);

  // TODO: Deallocate selected2
  // TODO: Deallocate selected1
  // TODO: Deallocate fitnesses
  // TODO: Deallocate originalPopulation
}

char* run(fitness_t f, int length, double p_c, double p_m)
{
  return runN(f, length, p_c, p_m, 100);
}