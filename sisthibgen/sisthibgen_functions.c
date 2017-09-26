#include <math.h>
#include <stdlib.h>
#include "sisthibgen_functions.h"

#include <../../src/test-suite/sisthibgen/ipvsolver.c>
double __ipvsolver(double aVpv,double bNp,double cNs,double dTpv,double elambdaph,double femax,double gitmax,double hIpvguess)
{
double iIpv;
iIpv = ipvsolver(aVpv,bNp,cNs,dTpv,elambdaph,femax,gitmax,hIpvguess);
return iIpv;
}
