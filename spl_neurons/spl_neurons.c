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
	int j = 0;
	j = i;
	if(j >=0 && j <= 2999)
	{
		dx[1] = d[(j)]*((__PAR_vrest-x[(j) * 3])+x[(j+3000) * 3]*(__PAR_Eex-x[(j) * 3])+x[(j+6000) * 3]*(__PAR_Einh-x[(j) * 3]))/__PAR_tau;
	}
	j = i-3000;
	if(j >=0 && j <= 2999)
	{
		dx[1] = -x[(j+3000) * 3]/__PAR_tauex;
	}
	j = i-6000;
	if(j >=0 && j <= 2999)
	{
		dx[1] = -x[(j+6000) * 3]/__PAR_tauinh;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 2999)
	{
	if (map[j] > NOT_ASSIGNED)
{
		der[(j) * 3 + 1] = d[(j)]*((__PAR_vrest-x[(j) * 3])+x[(j+3000) * 3]*(__PAR_Eex-x[(j) * 3])+x[(j+6000) * 3]*(__PAR_Einh-x[(j) * 3]))/__PAR_tau;
	}
	}
	j = i-3000;
	if(j >=0 && j <= 2999)
	{
	if (map[j] > NOT_ASSIGNED)
{
		der[(j) * 3 + 1] = d[(j)]*((__PAR_vrest-x[(j) * 3])+x[(j+3000) * 3]*(__PAR_Eex-x[(j) * 3])+x[(j+6000) * 3]*(__PAR_Einh-x[(j) * 3]))/__PAR_tau;
	}
	if (map[j+3000] > NOT_ASSIGNED)
{
		der[(j+3000) * 3 + 1] = -x[(j+3000) * 3]/__PAR_tauex;
	}
	}
	j = i-6000;
	if(j >=0 && j <= 2999)
	{
	if (map[j] > NOT_ASSIGNED)
{
		der[(j) * 3 + 1] = d[(j)]*((__PAR_vrest-x[(j) * 3])+x[(j+3000) * 3]*(__PAR_Eex-x[(j) * 3])+x[(j+6000) * 3]*(__PAR_Einh-x[(j) * 3]))/__PAR_tau;
	}
	if (map[j+6000] > NOT_ASSIGNED)
{
		der[(j+6000) * 3 + 1] = -x[(j+6000) * 3]/__PAR_tauinh;
	}
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	if(i >= 0 && i <= 2999)
	{
		zc[0] = x[(i) * 3]*__PAR_ex[(i)]-(__PAR_vthres);
	}
	if(i >= 3000 && i <= 5999)
	{
		zc[0] = x[(i-3000) * 3]*(1.0-__PAR_ex[(i-3000)])-(__PAR_vthres);
	}
	if(i >= 6000 && i <= 8999)
	{
		zc[0] = d[(i+200)]-(5.000000000000000000000000e-01);
	}
	if(i >= 9000 && i <= 11999)
	{
		zc[0] = d[(i+200)]-(5.000000000000000000000000e-01);
	}
	if(i >= 12000 && i <= 14999)
	{
		zc[0] = t-(d[(i-9000)]+__PAR_Trefrac);
	}
	if(i >= 15000 && i <= 15199)
	{
		zc[0] = t-(d[(i-9000)]+d[(i-2800)]);
	}
	if(i >= 15200 && i <= 15399)
	{
		zc[0] = t-(__PAR_lastTime);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 2999)
	{
		x[(i) * 3] = __PAR_vrest;
		x[(i+6000) * 3] = 0.0;
		x[(i+3000) * 3] = 0.0;
		d[(__PAR_NNmap1[i]+6199)] = 1.0;
		d[(__PAR_NNmap2[i]+6199)] = 1.0;
		d[(__PAR_NNmap3[i]+6199)] = 1.0;
		d[(__PAR_NNmap4[i]+6199)] = 1.0;
		d[(__PAR_NNmap5[i]+6199)] = 1.0;
		d[(__PAR_NNmap6[i]+6199)] = 1.0;
		d[(__PAR_NNmap7[i]+6199)] = 1.0;
		d[(__PAR_NNmap8[i]+6199)] = 1.0;
		d[(__PAR_NNmap9[i]+6199)] = 1.0;
		d[(__PAR_NNmap10[i]+6199)] = 1.0;
		d[(__PAR_NNmap11[i]+6199)] = 1.0;
		d[(__PAR_NNmap12[i]+6199)] = 1.0;
		d[(__PAR_NNmap13[i]+6199)] = 1.0;
		d[(__PAR_NNmap14[i]+6199)] = 1.0;
		d[(__PAR_NNmap15[i]+6199)] = 1.0;
		d[(__PAR_NNmap16[i]+6199)] = 1.0;
		d[(__PAR_NNmap17[i]+6199)] = 1.0;
		d[(__PAR_NNmap18[i]+6199)] = 1.0;
		d[(__PAR_NNmap19[i]+6199)] = 1.0;
		d[(__PAR_NNmap20[i]+6199)] = 1.0;
		d[(i)] = 0.0;
		d[(i+3000)] = t;
	}
	if(i >= 3000 && i <= 5999)
	{
		x[(i-3000) * 3] = __PAR_vrest;
		x[(i+3000) * 3] = 0.0;
		x[(i) * 3] = 0.0;
		d[(__PAR_NNmap1[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap2[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap3[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap4[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap5[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap6[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap7[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap8[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap9[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap10[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap11[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap12[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap13[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap14[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap15[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap16[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap17[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap18[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap19[i-3000]+9199)] = 1.0;
		d[(__PAR_NNmap20[i-3000]+9199)] = 1.0;
		d[(i-3000)] = 0.0;
		d[(i)] = t;
	}
	if(i >= 6000 && i <= 8999)
	{
		x[(i-3000) * 3] = x[(i-3000) * 3]+__PAR_dgex;
		d[(i+200)] = 0.0;
	}
	if(i >= 9000 && i <= 11999)
	{
		x[(i-3000) * 3] = x[(i-3000) * 3]+__PAR_dgin;
		d[(i+200)] = 0.0;
	}
	if(i >= 12000 && i <= 14999)
	{
		d[(i-12000)] = 1.0;
	}
	if(i >= 15000 && i <= 15199)
	{
		d[(__PAR_SNmap1[i-15000]+6199)] = 1.0;
		d[(i-2600)] = d[(i-2600)]+1.0;
		d[(i-9000)] = t+10.0*pow(sin((i-14999)*d[(i-2600)]),2.0);
	}
	if(i >= 15200 && i <= 15399)
	{
		d[(i-3000)] = 1.000000000000000000000000e+10;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 19)
	{
		out[0] = x[(150*j) * 3];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(12600*sizeof(int));
	int *events = (int*)malloc(15400*sizeof(int));
	int *outputs = (int*)malloc(20*sizeof(int));
	int *states = (int*)malloc(9000*sizeof(int));
	int i25;
	int i2;
	int i4;
	int i;
	int j = 0;
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
	for(i = 0; i <= 2999;i++)
	{
		modelData->d[i] = 1.0;
	}
	for(i = 0; i <= 199;i++)
	{
		modelData->d[(i+6000)] = __math__random(10.0);
	}
	for(i = 0; i <= 2999;i++)
	{
		modelData->d[(i+3000)] = 10000.0;
	}
	for(i = 0; i <= 2999;i++)
	{
		modelData->x[i * 3] = -60.0;
	}
	// Initialize model code.
	for(i2 = 0; i2 <= 199; i2++)
	{
		__PAR_SNmap1[(i2)] = __math__random(3000);
	__reverse__PAR_SNmap1[__PAR_SNmap1[(i2)]-1] = i2+1;
	}
	for(i4 = 0; i4 <= 2999; i4++)
	{
		__PAR_NNmap1[(i4)] = __math__random(100);
	__reverse__PAR_NNmap1[__PAR_NNmap1[(i4)]-1] = i4+1;
		__PAR_NNmap2[(i4)] = __math__random(100);
	__reverse__PAR_NNmap2[__PAR_NNmap2[(i4)]-1] = i4+1;
		__PAR_NNmap3[(i4)] = __math__random(100);
	__reverse__PAR_NNmap3[__PAR_NNmap3[(i4)]-1] = i4+1;
		__PAR_NNmap4[(i4)] = __math__random(100);
	__reverse__PAR_NNmap4[__PAR_NNmap4[(i4)]-1] = i4+1;
		__PAR_NNmap5[(i4)] = __math__random(100);
	__reverse__PAR_NNmap5[__PAR_NNmap5[(i4)]-1] = i4+1;
		__PAR_NNmap6[(i4)] = __math__random(100);
	__reverse__PAR_NNmap6[__PAR_NNmap6[(i4)]-1] = i4+1;
		__PAR_NNmap7[(i4)] = __math__random(100);
	__reverse__PAR_NNmap7[__PAR_NNmap7[(i4)]-1] = i4+1;
		__PAR_NNmap8[(i4)] = __math__random(100);
	__reverse__PAR_NNmap8[__PAR_NNmap8[(i4)]-1] = i4+1;
		__PAR_NNmap9[(i4)] = __math__random(100);
	__reverse__PAR_NNmap9[__PAR_NNmap9[(i4)]-1] = i4+1;
		__PAR_NNmap10[(i4)] = __math__random(100);
	__reverse__PAR_NNmap10[__PAR_NNmap10[(i4)]-1] = i4+1;
		__PAR_NNmap11[(i4)] = __math__random(100);
	__reverse__PAR_NNmap11[__PAR_NNmap11[(i4)]-1] = i4+1;
		__PAR_NNmap12[(i4)] = __math__random(100);
	__reverse__PAR_NNmap12[__PAR_NNmap12[(i4)]-1] = i4+1;
		__PAR_NNmap13[(i4)] = __math__random(100);
	__reverse__PAR_NNmap13[__PAR_NNmap13[(i4)]-1] = i4+1;
		__PAR_NNmap14[(i4)] = __math__random(100);
	__reverse__PAR_NNmap14[__PAR_NNmap14[(i4)]-1] = i4+1;
		__PAR_NNmap15[(i4)] = __math__random(100);
	__reverse__PAR_NNmap15[__PAR_NNmap15[(i4)]-1] = i4+1;
		__PAR_NNmap16[(i4)] = __math__random(100);
	__reverse__PAR_NNmap16[__PAR_NNmap16[(i4)]-1] = i4+1;
		__PAR_NNmap17[(i4)] = __math__random(100);
	__reverse__PAR_NNmap17[__PAR_NNmap17[(i4)]-1] = i4+1;
		__PAR_NNmap18[(i4)] = __math__random(100);
	__reverse__PAR_NNmap18[__PAR_NNmap18[(i4)]-1] = i4+1;
		__PAR_NNmap19[(i4)] = __math__random(100);
	__reverse__PAR_NNmap19[__PAR_NNmap19[(i4)]-1] = i4+1;
		__PAR_NNmap20[(i4)] = __math__random(100);
	__reverse__PAR_NNmap20[__PAR_NNmap20[(i4)]-1] = i4+1;
	}
	for(i25 = 0; i25 <= 2999; i25++)
	{
		__PAR_ex[(i25)] = __math__random(100.0);
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nDS[i] = 3;
		modelData->nDS[i+3000] = 1;
		modelData->nDS[i+6000] = 1;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nSD[i]++;
		modelData->nSD[i+3000]++;
		modelData->nSD[i+6000]++;
		modelData->nSD[i+3000]++;
		modelData->nSD[i+6000]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nZS[i] = 1;
		modelData->nZS[i+3000] = 1;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nSZ[i]++;
		modelData->nSZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 6000; i <= 8999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 9000; i <= 11999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 15000; i <= 15199; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 15000; i <= 15199; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 15200; i <= 15399; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 12000; i <= 14999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nSH[i]++;
		modelData->nSH[i+3000]++;
		modelData->nSH[i+6000]++;
		modelData->nSH[i]++;
		modelData->nSH[i+3000]++;
		modelData->nSH[i+6000]++;
		modelData->nSH[i+3000]++;
		modelData->nSH[i+6000]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->nDH[__PAR_NNmap1[i]+6199]++;
		modelData->nDH[__PAR_NNmap10[i]+6199]++;
		modelData->nDH[__PAR_NNmap11[i]+6199]++;
		modelData->nDH[__PAR_NNmap12[i]+6199]++;
		modelData->nDH[__PAR_NNmap13[i]+6199]++;
		modelData->nDH[__PAR_NNmap14[i]+6199]++;
		modelData->nDH[__PAR_NNmap15[i]+6199]++;
		modelData->nDH[__PAR_NNmap16[i]+6199]++;
		modelData->nDH[__PAR_NNmap17[i]+6199]++;
		modelData->nDH[__PAR_NNmap18[i]+6199]++;
		modelData->nDH[__PAR_NNmap19[i]+6199]++;
		modelData->nDH[__PAR_NNmap2[i]+6199]++;
		modelData->nDH[__PAR_NNmap20[i]+6199]++;
		modelData->nDH[__PAR_NNmap3[i]+6199]++;
		modelData->nDH[__PAR_NNmap4[i]+6199]++;
		modelData->nDH[__PAR_NNmap5[i]+6199]++;
		modelData->nDH[__PAR_NNmap6[i]+6199]++;
		modelData->nDH[__PAR_NNmap7[i]+6199]++;
		modelData->nDH[__PAR_NNmap8[i]+6199]++;
		modelData->nDH[__PAR_NNmap9[i]+6199]++;
		modelData->nDH[i]++;
		modelData->nDH[i+3000]++;
		modelData->nDH[__PAR_NNmap1[i]+9199]++;
		modelData->nDH[__PAR_NNmap10[i]+9199]++;
		modelData->nDH[__PAR_NNmap11[i]+9199]++;
		modelData->nDH[__PAR_NNmap12[i]+9199]++;
		modelData->nDH[__PAR_NNmap13[i]+9199]++;
		modelData->nDH[__PAR_NNmap14[i]+9199]++;
		modelData->nDH[__PAR_NNmap15[i]+9199]++;
		modelData->nDH[__PAR_NNmap16[i]+9199]++;
		modelData->nDH[__PAR_NNmap17[i]+9199]++;
		modelData->nDH[__PAR_NNmap18[i]+9199]++;
		modelData->nDH[__PAR_NNmap19[i]+9199]++;
		modelData->nDH[__PAR_NNmap2[i]+9199]++;
		modelData->nDH[__PAR_NNmap20[i]+9199]++;
		modelData->nDH[__PAR_NNmap3[i]+9199]++;
		modelData->nDH[__PAR_NNmap4[i]+9199]++;
		modelData->nDH[__PAR_NNmap5[i]+9199]++;
		modelData->nDH[__PAR_NNmap6[i]+9199]++;
		modelData->nDH[__PAR_NNmap7[i]+9199]++;
		modelData->nDH[__PAR_NNmap8[i]+9199]++;
		modelData->nDH[__PAR_NNmap9[i]+9199]++;
		modelData->nDH[i]++;
		modelData->nDH[i+3000]++;
		modelData->nDH[i+6200]++;
		modelData->nDH[i+9200]++;
		modelData->nDH[i]++;
	}
	for(i = 0; i <= 199; i++)
	{
	modelData->nDH[__PAR_SNmap1[i]+6199]++;
	modelData->nDH[i+6000]++;
	modelData->nDH[i+12400]++;
	modelData->nDH[i+12200]++;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i].nLHSSt = 3;
		modelData->event[i+3000].nLHSSt = 3;
		modelData->event[i+6000].nLHSSt = 1;
		modelData->event[i+9000].nLHSSt = 1;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i+6000].nRHSSt = 1;
		modelData->event[i+9000].nRHSSt = 1;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i].nLHSDsc = 22;
		modelData->event[i+3000].nLHSDsc = 22;
		modelData->event[i+6000].nLHSDsc = 1;
		modelData->event[i+9000].nLHSDsc = 1;
		modelData->event[i+12000].nLHSDsc = 1;
	}
	for(i = 0; i <= 199; i++)
	{
	modelData->event[i+15000].nLHSDsc = 3;
	modelData->event[i+15200].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,9000);

	for(i = 0; i <= 2999; i++)
	{
		modelData->DS[i][states[i]++] = i;
		modelData->DS[i][states[i]++] = i+3000;
		modelData->DS[i][states[i]++] = i+6000;
		modelData->DS[i+3000][states[i+3000]++] = i+3000;
		modelData->DS[i+6000][states[i+6000]++] = i+6000;
	}
		cleanVector(states,0,9000);

	for(i = 0; i <= 2999; i++)
	{
		modelData->SD[i][states[i]++] = i;
		modelData->SD[i+3000][states[i+3000]++] = i;
		modelData->SD[i+6000][states[i+6000]++] = i;
		modelData->SD[i+3000][states[i+3000]++] = i+3000;
		modelData->SD[i+6000][states[i+6000]++] = i+6000;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->ZS[i][events[i]++] = i;
		modelData->ZS[i+3000][events[i+3000]++] = i;
	}
		cleanVector(states,0,9000);

	for(i = 0; i <= 2999; i++)
	{
		modelData->SZ[i][states[i]++] = i;
		modelData->SZ[i][states[i]++] = i+3000;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap1[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap10[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap11[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap12[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap13[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap14[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap15[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap16[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap17[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap18[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap19[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap2[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap20[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap3[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap4[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap5[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap6[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap7[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap8[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap9[i]+5999;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = i+12000;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 0; i <= 2999; i++)
	{
		modelData->HZ[i][events[i]++] = i+3000;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap1[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap10[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap11[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap12[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap13[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap14[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap15[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap16[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap17[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap18[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap19[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap2[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap20[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap3[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap4[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap5[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap6[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap7[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap8[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_NNmap9[i-3000]+8999;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = i+9000;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = i-3000;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 6000; i <= 8999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 9000; i <= 11999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 15000; i <= 15199; i++)
	{
		modelData->HZ[i][events[i]++] = __PAR_SNmap1[i-15000]+5999;
	}
	for(i = 15000; i <= 15199; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 15200; i <= 15399; i++)
	{
		modelData->HZ[i][events[i]++] = i-200;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 3000; i <= 5999; i++)
	{
		modelData->HD[i][events[i]++] = i-3000;
	}
	for(i = 12000; i <= 14999; i++)
	{
		modelData->HD[i][events[i]++] = i-12000;
	}
		cleanVector(states,0,9000);

	for(i = 0; i <= 2999; i++)
	{
		modelData->SH[i][states[i]++] = i;
		modelData->SH[i+3000][states[i+3000]++] = i;
		modelData->SH[i+6000][states[i+6000]++] = i;
		modelData->SH[i][states[i]++] = i+3000;
		modelData->SH[i+3000][states[i+3000]++] = i+3000;
		modelData->SH[i+6000][states[i+6000]++] = i+3000;
		modelData->SH[i+3000][states[i+3000]++] = i+6000;
		modelData->SH[i+6000][states[i+6000]++] = i+9000;
	}
		cleanVector(discretes,0,12600);

	for(i = 0; i <= 2999; i++)
	{
		modelData->DH[__PAR_NNmap1[i]+6199][discretes[__PAR_NNmap1[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap10[i]+6199][discretes[__PAR_NNmap10[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap11[i]+6199][discretes[__PAR_NNmap11[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap12[i]+6199][discretes[__PAR_NNmap12[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap13[i]+6199][discretes[__PAR_NNmap13[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap14[i]+6199][discretes[__PAR_NNmap14[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap15[i]+6199][discretes[__PAR_NNmap15[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap16[i]+6199][discretes[__PAR_NNmap16[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap17[i]+6199][discretes[__PAR_NNmap17[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap18[i]+6199][discretes[__PAR_NNmap18[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap19[i]+6199][discretes[__PAR_NNmap19[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap2[i]+6199][discretes[__PAR_NNmap2[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap20[i]+6199][discretes[__PAR_NNmap20[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap3[i]+6199][discretes[__PAR_NNmap3[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap4[i]+6199][discretes[__PAR_NNmap4[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap5[i]+6199][discretes[__PAR_NNmap5[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap6[i]+6199][discretes[__PAR_NNmap6[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap7[i]+6199][discretes[__PAR_NNmap7[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap8[i]+6199][discretes[__PAR_NNmap8[i]+6199]++] = i;
		modelData->DH[__PAR_NNmap9[i]+6199][discretes[__PAR_NNmap9[i]+6199]++] = i;
		modelData->DH[i][discretes[i]++] = i;
		modelData->DH[i+3000][discretes[i+3000]++] = i;
		modelData->DH[__PAR_NNmap1[i]+9199][discretes[__PAR_NNmap1[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap10[i]+9199][discretes[__PAR_NNmap10[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap11[i]+9199][discretes[__PAR_NNmap11[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap12[i]+9199][discretes[__PAR_NNmap12[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap13[i]+9199][discretes[__PAR_NNmap13[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap14[i]+9199][discretes[__PAR_NNmap14[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap15[i]+9199][discretes[__PAR_NNmap15[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap16[i]+9199][discretes[__PAR_NNmap16[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap17[i]+9199][discretes[__PAR_NNmap17[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap18[i]+9199][discretes[__PAR_NNmap18[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap19[i]+9199][discretes[__PAR_NNmap19[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap2[i]+9199][discretes[__PAR_NNmap2[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap20[i]+9199][discretes[__PAR_NNmap20[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap3[i]+9199][discretes[__PAR_NNmap3[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap4[i]+9199][discretes[__PAR_NNmap4[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap5[i]+9199][discretes[__PAR_NNmap5[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap6[i]+9199][discretes[__PAR_NNmap6[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap7[i]+9199][discretes[__PAR_NNmap7[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap8[i]+9199][discretes[__PAR_NNmap8[i]+9199]++] = i+3000;
		modelData->DH[__PAR_NNmap9[i]+9199][discretes[__PAR_NNmap9[i]+9199]++] = i+3000;
		modelData->DH[i][discretes[i]++] = i+3000;
		modelData->DH[i+3000][discretes[i+3000]++] = i+3000;
		modelData->DH[i+6200][discretes[i+6200]++] = i+6000;
		modelData->DH[i+9200][discretes[i+9200]++] = i+9000;
		modelData->DH[i][discretes[i]++] = i+12000;
	}
	for(i = 0; i <= 199; i++)
	{
	modelData->DH[__PAR_SNmap1[i]+6199][discretes[__PAR_SNmap1[i]+6199]++] = i+15000;
	modelData->DH[i+6000][discretes[i+6000]++] = i+15000;
	modelData->DH[i+12400][discretes[i+12400]++] = i+15000;
	modelData->DH[i+12200][discretes[i+12200]++] = i+15200;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i].LHSSt[events[i]++] = i;
		modelData->event[i].LHSSt[events[i]++] = i+3000;
		modelData->event[i].LHSSt[events[i]++] = i+6000;
		modelData->event[i+3000].LHSSt[events[i+3000]++] = i;
		modelData->event[i+3000].LHSSt[events[i+3000]++] = i+3000;
		modelData->event[i+3000].LHSSt[events[i+3000]++] = i+6000;
		modelData->event[i+6000].LHSSt[events[i+6000]++] = i+3000;
		modelData->event[i+9000].LHSSt[events[i+9000]++] = i+6000;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i+6000].RHSSt[events[i+6000]++] = i+3000;
		modelData->event[i+9000].RHSSt[events[i+9000]++] = i+6000;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap1[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap10[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap11[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap12[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap13[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap14[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap15[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap16[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap17[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap18[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap19[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap2[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap20[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap3[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap4[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap5[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap6[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap7[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap8[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = __PAR_NNmap9[i]+6199;
		modelData->event[i].LHSDsc[events[i]++] = i;
		modelData->event[i].LHSDsc[events[i]++] = i+3000;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap1[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap10[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap11[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap12[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap13[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap14[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap15[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap16[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap17[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap18[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap19[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap2[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap20[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap3[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap4[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap5[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap6[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap7[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap8[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = __PAR_NNmap9[i]+9199;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = i;
		modelData->event[i+3000].LHSDsc[events[i+3000]++] = i+3000;
		modelData->event[i+6000].LHSDsc[events[i+6000]++] = i+6200;
		modelData->event[i+9000].LHSDsc[events[i+9000]++] = i+9200;
		modelData->event[i+12000].LHSDsc[events[i+12000]++] = i;
	}
	for(i = 0; i <= 199; i++)
	{
	modelData->event[i+15000].LHSDsc[events[i+15000]++] = __PAR_SNmap1[i]+6199;
	modelData->event[i+15000].LHSDsc[events[i+15000]++] = i+6000;
	modelData->event[i+15000].LHSDsc[events[i+15000]++] = i+12400;
	modelData->event[i+15200].LHSDsc[events[i+15200]++] = i+12200;
	}
		cleanVector(events,0,15400);

	for(i = 0; i <= 2999; i++)
	{
		modelData->event[i].direction = 1;
		modelData->event[i].relation = 2;
		modelData->event[i+3000].direction = 1;
		modelData->event[i+3000].relation = 2;
		modelData->event[i+6000].direction = 1;
		modelData->event[i+6000].relation = 2;
		modelData->event[i+9000].direction = 1;
		modelData->event[i+9000].relation = 2;
		modelData->event[i+12000].direction = 1;
		modelData->event[i+12000].relation = 2;
	}
	for(i = 0; i <= 199; i++)
	{
	modelData->event[i+15000].direction = 1;
	modelData->event[i+15000].relation = 2;
	modelData->event[i+15200].direction = 1;
	modelData->event[i+15200].relation = 2;
	}
	simulator->time = QSS_Time(9000,15400,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.06;
	simulator->output = SD_Output("spl_neurons",20,12600,9000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

	for(i = 0; i <= 19; i++)
	{
		modelOutput->nOS[i] = 1;
		modelOutput->nSO[150*i]++;
	}
	SD_allocOutputMatrix(modelOutput,9000,12600);
		cleanVector(states,0,9000);

		cleanVector(outputs,0,20);

	for(i = 0; i <= 19; i++)
	{
		sprintf(modelOutput->variable[i].name,"v[%d]",150*i+1);
	}
		cleanVector(outputs,0,20);

	for(i = 0; i <= 19; i++)
	{
		modelOutput->SO[150*i][states[150*i]++] = i;
		modelOutput->OS[i][outputs[i]++] = 150*i;
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
