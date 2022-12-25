#include "thresholds.h"
#include "memory.h"

static Thresholds failure(void)
{
  return (Thresholds){NULL, 0, false};
}

Thresholds thresholdsCreate(Fitnesses fitnesses)
{
  double* thresholds = NEW_ARRAY(double, fitnesses.size);

  if (thresholds == NULL) {
    return failure();
  }

  double previousProbability = 0.0;

  for (size_t i = 0; i < fitnesses.size; ++i) {
    double fitness = fitnesses.data[i];
    previousProbability += fitness / fitnesses.sum;
    thresholds[i] = previousProbability;
  }

  thresholds[fitnesses.size - 1] = 1.0;
  return (Thresholds){thresholds, fitnesses.size, true};
}

void thresholdsDestroy(Thresholds thresholds)
{
  DELETE_ARRAY(thresholds.data);
}
