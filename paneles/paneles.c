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
	double tmp20[3];
	int i0 = i;
	int j0 = 0;
	int j30= 0;
	int j40;
	switch(i)
	{
		case 10:
			for(j30 = 0; j30 <= 1; j30++)
			{
					alg[(j30+10) * 3] = (x[(j30) * 3]-x[30])/(__PAR_Rshunt+d[(j30+18)]);
			}
			alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+10) * 3 + 0];
		tmp20[1] += alg[(j40+10) * 3 + 1];
		tmp20[2] += alg[(j40+10) * 3 + 2];
	}
			dx[1] = (tmp20[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
			return;
		case 11:
			dx[1] = __PAR_VlineaRef-x[30];
			return;
		default:
			j0 = i0;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+6) * 3] = ((d[(j0+4)])*x[(j0+2) * 3]-x[(j0) * 3])/((d[(j0+2)])+(d[(j0+4)]));
				dx[1] = (alg[(j0+6) * 3]-x[(j0) * 3]/__PAR_R-(x[(j0) * 3]-x[30])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
			}
			j0 = i0-2;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0) * 3] = x[(j0+2) * 3];
				alg[(j0+2) * 3] = (log(d[(j0+16)]*(36*d[(j0+10)]-alg[(j0) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j0) * 3]*__PAR_Rspv/36)*36;
				alg[(j0+4) * 3] = alg[(j0+2) * 3];
				alg[(j0+6) * 3] = ((d[(j0+4)])*x[(j0+2) * 3]-x[(j0) * 3])/((d[(j0+2)])+(d[(j0+4)]));
				dx[1] = (alg[(j0+4) * 3]-d[(j0+4)]*(x[(j0+2) * 3]-alg[(j0+6) * 3]))/__PAR_Lboost;
			}
			j0 = i0-4;
			if(j0 >=0 && j0 <= 1)
			{
				dx[1] = d[(j0+28)]-d[(j0+26)];
			}
			j0 = i0-6;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+10) * 3] = (x[(j0) * 3]-x[30])/(__PAR_Rshunt+d[(j0+18)]);
				alg[(j0+12) * 3] = x[30]*alg[(j0+10) * 3];
				dx[1] = alg[(j0+12) * 3]/__PAR_TContMaxPot;
			}
			j0 = i0-8;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0) * 3] = x[(j0+2) * 3];
				alg[(j0+2) * 3] = (log(d[(j0+16)]*(36*d[(j0+10)]-alg[(j0) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j0) * 3]*__PAR_Rspv/36)*36;
				dx[1] = alg[(j0+2) * 3]/__PAR_Tboost[(j0)];
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp20[3];
	int i0 = i;
	int j0 = 0;
	int j060 = 0;
	int j30= 0;
	int j40;
	int j500 = 0;
	int j700 = 0;
	int j800 = 0;
	switch(i)
	{
		case 10:
			for(j0 = 0;j0 <= 1; j0++)
			{
				j060 = j0;
			if (j060 >= 0 && j060 <= 1)
			{
				alg[(j0+6) * 3] = ((d[(j060+4)])*x[(j060+2) * 3]-x[(j060) * 3])/((d[(j060+2)])+(d[(j060+4)]));
			}
			}
			for(j700 = 0; j700 <= 1; j700++)
	{
			alg[(j700+10) * 3] = (x[(j700) * 3]-x[30])/(__PAR_Rshunt+d[(j700+18)]);
	}
			for(j0 = 0;j0 <= 1; j0++)
			{
				j060 = j0;
			if (j060 >= 0 && j060 <= 1)
			{
				alg[(j0+12) * 3] = x[30]*alg[(j060+10) * 3];
			}
			}
			alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
			for(j0 = 0; j0 <= 1; j0++)
			{
				der[(j0) * 3 + 1] = (alg[(j0+6) * 3]-x[(j0) * 3]/__PAR_R-(x[(j0) * 3]-x[30])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
			}
			for(j0 = 0; j0 <= 1; j0++)
			{
				der[(j0+6) * 3 + 1] = alg[(j0+12) * 3]/__PAR_TContMaxPot;
			}
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+10) * 3 + 0];
		tmp20[1] += alg[(j40+10) * 3 + 1];
		tmp20[2] += alg[(j40+10) * 3 + 2];
	}
			der[30 + 1] = (tmp20[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
			der[33 + 1] = __PAR_VlineaRef-x[30];
			break;
		case 11:
			for(j800 = 0; j800 <= 1; j800++)
	{
			alg[(j800+10) * 3] = (x[(j800) * 3]-x[30])/(__PAR_Rshunt+d[(j800+18)]);
	}
			alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+10) * 3 + 0];
		tmp20[1] += alg[(j40+10) * 3 + 1];
		tmp20[2] += alg[(j40+10) * 3 + 2];
	}
			der[30 + 1] = (tmp20[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
			return;
	}
	j0 = i0;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = x[(j0+2) * 3];
		alg[(j0+2) * 3] = (log(d[(j0+16)]*(36*d[(j0+10)]-alg[(j0) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j0) * 3]*__PAR_Rspv/36)*36;
		alg[(j0+4) * 3] = alg[(j0+2) * 3];
		alg[(j0+6) * 3] = ((d[(j0+4)])*x[(j0+2) * 3]-x[(j0) * 3])/((d[(j0+2)])+(d[(j0+4)]));
		for(j500 = 0; j500 <= 1; j500++)
	{
		alg[(j500+10) * 3] = (x[(j500) * 3]-x[30])/(__PAR_Rshunt+d[(j500+18)]);
	}
		alg[(j0+12) * 3] = x[30]*alg[(j0+10) * 3];
		alg[42] = -(__PAR_VlineaRef-x[30])-x[33];
		der[(j0) * 3 + 1] = (alg[(j0+6) * 3]-x[(j0) * 3]/__PAR_R-(x[(j0) * 3]-x[30])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
		der[(j0+2) * 3 + 1] = (alg[(j0+4) * 3]-d[(j0+4)]*(x[(j0+2) * 3]-alg[(j0+6) * 3]))/__PAR_Lboost;
		der[(j0+6) * 3 + 1] = alg[(j0+12) * 3]/__PAR_TContMaxPot;
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+10) * 3 + 0];
		tmp20[1] += alg[(j40+10) * 3 + 1];
		tmp20[2] += alg[(j40+10) * 3 + 2];
	}
		der[30 + 1] = (tmp20[0]-__PAR_iLoad-alg[42])/__PAR_Cbatery;
	}
	j0 = i0-2;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = x[(j0+2) * 3];
		alg[(j0+2) * 3] = (log(d[(j0+16)]*(36*d[(j0+10)]-alg[(j0) * 3])/(36*d[(12)])+1.0)*__PAR_K*__PAR_Ac*d[(13)]/__PAR_q-alg[(j0) * 3]*__PAR_Rspv/36)*36;
		alg[(j0+4) * 3] = alg[(j0+2) * 3];
		alg[(j0+6) * 3] = ((d[(j0+4)])*x[(j0+2) * 3]-x[(j0) * 3])/((d[(j0+2)])+(d[(j0+4)]));
		der[(j0) * 3 + 1] = (alg[(j0+6) * 3]-x[(j0) * 3]/__PAR_R-(x[(j0) * 3]-x[30])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
		der[(j0+2) * 3 + 1] = (alg[(j0+4) * 3]-d[(j0+4)]*(x[(j0+2) * 3]-alg[(j0+6) * 3]))/__PAR_Lboost;
		der[(j0+8) * 3 + 1] = alg[(j0+2) * 3]/__PAR_Tboost[(j0)];
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1)
	{
		zc[0] = t-(d[(i0+30)]);
	}
	if(i0 >= 2 && i0 <= 3)
	{
		zc[0] = t-(d[(i0+4)]);
	}
	if(i0 >= 4 && i0 <= 5)
	{
		zc[0] = t-d[(i0+4)]-d[(i0-4)]*__PAR_Tboost[(i0-4)]-(0.0);
	}
	if(i0 >= 6 && i0 <= 7)
	{
		alg[(i0-6) * 3] = x[(i0-4) * 3];
		zc[0] = 36*d[(i0+4)]-alg[(i0-6) * 3]-(1.000000000000000055511151e-01);
	}
	if(i0 >= 8 && i0 <= 9)
	{
		alg[(i0-8) * 3] = x[(i0-6) * 3];
		zc[0] = 36*d[(i0+2)]-alg[(i0-8) * 3]-(1.000000000000000055511151e-01);
	}
	if(i0 >= 10 && i0 <= 11)
	{
		alg[(i0-4) * 3] = ((d[(i0-6)])*x[(i0-8) * 3]-x[(i0-10) * 3])/((d[(i0-8)])+(d[(i0-6)]));
		zc[0] = alg[(i0-4) * 3]-(0.0);
	}
	if(i0 >= 12 && i0 <= 13)
	{
		alg[(i0-6) * 3] = ((d[(i0-8)])*x[(i0-10) * 3]-x[(i0-12) * 3])/((d[(i0-10)])+(d[(i0-8)]));
		alg[(i0-4) * 3] = alg[(i0-6) * 3]*d[(i0-10)];
		zc[0] = alg[(i0-4) * 3]-(0.0);
	}
	if(i0 >= 14 && i0 <= 15)
	{
		zc[0] = x[(i0-14) * 3]-x[30]-(1.000000000000000055511151e-01);
	}
	if(i0 >= 16 && i0 <= 17)
	{
		zc[0] = (x[(i0-16) * 3]-x[30])/d[(i0+2)]-(1.000000000000000055511151e-01);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1)
	{
		d[(i0+30)] = d[(i0+30)]+__PAR_TContMaxPot;
		d[(i0+20)] = d[(i0+22)];
		d[(i0+22)] = x[(i0+6) * 3];
		d[(i0+24)] = d[(i0+22)]-d[(i0+20)];
		x[(i0+6) * 3] = 0.0;
	if(d[(i0+22)]<25.0)
	{
	if((d[(i0+24)]>0.0&&d[(i0+28)]<10.0))
	{
		d[(i0+28)] = d[(i0+28)]+1.0;
	}
	else
	{
	if((d[(i0+28)]>1.0))
	{
		d[(i0+28)] = d[(i0+28)]-1.0;
	}
	}
	}
		d[(i0+28)] = 2.0;
	}
	if(i0 >= 2 && i0 <= 3)
	{
		d[(i0+6)] = d[(i0+4)];
		d[(i0+4)] = d[(i0+4)]+__PAR_Tboost[(i0-2)];
		d[(i0+2)] = __PAR_ROn;
		d[(i0+24)] = x[(i0+6) * 3];
		x[(i0+6) * 3] = 0.0;
		d[(i0-2)] = -1.000000000000000020816682e-02*(d[(i0+26)]-d[(i0+24)])-x[(i0+2) * 3]*6.999999999999999555910790e-01;
	if((d[(i0-2)]>9.799999999999999822364316e-01))
	{
		d[(i0-2)] = 9.799999999999999822364316e-01;
	}
	if((d[(i0-2)]<2.000000000000000041633363e-02))
	{
		d[(i0-2)] = 2.000000000000000041633363e-02;
	}
	}
	if(i0 >= 4 && i0 <= 5)
	{
		d[(i0)] = __PAR_ROff;
	}
	if(i0 >= 8 && i0 <= 9)
	{
		d[(i0+8)] = 1.0;
	}
	if(i0 >= 12 && i0 <= 13)
	{
		d[(i0-10)] = __PAR_ROn;
	}
	if(i0 >= 14 && i0 <= 15)
	{
		d[(i0+4)] = __PAR_ROn;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 6 && i0 <= 7)
	{
		d[(i0+10)] = 0.0;
	}
	if(i0 >= 10 && i0 <= 11)
	{
		d[(i0-8)] = __PAR_ROff;
	}
	if(i0 >= 16 && i0 <= 17)
	{
		d[(i0+2)] = __PAR_ROff;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			out[0] = x[30];
			return;
		default:
			j0 = i0-1;
			if(j0 >=0 && j0 <= 1)
			{
				out[0] = d[(j0)];
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
	int i0;
	int j0 = 0;
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
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+22)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+16)] = 1.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+20)] = 0.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+2)] = 1.000000000000000000000000e+05;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+18)] = 1.000000000000000000000000e+05;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+4)] = 1.000000000000000000000000e+05;
	}
	modelData->x[30] = 0.0;
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+28)] = 3.0;
	}
	// Initialize model code.
		modelData->d[(13)] = 273.0+5.000000000000000000000000e+01;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_TContMaxPot = 5.000000000000000000000000e-01;
		modelData->d[(i0+30)] = 0.0;
		modelData->d[(i0+6)] = 1.000000000000000047921736e-04;
		modelData->d[(i0+4)] = 1.000000000000000000000000e+05;
		modelData->d[(i0+2)] = 1.000000000000000000000000e+05;
		__PAR_Tboost[(i0)] = 1.000000000000000047921736e-04;
		modelData->d[(i0)] = 9.000000000000000222044605e-01;
		modelData->d[(i0+14)] = 8.0;
		modelData->d[(i0+18)] = 1.000000000000000000000000e+05;
	}
		modelData->d[(28)] = 1.0;
		modelData->d[(29)] = 2.0;
		__PAR_Rspv = 1.000000000000000000000000e+00;
		modelData->d[(12)] = __PAR_Ior*pow((modelData->d[(13)]/__PAR_Tref),3.0)*exp(__PAR_q*__PAR_Eg*(1.0/__PAR_Tref-1.0/modelData->d[(13)])/__PAR_K/__PAR_Ac);
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->d[(i0+10)] = (__PAR_Isc+__PAR_K1*(modelData->d[(13)]-__PAR_Tref))*modelData->d[(i0+14)]/100.0;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nDS[i0] = 2;
		modelData->nDS[i0+2] = 1;
	}
	modelData->nDS[11] = 1;
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
	modelData->nDS[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+8]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[10]++;
	}
	modelData->nDS[10]++;
	modelData->nDS[10]++;
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nSD[i0]++;
		modelData->nSD[10]++;
		modelData->nSD[i0+2]++;
	}
	modelData->nSD[10]++;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0]++;
	}
	modelData->nSD[10] += 2;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0]++;
	}
	modelData->nSD[10]++;
	modelData->nSD[11]++;
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nZS[i0+14] = 2;
		modelData->nZS[i0+16] = 2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+8]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+10]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+10]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+12]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nZS[i0+12]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nSZ[i0]++;
		modelData->nSZ[10]++;
		modelData->nSZ[i0]++;
		modelData->nSZ[10]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+2]++;
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
	for(i0 = 2; i0 <= 3; i0++)
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
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 12; i0 <= 13; i0++)
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
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 12; i0 <= 13; i0++)
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
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 16; i0 <= 17; i0++)
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
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].nLHSSt = 1;
	modelData->event[i0+2].nLHSSt = 1;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].nRHSSt = 1;
	modelData->event[i0+2].nRHSSt = 2;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].nLHSDsc = 5;
	modelData->event[i0+2].nLHSDsc = 5;
	modelData->event[i0+4].nLHSDsc = 1;
	modelData->event[i0+6].nLHSDsc = 1;
	modelData->event[i0+8].nLHSDsc = 1;
	modelData->event[i0+10].nLHSDsc = 1;
	modelData->event[i0+12].nLHSDsc = 1;
		modelData->event[i0+14].nLHSDsc = 1;
		modelData->event[i0+16].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,12);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = 10;
		modelData->DS[i0+2][states[i0+2]++] = i0+2;
	}
	modelData->DS[11][states[11]++] = 10;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+2][states[i0+2]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+6][states[i0+6]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+6][states[i0+6]++] = 10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+8][states[i0+8]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[10][states[10]++] = i0;
	}
	modelData->DS[10][states[10]++] = 10;
	modelData->DS[10][states[10]++] = 11;
		cleanVector(states,0,12);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[10][states[10]++] = i0;
		modelData->SD[i0+2][states[i0+2]++] = i0+2;
	}
	modelData->SD[10][states[10]++] = 11;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+2][states[i0+2]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0+6;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[10][states[10]++] = i0+6;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+2][states[i0+2]++] = i0+8;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0][states[i0]++] = 10;
	}
	modelData->SD[10][states[10]++] = 10;
	modelData->SD[11][states[11]++] = 10;
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->ZS[i0+14][events[i0+14]++] = i0;
		modelData->ZS[i0+14][events[i0+14]++] = 10;
		modelData->ZS[i0+16][events[i0+16]++] = i0;
		modelData->ZS[i0+16][events[i0+16]++] = 10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+6][events[i0+6]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+8][events[i0+8]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+10][events[i0+10]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+10][events[i0+10]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+12][events[i0+12]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+12][events[i0+12]++] = i0+2;
	}
		cleanVector(states,0,12);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0+14;
		modelData->SZ[10][states[10]++] = i0+14;
		modelData->SZ[i0][states[i0]++] = i0+16;
		modelData->SZ[10][states[10]++] = i0+16;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+2][states[i0+2]++] = i0+6;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+2][states[i0+2]++] = i0+8;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+2][states[i0+2]++] = i0+10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+12;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+2][states[i0+2]++] = i0+12;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+8;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+10;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+6;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+8;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+2;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0-2;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,18);

	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-14;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-16;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-12;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-4;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-8;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-4;
	}
	for(i0 = 8; i0 <= 9; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-6;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+2;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+4;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-8;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 6; i0 <= 7; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+2;
	}
	for(i0 = 8; i0 <= 9; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->HD[i0][events[i0]++] = 10;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->HD[i0][events[i0]++] = 10;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].LHSSt[events[i0]++] = i0+6;
	modelData->event[i0+2].LHSSt[events[i0+2]++] = i0+8;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].RHSSt[events[i0]++] = i0+6;
	modelData->event[i0+2].RHSSt[events[i0+2]++] = i0+4;
	modelData->event[i0+2].RHSSt[events[i0+2]++] = i0+8;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].LHSDsc[events[i0]++] = i0+20;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+22;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+24;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+28;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+30;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+4;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+6;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+8;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+26;
	modelData->event[i0+4].LHSDsc[events[i0+4]++] = i0+4;
	modelData->event[i0+6].LHSDsc[events[i0+6]++] = i0+16;
	modelData->event[i0+8].LHSDsc[events[i0+8]++] = i0+16;
	modelData->event[i0+10].LHSDsc[events[i0+10]++] = i0+2;
	modelData->event[i0+12].LHSDsc[events[i0+12]++] = i0+2;
		modelData->event[i0+14].LHSDsc[events[i0+14]++] = i0+18;
		modelData->event[i0+16].LHSDsc[events[i0+16]++] = i0+18;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].direction = 1;
	modelData->event[i0].relation = 2;
	modelData->event[i0+2].direction = 1;
	modelData->event[i0+2].relation = 2;
	modelData->event[i0+4].direction = 1;
	modelData->event[i0+4].relation = 2;
	modelData->event[i0+6].direction = -1;
	modelData->event[i0+6].relation = 0;
	modelData->event[i0+8].direction = 1;
	modelData->event[i0+8].relation = 2;
	modelData->event[i0+10].direction = -1;
	modelData->event[i0+10].relation = 0;
	modelData->event[i0+12].direction = 1;
	modelData->event[i0+12].relation = 2;
		modelData->event[i0+14].direction = 1;
		modelData->event[i0+14].relation = 2;
		modelData->event[i0+16].direction = -1;
		modelData->event[i0+16].relation = 0;
	}
	simulator->time = QSS_Time(12,18,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("paneles",3,32,12,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[10]++;
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->nOD[i0+1] = 1;
		modelOutput->nDO[i0]++;
	}
	SD_allocOutputMatrix(modelOutput,12,32);
		cleanVector(states,0,12);

		cleanVector(discretes,0,32);

		cleanVector(outputs,0,3);

		sprintf(modelOutput->variable[0].name,"Uline");
	for(i0 = 0; i0 <= 1; i0++)
	{
		sprintf(modelOutput->variable[i0+1].name,"DCboost[%d]",i0+1);
	}
		cleanVector(outputs,0,3);

		modelOutput->SO[10][states[10]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 10;
		cleanVector(outputs,0,3);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->DO[i0][discretes[i0]++] = i0+1;
		modelOutput->OD[i0+1][outputs[i0+1]++] = i0;
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
