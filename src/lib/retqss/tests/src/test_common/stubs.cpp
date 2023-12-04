#include "stubs.h"

#include <stdlib.h>

#include "gtest/gtest.h"

extern "C"
{

int
dopri5(unsigned n, /* dimension of the system <= UINT_MAX-1*/
FcnEqDiff fcn, /* function computing the value of f(x,y) */
double x, /* initial x-value */
double* y, /* initial values for y */
double xend, /* final x-value (xend-x may be positive or negative) */
double* rtoler, /* relative error tolerance */
double* atoler, /* absolute error tolerance */
int itoler, /* switch for rtoler and atoler */
SolTrait solout, /* function providing the numerical solution during integration */
int iout, /* switch for calling solout */
FILE* fileout, /* messages stream */
double uround, /* rounding unit */
double safe, /* safety factor */
double fac1, /* parameters for step size selection */
double fac2, double beta, /* for stabilized step size control */
double hmax, /* maximal step size */
double h, /* initial step size */
long nmax, /* maximal number of allowed steps */
int meth, /* switch for the choice of the coefficients */
long nstiff, /* test for stiffness */
unsigned nrdens, /* number of components for which dense outpout is required */
unsigned* icont, /* indexes of components for which dense output is required, >= nrdens */
unsigned licont, /* declared length of icon */
ZeroCrossing g, /* function to evaluate the roots */
int nzc, /* number of roots functions */
int *jroot, /* array of size nzc to store the sign of the roots found */
double *troot /* time of the root(s) found */
)
{
}

double
contd5(unsigned ii, /* index of desired component */
double x /* approximation at x */
)
{
}

long
nfcnRead(void) /* encapsulation of statistical data */
{
}

long
nstepRead(void)
{
}

long
naccptRead(void)
{
}

long
nrejctRead(void)
{
}

void
ddaskr_(dae_system s,                 // 1
    const int *noOfEquations, // 2
    double *currentTime,      // 3
    double *x,                // 4
    double *dx,               // 5
    const double *finalTime,  // 6
    int info[20],             // 7
    double *relativeTolerance,             // 8
    double *absoluteTolerance,             // 9
    int *outputStatusFlag,    // 10
    double *dWorkArray,       // 11
    const int *lengthOfDWork, // 12
    int *iWorkArray,          // 13
    const int *lengthOfIWork, // 14
    const double *rParArray,  // 15
    const int *iParArray,     // 16
    nullfunc jac,             // 17
    nullfunc p,               // 18
    zero_crossing r,              // 19
    const int *numConstrains, // 20
    int *numOfOutputs         // 21
    )
{
}

#ifdef QSS_TEST_NO_MODEL
void
QSS_initializeDataStructs(QSS_simulator simulator)
{
}

void
CLC_initializeDataStructs(QSS_simulator simulator)
{
}
#endif

void
MOD_settings(SD_simulationSettings settings)
{
	int argc = 1;
	std::string argv_str = "null";
	char *argv = const_cast<char*>(argv_str.c_str());

	testing::InitGoogleTest(&argc, &argv);
	auto result = RUN_ALL_TESTS();

	exit(0);
}

}
