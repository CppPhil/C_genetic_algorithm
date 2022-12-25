#pragma once
#include <stdbool.h>
#include <stddef.h>

#include "fitnesses.h"

typedef struct {
  double* data;
  size_t  size;
  bool    isOk;
} Thresholds;

Thresholds thresholdsCreate(Fitnesses fitnesses);

void thresholdsDestroy(Thresholds thresholds);
