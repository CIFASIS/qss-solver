#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_K0 = 0;
double __PAR_rho0 = 0;
double __PAR_c = 0;
double __PAR_Z0 = 0;
double __PAR_u0 = 0;
double __PAR_beta = 0;
double __PAR_x0 = 0;
double __PAR_gam = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = FALSE;
	 settings->method = 5;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			dx[1] = __PAR_K0*((-x[800])+x[1596])*200;
			dx[2] = ((x[1597]-x[801])*200*__PAR_K0)/2;
			dx[3] = (200*(x[1598]*2-x[802]*2)*__PAR_K0)/6;
			return;
		case 200:
			dx[1] = 1.0/__PAR_rho0*((-x[4])+x[0])*200;
			dx[2] = ((1.0/(__PAR_rho0))*(x[1]-x[5])*200)/2;
			dx[3] = (200*(1.0/(__PAR_rho0))*(x[2]*2-x[6]*2))/6;
			return;
		case 199:
			dx[1] = __PAR_K0*((-x[1596])+x[1592])*200;
			dx[2] = (__PAR_K0*(x[1593]-x[1597])*200)/2;
			dx[3] = ((x[1594]*2-x[1598]*2)*200*__PAR_K0)/6;
			return;
		case 399:
			dx[1] = 1.0/__PAR_rho0*((-x[0])+x[796])*200;
			dx[2] = (-(x[1]-x[797])*200*(1.0/(__PAR_rho0)))/2;
			dx[3] = (-(1.0/(__PAR_rho0))*200*(x[2]*2-x[798]*2))/6;
			return;
		case 400:
			dx[1] = (__PAR_u0-__PAR_c)*((-x[1604])+x[1600])*200;
			dx[2] = ((x[1605]-x[1601])*200*(__PAR_c-__PAR_u0))/2;
			dx[3] = (-(__PAR_c-__PAR_u0)*200*(x[1602]*2-x[1606]*2))/6;
			return;
		case 600:
			dx[1] = (__PAR_u0+__PAR_c)*((-x[2400])+x[3196])*200;
			dx[2] = ((__PAR_u0+__PAR_c)*200*(x[3197]-x[2401]))/2;
			dx[3] = (-(x[2402]*2-x[3198]*2)*200*(__PAR_u0+__PAR_c))/6;
			return;
		case 599:
			dx[1] = (__PAR_u0-__PAR_c)*((-x[1600])+x[2396])*200;
			dx[2] = (-(__PAR_c-__PAR_u0)*200*(x[2397]-x[1601]))/2;
			dx[3] = (-200*(__PAR_u0-__PAR_c)*(x[1602]*2-x[2398]*2))/6;
			return;
		case 799:
			dx[1] = (__PAR_u0+__PAR_c)*((-x[3196])+x[3192])*200;
			dx[2] = ((__PAR_c+__PAR_u0)*(x[3193]-x[3197])*200)/2;
			dx[3] = ((x[3194]*2-x[3198]*2)*(__PAR_u0+__PAR_c)*200)/6;
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = __PAR_K0*((-x[(j0+200) * 4])+x[(j0+199) * 4])*200;
				dx[2] = (-(x[(j0+200) * 4 + 1]-x[(j0+199) * 4 + 1])*200*__PAR_K0)/2;
				dx[3] = (-__PAR_K0*(x[(j0+200) * 4 + 2]*2-x[(j0+199) * 4 + 2]*2)*200)/6;
			}
			j0 = i0-200;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = 1.0/__PAR_rho0*((-x[(j0+1) * 4])+x[(j0) * 4])*200;
				dx[2] = (200*(1.0/(__PAR_rho0))*(x[(j0) * 4 + 1]-x[(j0+1) * 4 + 1]))/2;
				dx[3] = ((1.0/(__PAR_rho0))*(x[(j0) * 4 + 2]*2-x[(j0+1) * 4 + 2]*2)*200)/6;
			}
			j0 = i0-400;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = (__PAR_u0-__PAR_c)*((-x[(j0+401) * 4])+x[(j0+400) * 4])*200;
				dx[2] = (-200*(__PAR_c-__PAR_u0)*(x[(j0+400) * 4 + 1]-x[(j0+401) * 4 + 1]))/2;
				dx[3] = ((x[(j0+400) * 4 + 2]*2-x[(j0+401) * 4 + 2]*2)*(__PAR_u0-__PAR_c)*200)/6;
			}
			j0 = i0-600;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = (__PAR_u0+__PAR_c)*((-x[(j0+600) * 4])+x[(j0+599) * 4])*200;
				dx[2] = (-(__PAR_c+__PAR_u0)*200*(x[(j0+600) * 4 + 1]-x[(j0+599) * 4 + 1]))/2;
				dx[3] = (-(__PAR_c+__PAR_u0)*(x[(j0+600) * 4 + 2]*2-x[(j0+599) * 4 + 2]*2)*200)/6;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			der[800 + 1] = 1.0/__PAR_rho0*((-x[4])+x[0])*200;
			der[800 + 2] = ((1.0/(__PAR_rho0))*(x[1]-x[5])*200)/2;
			der[800 + 3] = (200*(1.0/(__PAR_rho0))*(x[2]*2-x[6]*2))/6;
			der[1596 + 1] = 1.0/__PAR_rho0*((-x[0])+x[796])*200;
			der[1596 + 2] = (-(x[1]-x[797])*200*(1.0/(__PAR_rho0)))/2;
			der[1596 + 3] = (-(1.0/(__PAR_rho0))*200*(x[2]*2-x[798]*2))/6;
			return;
		case 1:
			der[800 + 1] = 1.0/__PAR_rho0*((-x[4])+x[0])*200;
			der[800 + 2] = ((1.0/(__PAR_rho0))*(x[1]-x[5])*200)/2;
			der[800 + 3] = (200*(1.0/(__PAR_rho0))*(x[2]*2-x[6]*2))/6;
			break;
		case 199:
			der[1596 + 1] = 1.0/__PAR_rho0*((-x[0])+x[796])*200;
			der[1596 + 2] = (-(x[1]-x[797])*200*(1.0/(__PAR_rho0)))/2;
			der[1596 + 3] = (-(1.0/(__PAR_rho0))*200*(x[2]*2-x[798]*2))/6;
			break;
		case 200:
			der[0 + 1] = __PAR_K0*((-x[800])+x[1596])*200;
			der[0 + 2] = ((x[1597]-x[801])*200*__PAR_K0)/2;
			der[0 + 3] = (200*(x[1598]*2-x[802]*2)*__PAR_K0)/6;
			break;
		case 398:
			der[796 + 1] = __PAR_K0*((-x[1596])+x[1592])*200;
			der[796 + 2] = (__PAR_K0*(x[1593]-x[1597])*200)/2;
			der[796 + 3] = ((x[1594]*2-x[1598]*2)*200*__PAR_K0)/6;
			break;
		case 399:
			der[0 + 1] = __PAR_K0*((-x[800])+x[1596])*200;
			der[0 + 2] = ((x[1597]-x[801])*200*__PAR_K0)/2;
			der[0 + 3] = (200*(x[1598]*2-x[802]*2)*__PAR_K0)/6;
			der[796 + 1] = __PAR_K0*((-x[1596])+x[1592])*200;
			der[796 + 2] = (__PAR_K0*(x[1593]-x[1597])*200)/2;
			der[796 + 3] = ((x[1594]*2-x[1598]*2)*200*__PAR_K0)/6;
			return;
		case 400:
			der[1600 + 1] = (__PAR_u0-__PAR_c)*((-x[1604])+x[1600])*200;
			der[1600 + 2] = ((x[1605]-x[1601])*200*(__PAR_c-__PAR_u0))/2;
			der[1600 + 3] = (-(__PAR_c-__PAR_u0)*200*(x[1602]*2-x[1606]*2))/6;
			der[2396 + 1] = (__PAR_u0-__PAR_c)*((-x[1600])+x[2396])*200;
			der[2396 + 2] = (-(__PAR_c-__PAR_u0)*200*(x[2397]-x[1601]))/2;
			der[2396 + 3] = (-200*(__PAR_u0-__PAR_c)*(x[1602]*2-x[2398]*2))/6;
			return;
		case 401:
			der[1600 + 1] = (__PAR_u0-__PAR_c)*((-x[1604])+x[1600])*200;
			der[1600 + 2] = ((x[1605]-x[1601])*200*(__PAR_c-__PAR_u0))/2;
			der[1600 + 3] = (-(__PAR_c-__PAR_u0)*200*(x[1602]*2-x[1606]*2))/6;
			break;
		case 599:
			der[2396 + 1] = (__PAR_u0-__PAR_c)*((-x[1600])+x[2396])*200;
			der[2396 + 2] = (-(__PAR_c-__PAR_u0)*200*(x[2397]-x[1601]))/2;
			der[2396 + 3] = (-200*(__PAR_u0-__PAR_c)*(x[1602]*2-x[2398]*2))/6;
			break;
		case 600:
			der[2400 + 1] = (__PAR_u0+__PAR_c)*((-x[2400])+x[3196])*200;
			der[2400 + 2] = ((__PAR_u0+__PAR_c)*200*(x[3197]-x[2401]))/2;
			der[2400 + 3] = (-(x[2402]*2-x[3198]*2)*200*(__PAR_u0+__PAR_c))/6;
			break;
		case 798:
			der[3196 + 1] = (__PAR_u0+__PAR_c)*((-x[3196])+x[3192])*200;
			der[3196 + 2] = ((__PAR_c+__PAR_u0)*(x[3193]-x[3197])*200)/2;
			der[3196 + 3] = ((x[3194]*2-x[3198]*2)*(__PAR_u0+__PAR_c)*200)/6;
			break;
		case 799:
			der[2400 + 1] = (__PAR_u0+__PAR_c)*((-x[2400])+x[3196])*200;
			der[2400 + 2] = ((__PAR_u0+__PAR_c)*200*(x[3197]-x[2401]))/2;
			der[2400 + 3] = (-(x[2402]*2-x[3198]*2)*200*(__PAR_u0+__PAR_c))/6;
			der[3196 + 1] = (__PAR_u0+__PAR_c)*((-x[3196])+x[3192])*200;
			der[3196 + 2] = ((__PAR_c+__PAR_u0)*(x[3193]-x[3197])*200)/2;
			der[3196 + 3] = ((x[3194]*2-x[3198]*2)*(__PAR_u0+__PAR_c)*200)/6;
			return;
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+200) * 4 + 1] = 1.0/__PAR_rho0*((-x[(j0+1) * 4])+x[(j0) * 4])*200;
		der[(j0+200) * 4 + 2] = (200*(1.0/(__PAR_rho0))*(x[(j0) * 4 + 1]-x[(j0+1) * 4 + 1]))/2;
		der[(j0+200) * 4 + 3] = ((1.0/(__PAR_rho0))*(x[(j0) * 4 + 2]*2-x[(j0+1) * 4 + 2]*2)*200)/6;
	}
	j0 = i0-1;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+200) * 4 + 1] = 1.0/__PAR_rho0*((-x[(j0+1) * 4])+x[(j0) * 4])*200;
		der[(j0+200) * 4 + 2] = (200*(1.0/(__PAR_rho0))*(x[(j0) * 4 + 1]-x[(j0+1) * 4 + 1]))/2;
		der[(j0+200) * 4 + 3] = ((1.0/(__PAR_rho0))*(x[(j0) * 4 + 2]*2-x[(j0+1) * 4 + 2]*2)*200)/6;
	}
	j0 = i0-199;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0) * 4 + 1] = __PAR_K0*((-x[(j0+200) * 4])+x[(j0+199) * 4])*200;
		der[(j0) * 4 + 2] = (-(x[(j0+200) * 4 + 1]-x[(j0+199) * 4 + 1])*200*__PAR_K0)/2;
		der[(j0) * 4 + 3] = (-__PAR_K0*(x[(j0+200) * 4 + 2]*2-x[(j0+199) * 4 + 2]*2)*200)/6;
	}
	j0 = i0-200;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0) * 4 + 1] = __PAR_K0*((-x[(j0+200) * 4])+x[(j0+199) * 4])*200;
		der[(j0) * 4 + 2] = (-(x[(j0+200) * 4 + 1]-x[(j0+199) * 4 + 1])*200*__PAR_K0)/2;
		der[(j0) * 4 + 3] = (-__PAR_K0*(x[(j0+200) * 4 + 2]*2-x[(j0+199) * 4 + 2]*2)*200)/6;
	}
	j0 = i0-400;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+400) * 4 + 1] = (__PAR_u0-__PAR_c)*((-x[(j0+401) * 4])+x[(j0+400) * 4])*200;
		der[(j0+400) * 4 + 2] = (-200*(__PAR_c-__PAR_u0)*(x[(j0+400) * 4 + 1]-x[(j0+401) * 4 + 1]))/2;
		der[(j0+400) * 4 + 3] = ((x[(j0+400) * 4 + 2]*2-x[(j0+401) * 4 + 2]*2)*(__PAR_u0-__PAR_c)*200)/6;
	}
	j0 = i0-401;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+400) * 4 + 1] = (__PAR_u0-__PAR_c)*((-x[(j0+401) * 4])+x[(j0+400) * 4])*200;
		der[(j0+400) * 4 + 2] = (-200*(__PAR_c-__PAR_u0)*(x[(j0+400) * 4 + 1]-x[(j0+401) * 4 + 1]))/2;
		der[(j0+400) * 4 + 3] = ((x[(j0+400) * 4 + 2]*2-x[(j0+401) * 4 + 2]*2)*(__PAR_u0-__PAR_c)*200)/6;
	}
	j0 = i0-599;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+600) * 4 + 1] = (__PAR_u0+__PAR_c)*((-x[(j0+600) * 4])+x[(j0+599) * 4])*200;
		der[(j0+600) * 4 + 2] = (-(__PAR_c+__PAR_u0)*200*(x[(j0+600) * 4 + 1]-x[(j0+599) * 4 + 1]))/2;
		der[(j0+600) * 4 + 3] = (-(__PAR_c+__PAR_u0)*(x[(j0+600) * 4 + 2]*2-x[(j0+599) * 4 + 2]*2)*200)/6;
	}
	j0 = i0-600;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+600) * 4 + 1] = (__PAR_u0+__PAR_c)*((-x[(j0+600) * 4])+x[(j0+599) * 4])*200;
		der[(j0+600) * 4 + 2] = (-(__PAR_c+__PAR_u0)*200*(x[(j0+600) * 4 + 1]-x[(j0+599) * 4 + 1]))/2;
		der[(j0+600) * 4 + 3] = (-(__PAR_c+__PAR_u0)*(x[(j0+600) * 4 + 2]*2-x[(j0+599) * 4 + 2]*2)*200)/6;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 4)
	{
		out[0] = x[(40*j0) * 4];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(5*sizeof(int));
	int *states = (int*)malloc(800*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(800,0,0,0,0,"acousticsRiemDes");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_K0 = 9.000000000000000000000000e+00;
	__PAR_rho0 = 1.000000000000000000000000e+00;
	__PAR_c = 3.000000000000000000000000e+00;
	__PAR_Z0 = 3.000000000000000000000000e+00;
	__PAR_u0 = 0.000000000000000000000000e+00;
	__PAR_beta = 1000.0;
	__PAR_x0 = 5.000000000000000000000000e-01;
	__PAR_gam = 0.000000000000000000000000e+00;
	// Initialize model code.
	for(i0 = 0; i0 <= 199; i0++)
	{
		modelData->x[(i0) * 4] = pow(2.717999999999999971578291e+00,(-__PAR_beta*pow((1.000000000000000000000000e+00*(i0+1)/200-__PAR_x0),2.0)))*cos(__PAR_gam*(1.000000000000000000000000e+00*(i0+1)/200-__PAR_x0));
		modelData->x[(i0+200) * 4] = 0.0;
		modelData->x[(i0+400) * 4] = 1.0/2.0/__PAR_Z0*((-modelData->x[(i0) * 4])+modelData->x[(i0+200) * 4]);
		modelData->x[(i0+600) * 4] = 1.0/2.0/__PAR_Z0*(modelData->x[(i0) * 4]+modelData->x[(i0+200) * 4]);
	}
	modelData->nDS[0] = 2;
	modelData->nDS[200] = 2;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nDS[i0] = 2;
		modelData->nDS[i0+200] = 2;
	}
	modelData->nDS[199] = 2;
	modelData->nDS[399] = 2;
	modelData->nDS[400] = 2;
	modelData->nDS[600] = 2;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nDS[i0+400] = 2;
		modelData->nDS[i0+600] = 2;
	}
	modelData->nDS[599] = 2;
	modelData->nDS[799] = 2;
	modelData->nSD[200]++;
	modelData->nSD[399]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nSD[i0+199]++;
		modelData->nSD[i0+200]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+1]++;
	}
	modelData->nSD[398]++;
	modelData->nSD[399]++;
	modelData->nSD[0]++;
	modelData->nSD[199]++;
	modelData->nSD[400]++;
	modelData->nSD[401]++;
	modelData->nSD[600]++;
	modelData->nSD[799]++;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nSD[i0+400]++;
		modelData->nSD[i0+401]++;
		modelData->nSD[i0+599]++;
		modelData->nSD[i0+600]++;
	}
	modelData->nSD[400]++;
	modelData->nSD[599]++;
	modelData->nSD[798]++;
	modelData->nSD[799]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,800);

	modelData->DS[0][states[0]++] = 200;
	modelData->DS[0][states[0]++] = 399;
	modelData->DS[200][states[200]++] = 0;
	modelData->DS[200][states[200]++] = 1;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0+199;
		modelData->DS[i0][states[i0]++] = i0+200;
		modelData->DS[i0+200][states[i0+200]++] = i0;
		modelData->DS[i0+200][states[i0+200]++] = i0+1;
	}
	modelData->DS[199][states[199]++] = 398;
	modelData->DS[199][states[199]++] = 399;
	modelData->DS[399][states[399]++] = 0;
	modelData->DS[399][states[399]++] = 199;
	modelData->DS[400][states[400]++] = 400;
	modelData->DS[400][states[400]++] = 401;
	modelData->DS[600][states[600]++] = 600;
	modelData->DS[600][states[600]++] = 799;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->DS[i0+400][states[i0+400]++] = i0+400;
		modelData->DS[i0+400][states[i0+400]++] = i0+401;
		modelData->DS[i0+600][states[i0+600]++] = i0+599;
		modelData->DS[i0+600][states[i0+600]++] = i0+600;
	}
	modelData->DS[599][states[599]++] = 400;
	modelData->DS[599][states[599]++] = 599;
	modelData->DS[799][states[799]++] = 798;
	modelData->DS[799][states[799]++] = 799;
		cleanVector(states,0,800);

	modelData->SD[200][states[200]++] = 0;
	modelData->SD[399][states[399]++] = 0;
	modelData->SD[0][states[0]++] = 200;
	modelData->SD[1][states[1]++] = 200;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->SD[i0+199][states[i0+199]++] = i0;
		modelData->SD[i0+200][states[i0+200]++] = i0;
		modelData->SD[i0][states[i0]++] = i0+200;
		modelData->SD[i0+1][states[i0+1]++] = i0+200;
	}
	modelData->SD[398][states[398]++] = 199;
	modelData->SD[399][states[399]++] = 199;
	modelData->SD[0][states[0]++] = 399;
	modelData->SD[199][states[199]++] = 399;
	modelData->SD[400][states[400]++] = 400;
	modelData->SD[401][states[401]++] = 400;
	modelData->SD[600][states[600]++] = 600;
	modelData->SD[799][states[799]++] = 600;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->SD[i0+400][states[i0+400]++] = i0+400;
		modelData->SD[i0+401][states[i0+401]++] = i0+400;
		modelData->SD[i0+599][states[i0+599]++] = i0+600;
		modelData->SD[i0+600][states[i0+600]++] = i0+600;
	}
	modelData->SD[400][states[400]++] = 599;
	modelData->SD[599][states[599]++] = 599;
	modelData->SD[798][states[798]++] = 799;
	modelData->SD[799][states[799]++] = 799;
	simulator->time = QSS_Time(800,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("acousticsRiemDes",5,0,800,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 4; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[40*i0]++;
	}
	SD_allocOutputMatrix(modelOutput,800,0);
		cleanVector(states,0,800);

		cleanVector(outputs,0,5);

	for(i0 = 0; i0 <= 4; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"p[%d]",40*i0+1);
	}
		cleanVector(outputs,0,5);

	for(i0 = 0; i0 <= 4; i0++)
	{
		modelOutput->SO[40*i0][states[40*i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = 40*i0;
	}
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
