#include <stdlib.h>
#include <time.h>
#include <stdio.h>
double rnd(double y)
{
//   srand(time(0));
    double r;
    r= 1.0*y*rand()/RAND_MAX;
    return r;
}  
