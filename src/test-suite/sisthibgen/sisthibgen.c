#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "sisthibgen_functions.h"
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
		case 16:
			for(j30 = 0; j30 <= 1; j30++)
			{
					alg[(j30+8) * 3] = (x[(j30+2) * 3]-x[48])/(__PAR_Rshunt+d[(j30+18)]);
			}
			alg[36] = -(__PAR_VlineaRef-x[48])*1.000000000000000055511151e-01-x[51];
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+8) * 3 + 0];
		tmp20[1] += alg[(j40+8) * 3 + 1];
		tmp20[2] += alg[(j40+8) * 3 + 2];
	}
			dx[1] = (tmp20[0]-__PAR_iLoad-alg[36])/__PAR_Cbatery;
			return;
		case 17:
			dx[1] = __PAR_VlineaRef-x[48];
			return;
		default:
			j0 = i0;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],36,36,d[(10)],d[(j0+14)],1.000000000000000081803054e-05,6.0,d[(j0+20)]);
				dx[1] = (alg[(j0) * 3]-x[(j0+4) * 3])/__PAR_Cpvin;
			}
			j0 = i0-2;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-x[(j0+2) * 3])/((d[(j0+2)])+(d[(j0+4)]));
				dx[1] = (alg[(j0+4) * 3]-x[(j0+2) * 3]/__PAR_R-(x[(j0+2) * 3]-x[48])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
			}
			j0 = i0-4;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+2) * 3] = x[(j0) * 3];
				alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-x[(j0+2) * 3])/((d[(j0+2)])+(d[(j0+4)]));
				dx[1] = (alg[(j0+2) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
			}
			j0 = i0-6;
			if(j0 >=0 && j0 <= 1)
			{
				dx[1] = d[(j0+30)]-x[(j0) * 3];
			}
			j0 = i0-8;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],36,36,d[(10)],d[(j0+14)],1.000000000000000081803054e-05,6.0,d[(j0+20)]);
				alg[(j0+10) * 3] = x[(j0) * 3]*alg[(j0) * 3];
				dx[1] = alg[(j0+10) * 3]/__PAR_TContMaxPot;
			}
			j0 = i0-10;
			if(j0 >=0 && j0 <= 1)
			{
				dx[1] = x[(j0) * 3]/__PAR_Tboost[(j0)];
			}
			j0 = i0-12;
			if(j0 >=0 && j0 <= 1)
			{
				dx[1] = (x[(j0) * 3]-x[(j0+12) * 3])*100.0;
			}
			j0 = i0-14;
			if(j0 >=0 && j0 <= 1)
			{
				alg[(j0+8) * 3] = (x[(j0+2) * 3]-x[48])/(__PAR_Rshunt+d[(j0+18)]);
				dx[1] = (alg[(j0+8) * 3]-x[(j0+14) * 3]);
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
		case 16:
			for(j0 = 0;j0 <= 1; j0++)
			{
				j060 = j0;
			if (j060 >= 0 && j060 <= 1)
			{
				alg[(j0+4) * 3] = ((d[(j060+4)])*x[(j060+4) * 3]-x[(j060+2) * 3])/((d[(j060+2)])+(d[(j060+4)]));
			}
			}
			for(j700 = 0; j700 <= 1; j700++)
	{
			alg[(j700+8) * 3] = (x[(j700+2) * 3]-x[48])/(__PAR_Rshunt+d[(j700+18)]);
	}
			alg[36] = -(__PAR_VlineaRef-x[48])*1.000000000000000055511151e-01-x[51];
			for(j0 = 0; j0 <= 1; j0++)
			{
				der[(j0+2) * 3 + 1] = (alg[(j0+4) * 3]-x[(j0+2) * 3]/__PAR_R-(x[(j0+2) * 3]-x[48])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
			}
			for(j0 = 0; j0 <= 1; j0++)
			{
				der[(j0+14) * 3 + 1] = (alg[(j0+8) * 3]-x[(j0+14) * 3]);
			}
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+8) * 3 + 0];
		tmp20[1] += alg[(j40+8) * 3 + 1];
		tmp20[2] += alg[(j40+8) * 3 + 2];
	}
			der[48 + 1] = (tmp20[0]-__PAR_iLoad-alg[36])/__PAR_Cbatery;
			der[51 + 1] = __PAR_VlineaRef-x[48];
			break;
		case 17:
			for(j800 = 0; j800 <= 1; j800++)
	{
			alg[(j800+8) * 3] = (x[(j800+2) * 3]-x[48])/(__PAR_Rshunt+d[(j800+18)]);
	}
			alg[36] = -(__PAR_VlineaRef-x[48])*1.000000000000000055511151e-01-x[51];
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+8) * 3 + 0];
		tmp20[1] += alg[(j40+8) * 3 + 1];
		tmp20[2] += alg[(j40+8) * 3 + 2];
	}
			der[48 + 1] = (tmp20[0]-__PAR_iLoad-alg[36])/__PAR_Cbatery;
			return;
	}
	j0 = i0;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],36,36,d[(10)],d[(j0+14)],1.000000000000000081803054e-05,6.0,d[(j0+20)]);
		alg[(j0+2) * 3] = x[(j0) * 3];
		alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-x[(j0+2) * 3])/((d[(j0+2)])+(d[(j0+4)]));
		alg[(j0+10) * 3] = x[(j0) * 3]*alg[(j0) * 3];
		der[(j0) * 3 + 1] = (alg[(j0) * 3]-x[(j0+4) * 3])/__PAR_Cpvin;
		der[(j0+4) * 3 + 1] = (alg[(j0+2) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
		der[(j0+6) * 3 + 1] = d[(j0+30)]-x[(j0) * 3];
		der[(j0+8) * 3 + 1] = alg[(j0+10) * 3]/__PAR_TContMaxPot;
		der[(j0+10) * 3 + 1] = x[(j0) * 3]/__PAR_Tboost[(j0)];
		der[(j0+12) * 3 + 1] = (x[(j0) * 3]-x[(j0+12) * 3])*100.0;
	}
	j0 = i0-2;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0+2) * 3] = x[(j0) * 3];
		alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-x[(j0+2) * 3])/((d[(j0+2)])+(d[(j0+4)]));
		for(j500 = 0; j500 <= 1; j500++)
	{
		alg[(j500+8) * 3] = (x[(j500+2) * 3]-x[48])/(__PAR_Rshunt+d[(j500+18)]);
	}
		alg[36] = -(__PAR_VlineaRef-x[48])*1.000000000000000055511151e-01-x[51];
		der[(j0+2) * 3 + 1] = (alg[(j0+4) * 3]-x[(j0+2) * 3]/__PAR_R-(x[(j0+2) * 3]-x[48])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
		der[(j0+4) * 3 + 1] = (alg[(j0+2) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
		der[(j0+14) * 3 + 1] = (alg[(j0+8) * 3]-x[(j0+14) * 3]);
	for(j40 = 0;j40 < 3; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+8) * 3 + 0];
		tmp20[1] += alg[(j40+8) * 3 + 1];
		tmp20[2] += alg[(j40+8) * 3 + 2];
	}
		der[48 + 1] = (tmp20[0]-__PAR_iLoad-alg[36])/__PAR_Cbatery;
	}
	j0 = i0-4;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],36,36,d[(10)],d[(j0+14)],1.000000000000000081803054e-05,6.0,d[(j0+20)]);
		alg[(j0+2) * 3] = x[(j0) * 3];
		alg[(j0+4) * 3] = ((d[(j0+4)])*x[(j0+4) * 3]-x[(j0+2) * 3])/((d[(j0+2)])+(d[(j0+4)]));
		der[(j0) * 3 + 1] = (alg[(j0) * 3]-x[(j0+4) * 3])/__PAR_Cpvin;
		der[(j0+2) * 3 + 1] = (alg[(j0+4) * 3]-x[(j0+2) * 3]/__PAR_R-(x[(j0+2) * 3]-x[48])/(__PAR_Rshunt+d[(j0+18)]))/__PAR_Cboost;
		der[(j0+4) * 3 + 1] = (alg[(j0+2) * 3]-d[(j0+4)]*(x[(j0+4) * 3]-alg[(j0+4) * 3]))/__PAR_Lboost;
	}
	j0 = i0-12;
	if(j0 >=0 && j0 <= 1)
	{
		der[(j0+12) * 3 + 1] = (x[(j0) * 3]-x[(j0+12) * 3])*100.0;
	}
	j0 = i0-14;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0+8) * 3] = (x[(j0+2) * 3]-x[48])/(__PAR_Rshunt+d[(j0+18)]);
		der[(j0+14) * 3 + 1] = (alg[(j0+8) * 3]-x[(j0+14) * 3]);
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1)
	{
		zc[0] = t-(d[(i0+32)]);
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
		alg[(i0-6) * 3] = __ipvsolver(x[(i0-6) * 3],36,36,d[(10)],d[(i0+8)],1.000000000000000081803054e-05,6.0,d[(i0+14)]);
		zc[0] = 36*d[(i0+5)]-alg[(i0-6) * 3]-(1.000000000000000020816682e-03);
	}
	if(i0 >= 8 && i0 <= 9)
	{
		alg[(i0-8) * 3] = __ipvsolver(x[(i0-8) * 3],36,36,d[(10)],d[(i0+6)],1.000000000000000081803054e-05,6.0,d[(i0+12)]);
		zc[0] = 36*d[(i0+3)]-alg[(i0-8) * 3]-(1.000000000000000020816682e-03);
	}
	if(i0 >= 10 && i0 <= 11)
	{
		alg[(i0-6) * 3] = ((d[(i0-6)])*x[(i0-6) * 3]-x[(i0-8) * 3])/((d[(i0-8)])+(d[(i0-6)]));
		zc[0] = alg[(i0-6) * 3]-(0.0);
	}
	if(i0 >= 12 && i0 <= 13)
	{
		alg[(i0-8) * 3] = ((d[(i0-8)])*x[(i0-8) * 3]-x[(i0-10) * 3])/((d[(i0-10)])+(d[(i0-8)]));
		alg[(i0-6) * 3] = alg[(i0-8) * 3]*d[(i0-10)];
		zc[0] = alg[(i0-6) * 3]-(0.0);
	}
	if(i0 >= 14 && i0 <= 15)
	{
		zc[0] = x[(i0-12) * 3]-x[48]-(1.000000000000000055511151e-01);
	}
	if(i0 >= 16 && i0 <= 17)
	{
		zc[0] = (x[(i0-14) * 3]-x[48])/d[(i0+2)]-(1.000000000000000055511151e-01);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1)
	{
		d[(i0+32)] = d[(i0+32)]+__PAR_TContMaxPot;
		d[(i0+22)] = d[(i0+24)];
		d[(i0+24)] = x[(i0+8) * 3];
		d[(i0+26)] = d[(i0+24)]-d[(i0+22)];
		x[(i0+8) * 3] = 0.0;
	if((d[(i0+26)]>0.0&&d[(i0+30)]<10.0))
	{
		d[(i0+30)] = d[(i0+30)]+5.000000000000000000000000e-01;
	}
	else
	{
	if((d[(i0+30)]>1.0))
	{
		d[(i0+30)] = d[(i0+30)]-5.000000000000000000000000e-01;
	}
	}
	}
	if(i0 >= 2 && i0 <= 3)
	{
		d[(i0+6)] = d[(i0+4)];
		d[(i0+4)] = d[(i0+4)]+__PAR_Tboost[(i0-2)];
		d[(i0+2)] = __PAR_ROn;
		alg[(i0-2) * 3] = __ipvsolver(x[(i0-2) * 3],36,36,d[(10)],d[(i0+12)],1.000000000000000081803054e-05,6.0,d[(i0+18)]);
		d[(i0+18)] = alg[(i0-2) * 3];
		d[(i0+26)] = x[(i0+8) * 3];
		x[(i0+8) * 3] = 0.0;
		d[(i0-2)] = -1.000000000000000020816682e-02*(d[(i0+28)]-x[(i0-2) * 3])-x[(i0+4) * 3]*6.999999999999999555910790e-01;
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
	j0 = i0;
	if(j0 >=0 && j0 <= 1)
	{
		out[0] = x[(j0) * 3];
	}
	j0 = i0-2;
	if(j0 >=0 && j0 <= 1)
	{
		out[0] = d[(j0+30)];
	}
	j0 = i0-4;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],36,36,d[(10)],d[(j0+14)],1.000000000000000081803054e-05,6.0,d[(j0+20)]);
		alg[(j0+10) * 3] = x[(j0) * 3]*alg[(j0) * 3];
		out[0] = alg[(j0+10) * 3];
	}
	j0 = i0-6;
	if(j0 >=0 && j0 <= 1)
	{
		alg[(j0) * 3] = __ipvsolver(x[(j0) * 3],36,36,d[(10)],d[(j0+14)],1.000000000000000081803054e-05,6.0,d[(j0+20)]);
		out[0] = alg[(j0) * 3];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(34*sizeof(int));
	int *events = (int*)malloc(18*sizeof(int));
	int *outputs = (int*)malloc(8*sizeof(int));
	int *states = (int*)malloc(18*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(18,34,18,0,13,"sisthibgen");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Cboost = 1.000000000000000047921736e-04;
	__PAR_Lboost = 1.000000000000000047921736e-04;
	__PAR_R = 1.000000000000000000000000e+02;
	__PAR_ROn = 1.000000000000000081803054e-05;
	__PAR_ROff = 1.000000000000000000000000e+05;
	__PAR_Cpvin = 1.000000000000000020816682e-03;
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
	__PAR_VlineaRef = 90.0;
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+24)] = 0.0;
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
		modelData->d[(i0+22)] = 0.0;
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
	modelData->x[48] = 20.0;
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->x[i0 * 3] = 2.0;
	}
	for(i0 = 0; i0 <= 1;i0++)
	{
		modelData->d[(i0+30)] = 3.0;
	}
	// Initialize model code.
		modelData->d[(10)] = 273.0+5.000000000000000000000000e+01;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_TContMaxPot = 1.000000000000000055511151e-01;
		modelData->d[(i0+32)] = 0.0;
		modelData->d[(i0+6)] = 0.000000000000000000000000e+00;
		modelData->d[(i0+4)] = 1.000000000000000000000000e+05;
		modelData->d[(i0+2)] = 1.000000000000000000000000e+05;
		__PAR_Tboost[(i0)] = 1.000000000000000047921736e-04;
		modelData->d[(i0)] = 9.000000000000000222044605e-01;
		modelData->d[(i0+14)] = 8.0;
		modelData->d[(i0+18)] = 1.000000000000000000000000e+05;
	}
		modelData->d[(0)] = 6.999999999999999555910790e-01;
		modelData->d[(1)] = 2.999999999999999888977698e-01;
		modelData->d[(30)] = 1.0;
		modelData->d[(31)] = 2.0;
		__PAR_Rspv = 1.000000000000000000000000e+00;
		modelData->d[(13)] = __PAR_Ior*pow((modelData->d[(10)]/__PAR_Tref),3.0)*exp(__PAR_q*__PAR_Eg*(1.0/__PAR_Tref-1.0/modelData->d[(10)])/__PAR_K/__PAR_Ac);
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->d[(i0+11)] = (__PAR_Isc+__PAR_K1*(modelData->d[(10)]-__PAR_Tref))*modelData->d[(i0+14)]/100.0;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nDS[i0] = 1;
		modelData->nDS[i0+2] = 2;
		modelData->nDS[i0+4] = 1;
		modelData->nDS[i0+6] = 1;
		modelData->nDS[i0+10] = 1;
		modelData->nDS[i0+12] = 2;
		modelData->nDS[i0+14] = 1;
	}
	modelData->nDS[17] = 1;
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
	modelData->nDS[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+8]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+14]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[i0+14]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nDS[16]++;
	}
	modelData->nDS[16]++;
	modelData->nDS[16]++;
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nSD[i0+4]++;
		modelData->nSD[i0+2]++;
		modelData->nSD[16]++;
		modelData->nSD[i0+4]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+12]++;
		modelData->nSD[i0+14]++;
	}
	modelData->nSD[16]++;
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
	modelData->nSD[i0]++;
	}
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
	modelData->nSD[i0+2]++;
	}
	modelData->nSD[16] += 2;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSD[i0+2]++;
	}
	modelData->nSD[16]++;
	modelData->nSD[17]++;
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
		modelData->nSZ[i0+2]++;
		modelData->nSZ[16]++;
		modelData->nSZ[i0+2]++;
		modelData->nSZ[16]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0]++;
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
	modelData->nSZ[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+2]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->nSZ[i0+4]++;
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
	for(i0 = 2; i0 <= 3; i0++)
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
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 2; i0 <= 3; i0++)
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
	modelData->event[i0+2].nRHSSt = 3;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].nLHSDsc = 5;
	modelData->event[i0+2].nLHSDsc = 6;
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
		cleanVector(states,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0+4;
		modelData->DS[i0+2][states[i0+2]++] = i0+2;
		modelData->DS[i0+2][states[i0+2]++] = 16;
		modelData->DS[i0+4][states[i0+4]++] = i0+4;
		modelData->DS[i0+6][states[i0+6]++] = i0;
		modelData->DS[i0+10][states[i0+10]++] = i0;
		modelData->DS[i0+12][states[i0+12]++] = i0;
		modelData->DS[i0+12][states[i0+12]++] = i0+12;
		modelData->DS[i0+14][states[i0+14]++] = i0+14;
	}
	modelData->DS[17][states[17]++] = 16;
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
	modelData->DS[i0+4][states[i0+4]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+4][states[i0+4]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+8][states[i0+8]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+14][states[i0+14]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[i0+14][states[i0+14]++] = 16;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->DS[16][states[16]++] = i0+2;
	}
	modelData->DS[16][states[16]++] = 16;
	modelData->DS[16][states[16]++] = 17;
		cleanVector(states,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->SD[i0+4][states[i0+4]++] = i0;
		modelData->SD[i0+2][states[i0+2]++] = i0+2;
		modelData->SD[16][states[16]++] = i0+2;
		modelData->SD[i0+4][states[i0+4]++] = i0+4;
		modelData->SD[i0][states[i0]++] = i0+6;
		modelData->SD[i0][states[i0]++] = i0+10;
		modelData->SD[i0][states[i0]++] = i0+12;
		modelData->SD[i0+12][states[i0+12]++] = i0+12;
		modelData->SD[i0+14][states[i0+14]++] = i0+14;
	}
	modelData->SD[16][states[16]++] = 17;
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
	modelData->SD[i0][states[i0]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+2][states[i0+2]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0+8;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+2][states[i0+2]++] = i0+14;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[16][states[16]++] = i0+14;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SD[i0+2][states[i0+2]++] = 16;
	}
	modelData->SD[16][states[16]++] = 16;
	modelData->SD[17][states[17]++] = 16;
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->ZS[i0+14][events[i0+14]++] = i0+2;
		modelData->ZS[i0+14][events[i0+14]++] = 16;
		modelData->ZS[i0+16][events[i0+16]++] = i0+2;
		modelData->ZS[i0+16][events[i0+16]++] = 16;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+6][events[i0+6]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+8][events[i0+8]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+10][events[i0+10]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+10][events[i0+10]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+12][events[i0+12]++] = i0+2;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->ZS[i0+12][events[i0+12]++] = i0+4;
	}
		cleanVector(states,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->SZ[i0+2][states[i0+2]++] = i0+14;
		modelData->SZ[16][states[16]++] = i0+14;
		modelData->SZ[i0+2][states[i0+2]++] = i0+16;
		modelData->SZ[16][states[16]++] = i0+16;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+6;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+8;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+2][states[i0+2]++] = i0+10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+4][states[i0+4]++] = i0+10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+2][states[i0+2]++] = i0+12;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelData->SZ[i0+4][states[i0+4]++] = i0+12;
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
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+4;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0+6;
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

	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-12;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-14;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-8;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+2;
	}
	for(i0 = 4; i0 <= 5; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 10; i0 <= 11; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-6;
	}
	for(i0 = 12; i0 <= 13; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-8;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+6;
	}
	for(i0 = 2; i0 <= 3; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+6;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-2;
	}
	for(i0 = 14; i0 <= 15; i0++)
	{
		modelData->HD[i0][events[i0]++] = 16;
	}
	for(i0 = 16; i0 <= 17; i0++)
	{
		modelData->HD[i0][events[i0]++] = 16;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].LHSSt[events[i0]++] = i0+8;
	modelData->event[i0+2].LHSSt[events[i0+2]++] = i0+10;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].RHSSt[events[i0]++] = i0+8;
	modelData->event[i0+2].RHSSt[events[i0+2]++] = i0;
	modelData->event[i0+2].RHSSt[events[i0+2]++] = i0+6;
	modelData->event[i0+2].RHSSt[events[i0+2]++] = i0+10;
	}
		cleanVector(events,0,18);

	for(i0 = 0; i0 <= 1; i0++)
	{
	modelData->event[i0].LHSDsc[events[i0]++] = i0+22;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+24;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+26;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+30;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+32;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+4;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+6;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+8;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+20;
	modelData->event[i0+2].LHSDsc[events[i0+2]++] = i0+28;
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
	simulator->time = QSS_Time(18,18,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("sisthibgen",8,34,18,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOS[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nSO[i0]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOS[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nSO[i0]++;
	}
	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->nOD[i0+2] = 1;
		modelOutput->nDO[i0+30]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOD[i0+4]++;
	}
	modelOutput->nDO[10] += 2;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOD[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nDO[i0+14]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOD[i0+4]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nDO[i0+20]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOD[i0+6]++;
	}
	modelOutput->nDO[10] += 2;
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOD[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nDO[i0+14]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nOD[i0+6]++;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->nDO[i0+20]++;
	}
	SD_allocOutputMatrix(modelOutput,18,34);
		cleanVector(states,0,18);

		cleanVector(discretes,0,34);

		cleanVector(outputs,0,8);

	for(i0 = 0; i0 <= 1; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"Vpv[%d]",i0+1);
		sprintf(modelOutput->variable[i0+2].name,"VpvRef[%d]",i0+1);
		sprintf(modelOutput->variable[i0+4].name,"PotPanel[%d]",i0+1);
		sprintf(modelOutput->variable[i0+6].name,"Ipv[%d]",i0+1);
	}
		cleanVector(outputs,0,8);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->SO[i0][states[i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OS[i0+4][outputs[i0+4]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->SO[i0][states[i0]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OS[i0+6][outputs[i0+6]++] = i0;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->SO[i0][states[i0]++] = i0+6;
	}
		cleanVector(outputs,0,8);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->DO[i0+30][discretes[i0+30]++] = i0+2;
		modelOutput->OD[i0+2][outputs[i0+2]++] = i0+30;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OD[i0+4][outputs[i0+4]++] = 10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->DO[10][discretes[10]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OD[i0+4][outputs[i0+4]++] = i0+14;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->DO[i0+14][discretes[i0+14]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OD[i0+4][outputs[i0+4]++] = i0+20;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->DO[i0+20][discretes[i0+20]++] = i0+4;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OD[i0+6][outputs[i0+6]++] = 10;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->DO[10][discretes[10]++] = i0+6;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OD[i0+6][outputs[i0+6]++] = i0+14;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->DO[i0+14][discretes[i0+14]++] = i0+6;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->OD[i0+6][outputs[i0+6]++] = i0+20;
	}
	for ( i0 = 0; i0 <= 1; i0++) 
	{
	modelOutput->DO[i0+20][discretes[i0+20]++] = i0+6;
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
