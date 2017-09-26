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

#ifndef RANDOM_H_
#define RANDOM_H_

#include <gsl/gsl_rng.h>

/**
 *
 */
const gsl_rng_type *_random_generator;
/**
 *
 */
gsl_rng *_rseed;
/**
 *
 * @param n
 */
unsigned long
getRandomValue(int n);
/**
 *
 * @param mu
 * @return
 */
double
exponential(double mu);
/**
 *
 * @param a
 * @param b
 * @return
 */
double
uniform(double a, double b);
/**
 *
 * @param sigma
 * @return
 */
double
normal(double sigma);
/**
 *
 *  @param max
 */
double
randomS(double max);
/**
 *
 * @param a
 * @param size
 */
void
shuffle(int *a, int size);
/**
 *
 */
void
Random();
/**
 *
 */
void
freeRandom();

#endif  /* RANDOM_H_ */
