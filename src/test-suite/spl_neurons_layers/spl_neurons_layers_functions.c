#include <math.h>
#include <stdlib.h>
#include "spl_neurons_layers_functions.h"

#include <../../src/test-suite/spl_neurons_layers/layermap.c>
double __laymap(double ind,double layers,double neurons)
{
double dest;
dest = layermap(ind,layers,neurons);
return dest;
}
