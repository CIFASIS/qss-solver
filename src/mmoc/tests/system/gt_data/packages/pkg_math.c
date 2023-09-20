#include <math.h>
#include <stdlib.h>
#include "/home/joaquin/work/qss-solver/packages/pkg_math.h"
#include <mmo_math.h>
double __math__exponential(double x)
{
double y;

y = mmo_exponential(x);
return y;
}

double __math__fabs(double x)
{
double y;

y = fabs(x);
return y;
}

double __math__getRandomMarkedValue(double x)
{
double y;

y = mmo_getRandomMarkedValue(x);
return y;
}

double __math__getRandomValue(double x)
{
double y;

y = mmo_getRandomValue(x);
return y;
}

double __math__integer(double x)
{
double y;

y = mmo_integer(x);
return y;
}

double __math__normal(double x)
{
double y;

y = mmo_normal(x);
return y;
}

double __math__pow(double a,double b)
{
double c;

c = pow(a,b);
return c;
}

double __math__rand(double x)
{
double y;

y = mmo_rand(x);
return y;
}

double __math__rand_int(double x,double diff)
{
double y;

y = mmo_rand_int(x,diff);
return y;
}

double __math__rand_seed(double x,double seed)
{
double y;

y = mmo_rand_seed(x,seed);
return y;
}

double __math__random(double x)
{
double y;

y = mmo_random(x);
return y;
}

double __math__root(double a,double b)
{
double c;

c = pow(a,1/b);
return c;
}

double __math__uniform(double a,double b)
{
double y;

y = mmo_uniform(a,b);
return y;
}

