#include <math.h>
#include <stdlib.h>
#include "/home/joaquin/PHD/qss-solver/testsuite/microgrid///pkg_fede.h"

#include "people.c"
double __fede__get_nextpeople_qss(double x)
{
double y;
y = get_nextpeople_qss(x);
return y;
}
double __fede__get_npeople_qss(double x)
{
double y;
y = get_npeople_qss(x);
return y;
}
