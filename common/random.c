/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "random.h"

#include <gsl/gsl_randist.h>
#include <time.h>

void
shuffle(int *a, int size)
{
  gsl_ran_shuffle(_rseed, a, size, sizeof(int));
}

unsigned long
getRandomValue(int n)
{
  unsigned long ret = gsl_rng_get(_rseed) % n;
  return ret;
}

double
exponential(double mu)
{
  return gsl_ran_exponential(_rseed, mu);
}

double
uniform(double a, double b)
{
  return gsl_ran_flat(_rseed, a, b);
}

double
normal(double sigma)
{
  return gsl_ran_gaussian(_rseed, sigma);
}

double
randomS(double max)
{
  return max * gsl_rng_uniform(_rseed);
}

void
Random()
{
  gsl_rng_env_setup();
  _random_generator = gsl_rng_default;
  _rseed = gsl_rng_alloc(_random_generator);
  gsl_rng_set(_rseed, time(NULL));
}

void
freeRandom()
{
  gsl_rng_free(_rseed);
}
