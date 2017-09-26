#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "spl_neurons_layers_functions.h"
#include "pkg_math.h"
#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_tau = 0;
double __PAR_vrest = 0;
double __PAR_vthres = 0;
double __PAR_Trefrac = 0;
double __PAR_Eex = 0;
double __PAR_Einh = 0;
double __PAR_tauex = 0;
double __PAR_tauinh = 0;
double __PAR_dgex = 0;
double __PAR_dgin = 0;
double __PAR_Period = 0;
int __reverse__PAR_SNmap1[1000];
int __PAR_SNmap1[1000];
int __reverse__PAR_NNmap1[1000];
int __PAR_NNmap1[1000];
int __reverse__PAR_NNmap2[1000];
int __PAR_NNmap2[1000];
int __reverse__PAR_NNmap3[1000];
int __PAR_NNmap3[1000];
int __reverse__PAR_NNmap4[1000];
int __PAR_NNmap4[1000];
int __reverse__PAR_NNmap5[1000];
int __PAR_NNmap5[1000];
int __reverse__PAR_NNmap6[1000];
int __PAR_NNmap6[1000];
int __reverse__PAR_NNmap7[1000];
int __PAR_NNmap7[1000];
int __reverse__PAR_NNmap8[1000];
int __PAR_NNmap8[1000];
int __reverse__PAR_NNmap9[1000];
int __PAR_NNmap9[1000];
int __reverse__PAR_NNmap10[1000];
int __PAR_NNmap10[1000];
int __reverse__PAR_NNmap11[1000];
int __PAR_NNmap11[1000];
int __reverse__PAR_NNmap12[1000];
int __PAR_NNmap12[1000];
int __reverse__PAR_NNmap13[1000];
int __PAR_NNmap13[1000];
int __reverse__PAR_NNmap14[1000];
int __PAR_NNmap14[1000];
int __reverse__PAR_NNmap15[1000];
int __PAR_NNmap15[1000];
int __reverse__PAR_NNmap16[1000];
int __PAR_NNmap16[1000];
int __reverse__PAR_NNmap17[1000];
int __PAR_NNmap17[1000];
int __reverse__PAR_NNmap18[1000];
int __PAR_NNmap18[1000];
int __reverse__PAR_NNmap19[1000];
int __PAR_NNmap19[1000];
int __reverse__PAR_NNmap20[1000];
int __PAR_NNmap20[1000];

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 6;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 999)
	{
		dx[1] = d[(j0)]*((__PAR_vrest-x[(j0) * 4])+x[(j0+1000) * 4]*(__PAR_Eex-x[(j0) * 4])+x[(j0+2000) * 4]*(__PAR_Einh-x[(j0) * 4]))/__PAR_tau;
	}
	j0 = i0-1000;
	if(j0 >=0 && j0 <= 999)
	{
		dx[1] = -x[(j0+1000) * 4]/__PAR_tauex;
	}
	j0 = i0-2000;
	if(j0 >=0 && j0 <= 999)
	{
		dx[1] = -x[(j0+2000) * 4]/__PAR_tauinh;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 999)
	{
		der[(j0) * 4 + 1] = d[(j0)]*((__PAR_vrest-x[(j0) * 4])+x[(j0+1000) * 4]*(__PAR_Eex-x[(j0) * 4])+x[(j0+2000) * 4]*(__PAR_Einh-x[(j0) * 4]))/__PAR_tau;
	}
	j0 = i0-1000;
	if(j0 >=0 && j0 <= 999)
	{
		der[(j0) * 4 + 1] = d[(j0)]*((__PAR_vrest-x[(j0) * 4])+x[(j0+1000) * 4]*(__PAR_Eex-x[(j0) * 4])+x[(j0+2000) * 4]*(__PAR_Einh-x[(j0) * 4]))/__PAR_tau;
		der[(j0+1000) * 4 + 1] = -x[(j0+1000) * 4]/__PAR_tauex;
	}
	j0 = i0-2000;
	if(j0 >=0 && j0 <= 999)
	{
		der[(j0) * 4 + 1] = d[(j0)]*((__PAR_vrest-x[(j0) * 4])+x[(j0+1000) * 4]*(__PAR_Eex-x[(j0) * 4])+x[(j0+2000) * 4]*(__PAR_Einh-x[(j0) * 4]))/__PAR_tau;
		der[(j0+2000) * 4 + 1] = -x[(j0+2000) * 4]/__PAR_tauinh;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 799)
	{
		zc[0] = x[(i0) * 4]-(__PAR_vthres);
	}
	if(i0 >= 1000 && i0 <= 1999)
	{
		zc[0] = t-(d[(i0)]+__PAR_Trefrac);
	}
	if(i0 >= 800 && i0 <= 999)
	{
		zc[0] = x[(i0) * 4]-(__PAR_vthres);
	}
	if(i0 >= 2000 && i0 <= 2200)
	{
		zc[0] = t-(d[(i0)]);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 799)
	{
		x[(i0) * 4] = __PAR_vrest;
		x[(__PAR_NNmap1[i0]+999) * 4] = x[(__PAR_NNmap1[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap2[i0]+999) * 4] = x[(__PAR_NNmap2[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap3[i0]+999) * 4] = x[(__PAR_NNmap3[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap4[i0]+999) * 4] = x[(__PAR_NNmap4[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap5[i0]+999) * 4] = x[(__PAR_NNmap5[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap6[i0]+999) * 4] = x[(__PAR_NNmap6[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap7[i0]+999) * 4] = x[(__PAR_NNmap7[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap8[i0]+999) * 4] = x[(__PAR_NNmap8[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap9[i0]+999) * 4] = x[(__PAR_NNmap9[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap10[i0]+999) * 4] = x[(__PAR_NNmap10[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap11[i0]+999) * 4] = x[(__PAR_NNmap11[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap12[i0]+999) * 4] = x[(__PAR_NNmap12[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap13[i0]+999) * 4] = x[(__PAR_NNmap13[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap14[i0]+999) * 4] = x[(__PAR_NNmap14[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap15[i0]+999) * 4] = x[(__PAR_NNmap15[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap16[i0]+999) * 4] = x[(__PAR_NNmap16[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap17[i0]+999) * 4] = x[(__PAR_NNmap17[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap18[i0]+999) * 4] = x[(__PAR_NNmap18[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap19[i0]+999) * 4] = x[(__PAR_NNmap19[i0]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap20[i0]+999) * 4] = x[(__PAR_NNmap20[i0]+999) * 4]+__PAR_dgex;
		d[(i0)] = 0.0;
		d[(i0+1000)] = t;
	}
	if(i0 >= 1000 && i0 <= 1999)
	{
		d[(i0-1000)] = 1.0;
	}
	if(i0 >= 800 && i0 <= 999)
	{
		x[(i0) * 4] = __PAR_vrest;
		x[(__PAR_NNmap1[i0]+1999) * 4] = x[(__PAR_NNmap1[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap2[i0]+1999) * 4] = x[(__PAR_NNmap2[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap3[i0]+1999) * 4] = x[(__PAR_NNmap3[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap4[i0]+1999) * 4] = x[(__PAR_NNmap4[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap5[i0]+1999) * 4] = x[(__PAR_NNmap5[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap6[i0]+1999) * 4] = x[(__PAR_NNmap6[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap7[i0]+1999) * 4] = x[(__PAR_NNmap7[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap8[i0]+1999) * 4] = x[(__PAR_NNmap8[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap9[i0]+1999) * 4] = x[(__PAR_NNmap9[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap10[i0]+1999) * 4] = x[(__PAR_NNmap10[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap11[i0]+1999) * 4] = x[(__PAR_NNmap11[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap12[i0]+1999) * 4] = x[(__PAR_NNmap12[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap13[i0]+1999) * 4] = x[(__PAR_NNmap13[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap14[i0]+1999) * 4] = x[(__PAR_NNmap14[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap15[i0]+1999) * 4] = x[(__PAR_NNmap15[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap16[i0]+1999) * 4] = x[(__PAR_NNmap16[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap17[i0]+1999) * 4] = x[(__PAR_NNmap17[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap18[i0]+1999) * 4] = x[(__PAR_NNmap18[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap19[i0]+1999) * 4] = x[(__PAR_NNmap19[i0]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap20[i0]+1999) * 4] = x[(__PAR_NNmap20[i0]+1999) * 4]+__PAR_dgin;
		d[(i0)] = 0.0;
		d[(i0+1000)] = t;
	}
	if(i0 >= 2000 && i0 <= 2200)
	{
		x[(__PAR_SNmap1[i0-2000]+999) * 4] = x[(__PAR_SNmap1[i0-2000]+999) * 4]+__PAR_dgex;
		d[(i0)] = t+__math__rand(__PAR_Period);
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[3996];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(2201*sizeof(int));
	int *events = (int*)malloc(2201*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(3000*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(3000,2201,2201,0,0,"spl_neurons_layers");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_tau = 20.0;
	__PAR_vrest = -60.0;
	__PAR_vthres = -50.0;
	__PAR_Trefrac = 5.0;
	__PAR_Eex = 0.0;
	__PAR_Einh = -80.0;
	__PAR_tauex = 5.0;
	__PAR_tauinh = 10.0;
	__PAR_dgex = 5.000000000000000000000000e-01;
	__PAR_dgin = 1.250000000000000000000000e+00;
	__PAR_Period = 10.0;
	modelData->d[(0)] = 1.0;
	for(i0 = 0; i0 <= 200;i0++)
	{
		modelData->d[(i0+2000)] = __math__rand(10.0);
	}
	modelData->d[(1000)] = 10000.0;
	for(i0 = 0; i0 <= 999;i0++)
	{
		modelData->x[i0 * 4] = -60.0;
	}
	// Initialize model code.
	for(i0 = 0; i0 <= 200; i0++)
	{
		__PAR_SNmap1[(i0)] = __laymap(0.0,5,1000);
	__reverse__PAR_SNmap1[__PAR_SNmap1[(i0)]-1] = i0+1;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		__PAR_NNmap1[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap1[__PAR_NNmap1[(i0)]-1] = i0+1;
		__PAR_NNmap2[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap2[__PAR_NNmap2[(i0)]-1] = i0+1;
		__PAR_NNmap3[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap3[__PAR_NNmap3[(i0)]-1] = i0+1;
		__PAR_NNmap4[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap4[__PAR_NNmap4[(i0)]-1] = i0+1;
		__PAR_NNmap5[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap5[__PAR_NNmap5[(i0)]-1] = i0+1;
		__PAR_NNmap6[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap6[__PAR_NNmap6[(i0)]-1] = i0+1;
		__PAR_NNmap7[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap7[__PAR_NNmap7[(i0)]-1] = i0+1;
		__PAR_NNmap8[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap8[__PAR_NNmap8[(i0)]-1] = i0+1;
		__PAR_NNmap9[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap9[__PAR_NNmap9[(i0)]-1] = i0+1;
		__PAR_NNmap10[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap10[__PAR_NNmap10[(i0)]-1] = i0+1;
		__PAR_NNmap11[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap11[__PAR_NNmap11[(i0)]-1] = i0+1;
		__PAR_NNmap12[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap12[__PAR_NNmap12[(i0)]-1] = i0+1;
		__PAR_NNmap13[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap13[__PAR_NNmap13[(i0)]-1] = i0+1;
		__PAR_NNmap14[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap14[__PAR_NNmap14[(i0)]-1] = i0+1;
		__PAR_NNmap15[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap15[__PAR_NNmap15[(i0)]-1] = i0+1;
		__PAR_NNmap16[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap16[__PAR_NNmap16[(i0)]-1] = i0+1;
		__PAR_NNmap17[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap17[__PAR_NNmap17[(i0)]-1] = i0+1;
		__PAR_NNmap18[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap18[__PAR_NNmap18[(i0)]-1] = i0+1;
		__PAR_NNmap19[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap19[__PAR_NNmap19[(i0)]-1] = i0+1;
		__PAR_NNmap20[(i0)] = __laymap((i0+1),5,1000);
	__reverse__PAR_NNmap20[__PAR_NNmap20[(i0)]-1] = i0+1;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nDS[i0] = 3;
		modelData->nDS[i0+1000] = 1;
		modelData->nDS[i0+2000] = 1;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nSD[i0]++;
		modelData->nSD[i0+1000]++;
		modelData->nSD[i0+2000]++;
		modelData->nSD[i0+1000]++;
		modelData->nSD[i0+2000]++;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->nZS[i0] = 1;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->nZS[i0] = 1;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->nSZ[i0]++;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->nSZ[i0]++;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 2000; i0 <= 2200; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 1000; i0 <= 1999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].nLHSSt = 21;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].nLHSSt = 21;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].nLHSSt = 1;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].nRHSSt = 20;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].nRHSSt = 20;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].nRHSSt = 1;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].nLHSDsc = 2;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
	modelData->event[i0+1000].nLHSDsc = 1;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].nLHSDsc = 2;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,3000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = i0+1000;
		modelData->DS[i0][states[i0]++] = i0+2000;
		modelData->DS[i0+1000][states[i0+1000]++] = i0+1000;
		modelData->DS[i0+2000][states[i0+2000]++] = i0+2000;
	}
		cleanVector(states,0,3000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+1000][states[i0+1000]++] = i0;
		modelData->SD[i0+2000][states[i0+2000]++] = i0;
		modelData->SD[i0+1000][states[i0+1000]++] = i0+1000;
		modelData->SD[i0+2000][states[i0+2000]++] = i0+2000;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
	}
		cleanVector(states,0,3000);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+1000;
	}
	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+1000;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 2000; i0 <= 2200; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 1000; i0 <= 1999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-1000;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap1[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap10[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap11[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap12[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap13[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap14[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap15[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap16[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap17[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap18[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap19[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap2[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap20[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap3[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap4[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap5[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap6[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap7[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap8[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap9[i0]+999;
		modelData->event[i0].LHSSt[events[i0]++] = i0;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap1[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap10[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap11[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap12[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap13[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap14[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap15[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap16[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap17[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap18[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap19[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap2[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap20[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap3[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap4[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap5[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap6[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap7[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap8[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = __PAR_NNmap9[i0]+1999;
		modelData->event[i0].LHSSt[events[i0]++] = i0;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].LHSSt[events[i0+2000]++] = __PAR_SNmap1[i0]+999;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap1[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap10[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap11[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap12[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap13[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap14[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap15[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap16[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap17[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap18[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap19[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap2[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap20[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap3[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap4[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap5[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap6[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap7[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap8[i0]+999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap9[i0]+999;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap1[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap10[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap11[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap12[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap13[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap14[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap15[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap16[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap17[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap18[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap19[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap2[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap20[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap3[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap4[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap5[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap6[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap7[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap8[i0]+1999;
		modelData->event[i0].RHSSt[events[i0]++] = __PAR_NNmap9[i0]+1999;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].RHSSt[events[i0+2000]++] = __PAR_SNmap1[i0]+999;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].LHSDsc[events[i0]++] = i0;
		modelData->event[i0].LHSDsc[events[i0]++] = i0+1000;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
	modelData->event[i0+1000].LHSDsc[events[i0+1000]++] = i0;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].LHSDsc[events[i0]++] = i0;
		modelData->event[i0].LHSDsc[events[i0]++] = i0+1000;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].LHSDsc[events[i0+2000]++] = i0+2000;
	}
		cleanVector(events,0,2201);

	for(i0 = 0; i0 <= 799; i0++)
	{
		modelData->event[i0].direction = 1;
		modelData->event[i0].relation = 2;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
	modelData->event[i0+1000].direction = 1;
	modelData->event[i0+1000].relation = 2;
	}
	for(i0 = 800; i0 <= 999; i0++)
	{
		modelData->event[i0].direction = 1;
		modelData->event[i0].relation = 2;
	}
	for(i0 = 0; i0 <= 200; i0++)
	{
	modelData->event[i0+2000].direction = 1;
	modelData->event[i0+2000].relation = 2;
	}
	simulator->time = QSS_Time(3000,2201,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("spl_neurons_layers",1,2201,3000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[999]++;
	SD_allocOutputMatrix(modelOutput,3000,2201);
		cleanVector(states,0,3000);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"v[1000]");
		cleanVector(outputs,0,1);

		modelOutput->SO[999][states[999]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 999;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,MOD_zeroCrossing,MOD_handlerPos,NULL);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
