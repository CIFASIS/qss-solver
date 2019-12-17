#include <stdlib.h>
#include <math.h>

#include "../../engine/common/random.h"
#include "mmo_math.h"

static unsigned long *marked = NULL;

double
mmo_rand(double v)
{
  double y;
  y = v * rand() / RAND_MAX;
  return y;
}

unsigned long
mmo_getRandomValue(int n)
{
  return getRandomValue(n);
}

unsigned long
mmo_getRandomMarkedValue(int n)
{
  if(marked == NULL)
  {
    marked = (unsigned long*) malloc(n * sizeof(unsigned long));
    int i;
    for(i = 0; i < n; i++)
    {
      marked[i] = n + 1;
    }
  }
  unsigned long p = getRandomValue(n);
  while(marked[p] != n + 1)
  {
    p = getRandomValue(n);
  }
  marked[p] = p;
  return p;
}

double
mmo_exponential(double mu)
{
  return exponential(mu);
}

double
mmo_uniform(double a, double b)
{
  return uniform(a, b);
}

double
mmo_normal(double sigma)
{
  return normal(sigma);
}

void
mmo_shuffle(int *a, int size)
{
  return shuffle(a, size);
}

double
mmo_random(double max)
{
  return randomS(max);
}
