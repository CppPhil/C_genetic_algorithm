#pragma once
#include <stdbool.h>
#include <stddef.h>

void setExpectedChromosome(const char* expectedChromosome);

typedef double   fitness_t(const char*, ...);
extern fitness_t fitness;
