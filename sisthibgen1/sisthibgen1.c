#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "sisthibgen1_functions.h"
#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_Cboost = 0;
double __PAR_Lboost = 0;
double __PAR_R = 0;
double __PAR_ROn = 0;
double __PAR_ROff = 0;
double __PAR_Tboost[2];
double __PAR_Cpvin = 0;
double __PAR_Cbus = 0;
double __PAR_Rshunt = 0;
double __PAR_iLoad = 0;
double __PAR_TContMaxPot = 0;
double __PAR_UlineRef = 0;
double __PAR_deltaVpvRefMPPT = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 4;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	double tmp20[3];
	int i0 = i;
	int j0 = 0;
	int j50= 0;
	int j60;
	switch(i)
	{
		case 10:
			dx[1] = __PAR_UlineRef-x[33];
			return;
		case 11:
			for(j50 = 0; j50 <= 1; j50++)
			{
					alg[(j50+8) * 3] = (x[(j50+2) * 3]-x[33])/1.000000000000000055511151e-01;
			}
			alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
	for(j60 = 0;j60 < 3; j60++)
	{
		tmp20[j60] = 0;
	}
	for(j60 = 0; j60 <= 1; j60++)
	{
		tmp20[0] += alg[(j60+8) * 3 + 0];
		tmp20[1] += alg[(j60+8) * 3 + 1];
		tmp20[2] += alg[(j60+8) * 3 + 2];
	}
			dx[1] = (tmp20[0]+alg[30])/__PAR_Cbus;
			return;
		default:
			j0 = i0;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],1,60,d[(10)],d[(j0+11)],1.000000000000000081803054e-05,6.0,d[(j0+19)]);
				dx[1] = (alg[(j0) * 3]-x[(j0+4) * 3])/__PAR_Cpvin;
			}
			j0 = i0-2;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-80.0-0.0)/((d[(j0+2)])+(d[(j0+4)]));
				dx[1] = (alg[(j0+4) * 3]-0.0/__PAR_R-0.0/1.000000000000000055511151e-01)/__PAR_Cboost;
			}
			j0 = i0-4;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-80.0-0.0)/((d[(j0+2)])+(d[(j0+4)]));
				dx[1] = (x[(j0) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
			}
			j0 = i0-6;
			if(j0 >=0 && j0 <= 1)
			{
				dx[1] = d[(j0+17)]-x[(j0) * 3];
			}
			j0 = i0-8;
			if(j0 >=0 && j0 <= 1)
			{
				dx[1] = (x[(j0) * 3]-x[(j0+8) * 3])*100.0;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp20[3];
	int i0 = i;
	int j0 = 0;
	int j50= 0;
	int j60;
	int j700 = 0;
	int j800 = 0;
	int j900 = 0;
	switch(i)
	{
		case 10:
			for(j800 = 0; j800 <= 1; j800++)
	{
			alg[(j800+8) * 3] = (x[(j800+2) * 3]-x[33])/1.000000000000000055511151e-01;
	}
			alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
	for(j60 = 0;j60 < 3; j60++)
	{
		tmp20[j60] = 0;
	}
	for(j60 = 0; j60 <= 1; j60++)
	{
		tmp20[0] += alg[(j60+8) * 3 + 0];
		tmp20[1] += alg[(j60+8) * 3 + 1];
		tmp20[2] += alg[(j60+8) * 3 + 2];
	}
			der[33 + 1] = (tmp20[0]+alg[30])/__PAR_Cbus;
			return;
		case 11:
			for(j900 = 0; j900 <= 1; j900++)
	{
			alg[(j900+8) * 3] = (x[(j900+2) * 3]-x[33])/1.000000000000000055511151e-01;
	}
			alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
			der[30 + 1] = __PAR_UlineRef-x[33];
	for(j60 = 0;j60 < 3; j60++)
	{
		tmp20[j60] = 0;
	}
	for(j60 = 0; j60 <= 1; j60++)
	{
		tmp20[0] += alg[(j60+8) * 3 + 0];
		tmp20[1] += alg[(j60+8) * 3 + 1];
		tmp20[2] += alg[(j60+8) * 3 + 2];
	}
			der[33 + 1] = (tmp20[0]+alg[30])/__PAR_Cbus;
			return;
	}
	j0 = i0;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],1,60,d[(10)],d[(j0+11)],1.000000000000000081803054e-05,6.0,d[(j0+19)]);
		alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-80.0-0.0)/((d[(j0+2)])+(d[(j0+4)]));
		der[(j0) * 3 + 1] = (alg[(j0) * 3]-x[(j0+4) * 3])/__PAR_Cpvin;
		der[(j0+4) * 3 + 1] = (x[(j0) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
		der[(j0+6) * 3 + 1] = d[(j0+17)]-x[(j0) * 3];
		der[(j0+8) * 3 + 1] = (x[(j0) * 3]-x[(j0+8) * 3])*100.0;
	}
	j0 = i0-2;
	if(j0 >=0 && j0 <= 1)
	{
		for(j700 = 0; j700 <= 1; j700++)
	{
		alg[(j700+8) * 3] = (x[(j700+2) * 3]-x[33])/1.000000000000000055511151e-01;
	}
		alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
	for(j60 = 0;j60 < 3; j60++)
	{
		tmp20[j60] = 0;
	}
	for(j60 = 0; j60 <= 1; j60++)
	{
		tmp20[0] += alg[(j60+8) * 3 + 0];
		tmp20[1] += alg[(j60+8) * 3 + 1];
		tmp20[2] += alg[(j60+8) * 3 + 2];
	}
		der[33 + 1] = (tmp20[0]+alg[30])/__PAR_Cbus;
	}
	j0 = i0-4;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],1,60,d[(10)],d[(j0+11)],1.000000000000000081803054e-05,6.0,d[(j0+19)]);
		alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-80.0-0.0)/((d[(j0+2)])+(d[(j0+4)]));
		der[(j0) * 3 + 1] = (alg[(j0) * 3]-x[(j0+4) * 3])/__PAR_Cpvin;
		der[(j0+2) * 3 + 1] = (alg[(j0+4) * 3]-0.0/__PAR_R-0.0/1.000000000000000055511151e-01)/__PAR_Cboost;
		der[(j0+4) * 3 + 1] = (x[(j0) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
	}
	j0 = i0-8;
	if(j0 >=0 && j0 <= 1)
	{
		der[(j0+8) * 3 + 1] = (x[(j0) * 3]-x[(j0+8) * 3])*100.0;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1)
	{
		zc[0] = t-(d[(i0+6)]);
	}
	if(i0 >= 2 && i0 <= 3)
	{
		zc[0] = t-d[(i0+6)]-d[(i0-2)]*__PAR_Tboost[(i0-2)]-(0.0);
	}
	if(i0 >= 4 && i0 <= 5)
	{
		alg[(i0) * 3] = ((d[(i0)])*x[(i0) * 3]-80.0-0.0)/((d[(i0-2)])+(d[(i0)]));
		zc[0] = alg[(i0) * 3]-(0.0);
	}
	if(i0 >= 6 && i0 <= 7)
	{
		alg[(i0-2) * 3] = ((d[(i0-2)])*x[(i0-2) * 3]-80.0-0.0)/((d[(i0-4)])+(d[(i0-2)]));
		alg[(i0) * 3] = alg[(i0-2) * 3]*d[(i0-4)];
		zc[0] = alg[(i0) * 3]-(0.0);
	}
	if(i0 >= 8 && i0 <= 9)
	{
		zc[0] = t-(d[(i0+7)]);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1)
	{
		d[(i0+8)] = d[(i0+6)];
		d[(i0+6)] = d[(i0+6)]+__PAR_Tboost[(i0)];
		alg[(i0) * 3] = __ipvsolver(x[(i0) * 3],1,60,d[(10)],d[(i0+11)],1.000000000000000081803054e-05,6.0,d[(i0+19)]);
		d[(i0+19)] = alg[(i0) * 3];
		d[(i0+4)] = __PAR_ROn;
		d[(i0+23)] = d[(i0+23)]+(d[(i0+17)]-x[(i0) * 3])*__PAR_Tboost[(i0)];
		d[(i0)] = 8.499999999999999777955395e-01-2.999999999999999888977698e-01*(d[(i0+17)]-x[(i0) * 3])-d[(i0+23)]*6.0;
	if((d[(i0)]>9.799999999999999822364316e-01))
	{
		d[(i0)] = 9.799999999999999822364316e-01;
	}
	if((d[(i0)]<4.000000000000000222044605e-01))
	{
		d[(i0)] = 4.000000000000000222044605e-01;
	}
	}
	if(i0 >= 2 && i0 <= 3)
	{
		d[(i0+2)] = __PAR_ROff;
	}
	if(i0 >= 6 && i0 <= 7)
	{
		d[(i0-4)] = __PAR_ROn;
	}
	if(i0 >= 8 && i0 <= 9)
	{
		d[(i0+7)] = d[(i0+7)]+__PAR_TContMaxPot;
		d[(i0+17)] = d[(i0+19)];
		alg[(i0-8) * 3] = __ipvsolver(x[(i0-8) * 3],1,60,d[(10)],d[(i0+3)],1.000000000000000081803054e-05,6.0,d[(i0+11)]);
		alg[(i0-6) * 3] = x[(i0-8) * 3]*alg[(i0-8) * 3];
		d[(i0+19)] = alg[(i0-6) * 3];
		d[(i0+21)] = d[(i0+19)]-d[(i0+17)];
		d[(i0+23)] = d[(i0+25)];
		d[(i0+25)] = d[(i0+9)];
		d[(i0+27)] = d[(i0+25)]-d[(i0+23)];
	if((fabs(d[(i0+27)])>1.000000000000000055511151e-01*__PAR_deltaVpvRefMPPT))
	{
	if((d[(i0+21)]!=0.0))
	{
	if((d[(i0+21)]/d[(i0+27)]>0.0))
	{
		d[(i0+9)] = d[(i0+9)]+__PAR_deltaVpvRefMPPT;
	}
	else
	{
		d[(i0+9)] = d[(i0+9)]-__PAR_deltaVpvRefMPPT;
	}
	}
		d[(i0+9)] = 30.0;
	}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 4 && i0 <= 5)
	{
		d[(i0-2)] = __PAR_ROff;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			alg[0] = __ipvsolver(x[0],1,60,d[(10)],d[11],1.000000000000000081803054e-05,6.0,d[19]);
			out[0] = alg[0];
			return;
		case 1:
			out[0] = d[(27)];
			return;
		case 2:
			out[0] = x[0];
			return;
		case 3:
			out[0] = d[(0)];
			return;
		case 4:
			out[0] = d[(17)];
			return;
		case 5:
			out[0] = x[24];
			return;
		case 6:
			out[0] = d[(29)];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(37*sizeof(int));
	int *events = (int*)malloc(10*sizeof(int));
	int *outputs = (int*)malloc(7*sizeof(int));
	int *states = (int*)malloc(12*sizeof(int));
	int i0;
	int i40 = 0;
	int j0 = 0;
	simulator->data = QSS_Data(12,37,10,0,11,"sisthibgen1");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Cboost = 4.700000000000000184574578e-03;
	__PAR_Lboost = 1.499999999999999944488849e-02;
	__PAR_R = 1.000000000000000000000000e+03;
	__PAR_ROn = 1.000000000000000081803054e-05;
	__PAR_ROff = 1.000000000000000000000000e+05;
	__PAR_Cpvin = 4.700000000000000184574578e-03;
	__PAR_Cbus = 1.000000000000000047921736e-04;
	__PAR_Rshunt = 1.000000000000000020816682e-02;
	__PAR_iLoad = 1.000000000000000055511151e-01;
	__PAR_UlineRef = 48.0;
	__PAR_deltaVpvRefMPPT = 5.000000000000000000000000e-01;
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+27)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+33)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+21)] = 1.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+19)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+25)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+31)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+2)] = 1.000000000000000000000000e+05;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+13)] = 1.000000000000000000000000e+05;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+4)] = 1.000000000000000000000000e+05;
	}
	modelData->x[33] = 20.0;
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->x[i0 * 3] = 2.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+17)] = 35.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+29)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+35)] = 0.0;
	}
	// Initialize model code.
		modelData->d[(10)] = 273.0+2.500000000000000000000000e+01;
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->d[(i0+11)] = 80.0;
		__PAR_TContMaxPot = 2.000000000000000111022302e-01;
		modelData->d[(i0+15)] = __PAR_TContMaxPot;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->d[(i0+4)] = 1.000000000000000000000000e+05;
		modelData->d[(i0+2)] = 1.000000000000000000000000e+05;
		__PAR_Tboost[(i0)] = 2.000000000000000163606108e-05;
		modelData->d[(i0)] = 9.000000000000000222044605e-01;
		modelData->d[(i0+6)] = __PAR_Tboost[(i0)];
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nDS[i0] = 1;
		modelData->nDS[i0+4] = 2;
		modelData->nDS[i0+6] = 1;
		modelData->nDS[i0+8] = 2;
	}
	modelData->nDS[10] = 1;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[11]++;
	}
	modelData->nDS[11]++;
	modelData->nDS[11]++;
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nSD[i0+4]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+4]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+8]++;
	}
	modelData->nSD[11]++;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0+2]++;
	}
	modelData->nSD[10]++;
	modelData->nSD[11]++;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+4]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 8; i0 <= 9; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 8; i0 <= 9; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].nRHSSt = 1;
	modelData->event[i0].nRHSSt += 2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->event[i0+8].nRHSSt++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].nLHSDsc = 6;
	modelData->event[i0+2].nLHSDsc = 1;
	modelData->event[i0+4].nLHSDsc = 1;
	modelData->event[i0+6].nLHSDsc = 1;
	modelData->event[i0+8].nLHSDsc = 8;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,12);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0+4;
		modelData->DS[i0+4][states[i0+4]++] = i0;
		modelData->DS[i0+4][states[i0+4]++] = i0+4;
		modelData->DS[i0+6][states[i0+6]++] = i0;
		modelData->DS[i0+8][states[i0+8]++] = i0;
		modelData->DS[i0+8][states[i0+8]++] = i0+8;
	}
	modelData->DS[10][states[10]++] = 11;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+2][states[i0+2]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[11][states[11]++] = i0+2;
	}
	modelData->DS[11][states[11]++] = 10;
	modelData->DS[11][states[11]++] = 11;
		cleanVector(states,0,12);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->SD[i0+4][states[i0+4]++] = i0;
		modelData->SD[i0][states[i0]++] = i0+4;
		modelData->SD[i0+4][states[i0+4]++] = i0+4;
		modelData->SD[i0][states[i0]++] = i0+6;
		modelData->SD[i0][states[i0]++] = i0+8;
		modelData->SD[i0+8][states[i0+8]++] = i0+8;
	}
	modelData->SD[11][states[11]++] = 10;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+4][states[i0+4]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+2][states[i0+2]++] = 11;
	}
	modelData->SD[10][states[10]++] = 11;
	modelData->SD[11][states[11]++] = 11;
		cleanVector(events,0,10);

	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+4][events[i0+4]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+6][events[i0+6]++] = i0+4;
	}
		cleanVector(states,0,12);

	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+4][states[i0+4]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+4][states[i0+4]++] = i0+6;
	}
		cleanVector(events,0,10);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+4;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+6;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+4;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0-2;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 8; i0 <= 9; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,10);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-4;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+2;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+4;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+2;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 8; i0 <= 9; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
		cleanVector(events,0,10);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].RHSSt[events[i0]++] = i0;
	for(i40 = 0; i40 <= 1; i40++)
	{
	modelData->event[i0].RHSSt[events[i0]++] = i40;
	}
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->event[i0+8].RHSSt[events[i0+8]++] = i0;
	}
		cleanVector(events,0,10);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].LHSDsc[events[i0]++] = i0;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+4;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+6;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+8;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+19;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+23;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+4;
	modelData->event[i0+4].LHSDsc[events[i0+4]++] = i0+2;
	modelData->event[i0+6].LHSDsc[events[i0+6]++] = i0+2;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+15;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+17;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+25;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+27;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+29;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+31;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+33;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+35;
	}
		cleanVector(events,0,10);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].direction = 1;
	modelData->event[i0].relation = 2;
	modelData->event[i0+2].direction = 1;
	modelData->event[i0+2].relation = 2;
	modelData->event[i0+4].direction = -1;
	modelData->event[i0+4].relation = 0;
	modelData->event[i0+6].direction = 1;
	modelData->event[i0+6].relation = 2;
	modelData->event[i0+8].direction = 1;
	modelData->event[i0+8].relation = 2;
	}
	simulator->time = QSS_Time(12,10,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("sisthibgen1",7,37,12,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[2] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[5] = 1;
		modelOutput->nSO[8]++;
	modelOutput->nOS[0]++;
	modelOutput->nSO[0]++;
		modelOutput->nOD[1] = 1;
		modelOutput->nDO[27]++;
		modelOutput->nOD[3] = 1;
		modelOutput->nDO[0]++;
		modelOutput->nOD[4] = 1;
		modelOutput->nDO[17]++;
		modelOutput->nOD[6] = 1;
		modelOutput->nDO[29]++;
	modelOutput->nOD[0]++;
	modelOutput->nDO[10]++;
	modelOutput->nOD[0]++;
	modelOutput->nDO[11]++;
	modelOutput->nOD[0]++;
	modelOutput->nDO[19]++;
	SD_allocOutputMatrix(modelOutput,12,37);
		cleanVector(states,0,12);

		cleanVector(discretes,0,37);

		cleanVector(outputs,0,7);

		sprintf(modelOutput->variable[0].name,"Ipv[1]");
		sprintf(modelOutput->variable[1].name,"ActualPotPanel[1]");
		sprintf(modelOutput->variable[2].name,"Vpv[1]");
		sprintf(modelOutput->variable[3].name,"DCboost[1]");
		sprintf(modelOutput->variable[4].name,"VpvRef[1]");
		sprintf(modelOutput->variable[5].name,"VpvFiltrada[1]");
		sprintf(modelOutput->variable[6].name,"deltaPot[1]");
		cleanVector(outputs,0,7);

		modelOutput->SO[0][states[0]++] = 2;
		modelOutput->OS[2][outputs[2]++] = 0;
		modelOutput->SO[8][states[8]++] = 5;
		modelOutput->OS[5][outputs[5]++] = 8;
	modelOutput->OS[0][outputs[0]++] = 0;
	modelOutput->SO[0][states[0]++] = 0;
		cleanVector(outputs,0,7);

		modelOutput->DO[27][discretes[27]++] = 1;
		modelOutput->OD[1][outputs[1]++] = 27;
		modelOutput->DO[0][discretes[0]++] = 3;
		modelOutput->OD[3][outputs[3]++] = 0;
		modelOutput->DO[17][discretes[17]++] = 4;
		modelOutput->OD[4][outputs[4]++] = 17;
		modelOutput->DO[29][discretes[29]++] = 6;
		modelOutput->OD[6][outputs[6]++] = 29;
	modelOutput->OD[0][outputs[0]++] = 10;
	modelOutput->DO[10][discretes[10]++] = 0;
	modelOutput->OD[0][outputs[0]++] = 11;
	modelOutput->DO[11][discretes[11]++] = 0;
	modelOutput->OD[0][outputs[0]++] = 19;
	modelOutput->DO[19][discretes[19]++] = 0;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
