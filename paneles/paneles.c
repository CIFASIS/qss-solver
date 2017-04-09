#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
double __PAR_q = 0;
double __PAR_Ac = 0;
double __PAR_K = 0;
double __PAR_K1 = 0;
double __PAR_Ior = 0;
double __PAR_Tref = 0;
double __PAR_Eg = 0;
double __PAR_Isc = 0;
double __PAR_Rspv = 0;
double __PAR_Cbatery = 0;
double __PAR_Rshunt = 0;
double __PAR_iLoad = 0;
double __PAR_TContMaxPot = 0;
double __PAR_VlineaRef = 0;

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
	int j2= 0;
	int j3;
	switch(i)
	{
		case 10:
			for(j2 = 0; j2 <= 1; j2++)
			{
					alg[(j2+10) * 3] = (x[(j2) * 3]-x[30])/(__PAR_Rshunt+d[(j2+18)]);
			}
			alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
	for(j3 = 0;j3 < 3; j3++)
	{
		tmp1[j3] = 0;
	}
	for(j3 = 0; j3 <= 1; j3++)
	{
		tmp1[0] += alg[(j3+10) * 3 + 0];
		tmp1[1] += alg[(j3+10) * 3 + 1];
		tmp1[2] += alg[(j3+10) * 3 + 2];
	}
			dx[1] = (tmp1[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
			return;
		case 11:
			dx[1] = __PAR_VlineaRef-x[30];
			return;
		default:
			j = i;
			if(j >=0 && j <= 1)
			{
				alg[(j+6) * 3] = ((d[(j+4)])*x[(j+2) * 3]-x[(j) * 3])/((d[(j+2)])+(d[(j+4)]));
				dx[1] = (alg[(j+6) * 3]-x[(j) * 3]/__PAR_R-(x[(j) * 3]-x[30])/(__PAR_Rshunt+d[(j+18)]))/__PAR_Cboost;
			}
			j = i-2;
			if(j >=0 && j <= 1)
			{
				alg[(j) * 3] = x[(j+2) * 3];
				alg[(j+2) * 3] = (log(d[(j+16)]*(36*d[(j+10)]-alg[(j) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j) * 3]*__PAR_Rspv/36)*36;
				alg[(j+4) * 3] = alg[(j+2) * 3];
				alg[(j+6) * 3] = ((d[(j+4)])*x[(j+2) * 3]-x[(j) * 3])/((d[(j+2)])+(d[(j+4)]));
				dx[1] = (alg[(j+4) * 3]-d[(j+4)]*(x[(j+2) * 3]-alg[(j+6) * 3]))/__PAR_Lboost;
			}
			j = i-4;
			if(j >=0 && j <= 1)
			{
				dx[1] = d[(j+28)]-d[(j+26)];
			}
			j = i-6;
			if(j >=0 && j <= 1)
			{
				alg[(j+10) * 3] = (x[(j) * 3]-x[30])/(__PAR_Rshunt+d[(j+18)]);
				alg[(j+12) * 3] = x[30]*alg[(j+10) * 3];
				dx[1] = alg[(j+12) * 3]/__PAR_TContMaxPot;
			}
			j = i-8;
			if(j >=0 && j <= 1)
			{
				alg[(j) * 3] = x[(j+2) * 3];
				alg[(j+2) * 3] = (log(d[(j+16)]*(36*d[(j+10)]-alg[(j) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j) * 3]*__PAR_Rspv/36)*36;
				dx[1] = alg[(j+2) * 3]/__PAR_Tboost[(j)];
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp1[3];
	int j = 0;
	int j2= 0;
	int j3;
	int j4 = 0;
	int j5 = 0;
	int j6 = 0;
	int j7 = 0;
	switch(i)
	{
		case 10:
			for(j = 0;j <= 1; j++)
			{
				j5 = j;
			if (j5 >= 0 && j5 <= 1)
			{
				alg[(j+6) * 3] = ((d[(j5+4)])*x[(j5+2) * 3]-x[(j5) * 3])/((d[(j5+2)])+(d[(j5+4)]));
			}
			}
			for(j6 = 0; j6 <= 1; j6++)
	{
			alg[(j6+10) * 3] = (x[(j6) * 3]-x[30])/(__PAR_Rshunt+d[(j6+18)]);
	}
			for(j = 0;j <= 1; j++)
			{
				j5 = j;
			if (j5 >= 0 && j5 <= 1)
			{
				alg[(j+12) * 3] = x[30]*alg[(j5+10) * 3];
			}
			}
			alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
			for(j = 0; j <= 1; j++)
			{
				der[(j) * 3 + 1] = (alg[(j+6) * 3]-x[(j) * 3]/__PAR_R-(x[(j) * 3]-x[30])/(__PAR_Rshunt+d[(j+18)]))/__PAR_Cboost;
			}
			for(j = 0; j <= 1; j++)
			{
				der[(j+6) * 3 + 1] = alg[(j+12) * 3]/__PAR_TContMaxPot;
			}
	for(j3 = 0;j3 < 3; j3++)
	{
		tmp1[j3] = 0;
	}
	for(j3 = 0; j3 <= 1; j3++)
	{
		tmp1[0] += alg[(j3+10) * 3 + 0];
		tmp1[1] += alg[(j3+10) * 3 + 1];
		tmp1[2] += alg[(j3+10) * 3 + 2];
	}
			der[30 + 1] = (tmp1[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
			der[33 + 1] = __PAR_VlineaRef-x[30];
			break;
		case 11:
			for(j7 = 0; j7 <= 1; j7++)
	{
			alg[(j7+10) * 3] = (x[(j7) * 3]-x[30])/(__PAR_Rshunt+d[(j7+18)]);
	}
			alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
	for(j3 = 0;j3 < 3; j3++)
	{
		tmp1[j3] = 0;
	}
	for(j3 = 0; j3 <= 1; j3++)
	{
		tmp1[0] += alg[(j3+10) * 3 + 0];
		tmp1[1] += alg[(j3+10) * 3 + 1];
		tmp1[2] += alg[(j3+10) * 3 + 2];
	}
			der[30 + 1] = (tmp1[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
			return;
	}
	j = i;
	if(j >=0 && j <= 1)
	{
		alg[(j) * 3] = x[(j+2) * 3];
		alg[(j+2) * 3] = (log(d[(j+16)]*(36*d[(j+10)]-alg[(j) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j) * 3]*__PAR_Rspv/36)*36;
		alg[(j+4) * 3] = alg[(j+2) * 3];
		alg[(j+6) * 3] = ((d[(j+4)])*x[(j+2) * 3]-x[(j) * 3])/((d[(j+2)])+(d[(j+4)]));
		for(j4 = 0; j4 <= 1; j4++)
	{
		alg[(j4+10) * 3] = (x[(j4) * 3]-x[30])/(__PAR_Rshunt+d[(j4+18)]);
	}
		alg[(j+12) * 3] = x[30]*alg[(j+10) * 3];
		alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
		der[(j) * 3 + 1] = (alg[(j+6) * 3]-x[(j) * 3]/__PAR_R-(x[(j) * 3]-x[30])/(__PAR_Rshunt+d[(j+18)]))/__PAR_Cboost;
		der[(j+2) * 3 + 1] = (alg[(j+4) * 3]-d[(j+4)]*(x[(j+2) * 3]-alg[(j+6) * 3]))/__PAR_Lboost;
		der[(j+6) * 3 + 1] = alg[(j+12) * 3]/__PAR_TContMaxPot;
	for(j3 = 0;j3 < 3; j3++)
	{
		tmp1[j3] = 0;
	}
	for(j3 = 0; j3 <= 1; j3++)
	{
		tmp1[0] += alg[(j3+10) * 3 + 0];
		tmp1[1] += alg[(j3+10) * 3 + 1];
		tmp1[2] += alg[(j3+10) * 3 + 2];
	}
		der[30 + 1] = (tmp1[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
	}
	j = i-2;
	if(j >=0 && j <= 1)
	{
		alg[(j) * 3] = x[(j+2) * 3];
		alg[(j+2) * 3] = (log(d[(j+16)]*(36*d[(j+10)]-alg[(j) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j) * 3]*__PAR_Rspv/36)*36;
		alg[(j+4) * 3] = alg[(j+2) * 3];
		alg[(j+6) * 3] = ((d[(j+4)])*x[(j+2) * 3]-x[(j) * 3])/((d[(j+2)])+(d[(j+4)]));
		der[(j) * 3 + 1] = (alg[(j+6) * 3]-x[(j) * 3]/__PAR_R-(x[(j) * 3]-x[30])/(__PAR_Rshunt+d[(j+18)]))/__PAR_Cboost;
		der[(j+2) * 3 + 1] = (alg[(j+4) * 3]-d[(j+4)]*(x[(j+2) * 3]-alg[(j+6) * 3]))/__PAR_Lboost;
		der[(j+8) * 3 + 1] = alg[(j+2) * 3]/__PAR_Tboost[(j)];
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	if(i >= 0 && i <= 1)
	{
		zc[0] = t-(d[(i+30)]);
	}
	if(i >= 2 && i <= 3)
	{
		zc[0] = t-(d[(i+4)]);
	}
	if(i >= 4 && i <= 5)
	{
		zc[0] = t-d[(i+4)]-d[(i-4)]*__PAR_Tboost[(i-4)]-(0.0);
	}
	if(i >= 6 && i <= 7)
	{
		alg[(i-6) * 3] = x[(i-4) * 3];
		zc[0] = 36*d[(i+4)]-alg[(i-6) * 3]-(1.000000000000000055511151e-01);
	}
	if(i >= 8 && i <= 9)
	{
		alg[(i-8) * 3] = x[(i-6) * 3];
		zc[0] = 36*d[(i+2)]-alg[(i-8) * 3]-(1.000000000000000055511151e-01);
	}
	if(i >= 10 && i <= 11)
	{
		alg[(i-4) * 3] = ((d[(i-6)])*x[(i-8) * 3]-x[(i-10) * 3])/((d[(i-8)])+(d[(i-6)]));
		zc[0] = alg[(i-4) * 3]-(0.0);
	}
	if(i >= 12 && i <= 13)
	{
		alg[(i-6) * 3] = ((d[(i-8)])*x[(i-10) * 3]-x[(i-12) * 3])/((d[(i-10)])+(d[(i-8)]));
		alg[(i-4) * 3] = alg[(i-6) * 3]*d[(i-10)];
		zc[0] = alg[(i-4) * 3]-(0.0);
	}
	if(i >= 14 && i <= 15)
	{
		zc[0] = x[(i-14) * 3]-x[30]-(1.000000000000000055511151e-01);
	}
	if(i >= 16 && i <= 17)
	{
		zc[0] = (x[(i-16) * 3]-x[30])/d[(i+2)]-(1.000000000000000055511151e-01);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 1)
	{
		d[(i+30)] = d[(i+30)]+__PAR_TContMaxPot;
		d[(i+20)] = d[(i+22)];
		d[(i+22)] = x[(i+6) * 3];
		d[(i+24)] = d[(i+22)]-d[(i+20)];
		x[(i+6) * 3] = 0.0;
	if(d[(i+22)]<25.0)
	{
	if((d[(i+24)]>0.0&&d[(i+28)]<10.0))
	{
		d[(i+28)] = d[(i+28)]+1.0;
	}
	else
	{
	if((d[(i+28)]>1.0))
	{
		d[(i+28)] = d[(i+28)]-1.0;
	}
	}
	}
		d[(i+28)] = 2.0;
	}
	if(i >= 2 && i <= 3)
	{
		d[(i+6)] = d[(i+4)];
		d[(i+4)] = d[(i+4)]+__PAR_Tboost[(i-2)];
		d[(i+2)] = __PAR_ROn;
		d[(i+24)] = x[(i+6) * 3];
		x[(i+6) * 3] = 0.0;
		d[(i-2)] = -1.000000000000000020816682e-02*(d[(i+26)]-d[(i+24)])-x[(i+2) * 3]*6.999999999999999555910790e-01;
	if((d[(i-2)]>9.799999999999999822364316e-01))
	{
		d[(i-2)] = 9.799999999999999822364316e-01;
	}
	if((d[(i-2)]<2.000000000000000041633363e-02))
	{
		d[(i-2)] = 2.000000000000000041633363e-02;
	}
	}
	if(i >= 4 && i <= 5)
	{
		d[(i)] = __PAR_ROff;
	}
	if(i >= 8 && i <= 9)
	{
		d[(i+8)] = 1.0;
	}
	if(i >= 12 && i <= 13)
	{
		d[(i-10)] = __PAR_ROn;
	}
	if(i >= 14 && i <= 15)
	{
		d[(i+4)] = __PAR_ROn;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 6 && i <= 7)
	{
		d[(i+10)] = 0.0;
	}
	if(i >= 10 && i <= 11)
	{
		d[(i-8)] = __PAR_ROff;
	}
	if(i >= 16 && i <= 17)
	{
		d[(i+2)] = __PAR_ROff;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int j = 0;
	switch(i)
	{
		case 0:
			out[0] = x[30];
			return;
		default:
			j = i-1;
			if(j >=0 && j <= 1)
			{
				out[0] = d[(j)];
			}
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(32*sizeof(int));
	int *events = (int*)malloc(18*sizeof(int));
	int *outputs = (int*)malloc(3*sizeof(int));
	int *states = (int*)malloc(12*sizeof(int));
	int i10;
	int i8;
	int i;
	int j = 0;
	simulator->data = QSS_Data(12,32,18,0,15,"paneles");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Cboost = 1.000000000000000047921736e-04;
	__PAR_Lboost = 1.000000000000000047921736e-04;
	__PAR_R = 1.000000000000000000000000e+02;
	__PAR_ROn = 1.000000000000000081803054e-05;
	__PAR_ROff = 1.000000000000000000000000e+05;
	__PAR_q = 1.599999999999999912245234e-19;
	__PAR_Ac = 1.600000000000000088817842e+00;
	__PAR_K = 1.380499999999999856556809e-23;
	__PAR_K1 = 1.699999999999999905284098e-03;
	__PAR_Ior = 2.079300000000000149747359e-06;
	__PAR_Tref = 303.0;
	__PAR_Eg = 1.100000000000000088817842e+00;
	__PAR_Isc = 3.270000000000000017763568e+00;
	__PAR_Cbatery = 4.700000000000000184574578e-03;
	__PAR_Rshunt = 1.000000000000000047921736e-04;
	__PAR_iLoad = 1.000000000000000055511151e-01;
	__PAR_VlineaRef = 10.0;
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+22)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+16)] = 1.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+20)] = 0.0;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+2)] = 1.000000000000000000000000e+05;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+18)] = 1.000000000000000000000000e+05;
	}
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+4)] = 1.000000000000000000000000e+05;
	}
	modelData->x[30] = 0.0;
	for(i = 0; i <= 1;i++)
	{
		modelData->d[(i+28)] = 3.0;
	}
	// Initialize model code.
		modelData->d[(13)] = 273.0+5.000000000000000000000000e+01;
	for(i8 = 0; i8 <= 1; i8++)
	{
		__PAR_TContMaxPot = 5.000000000000000000000000e-01;
		modelData->d[(i8+30)] = 0.0;
		modelData->d[(i8+6)] = 0.000000000000000000000000e+00;
		modelData->d[(i8+4)] = 1.000000000000000000000000e+05;
		modelData->d[(i8+2)] = 1.000000000000000000000000e+05;
		__PAR_Tboost[(i8)] = 1.000000000000000047921736e-04;
		modelData->d[(i8)] = 9.000000000000000222044605e-01;
		modelData->d[(i8+14)] = 8.0;
		modelData->d[(i8+18)] = 1.000000000000000000000000e+05;
	}
		modelData->d[(0)] = 6.999999999999999555910790e-01;
		modelData->d[(1)] = 2.999999999999999888977698e-01;
		modelData->d[(28)] = 1.0;
		modelData->d[(29)] = 2.0;
		__PAR_Rspv = 1.000000000000000000000000e+00;
		modelData->d[(12)] = __PAR_Ior*pow((modelData->d[(13)]/__PAR_Tref),3.0)*exp(__PAR_q*__PAR_Eg*(1.0/__PAR_Tref-1.0/modelData->d[(13)])/__PAR_K/__PAR_Ac);
	for(i10 = 0; i10 <= 1; i10++)
	{
		modelData->d[(i10+10)] = (__PAR_Isc+__PAR_K1*(modelData->d[(13)]-__PAR_Tref))*modelData->d[(i10+14)]/100.0;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nDS[i] = 2;
		modelData->nDS[i+2] = 1;
	}
	modelData->nDS[11] = 1;
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
	modelData->nDS[i+6]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nDS[i+6]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nDS[i+8]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nDS[10]++;
	}
	modelData->nDS[10]++;
	modelData->nDS[10]++;
	for(i = 0; i <= 1; i++)
	{
		modelData->nSD[i]++;
		modelData->nSD[10]++;
		modelData->nSD[i+2]++;
	}
	modelData->nSD[10]++;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i+2]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i]++;
	}
	modelData->nSD[10] += 2;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i+2]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSD[i]++;
	}
	modelData->nSD[10]++;
	modelData->nSD[11]++;
	for(i = 0; i <= 1; i++)
	{
		modelData->nZS[i+14] = 2;
		modelData->nZS[i+16] = 2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+6]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+8]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+10]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+10]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+12]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nZS[i+12]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nSZ[i]++;
		modelData->nSZ[10]++;
		modelData->nSZ[i]++;
		modelData->nSZ[10]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i+2]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i+2]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i+2]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i]++;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->nSZ[i+2]++;
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
	for(i = 2; i <= 3; i++)
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
	for(i = 10; i <= 11; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 12; i <= 13; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 12; i <= 13; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 14; i <= 15; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 16; i <= 17; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 12; i <= 13; i++)
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
	for(i = 2; i <= 3; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 12; i <= 13; i++)
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
	for(i = 2; i <= 3; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 14; i <= 15; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 16; i <= 17; i++)
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
	for(i = 14; i <= 15; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 16; i <= 17; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].nLHSSt = 1;
	modelData->event[i+2].nLHSSt = 1;
	}
	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].nRHSSt = 1;
	modelData->event[i+2].nRHSSt = 2;
	}
	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].nLHSDsc = 5;
	modelData->event[i+2].nLHSDsc = 5;
	modelData->event[i+4].nLHSDsc = 1;
	modelData->event[i+6].nLHSDsc = 1;
	modelData->event[i+8].nLHSDsc = 1;
	modelData->event[i+10].nLHSDsc = 1;
	modelData->event[i+12].nLHSDsc = 1;
		modelData->event[i+14].nLHSDsc = 1;
		modelData->event[i+16].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,12);

	for(i = 0; i <= 1; i++)
	{
		modelData->DS[i][states[i]++] = i;
		modelData->DS[i][states[i]++] = 10;
		modelData->DS[i+2][states[i+2]++] = i+2;
	}
	modelData->DS[11][states[11]++] = 10;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i][states[i]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i+2][states[i+2]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i+6][states[i+6]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i+6][states[i+6]++] = 10;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[i+8][states[i+8]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->DS[10][states[10]++] = i;
	}
	modelData->DS[10][states[10]++] = 10;
	modelData->DS[10][states[10]++] = 11;
		cleanVector(states,0,12);

	for(i = 0; i <= 1; i++)
	{
		modelData->SD[i][states[i]++] = i;
		modelData->SD[10][states[10]++] = i;
		modelData->SD[i+2][states[i+2]++] = i+2;
	}
	modelData->SD[10][states[10]++] = 11;
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i+2][states[i+2]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i][states[i]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i][states[i]++] = i+6;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[10][states[10]++] = i+6;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i+2][states[i+2]++] = i+8;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SD[i][states[i]++] = 10;
	}
	modelData->SD[10][states[10]++] = 10;
	modelData->SD[11][states[11]++] = 10;
		cleanVector(events,0,18);

	for(i = 0; i <= 1; i++)
	{
		modelData->ZS[i+14][events[i+14]++] = i;
		modelData->ZS[i+14][events[i+14]++] = 10;
		modelData->ZS[i+16][events[i+16]++] = i;
		modelData->ZS[i+16][events[i+16]++] = 10;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+6][events[i+6]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+8][events[i+8]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+10][events[i+10]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+10][events[i+10]++] = i+2;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+12][events[i+12]++] = i;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->ZS[i+12][events[i+12]++] = i+2;
	}
		cleanVector(states,0,12);

	for(i = 0; i <= 1; i++)
	{
		modelData->SZ[i][states[i]++] = i+14;
		modelData->SZ[10][states[10]++] = i+14;
		modelData->SZ[i][states[i]++] = i+16;
		modelData->SZ[10][states[10]++] = i+16;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i+2][states[i+2]++] = i+6;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i+2][states[i+2]++] = i+8;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i][states[i]++] = i+10;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i+2][states[i+2]++] = i+10;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i][states[i]++] = i+12;
	}
	for ( i = 0; i <= 1; i++) 
	{
	modelData->SZ[i+2][states[i+2]++] = i+12;
	}
		cleanVector(events,0,18);

	for(i = 0; i <= 1; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i+8;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HZ[i][events[i]++] = i+10;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HZ[i][events[i]++] = i+6;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HZ[i][events[i]++] = i+8;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->HZ[i][events[i]++] = i+2;
	}
	for(i = 12; i <= 13; i++)
	{
		modelData->HZ[i][events[i]++] = i-2;
	}
	for(i = 12; i <= 13; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
		cleanVector(events,0,18);

	for(i = 14; i <= 15; i++)
	{
		modelData->HD[i][events[i]++] = i-14;
	}
	for(i = 16; i <= 17; i++)
	{
		modelData->HD[i][events[i]++] = i-16;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->HD[i][events[i]++] = i-10;
	}
	for(i = 12; i <= 13; i++)
	{
		modelData->HD[i][events[i]++] = i-12;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HD[i][events[i]++] = i-2;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HD[i][events[i]++] = i-4;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 4; i <= 5; i++)
	{
		modelData->HD[i][events[i]++] = i-2;
	}
	for(i = 10; i <= 11; i++)
	{
		modelData->HD[i][events[i]++] = i-8;
	}
	for(i = 12; i <= 13; i++)
	{
		modelData->HD[i][events[i]++] = i-10;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->HD[i][events[i]++] = i-4;
	}
	for(i = 8; i <= 9; i++)
	{
		modelData->HD[i][events[i]++] = i-6;
	}
	for(i = 2; i <= 3; i++)
	{
		modelData->HD[i][events[i]++] = i+2;
	}
	for(i = 0; i <= 1; i++)
	{
		modelData->HD[i][events[i]++] = i+4;
	}
	for(i = 14; i <= 15; i++)
	{
		modelData->HD[i][events[i]++] = i-8;
	}
	for(i = 16; i <= 17; i++)
	{
		modelData->HD[i][events[i]++] = i-10;
	}
	for(i = 6; i <= 7; i++)
	{
		modelData->HD[i][events[i]++] = i+2;
	}
	for(i = 8; i <= 9; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 14; i <= 15; i++)
	{
		modelData->HD[i][events[i]++] = 10;
	}
	for(i = 16; i <= 17; i++)
	{
		modelData->HD[i][events[i]++] = 10;
	}
		cleanVector(events,0,18);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].LHSSt[events[i]++] = i+6;
	modelData->event[i+2].LHSSt[events[i+2]++] = i+8;
	}
		cleanVector(events,0,18);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].RHSSt[events[i]++] = i+6;
	modelData->event[i+2].RHSSt[events[i+2]++] = i+4;
	modelData->event[i+2].RHSSt[events[i+2]++] = i+8;
	}
		cleanVector(events,0,18);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].LHSDsc[events[i]++] = i+20;
	modelData->event[i].LHSDsc[events[i]++] = i+22;
	modelData->event[i].LHSDsc[events[i]++] = i+24;
	modelData->event[i].LHSDsc[events[i]++] = i+28;
	modelData->event[i].LHSDsc[events[i]++] = i+30;
	modelData->event[i+2].LHSDsc[events[i+2]++] = i;
	modelData->event[i+2].LHSDsc[events[i+2]++] = i+4;
	modelData->event[i+2].LHSDsc[events[i+2]++] = i+6;
	modelData->event[i+2].LHSDsc[events[i+2]++] = i+8;
	modelData->event[i+2].LHSDsc[events[i+2]++] = i+26;
	modelData->event[i+4].LHSDsc[events[i+4]++] = i+4;
	modelData->event[i+6].LHSDsc[events[i+6]++] = i+16;
	modelData->event[i+8].LHSDsc[events[i+8]++] = i+16;
	modelData->event[i+10].LHSDsc[events[i+10]++] = i+2;
	modelData->event[i+12].LHSDsc[events[i+12]++] = i+2;
		modelData->event[i+14].LHSDsc[events[i+14]++] = i+18;
		modelData->event[i+16].LHSDsc[events[i+16]++] = i+18;
	}
		cleanVector(events,0,18);

	for(i = 0; i <= 1; i++)
	{
	modelData->event[i].direction = 1;
	modelData->event[i].relation = 2;
	modelData->event[i+2].direction = 1;
	modelData->event[i+2].relation = 2;
	modelData->event[i+4].direction = 1;
	modelData->event[i+4].relation = 2;
	modelData->event[i+6].direction = -1;
	modelData->event[i+6].relation = 0;
	modelData->event[i+8].direction = 1;
	modelData->event[i+8].relation = 2;
	modelData->event[i+10].direction = -1;
	modelData->event[i+10].relation = 0;
	modelData->event[i+12].direction = 1;
	modelData->event[i+12].relation = 2;
		modelData->event[i+14].direction = 1;
		modelData->event[i+14].relation = 2;
		modelData->event[i+16].direction = -1;
		modelData->event[i+16].relation = 0;
	}
	simulator->time = QSS_Time(12,18,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("paneles",3,32,12,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[10]++;
	for(i = 0; i <= 1; i++)
	{
		modelOutput->nOD[i+1] = 1;
		modelOutput->nDO[i]++;
	}
	SD_allocOutputMatrix(modelOutput,12,32);
		cleanVector(states,0,12);

		cleanVector(discretes,0,32);

		cleanVector(outputs,0,3);

		sprintf(modelOutput->variable[0].name,"Uline");
	for(i = 0; i <= 1; i++)
	{
		sprintf(modelOutput->variable[i+1].name,"DCboost[%d]",i+1);
	}
		cleanVector(outputs,0,3);

		modelOutput->SO[10][states[10]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 10;
		cleanVector(outputs,0,3);

	for(i = 0; i <= 1; i++)
	{
		modelOutput->DO[i][discretes[i]++] = i+1;
		modelOutput->OD[i+1][outputs[i+1]++] = i;
	}
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
