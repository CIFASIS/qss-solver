#ifndef MMO_MATH_H
#define MMO_MATH_H

double
mmo_rand(double v);

unsigned long
mmo_getRandomValue(int n);

double
mmo_exponential(double mu);

double
mmo_uniform(double a, double b);

double
mmo_normal(double sigma);

void
mmo_shuffle(int *a, int size);

double
mmo_random(double max);

unsigned long
mmo_getRandomMarkedValue(int n);

int
mmo_integer(double v);

double
mmo_rand_seed(double v, double seed);

double
mmo_rand_int(double v, double i);

#endif // MMO_MATH_H
