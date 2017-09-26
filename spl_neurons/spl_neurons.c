#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
double __PAR_lastTime = 0;
int __reverse__PAR_SNmap1[3000];
int __PAR_SNmap1[3000];
int __reverse__PAR_NNmap1[3000];
int __PAR_NNmap1[3000];
int __reverse__PAR_NNmap2[3000];
int __PAR_NNmap2[3000];
int __reverse__PAR_NNmap3[3000];
int __PAR_NNmap3[3000];
int __reverse__PAR_NNmap4[3000];
int __PAR_NNmap4[3000];
int __reverse__PAR_NNmap5[3000];
int __PAR_NNmap5[3000];
int __reverse__PAR_NNmap6[3000];
int __PAR_NNmap6[3000];
int __reverse__PAR_NNmap7[3000];
int __PAR_NNmap7[3000];
int __reverse__PAR_NNmap8[3000];
int __PAR_NNmap8[3000];
int __reverse__PAR_NNmap9[3000];
int __PAR_NNmap9[3000];
int __reverse__PAR_NNmap10[3000];
int __PAR_NNmap10[3000];
int __reverse__PAR_NNmap11[3000];
int __PAR_NNmap11[3000];
int __reverse__PAR_NNmap12[3000];
int __PAR_NNmap12[3000];
int __reverse__PAR_NNmap13[3000];
int __PAR_NNmap13[3000];
int __reverse__PAR_NNmap14[3000];
int __PAR_NNmap14[3000];
int __reverse__PAR_NNmap15[3000];
int __PAR_NNmap15[3000];
int __reverse__PAR_NNmap16[3000];
int __PAR_NNmap16[3000];
int __reverse__PAR_NNmap17[3000];
int __PAR_NNmap17[3000];
int __reverse__PAR_NNmap18[3000];
int __PAR_NNmap18[3000];
int __reverse__PAR_NNmap19[3000];
int __PAR_NNmap19[3000];
int __reverse__PAR_NNmap20[3000];
int __PAR_NNmap20[3000];
double __PAR_ex[3000];

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = TRUE;
	 settings->hybrid = TRUE;
	 settings->method = 3;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 2999)
	{
		dx[1] = d[(j0)]*((__PAR_vrest-x[(j0) * 3])+x[(j0+3000) * 3]*(__PAR_Eex-x[(j0) * 3])+x[(j0+6000) * 3]*(__PAR_Einh-x[(j0) * 3]))/__PAR_tau;
	}
	j0 = i0-3000;
	if(j0 >=0 && j0 <= 2999)
	{
		dx[1] = -x[(j0+3000) * 3]/__PAR_tauex;
	}
	j0 = i0-6000;
	if(j0 >=0 && j0 <= 2999)
	{
		dx[1] = -x[(j0+6000) * 3]/__PAR_tauinh;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 2999)
	{
	if (map[j0] > NOT_ASSIGNED)
{
		der[(j0) * 3 + 1] = d[(j0)]*((__PAR_vrest-x[(j0) * 3])+x[(j0+3000) * 3]*(__PAR_Eex-x[(j0) * 3])+x[(j0+6000) * 3]*(__PAR_Einh-x[(j0) * 3]))/__PAR_tau;
	}
	}
	j0 = i0-3000;
	if(j0 >=0 && j0 <= 2999)
	{
	if (map[j0] > NOT_ASSIGNED)
{
		der[(j0) * 3 + 1] = d[(j0)]*((__PAR_vrest-x[(j0) * 3])+x[(j0+3000) * 3]*(__PAR_Eex-x[(j0) * 3])+x[(j0+6000) * 3]*(__PAR_Einh-x[(j0) * 3]))/__PAR_tau;
	}
	if (map[j0+3000] > NOT_ASSIGNED)
{
		der[(j0+3000) * 3 + 1] = -x[(j0+3000) * 3]/__PAR_tauex;
	}
	}
	j0 = i0-6000;
	if(j0 >=0 && j0 <= 2999)
	{
	if (map[j0] > NOT_ASSIGNED)
{
		der[(j0) * 3 + 1] = d[(j0)]*((__PAR_vrest-x[(j0) * 3])+x[(j0+3000) * 3]*(__PAR_Eex-x[(j0) * 3])+x[(j0+6000) * 3]*(__PAR_Einh-x[(j0) * 3]))/__PAR_tau;
	}
	if (map[j0+6000] > NOT_ASSIGNED)
{
		der[(j0+6000) * 3 + 1] = -x[(j0+6000) * 3]/__PAR_tauinh;
	}
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 2999)
	{
		zc[0] = x[(i0) * 3]*__PAR_ex[(i0)]-(__PAR_vthres);
	}
	if(i0 >= 3000 && i0 <= 5999)
	{
		zc[0] = x[(i0-3000) * 3]*(1.0-__PAR_ex[(i0-3000)])-(__PAR_vthres);
	}
	if(i0 >= 6000 && i0 <= 8999)
	{
		zc[0] = d[(i0+200)]-(5.000000000000000000000000e-01);
	}
	if(i0 >= 9000 && i0 <= 11999)
	{
		zc[0] = d[(i0+200)]-(5.000000000000000000000000e-01);
	}
	if(i0 >= 12000 && i0 <= 14999)
	{
		zc[0] = t-(d[(i0-9000)]+__PAR_Trefrac);
	}
	if(i0 >= 15000 && i0 <= 15199)
	{
		zc[0] = t-(d[(i0-9000)]+d[(i0-2800)]);
	}
	if(i0 >= 15200 && i0 <= 15399)
	{
		zc[0] = t-(__PAR_lastTime);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 2999)
	{
		x[(i0) * 3] = __PAR_vrest;
		x[(i0+6000) * 3] = 0.0;
		x[(i0+3000) * 3] = 0.0;
		d[(__PAR_NNmap1[i0]+6199)] = 1.0;
		d[(__PAR_NNmap2[i0]+6199)] = 1.0;
		d[(__PAR_NNmap3[i0]+6199)] = 1.0;
		d[(__PAR_NNmap4[i0]+6199)] = 1.0;
		d[(__PAR_NNmap5[i0]+6199)] = 1.0;
		d[(__PAR_NNmap6[i0]+6199)] = 1.0;
		d[(__PAR_NNmap7[i0]+6199)] = 1.0;
		d[(__PAR_NNmap8[i0]+6199)] = 1.0;
		d[(__PAR_NNmap9[i0]+6199)] = 1.0;
		d[(__PAR_NNmap10[i0]+6199)] = 1.0;
		d[(__PAR_NNmap11[i0]+6199)] = 1.0;
		d[(__PAR_NNmap12[i0]+6199)] = 1.0;
		d[(__PAR_NNmap13[i0]+6199)] = 1.0;
		d[(__PAR_NNmap14[i0]+6199)] = 1.0;
		d[(__PAR_NNmap15[i0]+6199)] = 1.0;
		d[(__PAR_NNmap16[i0]+6199)] = 1.0;
		d[(__PAR_NNmap17[i0]+6199)] = 1.0;
		d[(__PAR_NNmap18[i0]+6199)] = 1.0;
		d[(__PAR_NNmap19[i0]+6199)] = 1.0;
		d[(__PAR_NNmap20[i0]+6199)] = 1.0;
		d[(i0)] = 0.0;
		d[(i0+3000)] = t;
	}
	if(i0 >= 3000 && i0 <= 5999)
	{
		x[(i0-3000) * 3] = __PAR_vrest;
		x[(i0+3000) * 3] = 0.0;
		x[(i0) * 3] = 0.0;
		d[(__PAR_NNmap1[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap2[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap3[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap4[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap5[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap6[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap7[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap8[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap9[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap10[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap11[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap12[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap13[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap14[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap15[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap16[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap17[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap18[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap19[i0-3000]+9199)] = 1.0;
		d[(__PAR_NNmap20[i0-3000]+9199)] = 1.0;
		d[(i0-3000)] = 0.0;
		d[(i0)] = t;
	}
	if(i0 >= 6000 && i0 <= 8999)
	{
		x[(i0-3000) * 3] = x[(i0-3000) * 3]+__PAR_dgex;
		d[(i0+200)] = 0.0;
	}
	if(i0 >= 9000 && i0 <= 11999)
	{
		x[(i0-3000) * 3] = x[(i0-3000) * 3]+__PAR_dgin;
		d[(i0+200)] = 0.0;
	}
	if(i0 >= 12000 && i0 <= 14999)
	{
		d[(i0-12000)] = 1.0;
	}
	if(i0 >= 15000 && i0 <= 15199)
	{
		d[(__PAR_SNmap1[i0-15000]+6199)] = 1.0;
		d[(i0-2600)] = d[(i0-2600)]+1.0;
		d[(i0-9000)] = t+10.0*pow(sin((i0-14999)*d[(i0-2600)]),2.0);
	}
	if(i0 >= 15200 && i0 <= 15399)
	{
		d[(i0-3000)] = 1.000000000000000000000000e+10;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 19)
	{
		out[0] = x[(150*j0) * 3];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(12600*sizeof(int));
	int *events = (int*)malloc(15400*sizeof(int));
	int *outputs = (int*)malloc(20*sizeof(int));
	int *states = (int*)malloc(9000*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(9000,12600,15400,0,0,"spl_neurons");
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
	__PAR_dgex = 4.000000000000000222044605e-01;
	__PAR_dgin = 1.600000000000000088817842e+00;
	__PAR_Period = 10.0;
	__PAR_lastTime = 100.0;
	for(i0 = 0; i0 <= 2999;i0++)
	{
		modelData->d[i0] = 1.0;
	}
	for(i0 = 0; i0 <= 199;i0++)
	{
		modelData->d[(i0+6000)] = __math__random(10.0);
	}
	for(i0 = 0; i0 <= 2999;i0++)
	{
		modelData->d[(i0+3000)] = 10000.0;
	}
	for(i0 = 0; i0 <= 2999;i0++)
	{
		modelData->x[i0 * 3] = -60.0;
	}
	// Initialize model code.
	for(i0 = 0; i0 <= 199; i0++)
	{
		__PAR_SNmap1[(i0)] = __math__random(3000);
	__reverse__PAR_SNmap1[__PAR_SNmap1[(i0)]-1] = i0+1;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		__PAR_NNmap1[(i0)] = __math__random(100);
	__reverse__PAR_NNmap1[__PAR_NNmap1[(i0)]-1] = i0+1;
		__PAR_NNmap2[(i0)] = __math__random(100);
	__reverse__PAR_NNmap2[__PAR_NNmap2[(i0)]-1] = i0+1;
		__PAR_NNmap3[(i0)] = __math__random(100);
	__reverse__PAR_NNmap3[__PAR_NNmap3[(i0)]-1] = i0+1;
		__PAR_NNmap4[(i0)] = __math__random(100);
	__reverse__PAR_NNmap4[__PAR_NNmap4[(i0)]-1] = i0+1;
		__PAR_NNmap5[(i0)] = __math__random(100);
	__reverse__PAR_NNmap5[__PAR_NNmap5[(i0)]-1] = i0+1;
		__PAR_NNmap6[(i0)] = __math__random(100);
	__reverse__PAR_NNmap6[__PAR_NNmap6[(i0)]-1] = i0+1;
		__PAR_NNmap7[(i0)] = __math__random(100);
	__reverse__PAR_NNmap7[__PAR_NNmap7[(i0)]-1] = i0+1;
		__PAR_NNmap8[(i0)] = __math__random(100);
	__reverse__PAR_NNmap8[__PAR_NNmap8[(i0)]-1] = i0+1;
		__PAR_NNmap9[(i0)] = __math__random(100);
	__reverse__PAR_NNmap9[__PAR_NNmap9[(i0)]-1] = i0+1;
		__PAR_NNmap10[(i0)] = __math__random(100);
	__reverse__PAR_NNmap10[__PAR_NNmap10[(i0)]-1] = i0+1;
		__PAR_NNmap11[(i0)] = __math__random(100);
	__reverse__PAR_NNmap11[__PAR_NNmap11[(i0)]-1] = i0+1;
		__PAR_NNmap12[(i0)] = __math__random(100);
	__reverse__PAR_NNmap12[__PAR_NNmap12[(i0)]-1] = i0+1;
		__PAR_NNmap13[(i0)] = __math__random(100);
	__reverse__PAR_NNmap13[__PAR_NNmap13[(i0)]-1] = i0+1;
		__PAR_NNmap14[(i0)] = __math__random(100);
	__reverse__PAR_NNmap14[__PAR_NNmap14[(i0)]-1] = i0+1;
		__PAR_NNmap15[(i0)] = __math__random(100);
	__reverse__PAR_NNmap15[__PAR_NNmap15[(i0)]-1] = i0+1;
		__PAR_NNmap16[(i0)] = __math__random(100);
	__reverse__PAR_NNmap16[__PAR_NNmap16[(i0)]-1] = i0+1;
		__PAR_NNmap17[(i0)] = __math__random(100);
	__reverse__PAR_NNmap17[__PAR_NNmap17[(i0)]-1] = i0+1;
		__PAR_NNmap18[(i0)] = __math__random(100);
	__reverse__PAR_NNmap18[__PAR_NNmap18[(i0)]-1] = i0+1;
		__PAR_NNmap19[(i0)] = __math__random(100);
	__reverse__PAR_NNmap19[__PAR_NNmap19[(i0)]-1] = i0+1;
		__PAR_NNmap20[(i0)] = __math__random(100);
	__reverse__PAR_NNmap20[__PAR_NNmap20[(i0)]-1] = i0+1;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		__PAR_ex[(i0)] = __math__random(100.0);
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nDS[i0] = 3;
		modelData->nDS[i0+3000] = 1;
		modelData->nDS[i0+6000] = 1;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nSD[i0]++;
		modelData->nSD[i0+3000]++;
		modelData->nSD[i0+6000]++;
		modelData->nSD[i0+3000]++;
		modelData->nSD[i0+6000]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nZS[i0] = 1;
		modelData->nZS[i0+3000] = 1;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nSZ[i0]++;
		modelData->nSZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 6000; i0 <= 8999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 9000; i0 <= 11999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 15000; i0 <= 15199; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 15000; i0 <= 15199; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 15200; i0 <= 15399; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 12000; i0 <= 14999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nSH[i0]++;
		modelData->nSH[i0+3000]++;
		modelData->nSH[i0+6000]++;
		modelData->nSH[i0]++;
		modelData->nSH[i0+3000]++;
		modelData->nSH[i0+6000]++;
		modelData->nSH[i0+3000]++;
		modelData->nSH[i0+6000]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->nDH[__PAR_NNmap1[i0]+6199]++;
		modelData->nDH[__PAR_NNmap10[i0]+6199]++;
		modelData->nDH[__PAR_NNmap11[i0]+6199]++;
		modelData->nDH[__PAR_NNmap12[i0]+6199]++;
		modelData->nDH[__PAR_NNmap13[i0]+6199]++;
		modelData->nDH[__PAR_NNmap14[i0]+6199]++;
		modelData->nDH[__PAR_NNmap15[i0]+6199]++;
		modelData->nDH[__PAR_NNmap16[i0]+6199]++;
		modelData->nDH[__PAR_NNmap17[i0]+6199]++;
		modelData->nDH[__PAR_NNmap18[i0]+6199]++;
		modelData->nDH[__PAR_NNmap19[i0]+6199]++;
		modelData->nDH[__PAR_NNmap2[i0]+6199]++;
		modelData->nDH[__PAR_NNmap20[i0]+6199]++;
		modelData->nDH[__PAR_NNmap3[i0]+6199]++;
		modelData->nDH[__PAR_NNmap4[i0]+6199]++;
		modelData->nDH[__PAR_NNmap5[i0]+6199]++;
		modelData->nDH[__PAR_NNmap6[i0]+6199]++;
		modelData->nDH[__PAR_NNmap7[i0]+6199]++;
		modelData->nDH[__PAR_NNmap8[i0]+6199]++;
		modelData->nDH[__PAR_NNmap9[i0]+6199]++;
		modelData->nDH[i0]++;
		modelData->nDH[i0+3000]++;
		modelData->nDH[__PAR_NNmap1[i0]+9199]++;
		modelData->nDH[__PAR_NNmap10[i0]+9199]++;
		modelData->nDH[__PAR_NNmap11[i0]+9199]++;
		modelData->nDH[__PAR_NNmap12[i0]+9199]++;
		modelData->nDH[__PAR_NNmap13[i0]+9199]++;
		modelData->nDH[__PAR_NNmap14[i0]+9199]++;
		modelData->nDH[__PAR_NNmap15[i0]+9199]++;
		modelData->nDH[__PAR_NNmap16[i0]+9199]++;
		modelData->nDH[__PAR_NNmap17[i0]+9199]++;
		modelData->nDH[__PAR_NNmap18[i0]+9199]++;
		modelData->nDH[__PAR_NNmap19[i0]+9199]++;
		modelData->nDH[__PAR_NNmap2[i0]+9199]++;
		modelData->nDH[__PAR_NNmap20[i0]+9199]++;
		modelData->nDH[__PAR_NNmap3[i0]+9199]++;
		modelData->nDH[__PAR_NNmap4[i0]+9199]++;
		modelData->nDH[__PAR_NNmap5[i0]+9199]++;
		modelData->nDH[__PAR_NNmap6[i0]+9199]++;
		modelData->nDH[__PAR_NNmap7[i0]+9199]++;
		modelData->nDH[__PAR_NNmap8[i0]+9199]++;
		modelData->nDH[__PAR_NNmap9[i0]+9199]++;
		modelData->nDH[i0]++;
		modelData->nDH[i0+3000]++;
		modelData->nDH[i0+6200]++;
		modelData->nDH[i0+9200]++;
		modelData->nDH[i0]++;
	}
	for(i0 = 0; i0 <= 199; i0++)
	{
	modelData->nDH[__PAR_SNmap1[i0]+6199]++;
	modelData->nDH[i0+6000]++;
	modelData->nDH[i0+12400]++;
	modelData->nDH[i0+12200]++;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0].nLHSSt = 3;
		modelData->event[i0+3000].nLHSSt = 3;
		modelData->event[i0+6000].nLHSSt = 1;
		modelData->event[i0+9000].nLHSSt = 1;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0+6000].nRHSSt = 1;
		modelData->event[i0+9000].nRHSSt = 1;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0].nLHSDsc = 22;
		modelData->event[i0+3000].nLHSDsc = 22;
		modelData->event[i0+6000].nLHSDsc = 1;
		modelData->event[i0+9000].nLHSDsc = 1;
		modelData->event[i0+12000].nLHSDsc = 1;
	}
	for(i0 = 0; i0 <= 199; i0++)
	{
	modelData->event[i0+15000].nLHSDsc = 3;
	modelData->event[i0+15200].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,9000);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = i0+3000;
		modelData->DS[i0][states[i0]++] = i0+6000;
		modelData->DS[i0+3000][states[i0+3000]++] = i0+3000;
		modelData->DS[i0+6000][states[i0+6000]++] = i0+6000;
	}
		cleanVector(states,0,9000);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+3000][states[i0+3000]++] = i0;
		modelData->SD[i0+6000][states[i0+6000]++] = i0;
		modelData->SD[i0+3000][states[i0+3000]++] = i0+3000;
		modelData->SD[i0+6000][states[i0+6000]++] = i0+6000;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
		modelData->ZS[i0+3000][events[i0+3000]++] = i0;
	}
		cleanVector(states,0,9000);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
		modelData->SZ[i0][states[i0]++] = i0+3000;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap1[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap10[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap11[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap12[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap13[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap14[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap15[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap16[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap17[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap18[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap19[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap2[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap20[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap3[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap4[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap5[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap6[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap7[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap8[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap9[i0]+5999;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+12000;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+3000;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap1[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap10[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap11[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap12[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap13[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap14[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap15[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap16[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap17[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap18[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap19[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap2[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap20[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap3[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap4[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap5[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap6[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap7[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap8[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_NNmap9[i0-3000]+8999;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+9000;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0-3000;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 6000; i0 <= 8999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 9000; i0 <= 11999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 15000; i0 <= 15199; i0++)
	{
		modelData->HZ[i0][events[i0]++] = __PAR_SNmap1[i0-15000]+5999;
	}
	for(i0 = 15000; i0 <= 15199; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 15200; i0 <= 15399; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0-200;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 3000; i0 <= 5999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-3000;
	}
	for(i0 = 12000; i0 <= 14999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-12000;
	}
		cleanVector(states,0,9000);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->SH[i0][states[i0]++] = i0;
		modelData->SH[i0+3000][states[i0+3000]++] = i0;
		modelData->SH[i0+6000][states[i0+6000]++] = i0;
		modelData->SH[i0][states[i0]++] = i0+3000;
		modelData->SH[i0+3000][states[i0+3000]++] = i0+3000;
		modelData->SH[i0+6000][states[i0+6000]++] = i0+3000;
		modelData->SH[i0+3000][states[i0+3000]++] = i0+6000;
		modelData->SH[i0+6000][states[i0+6000]++] = i0+9000;
	}
		cleanVector(discretes,0,12600);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->DH[__PAR_NNmap1[i0]+6199][discretes[__PAR_NNmap1[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap10[i0]+6199][discretes[__PAR_NNmap10[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap11[i0]+6199][discretes[__PAR_NNmap11[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap12[i0]+6199][discretes[__PAR_NNmap12[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap13[i0]+6199][discretes[__PAR_NNmap13[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap14[i0]+6199][discretes[__PAR_NNmap14[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap15[i0]+6199][discretes[__PAR_NNmap15[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap16[i0]+6199][discretes[__PAR_NNmap16[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap17[i0]+6199][discretes[__PAR_NNmap17[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap18[i0]+6199][discretes[__PAR_NNmap18[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap19[i0]+6199][discretes[__PAR_NNmap19[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap2[i0]+6199][discretes[__PAR_NNmap2[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap20[i0]+6199][discretes[__PAR_NNmap20[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap3[i0]+6199][discretes[__PAR_NNmap3[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap4[i0]+6199][discretes[__PAR_NNmap4[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap5[i0]+6199][discretes[__PAR_NNmap5[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap6[i0]+6199][discretes[__PAR_NNmap6[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap7[i0]+6199][discretes[__PAR_NNmap7[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap8[i0]+6199][discretes[__PAR_NNmap8[i0]+6199]++] = i0;
		modelData->DH[__PAR_NNmap9[i0]+6199][discretes[__PAR_NNmap9[i0]+6199]++] = i0;
		modelData->DH[i0][discretes[i0]++] = i0;
		modelData->DH[i0+3000][discretes[i0+3000]++] = i0;
		modelData->DH[__PAR_NNmap1[i0]+9199][discretes[__PAR_NNmap1[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap10[i0]+9199][discretes[__PAR_NNmap10[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap11[i0]+9199][discretes[__PAR_NNmap11[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap12[i0]+9199][discretes[__PAR_NNmap12[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap13[i0]+9199][discretes[__PAR_NNmap13[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap14[i0]+9199][discretes[__PAR_NNmap14[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap15[i0]+9199][discretes[__PAR_NNmap15[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap16[i0]+9199][discretes[__PAR_NNmap16[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap17[i0]+9199][discretes[__PAR_NNmap17[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap18[i0]+9199][discretes[__PAR_NNmap18[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap19[i0]+9199][discretes[__PAR_NNmap19[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap2[i0]+9199][discretes[__PAR_NNmap2[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap20[i0]+9199][discretes[__PAR_NNmap20[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap3[i0]+9199][discretes[__PAR_NNmap3[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap4[i0]+9199][discretes[__PAR_NNmap4[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap5[i0]+9199][discretes[__PAR_NNmap5[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap6[i0]+9199][discretes[__PAR_NNmap6[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap7[i0]+9199][discretes[__PAR_NNmap7[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap8[i0]+9199][discretes[__PAR_NNmap8[i0]+9199]++] = i0+3000;
		modelData->DH[__PAR_NNmap9[i0]+9199][discretes[__PAR_NNmap9[i0]+9199]++] = i0+3000;
		modelData->DH[i0][discretes[i0]++] = i0+3000;
		modelData->DH[i0+3000][discretes[i0+3000]++] = i0+3000;
		modelData->DH[i0+6200][discretes[i0+6200]++] = i0+6000;
		modelData->DH[i0+9200][discretes[i0+9200]++] = i0+9000;
		modelData->DH[i0][discretes[i0]++] = i0+12000;
	}
	for(i0 = 0; i0 <= 199; i0++)
	{
	modelData->DH[__PAR_SNmap1[i0]+6199][discretes[__PAR_SNmap1[i0]+6199]++] = i0+15000;
	modelData->DH[i0+6000][discretes[i0+6000]++] = i0+15000;
	modelData->DH[i0+12400][discretes[i0+12400]++] = i0+15000;
	modelData->DH[i0+12200][discretes[i0+12200]++] = i0+15200;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0].LHSSt[events[i0]++] = i0;
		modelData->event[i0].LHSSt[events[i0]++] = i0+3000;
		modelData->event[i0].LHSSt[events[i0]++] = i0+6000;
		modelData->event[i0+3000].LHSSt[events[i0+3000]++] = i0;
		modelData->event[i0+3000].LHSSt[events[i0+3000]++] = i0+3000;
		modelData->event[i0+3000].LHSSt[events[i0+3000]++] = i0+6000;
		modelData->event[i0+6000].LHSSt[events[i0+6000]++] = i0+3000;
		modelData->event[i0+9000].LHSSt[events[i0+9000]++] = i0+6000;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0+6000].RHSSt[events[i0+6000]++] = i0+3000;
		modelData->event[i0+9000].RHSSt[events[i0+9000]++] = i0+6000;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap1[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap10[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap11[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap12[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap13[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap14[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap15[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap16[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap17[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap18[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap19[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap2[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap20[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap3[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap4[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap5[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap6[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap7[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap8[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = __PAR_NNmap9[i0]+6199;
		modelData->event[i0].LHSDsc[events[i0]++] = i0;
		modelData->event[i0].LHSDsc[events[i0]++] = i0+3000;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap1[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap10[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap11[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap12[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap13[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap14[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap15[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap16[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap17[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap18[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap19[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap2[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap20[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap3[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap4[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap5[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap6[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap7[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap8[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = __PAR_NNmap9[i0]+9199;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = i0;
		modelData->event[i0+3000].LHSDsc[events[i0+3000]++] = i0+3000;
		modelData->event[i0+6000].LHSDsc[events[i0+6000]++] = i0+6200;
		modelData->event[i0+9000].LHSDsc[events[i0+9000]++] = i0+9200;
		modelData->event[i0+12000].LHSDsc[events[i0+12000]++] = i0;
	}
	for(i0 = 0; i0 <= 199; i0++)
	{
	modelData->event[i0+15000].LHSDsc[events[i0+15000]++] = __PAR_SNmap1[i0]+6199;
	modelData->event[i0+15000].LHSDsc[events[i0+15000]++] = i0+6000;
	modelData->event[i0+15000].LHSDsc[events[i0+15000]++] = i0+12400;
	modelData->event[i0+15200].LHSDsc[events[i0+15200]++] = i0+12200;
	}
		cleanVector(events,0,15400);

	for(i0 = 0; i0 <= 2999; i0++)
	{
		modelData->event[i0].direction = 1;
		modelData->event[i0].relation = 2;
		modelData->event[i0+3000].direction = 1;
		modelData->event[i0+3000].relation = 2;
		modelData->event[i0+6000].direction = 1;
		modelData->event[i0+6000].relation = 2;
		modelData->event[i0+9000].direction = 1;
		modelData->event[i0+9000].relation = 2;
		modelData->event[i0+12000].direction = 1;
		modelData->event[i0+12000].relation = 2;
	}
	for(i0 = 0; i0 <= 199; i0++)
	{
	modelData->event[i0+15000].direction = 1;
	modelData->event[i0+15000].relation = 2;
	modelData->event[i0+15200].direction = 1;
	modelData->event[i0+15200].relation = 2;
	}
	simulator->time = QSS_Time(9000,15400,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.06;
	simulator->output = SD_Output("spl_neurons",20,12600,9000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 19; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[150*i0]++;
	}
	SD_allocOutputMatrix(modelOutput,9000,12600);
		cleanVector(states,0,9000);

		cleanVector(outputs,0,20);

	for(i0 = 0; i0 <= 19; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"v[%d]",150*i0+1);
	}
		cleanVector(outputs,0,20);

	for(i0 = 0; i0 <= 19; i0++)
	{
		modelOutput->SO[150*i0][states[150*i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = 150*i0;
	}
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
