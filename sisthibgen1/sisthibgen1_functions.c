#include <math.h>
#include <stdlib.h>
#include "sisthibgen1_functions.h"

#include <../../testsuite/sisthibgen1/ipvsolver.c>
double
__ipvsolver (double aVpv, double bNp, double cNs, double dTpv, double elambdaph, double femax, double gitmax, double hIpvguess)
{
    double iIpv;
    iIpv = ipvsolver (aVpv, bNp, cNs, dTpv, elambdaph, femax, gitmax, hIpvguess);
    return iIpv;
}
