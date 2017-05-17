#include <math.h>
#include <stdlib.h>
double
layermap (double ind, double layers, double neurons)
{
    int index = ind;
    int numlayers = layers;
    int layer = ((index - 1) % numlayers) + 1; //actual layer
    int r = neurons / numlayers * rand () / RAND_MAX; //between 0 and neurons/layers-1
    double dres;

    if (index == 0)
    {
        dres = r * numlayers + 1;
    }
    else if (layer < numlayers)
    {
        dres = r * numlayers + layer + 1;
    }
    else
    {
        dres = neurons;
    };
    return dres;
}

