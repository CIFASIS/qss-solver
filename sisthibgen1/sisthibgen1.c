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
	double tmp1[3];
	int j = 0;
	int j4= 0;
	int j5;
	switch(i)
	{
		case 10:
			dx[1] = __PAR_UlineRef-x[33];
			return;
		case 11:
			for(j4 = 0; j4 <= 1; j4++)
			{
					alg[(j4+8) * 3] = (x[(j4+2) * 3]-x[33])/1.000000000000000055511151e-01;
			}
			alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
	for(j5 = 0;j5 < 3; j5++)
	{
		tmp1[j5] = 0;
	}
	for(j5 = 0; j5 <= 1; j5++)
	{
		tmp1[0] += alg[(j5+8) * 3 + 0];
		tmp1[1] += alg[(j5+8) * 3 + 1];
		tmp1[2] += alg[(j5+8) * 3 + 2];
	}
			dx[1] = (tmp1[0]+alg[30])/__PAR_Cbus;
			return;
		default:
			j = i;
			if(j >=0 && j <= 1)
			{
				alg[(j) * 3] = __ipvsolver(x[(j) * 3],1,60,d[(10)],d[(j+11)],1.000000000000000081803054e-05,6.0,d[(j+19)]);
				dx[1] = (alg[(j) * 3]-x[(j+4) * 3])/__PAR_Cpvin;
			}
			j = i-2;
			if(j >=0 && j <= 1)
			{
				alg[(j+4) * 3] = ((d[(j+4)])*x[(j+4) * 3]-80.0-0.0)/((d[(j+2)])+(d[(j+4)]));
				dx[1] = (alg[(j+4) * 3]-0.0/__PAR_R-0.0/1.000000000000000055511151e-01)/__PAR_Cboost;
			}
			j = i-4;
			if(j >=0 && j <= 1)
			{
				alg[(j+4) * 3] = ((d[(j+4)])*x[(j+4) * 3]-80.0-0.0)/((d[(j+2)])+(d[(j+4)]));
				dx[1] = (x[(j) * 3]-d[(j+4)]*(x[(j+4) * 3]-alg[(j+4) * 3]))/__PAR_Lboost;
			}
			j = i-6;
			if(j >=0 && j <= 1)
			{
				dx[1] = d[(j+17)]-x[(j) * 3];
			}
			j = i-8;
			if(j >=0 && j <= 1)
			{
				dx[1] = (x[(j) * 3]-x[(j+8) * 3])*100.0;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp1[3];
	int j = 0;
	int j4= 0;
	int j5;
	int j6 = 0;
	int j7 = 0;
	int j8 = 0;
	switch(i)
	{
		case 10:
			for(j7 = 0; j7 <= 1; j7++)
	{
			alg[(j7+8) * 3] = (x[(j7+2) * 3]-x[33])/1.000000000000000055511151e-01;
	}
			alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
	for(j5 = 0;j5 < 3; j5++)
	{
		tmp1[j5] = 0;
	}
	for(j5 = 0; j5 <= 1; j5++)
	{
		tmp1[0] += alg[(j5+8) * 3 + 0];
		tmp1[1] += alg[(j5+8) * 3 + 1];
		tmp1[2] += alg[(j5+8) * 3 + 2];
	}
			der[33 + 1] = (tmp1[0]+alg[30])/__PAR_Cbus;
			return;
		case 11:
			for(j8 = 0; j8 <= 1; j8++)
	{
			alg[(j8+8) * 3] = (x[(j8+2) * 3]-x[33])/1.000000000000000055511151e-01;
	}
			alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
			der[30 + 1] = __PAR_UlineRef-x[33];
	for(j5 = 0;j5 < 3; j5++)
	{
		tmp1[j5] = 0;
	}
	for(j5 = 0; j5 <= 1; j5++)
	{
		tmp1[0] += alg[(j5+8) * 3 + 0];
		tmp1[1] += alg[(j5+8) * 3 + 1];
		tmp1[2] += alg[(j5+8) * 3 + 2];
	}
			der[33 + 1] = (tmp1[0]+alg[30])/__PAR_Cbus;
			return;
	}
	j = i;
	if(j >=0 && j <= 1)
	{
		alg[(j) * 3] = __ipvsolver(x[(j) * 3],1,60,d[(10)],d[(j+11)],1.000000000000000081803054e-05,6.0,d[(j+19)]);
		alg[(j+4) * 3] = ((d[(j+4)])*x[(j+4) * 3]-80.0-0.0)/((d[(j+2)])+(d[(j+4)]));
		der[(j) * 3 + 1] = (alg[(j) * 3]-x[(j+4) * 3])/__PAR_Cpvin;
		der[(j+4) * 3 + 1] = (x[(j) * 3]-d[(j+4)]*(x[(j+4) * 3]-alg[(j+4) * 3]))/__PAR_Lboost;
		der[(j+6) * 3 + 1] = d[(j+17)]-x[(j) * 3];
		der[(j+8) * 3 + 1] = (x[(j) * 3]-x[(j+8) * 3])*100.0;
	}
	j = i-2;
	if(j >=0 && j <= 1)
	{
		for(j6 = 0; j6 <= 1; j6++)
	{
		alg[(j6+8) * 3] = (x[(j6+2) * 3]-x[33])/1.000000000000000055511151e-01;
	}
		alg[30] = 2.0*x[30]+5.000000000000000277555756e-02*(__PAR_UlineRef-x[33]);
	for(j5 = 0;j5 < 3; j5++)
	{
		tmp1[j5] = 0;
	}
	for(j5 = 0; j5 <= 1; j5++)
	{
		tmp1[0] += alg[(j5+8) * 3 + 0];
		tmp1[1] += alg[(j5+8) * 3 + 1];
		tmp1[2] += alg[(j5+8) * 3 + 2];
	}
		der[33 + 1] = (tmp1[0]+alg[30])/__PAR_Cbus;
	}
	j = i-4;
	if(j >=0 && j <= 1)
	{
		alg[(j) * 3] = __ipvsolver(x[(j) * 3],1,60,d[(10)],d[(j+11)],1.000000000000000081803054e-05,6.0,d[(j+19)]);
		alg[(j+4) * 3] = ((d[(j+4)])*x[(j+4) * 3]-80.0-0.0)/((d[(j+2)])+(d[(j+4)]));
		der[(j) * 3 + 1] = (alg[(j) * 3]-x[(j+4) * 3])/__PAR_Cpvin;
		der[(j+2) * 3 + 1] = (alg[(j+4) * 3]-0.0/__PAR_R-0.0/1.000000000000000055511151e-01)/__PAR_Cboost;
		der[(j+4) * 3 + 1] = (x[(j) * 3]-d[(j+4)]*(x[(j+4) * 3]-alg[(j+4) * 3]))/__PAR_Lboost;
	}
	j = i-8;
	if(j >=0 && j <= 1)
	{
		der[(j+8) * 3 + 1] = (x[(j) * 3]-x[(j+8) * 3])*100.0;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	if(i >= 0 && i <= 1)
	{
		zc[0] = t-(d[(i+6)]);
	}
	if(i >= 2 && i <= 3)
	{
		zc[0] = t-d[(i+6)]-d[(i-2)]*__PAR_Tboost[(i-2)]-(0.0);
	}
	if(i >= 4 && i <= 5)
	{
		alg[(i) * 3] = ((d[(i)])*x[(i) * 3]-80.0-0.0)/((d[(i-2)])+(d[(i)]));
		zc[0] = alg[(i) * 3]-(0.0);
	}
	if(i >= 6 && i <= 7)
	{
		alg[(i-2) * 3] = ((d[(i-2)])*x[(i-2) * 3]-80.0-0.0)/((d[(i-4)])+(d[(i-2)]));
		alg[(i) * 3] = alg[(i-2) * 3]*d[(i-4)];
		zc[0] = alg[(i) * 3]-(0.0);
	}
	if(i >= 8 && i <= 9)
	{
		zc[0] = t-(d[(i+7)]);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 1)
	{
		d[(i+8)] = d[(i+6)];
		d[(i+6)] = d[(i+6)]+__PAR_Tboost[(i)];
		alg[(i) * 3] = __ipvsolver(x[(i) * 3],1,60,d[(10)],d[(i+11)],1.000000000000000081803054e-05,6.0,d[(i+19)]);
		d[(i+19)] = alg[(i) * 3];
		d[(i+4)] = __PAR_ROn;
		d[(i+23)] = d[(i+23)]+(d[(i+17)]-x[(i) * 3])*__PAR_Tboost[(i)];
		d[(i)] = 8.499999999999999777955395e-01-2.999999999999999888977698e-01*(d[(i+17)]-x[(i) * 3])-d[(i+23)]*6.0;
	if((d[(i)]>9.799999999999999822364316e-01))
	{
		d[(i)] = 9.799999999999999822364316e-01;
	}
	if((d[(i)]<4.000000000000000222044605e-01))
	{
		d[(i)] = 4.000000000000000222044605e-01;
	}
	}
	if(i >= 2 && i <= 3)
	{
		d[(i+2)] = __PAR_ROff;
	}
	if(i >= 6 && i <= 7)
	{
		d[(i-4)] = __PAR_ROn;
	}
	if(i >= 8 && i <= 9)
	{
		d[(i+7)] = d[(i+7)]+__PAR_TContMaxPot;
		d[(i+17)] = d[(i+19)];
		alg[(i-8) * 3] = __ipvsolver(x[(i-8) * 3],1,60,d[(10)],d[(i+3)],1.000000000000000081803054e-05,6.0,d[(i+11)]);
		alg[(i-6) * 3] = x[(i-8) * 3]*alg[(i-8) * 3];
		d[(i+19)] = alg[(i-6) * 3];
		d[(i+21)] = d[(i+19)]-d[(i+17)];
		d[(i+23)] = d[(i+25)];
		d[(i+25)] = d[(i+9)];
		d[(i+27)] = d[(i+25)]-d[(i+23)];
	if((fabs(d[(i+27)])>1.000000000000000055511151e-01*__PAR_deltaVpvRefMPPT))
	{
	if((d[(i+21)]!=0.0))
	{
	if((d[(i+21)]/d[(i+27)]>0.0))
	{
		d[(i+9)] = d[(i+9)]+__PAR_deltaVpvRefMPPT;
	}
	else
	{
		d[(i+9)] = d[(i+9)]-__PAR_deltaVpvRefMPPT;
	}
	}
		d[(i+9)] = 30.0;
	}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 4 && i <= 5)
	{
		d[(i-2)] = __PAR_ROff;
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
	int i10;
	int i11;
	int i3 = 0;
	int i;
	int j = 0;
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
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+27)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+33)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+21)] = 1.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+19)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+25)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+31)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+2)] = 1.000000000000000000000000e+05;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+13)] = 1.000000000000000000000000e+05;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+4)] = 1.000000000000000000000000e+05;
	}
	modelData->x[33] = 20.0;
	for(i = 0; i <= 1;i++)
	{
		modelData->x[i * 3] = 2.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+17)] = 35.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+29)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+35)] = 0.0;
	}
	// Initialize model code.
		modelData->d[(10)] = 273.0+2.500000000000000000000000e+01;
	for(i10 = 0; i10 <= 1; i10++)
	{
		modelData->d[(i10+11)] = 80.0;
		__PAR_TContMaxPot = 2.000000000000000111022302e-01;
		modelData->d[(i10+15)] = __PAR_TContMaxPot;
	}
	for(i11 = 0; i11 <= 1; i11++)
	{
		modelData->d[(i11+4)] = 1.000000000000000000000000e+05;
		modelData->d[(i11+2)] = 1.000000000000000000000000e+05;
		__PAR_Tboost[(i11)] = 2.000000000000000163606108e-05;
		modelData->d[(i11)] = 9.000000000000000222044605e-01;
		modelData->d[(i11+6)] = __PAR_Tboost[(i11)];
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nDS[i] = 1;
		modelData->nDS[i+4] = 2;
		modelData->nDS[i+6] = 1;
		modelData->nDS[i+8] = 2;
	}
	modelData->nDS[10] = 1;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nDS[i]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nDS[i+2]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nDS[11]++;
	}
	modelData->nDS[11]++;
	modelData->nDS[11]++;
	for(i = 0; i <= 1; i++)
	{
		modelData->nSD[i+4]++;
		modelData->nSD[i]++;
		modelData->nSD[i+4]++;
		modelData->nSD[i]++;
		modelData->nSD[i]++;
		modelData->nSD[i+8]++;
	}
	modelData->nSD[11]++;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i+4]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i+2]++;
	}
	modelData->nSD[10]++;
	modelData->nSD[11]++;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+4]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+6]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i+4]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i+4]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 8; i <= 9; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 8; i <= 9; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].nRHSSt = 1;
	modelData->event[i].nRHSSt += 2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->event[i+8].nRHSSt++;
	}
	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].nLHSDsc = 6;
	modelData->event[i+2].nLHSDsc = 1;
	modelData->event[i+4].nLHSDsc = 1;
	modelData->event[i+6].nLHSDsc = 1;
	modelData->event[i+8].nLHSDsc = 8;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,12);

	for(i = 0; i <= 1; i++)
	{
		modelData->DS[i][states[i]++] = i+4;
		modelData->DS[i+4][states[i+4]++] = i;
		modelData->DS[i+4][states[i+4]++] = i+4;
		modelData->DS[i+6][states[i+6]++] = i;
		modelData->DS[i+8][states[i+8]++] = i;
		modelData->DS[i+8][states[i+8]++] = i+8;
	}
	modelData->DS[10][states[10]++] = 11;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i][states[i]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i+2][states[i+2]++] = i+4;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[11][states[11]++] = i+2;
	}
	modelData->DS[11][states[11]++] = 10;
	modelData->DS[11][states[11]++] = 11;
		cleanVector(states,0,12);

	for(i = 0; i <= 1; i++)
	{
		modelData->SD[i+4][states[i+4]++] = i;
		modelData->SD[i][states[i]++] = i+4;
		modelData->SD[i+4][states[i+4]++] = i+4;
		modelData->SD[i][states[i]++] = i+6;
		modelData->SD[i][states[i]++] = i+8;
		modelData->SD[i+8][states[i+8]++] = i+8;
	}
	modelData->SD[11][states[11]++] = 10;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i][states[i]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i+4][states[i+4]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i+2][states[i+2]++] = 11;
	}
	modelData->SD[10][states[10]++] = 11;
	modelData->SD[11][states[11]++] = 11;
		cleanVector(events,0,10);

	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+4][events[i+4]++] = i+4;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+6][events[i+6]++] = i+4;
	}
		cleanVector(states,0,12);

	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i+4][states[i+4]++] = i+4;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i+4][states[i+4]++] = i+6;
	}
		cleanVector(events,0,10);

	for(i = 0; i <= 1; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HZ[i][events[i]++] = i+4;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HZ[i][events[i]++] = i+6;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i+4;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->HZ[i][events[i]++] = i-2;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 8; i <= 9; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
		cleanVector(events,0,10);

	for(i = 0; i <= 1; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HD[i][events[i]++] = i-2;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->HD[i][events[i]++] = i-4;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HD[i][events[i]++] = i+2;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HD[i][events[i]++] = i+4;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HD[i][events[i]++] = i+2;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->HD[i][events[i]++] = i-2;
	}
	for(i = 8; i <= 9; i++)
	{
		modelData->HD[i][events[i]++] = i-2;
	}
		cleanVector(events,0,10);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].RHSSt[events[i]++] = i;
	for(i3 = 0; i3 <= 1; i3++)
	{
	modelData->event[i].RHSSt[events[i]++] = i3;
	}
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->event[i+8].RHSSt[events[i+8]++] = i;
	}
		cleanVector(events,0,10);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].LHSDsc[events[i]++] = i;
	modelData->event[i].LHSDsc[events[i]++] = i+4;
	modelData->event[i].LHSDsc[events[i]++] = i+6;
	modelData->event[i].LHSDsc[events[i]++] = i+8;
	modelData->event[i].LHSDsc[events[i]++] = i+19;
	modelData->event[i].LHSDsc[events[i]++] = i+23;
	modelData->event[i+2].LHSDsc[events[i+2]++] = i+4;
	modelData->event[i+4].LHSDsc[events[i+4]++] = i+2;
	modelData->event[i+6].LHSDsc[events[i+6]++] = i+2;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+15;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+17;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+25;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+27;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+29;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+31;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+33;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+35;
	}
		cleanVector(events,0,10);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].direction = 1;
	modelData->event[i].relation = 2;
	modelData->event[i+2].direction = 1;
	modelData->event[i+2].relation = 2;
	modelData->event[i+4].direction = -1;
	modelData->event[i+4].relation = 0;
	modelData->event[i+6].direction = 1;
	modelData->event[i+6].relation = 2;
	modelData->event[i+8].direction = 1;
	modelData->event[i+8].relation = 2;
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
