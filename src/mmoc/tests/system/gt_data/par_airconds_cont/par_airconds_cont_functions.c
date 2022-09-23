#include <math.h>
#include <stdlib.h>
#include "./system/test_data/par_airconds_cont_functions.h"
#include "sections.c"
double __getSection(double x)
{
  double y;

  y = getSection(x);
  return y;
}
