#include <math.h>
#include <stdlib.h>
#include "airconds_cont_functions.h"

#include "sections.c"
double
__getSection (double x)
{
    double y;
    y = getSection (x);
    return y;
}
