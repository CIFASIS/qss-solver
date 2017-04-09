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
	int j = 0;
	j = i;
	if(j >=0 && j <= 999)
	{
		dx[1] = d[(j)]*((__PAR_vrest-x[(j) * 4])+x[(j+1000) * 4]*(__PAR_Eex-x[(j) * 4])+x[(j+2000) * 4]*(__PAR_Einh-x[(j) * 4]))/__PAR_tau;
	}
	j = i-1000;
	if(j >=0 && j <= 999)
	{
		dx[1] = -x[(j+1000) * 4]/__PAR_tauex;
	}
	j = i-2000;
	if(j >=0 && j <= 999)
	{
		dx[1] = -x[(j+2000) * 4]/__PAR_tauinh;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 999)
	{
		der[(j) * 4 + 1] = d[(j)]*((__PAR_vrest-x[(j) * 4])+x[(j+1000) * 4]*(__PAR_Eex-x[(j) * 4])+x[(j+2000) * 4]*(__PAR_Einh-x[(j) * 4]))/__PAR_tau;
	}
	j = i-1000;
	if(j >=0 && j <= 999)
	{
		der[(j) * 4 + 1] = d[(j)]*((__PAR_vrest-x[(j) * 4])+x[(j+1000) * 4]*(__PAR_Eex-x[(j) * 4])+x[(j+2000) * 4]*(__PAR_Einh-x[(j) * 4]))/__PAR_tau;
		der[(j+1000) * 4 + 1] = -x[(j+1000) * 4]/__PAR_tauex;
	}
	j = i-2000;
	if(j >=0 && j <= 999)
	{
		der[(j) * 4 + 1] = d[(j)]*((__PAR_vrest-x[(j) * 4])+x[(j+1000) * 4]*(__PAR_Eex-x[(j) * 4])+x[(j+2000) * 4]*(__PAR_Einh-x[(j) * 4]))/__PAR_tau;
		der[(j+2000) * 4 + 1] = -x[(j+2000) * 4]/__PAR_tauinh;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	if(i >= 0 && i <= 799)
	{
		zc[0] = x[(i) * 4]-(__PAR_vthres);
	}
	if(i >= 1000 && i <= 1999)
	{
		zc[0] = t-(d[(i)]+__PAR_Trefrac);
	}
	if(i >= 800 && i <= 999)
	{
		zc[0] = x[(i) * 4]-(__PAR_vthres);
	}
	if(i >= 2000 && i <= 2200)
	{
		zc[0] = t-(d[(i)]);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 799)
	{
		x[(i) * 4] = __PAR_vrest;
		x[(__PAR_NNmap1[i]+999) * 4] = x[(__PAR_NNmap1[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap2[i]+999) * 4] = x[(__PAR_NNmap2[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap3[i]+999) * 4] = x[(__PAR_NNmap3[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap4[i]+999) * 4] = x[(__PAR_NNmap4[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap5[i]+999) * 4] = x[(__PAR_NNmap5[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap6[i]+999) * 4] = x[(__PAR_NNmap6[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap7[i]+999) * 4] = x[(__PAR_NNmap7[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap8[i]+999) * 4] = x[(__PAR_NNmap8[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap9[i]+999) * 4] = x[(__PAR_NNmap9[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap10[i]+999) * 4] = x[(__PAR_NNmap10[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap11[i]+999) * 4] = x[(__PAR_NNmap11[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap12[i]+999) * 4] = x[(__PAR_NNmap12[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap13[i]+999) * 4] = x[(__PAR_NNmap13[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap14[i]+999) * 4] = x[(__PAR_NNmap14[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap15[i]+999) * 4] = x[(__PAR_NNmap15[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap16[i]+999) * 4] = x[(__PAR_NNmap16[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap17[i]+999) * 4] = x[(__PAR_NNmap17[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap18[i]+999) * 4] = x[(__PAR_NNmap18[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap19[i]+999) * 4] = x[(__PAR_NNmap19[i]+999) * 4]+__PAR_dgex;
		x[(__PAR_NNmap20[i]+999) * 4] = x[(__PAR_NNmap20[i]+999) * 4]+__PAR_dgex;
		d[(i)] = 0.0;
		d[(i+1000)] = t;
	}
	if(i >= 1000 && i <= 1999)
	{
		d[(i-1000)] = 1.0;
	}
	if(i >= 800 && i <= 999)
	{
		x[(i) * 4] = __PAR_vrest;
		x[(__PAR_NNmap1[i]+1999) * 4] = x[(__PAR_NNmap1[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap2[i]+1999) * 4] = x[(__PAR_NNmap2[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap3[i]+1999) * 4] = x[(__PAR_NNmap3[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap4[i]+1999) * 4] = x[(__PAR_NNmap4[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap5[i]+1999) * 4] = x[(__PAR_NNmap5[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap6[i]+1999) * 4] = x[(__PAR_NNmap6[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap7[i]+1999) * 4] = x[(__PAR_NNmap7[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap8[i]+1999) * 4] = x[(__PAR_NNmap8[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap9[i]+1999) * 4] = x[(__PAR_NNmap9[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap10[i]+1999) * 4] = x[(__PAR_NNmap10[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap11[i]+1999) * 4] = x[(__PAR_NNmap11[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap12[i]+1999) * 4] = x[(__PAR_NNmap12[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap13[i]+1999) * 4] = x[(__PAR_NNmap13[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap14[i]+1999) * 4] = x[(__PAR_NNmap14[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap15[i]+1999) * 4] = x[(__PAR_NNmap15[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap16[i]+1999) * 4] = x[(__PAR_NNmap16[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap17[i]+1999) * 4] = x[(__PAR_NNmap17[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap18[i]+1999) * 4] = x[(__PAR_NNmap18[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap19[i]+1999) * 4] = x[(__PAR_NNmap19[i]+1999) * 4]+__PAR_dgin;
		x[(__PAR_NNmap20[i]+1999) * 4] = x[(__PAR_NNmap20[i]+1999) * 4]+__PAR_dgin;
		d[(i)] = 0.0;
		d[(i+1000)] = t;
	}
	if(i >= 2000 && i <= 2200)
	{
		x[(__PAR_SNmap1[i-2000]+999) * 4] = x[(__PAR_SNmap1[i-2000]+999) * 4]+__PAR_dgex;
		d[(i)] = t+__math__rand(__PAR_Period);
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
	int i2;
	int i4;
	int i;
	int j = 0;
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
	for(i = 0; i <= 200;i++)
	{
		modelData->d[(i+2000)] = __math__rand(10.0);
	}
	modelData->d[(1000)] = 10000.0;
	for(i = 0; i <= 999;i++)
	{
		modelData->x[i * 4] = -60.0;
	}
	// Initialize model code.
	for(i2 = 0; i2 <= 200; i2++)
	{
		__PAR_SNmap1[(i2)] = __laymap(0.0,5,1000);
	__reverse__PAR_SNmap1[__PAR_SNmap1[(i2)]-1] = i2+1;
	}
	for(i4 = 0; i4 <= 999; i4++)
	{
		__PAR_NNmap1[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap1[__PAR_NNmap1[(i4)]-1] = i4+1;
		__PAR_NNmap2[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap2[__PAR_NNmap2[(i4)]-1] = i4+1;
		__PAR_NNmap3[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap3[__PAR_NNmap3[(i4)]-1] = i4+1;
		__PAR_NNmap4[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap4[__PAR_NNmap4[(i4)]-1] = i4+1;
		__PAR_NNmap5[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap5[__PAR_NNmap5[(i4)]-1] = i4+1;
		__PAR_NNmap6[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap6[__PAR_NNmap6[(i4)]-1] = i4+1;
		__PAR_NNmap7[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap7[__PAR_NNmap7[(i4)]-1] = i4+1;
		__PAR_NNmap8[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap8[__PAR_NNmap8[(i4)]-1] = i4+1;
		__PAR_NNmap9[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap9[__PAR_NNmap9[(i4)]-1] = i4+1;
		__PAR_NNmap10[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap10[__PAR_NNmap10[(i4)]-1] = i4+1;
		__PAR_NNmap11[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap11[__PAR_NNmap11[(i4)]-1] = i4+1;
		__PAR_NNmap12[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap12[__PAR_NNmap12[(i4)]-1] = i4+1;
		__PAR_NNmap13[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap13[__PAR_NNmap13[(i4)]-1] = i4+1;
		__PAR_NNmap14[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap14[__PAR_NNmap14[(i4)]-1] = i4+1;
		__PAR_NNmap15[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap15[__PAR_NNmap15[(i4)]-1] = i4+1;
		__PAR_NNmap16[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap16[__PAR_NNmap16[(i4)]-1] = i4+1;
		__PAR_NNmap17[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap17[__PAR_NNmap17[(i4)]-1] = i4+1;
		__PAR_NNmap18[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap18[__PAR_NNmap18[(i4)]-1] = i4+1;
		__PAR_NNmap19[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap19[__PAR_NNmap19[(i4)]-1] = i4+1;
		__PAR_NNmap20[(i4)] = __laymap((i4+1),5,1000);
	__reverse__PAR_NNmap20[__PAR_NNmap20[(i4)]-1] = i4+1;
	}
	for(i = 0; i <= 999; i++)
	{
		modelData->nDS[i] = 3;
		modelData->nDS[i+1000] = 1;
		modelData->nDS[i+2000] = 1;
	}
	for(i = 0; i <= 999; i++)
	{
		modelData->nSD[i]++;
		modelData->nSD[i+1000]++;
		modelData->nSD[i+2000]++;
		modelData->nSD[i+1000]++;
		modelData->nSD[i+2000]++;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->nZS[i] = 1;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->nZS[i] = 1;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->nSZ[i]++;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->nSZ[i]++;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 2000; i <= 2200; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 1000; i <= 1999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].nLHSSt = 21;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].nLHSSt = 21;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].nLHSSt = 1;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].nRHSSt = 20;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].nRHSSt = 20;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].nRHSSt = 1;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].nLHSDsc = 2;
	}
	for(i = 0; i <= 999; i++)
	{
	modelData->event[i+1000].nLHSDsc = 1;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].nLHSDsc = 2;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,3000);

	for(i = 0; i <= 999; i++)
	{
		modelData->DS[i][states[i]++] = i;
		modelData->DS[i][states[i]++] = i+1000;
		modelData->DS[i][states[i]++] = i+2000;
		modelData->DS[i+1000][states[i+1000]++] = i+1000;
		modelData->DS[i+2000][states[i+2000]++] = i+2000;
	}
		cleanVector(states,0,3000);

	for(i = 0; i <= 999; i++)
	{
		modelData->SD[i][states[i]++] = i;
		modelData->SD[i+1000][states[i+1000]++] = i;
		modelData->SD[i+2000][states[i+2000]++] = i;
		modelData->SD[i+1000][states[i+1000]++] = i+1000;
		modelData->SD[i+2000][states[i+2000]++] = i+2000;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->ZS[i][events[i]++] = i;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->ZS[i][events[i]++] = i;
	}
		cleanVector(states,0,3000);

	for(i = 0; i <= 799; i++)
	{
		modelData->SZ[i][states[i]++] = i;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->SZ[i][states[i]++] = i;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->HZ[i][events[i]++] = i+1000;
	}
	for(i = 0; i <= 799; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->HZ[i][events[i]++] = i+1000;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 2000; i <= 2200; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 1000; i <= 1999; i++)
	{
		modelData->HD[i][events[i]++] = i-1000;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap1[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap10[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap11[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap12[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap13[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap14[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap15[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap16[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap17[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap18[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap19[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap2[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap20[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap3[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap4[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap5[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap6[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap7[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap8[i]+999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap9[i]+999;
		modelData->event[i].LHSSt[events[i]++] = i;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap1[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap10[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap11[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap12[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap13[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap14[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap15[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap16[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap17[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap18[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap19[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap2[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap20[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap3[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap4[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap5[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap6[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap7[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap8[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = __PAR_NNmap9[i]+1999;
		modelData->event[i].LHSSt[events[i]++] = i;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].LHSSt[events[i+2000]++] = __PAR_SNmap1[i]+999;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap1[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap10[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap11[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap12[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap13[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap14[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap15[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap16[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap17[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap18[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap19[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap2[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap20[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap3[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap4[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap5[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap6[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap7[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap8[i]+999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap9[i]+999;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap1[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap10[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap11[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap12[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap13[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap14[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap15[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap16[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap17[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap18[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap19[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap2[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap20[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap3[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap4[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap5[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap6[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap7[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap8[i]+1999;
		modelData->event[i].RHSSt[events[i]++] = __PAR_NNmap9[i]+1999;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].RHSSt[events[i+2000]++] = __PAR_SNmap1[i]+999;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].LHSDsc[events[i]++] = i;
		modelData->event[i].LHSDsc[events[i]++] = i+1000;
	}
	for(i = 0; i <= 999; i++)
	{
	modelData->event[i+1000].LHSDsc[events[i+1000]++] = i;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].LHSDsc[events[i]++] = i;
		modelData->event[i].LHSDsc[events[i]++] = i+1000;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].LHSDsc[events[i+2000]++] = i+2000;
	}
		cleanVector(events,0,2201);

	for(i = 0; i <= 799; i++)
	{
		modelData->event[i].direction = 1;
		modelData->event[i].relation = 2;
	}
	for(i = 0; i <= 999; i++)
	{
	modelData->event[i+1000].direction = 1;
	modelData->event[i+1000].relation = 2;
	}
	for(i = 800; i <= 999; i++)
	{
		modelData->event[i].direction = 1;
		modelData->event[i].relation = 2;
	}
	for(i = 0; i <= 200; i++)
	{
	modelData->event[i+2000].direction = 1;
	modelData->event[i+2000].relation = 2;
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
