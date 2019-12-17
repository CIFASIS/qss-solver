#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_fede.h"
#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_Cza = 0;
double __PAR_kout = 0;
double __PAR_kiz = 0;
double __PAR_kpz = 0;
double __PAR_Ccw = 0;
double __PAR_kcw = 0;
double __PAR_Tcwsp = 0;
double __PAR_kicw = 0;
double __PAR_kpcw = 0;
double __PAR_Qcmax = 0;
double __PAR_p1 = 0;
double __PAR_p2 = 0;
double __PAR_p3 = 0;
double __PAR_Tzasp = 0;

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
	double tmp230[3];
	int i0 = i;
	int j0 = 0;
	int j260;
	switch(i)
	{
		case 30:
			alg[66] = 0.000000000000000000000000e+00+(1.0-d[(20)])*(d[(21)]*__PAR_Qcmax+(1.0-d[(21)])*(__PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]));
			alg[66 + 1] = (__PAR_kpcw*x[94]+x[91])*(-1.0+d[(20)])*(-1.0+d[(21)]);
			dx[1] = __PAR_kicw/__PAR_kpcw*(alg[66]-x[90]);
			dx[2] = (-(1.0/(__PAR_kpcw))*__PAR_kicw*(x[91]-alg[67]))/2;
			return;
		case 31:
			alg[66] = 0.000000000000000000000000e+00+(1.0-d[(20)])*(d[(21)]*__PAR_Qcmax+(1.0-d[(21)])*(__PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]));
			alg[66 + 1] = (__PAR_kpcw*x[94]+x[91])*(-1.0+d[(20)])*(-1.0+d[(21)]);
	for(j260 = 0;j260 < 3; j260++)
	{
		tmp230[j260] = 0;
	}
	for(j260 = 0; j260 <= 9; j260++)
	{
		tmp230[0] += x[(j260+10) * 3 + 0];
		tmp230[1] += x[(j260+10) * 3 + 1];
		tmp230[2] += x[(j260+10) * 3 + 2];
	}
			dx[1] = (tmp230[0]-alg[66])/__PAR_Ccw;
			dx[2] = ((1.0/(__PAR_Ccw))*(tmp230[1]-alg[67]))/2;
			return;
		default:
			j0 = i0;
			if(j0 >=0 && j0 <= 9)
			{
				dx[1] = -__PAR_kiz/__PAR_kpz*x[(j0) * 3]+__PAR_kiz/__PAR_kpz*(d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3])));
				dx[2] = (-x[(j0) * 3 + 1]*__PAR_kiz*(1.0/(__PAR_kpz))+(-1.0+d[(j0+32)])*__PAR_kiz*(x[(j0+20) * 3 + 1]*__PAR_kpz+x[(j0) * 3 + 1])*(1.0/(__PAR_kpz))*(-1.0+d[(j0+22)]))/2;
			}
			j0 = i0-10;
			if(j0 >=0 && j0 <= 9)
			{
				alg[(j0+1) * 3] = d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3]));
				alg[(j0+1) * 3 + 1] = (-1.0+d[(j0+22)])*(__PAR_kpz*x[(j0+20) * 3 + 1]+x[(j0) * 3 + 1])*(-1.0+d[(j0+32)]);
				dx[1] = alg[(j0+1) * 3]*__PAR_kcw*(x[(j0+20) * 3]-x[93])-x[(j0+10) * 3];
				dx[2] = (-__PAR_kcw*(x[94]-x[(j0+20) * 3 + 1])*alg[(j0+1) * 3]-x[(j0+10) * 3 + 1]+alg[(j0+1) * 3 + 1]*__PAR_kcw*(x[(j0+20) * 3]-x[93]))/2;
			}
			j0 = i0-20;
			if(j0 >=0 && j0 <= 9)
			{
				alg[0] = 26.0+4.0*sin(t/3600.0/24.0*6.280000000000000248689958e+00)+9.999999999999999451532715e-21*x[87];
				alg[0 + 1] = (9.999999999999999451532715e-21)*x[88]+(2.907407407407407704848235e-04)*cos((7.268518518518519262120586e-05)*t);
				alg[(j0+11) * 3] = (__PAR_p1*pow(x[(j0+20) * 3],2.0)+__PAR_p2*x[(j0+20) * 3]+__PAR_p3)*d[(j0)];
				alg[(j0+11) * 3 + 1] = d[(j0)]*(2.0*x[(j0+20) * 3 + 1]*__PAR_p1*x[(j0+20) * 3]+x[(j0+20) * 3 + 1]*__PAR_p2);
				dx[1] = (-x[(j0+10) * 3]+__PAR_kout*(alg[0]-x[(j0+20) * 3])+alg[(j0+11) * 3])/__PAR_Cza;
				dx[2] = ((alg[(j0+11) * 3 + 1]+__PAR_kout*(alg[1]-x[(j0+20) * 3 + 1])-x[(j0+10) * 3 + 1])*(1.0/(__PAR_Cza)))/2;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp230[3];
	int i0 = i;
	int j0 = 0;
	int j0270 = 0;
	int j260;
	switch(i)
	{
		case 29:
			alg[0] = 26.0+4.0*sin(t/3600.0/24.0*6.280000000000000248689958e+00)+9.999999999999999451532715e-21*x[87];
			alg[0 + 1] = (9.999999999999999451532715e-21)*x[88]+(2.907407407407407704848235e-04)*cos((7.268518518518519262120586e-05)*t);
			for(j0 = 0;j0 <= 9; j0++)
			{
				alg[(j0+11) * 3] = (__PAR_p1*pow(x[(j0+20) * 3],2.0)+__PAR_p2*x[(j0+20) * 3]+__PAR_p3)*d[(j0)];
				alg[(j0+11) * 3 + 1] = d[(j0)]*(2.0*x[(j0+20) * 3 + 1]*__PAR_p1*x[(j0+20) * 3]+x[(j0+20) * 3 + 1]*__PAR_p2);
			}
			for(j0 = 0; j0 <= 9; j0++)
			{
				der[(j0+20) * 3 + 1] = (-x[(j0+10) * 3]+__PAR_kout*(alg[0]-x[(j0+20) * 3])+alg[(j0+11) * 3])/__PAR_Cza;
				der[(j0+20) * 3 + 2] = ((alg[(j0+11) * 3 + 1]+__PAR_kout*(alg[1]-x[(j0+20) * 3 + 1])-x[(j0+10) * 3 + 1])*(1.0/(__PAR_Cza)))/2;
			}
			break;
		case 30:
			alg[66] = 0.000000000000000000000000e+00+(1.0-d[(20)])*(d[(21)]*__PAR_Qcmax+(1.0-d[(21)])*(__PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]));
			alg[66 + 1] = (__PAR_kpcw*x[94]+x[91])*(-1.0+d[(20)])*(-1.0+d[(21)]);
			der[90 + 1] = __PAR_kicw/__PAR_kpcw*(alg[66]-x[90]);
			der[90 + 2] = (-(1.0/(__PAR_kpcw))*__PAR_kicw*(x[91]-alg[67]))/2;
	for(j260 = 0;j260 < 3; j260++)
	{
		tmp230[j260] = 0;
	}
	for(j260 = 0; j260 <= 9; j260++)
	{
		tmp230[0] += x[(j260+10) * 3 + 0];
		tmp230[1] += x[(j260+10) * 3 + 1];
		tmp230[2] += x[(j260+10) * 3 + 2];
	}
			der[93 + 1] = (tmp230[0]-alg[66])/__PAR_Ccw;
			der[93 + 2] = ((1.0/(__PAR_Ccw))*(tmp230[1]-alg[67]))/2;
			return;
		case 31:
			for(j0 = 0;j0 <= 9; j0++)
			{
				j0270 = j0;
			if (j0270 >= 0 && j0270 <= 9)
			{
				alg[(j0+1) * 3] = d[(j0270+32)]+(1.0-d[(j0270+32)])*(0.0+(1.0-d[(j0270+22)])*(__PAR_kpz*(x[(j0270+20) * 3]-__PAR_Tzasp)+x[(j0270) * 3]));
				alg[(j0+1) * 3 + 1] = (-1.0+d[(j0270+22)])*(__PAR_kpz*x[(j0270+20) * 3 + 1]+x[(j0270) * 3 + 1])*(-1.0+d[(j0270+32)]);
			}
			}
			alg[66] = 0.000000000000000000000000e+00+(1.0-d[(20)])*(d[(21)]*__PAR_Qcmax+(1.0-d[(21)])*(__PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]));
			alg[66 + 1] = (__PAR_kpcw*x[94]+x[91])*(-1.0+d[(20)])*(-1.0+d[(21)]);
			for(j0 = 0; j0 <= 9; j0++)
			{
				der[(j0+10) * 3 + 1] = alg[(j0+1) * 3]*__PAR_kcw*(x[(j0+20) * 3]-x[93])-x[(j0+10) * 3];
				der[(j0+10) * 3 + 2] = (-__PAR_kcw*(x[94]-x[(j0+20) * 3 + 1])*alg[(j0+1) * 3]-x[(j0+10) * 3 + 1]+alg[(j0+1) * 3 + 1]*__PAR_kcw*(x[(j0+20) * 3]-x[93]))/2;
			}
			der[90 + 1] = __PAR_kicw/__PAR_kpcw*(alg[66]-x[90]);
			der[90 + 2] = (-(1.0/(__PAR_kpcw))*__PAR_kicw*(x[91]-alg[67]))/2;
	for(j260 = 0;j260 < 3; j260++)
	{
		tmp230[j260] = 0;
	}
	for(j260 = 0; j260 <= 9; j260++)
	{
		tmp230[0] += x[(j260+10) * 3 + 0];
		tmp230[1] += x[(j260+10) * 3 + 1];
		tmp230[2] += x[(j260+10) * 3 + 2];
	}
			der[93 + 1] = (tmp230[0]-alg[66])/__PAR_Ccw;
			der[93 + 2] = ((1.0/(__PAR_Ccw))*(tmp230[1]-alg[67]))/2;
			break;
	}
	j0 = i0;
	if(j0 >=0 && j0 <= 9)
	{
		alg[(j0+1) * 3] = d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3]));
		alg[(j0+1) * 3 + 1] = (-1.0+d[(j0+22)])*(__PAR_kpz*x[(j0+20) * 3 + 1]+x[(j0) * 3 + 1])*(-1.0+d[(j0+32)]);
		der[(j0) * 3 + 1] = -__PAR_kiz/__PAR_kpz*x[(j0) * 3]+__PAR_kiz/__PAR_kpz*(d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3])));
		der[(j0) * 3 + 2] = (-x[(j0) * 3 + 1]*__PAR_kiz*(1.0/(__PAR_kpz))+(-1.0+d[(j0+32)])*__PAR_kiz*(x[(j0+20) * 3 + 1]*__PAR_kpz+x[(j0) * 3 + 1])*(1.0/(__PAR_kpz))*(-1.0+d[(j0+22)]))/2;
		der[(j0+10) * 3 + 1] = alg[(j0+1) * 3]*__PAR_kcw*(x[(j0+20) * 3]-x[93])-x[(j0+10) * 3];
		der[(j0+10) * 3 + 2] = (-__PAR_kcw*(x[94]-x[(j0+20) * 3 + 1])*alg[(j0+1) * 3]-x[(j0+10) * 3 + 1]+alg[(j0+1) * 3 + 1]*__PAR_kcw*(x[(j0+20) * 3]-x[93]))/2;
	}
	j0 = i0-10;
	if(j0 >=0 && j0 <= 9)
	{
		alg[0] = 26.0+4.0*sin(t/3600.0/24.0*6.280000000000000248689958e+00)+9.999999999999999451532715e-21*x[87];
		alg[0 + 1] = (9.999999999999999451532715e-21)*x[88]+(2.907407407407407704848235e-04)*cos((7.268518518518519262120586e-05)*t);
		alg[(j0+1) * 3] = d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3]));
		alg[(j0+1) * 3 + 1] = (-1.0+d[(j0+22)])*(__PAR_kpz*x[(j0+20) * 3 + 1]+x[(j0) * 3 + 1])*(-1.0+d[(j0+32)]);
		alg[(j0+11) * 3] = (__PAR_p1*pow(x[(j0+20) * 3],2.0)+__PAR_p2*x[(j0+20) * 3]+__PAR_p3)*d[(j0)];
		alg[(j0+11) * 3 + 1] = d[(j0)]*(2.0*x[(j0+20) * 3 + 1]*__PAR_p1*x[(j0+20) * 3]+x[(j0+20) * 3 + 1]*__PAR_p2);
		der[(j0+10) * 3 + 1] = alg[(j0+1) * 3]*__PAR_kcw*(x[(j0+20) * 3]-x[93])-x[(j0+10) * 3];
		der[(j0+10) * 3 + 2] = (-__PAR_kcw*(x[94]-x[(j0+20) * 3 + 1])*alg[(j0+1) * 3]-x[(j0+10) * 3 + 1]+alg[(j0+1) * 3 + 1]*__PAR_kcw*(x[(j0+20) * 3]-x[93]))/2;
		der[(j0+20) * 3 + 1] = (-x[(j0+10) * 3]+__PAR_kout*(alg[0]-x[(j0+20) * 3])+alg[(j0+11) * 3])/__PAR_Cza;
		der[(j0+20) * 3 + 2] = ((alg[(j0+11) * 3 + 1]+__PAR_kout*(alg[1]-x[(j0+20) * 3 + 1])-x[(j0+10) * 3 + 1])*(1.0/(__PAR_Cza)))/2;
	}
	j0 = i0-20;
	if(j0 >=0 && j0 <= 9)
	{
		alg[0] = 26.0+4.0*sin(t/3600.0/24.0*6.280000000000000248689958e+00)+9.999999999999999451532715e-21*x[87];
		alg[0 + 1] = (9.999999999999999451532715e-21)*x[88]+(2.907407407407407704848235e-04)*cos((7.268518518518519262120586e-05)*t);
		alg[(j0+1) * 3] = d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3]));
		alg[(j0+1) * 3 + 1] = (-1.0+d[(j0+22)])*(__PAR_kpz*x[(j0+20) * 3 + 1]+x[(j0) * 3 + 1])*(-1.0+d[(j0+32)]);
		alg[(j0+11) * 3] = (__PAR_p1*pow(x[(j0+20) * 3],2.0)+__PAR_p2*x[(j0+20) * 3]+__PAR_p3)*d[(j0)];
		alg[(j0+11) * 3 + 1] = d[(j0)]*(2.0*x[(j0+20) * 3 + 1]*__PAR_p1*x[(j0+20) * 3]+x[(j0+20) * 3 + 1]*__PAR_p2);
		der[(j0) * 3 + 1] = -__PAR_kiz/__PAR_kpz*x[(j0) * 3]+__PAR_kiz/__PAR_kpz*(d[(j0+32)]+(1.0-d[(j0+32)])*(0.0+(1.0-d[(j0+22)])*(__PAR_kpz*(x[(j0+20) * 3]-__PAR_Tzasp)+x[(j0) * 3])));
		der[(j0) * 3 + 2] = (-x[(j0) * 3 + 1]*__PAR_kiz*(1.0/(__PAR_kpz))+(-1.0+d[(j0+32)])*__PAR_kiz*(x[(j0+20) * 3 + 1]*__PAR_kpz+x[(j0) * 3 + 1])*(1.0/(__PAR_kpz))*(-1.0+d[(j0+22)]))/2;
		der[(j0+10) * 3 + 1] = alg[(j0+1) * 3]*__PAR_kcw*(x[(j0+20) * 3]-x[93])-x[(j0+10) * 3];
		der[(j0+10) * 3 + 2] = (-__PAR_kcw*(x[94]-x[(j0+20) * 3 + 1])*alg[(j0+1) * 3]-x[(j0+10) * 3 + 1]+alg[(j0+1) * 3 + 1]*__PAR_kcw*(x[(j0+20) * 3]-x[93]))/2;
		der[(j0+20) * 3 + 1] = (-x[(j0+10) * 3]+__PAR_kout*(alg[0]-x[(j0+20) * 3])+alg[(j0+11) * 3])/__PAR_Cza;
		der[(j0+20) * 3 + 2] = ((alg[(j0+11) * 3 + 1]+__PAR_kout*(alg[1]-x[(j0+20) * 3 + 1])-x[(j0+10) * 3 + 1])*(1.0/(__PAR_Cza)))/2;
	}
	j0 = i0-10;
	if(j0 >=0 && j0 <= 9)
	{
		alg[66] = 0.000000000000000000000000e+00+(1.0-d[(20)])*(d[(21)]*__PAR_Qcmax+(1.0-d[(21)])*(__PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]));
		alg[66 + 1] = (__PAR_kpcw*x[94]+x[91])*(-1.0+d[(20)])*(-1.0+d[(21)]);
	for(j260 = 0;j260 < 3; j260++)
	{
		tmp230[j260] = 0;
	}
	for(j260 = 0; j260 <= 9; j260++)
	{
		tmp230[0] += x[(j260+10) * 3 + 0];
		tmp230[1] += x[(j260+10) * 3 + 1];
		tmp230[2] += x[(j260+10) * 3 + 2];
	}
		der[93 + 1] = (tmp230[0]-alg[66])/__PAR_Ccw;
		der[93 + 2] = ((1.0/(__PAR_Ccw))*(tmp230[1]-alg[67]))/2;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 30:
			zc[0] = __PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]-(0.0);
			zc[1] = x[91]+__PAR_kpcw*x[94];
			return;
		case 31:
			zc[0] = __PAR_kpcw*(x[93]-__PAR_Tcwsp)+x[90]-(__PAR_Qcmax);
			zc[1] = __PAR_kpcw*x[94]+x[91];
			return;
		default:
			if(i0 >= 0 && i0 <= 9)
			{
				zc[0] = t-(d[(i0+10)]);
				zc[1] = 1.0;
			}
			if(i0 >= 10 && i0 <= 19)
			{
				zc[0] = __PAR_kpz*(x[(i0+10) * 3]-__PAR_Tzasp)+x[(i0-10) * 3]-(0.0);
				zc[1] = x[(i0+10) * 3 + 1]*__PAR_kpz+x[(i0-10) * 3 + 1];
			}
			if(i0 >= 20 && i0 <= 29)
			{
				zc[0] = __PAR_kpz*(x[(i0) * 3]-__PAR_Tzasp)+x[(i0-20) * 3]-(1.0);
				zc[1] = __PAR_kpz*x[(i0) * 3 + 1]+x[(i0-20) * 3 + 1];
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 30:
			d[(20)] = 0.0;
			return;
		case 31:
			d[(21)] = 1.0;
			return;
		default:
			if(i0 >= 0 && i0 <= 9)
			{
				d[(i0)] = __fede__get_npeople_qss((i0+1));
				d[(i0+10)] = t+__fede__get_nextpeople_qss((i0+1));
			}
			if(i0 >= 10 && i0 <= 19)
			{
				d[(i0+12)] = 0.0;
			}
			if(i0 >= 20 && i0 <= 29)
			{
				d[(i0+12)] = 1.0;
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 30:
			d[(20)] = 1.0;
			return;
		case 31:
			d[(21)] = 0.0;
			return;
		default:
			if(i0 >= 10 && i0 <= 19)
			{
				d[(i0+12)] = 1.0;
			}
			if(i0 >= 20 && i0 <= 29)
			{
				d[(i0+12)] = 0.0;
			}
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[87];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(42*sizeof(int));
	int *events = (int*)malloc(32*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(32*sizeof(int));
	int i0;
	int j0 = 0;
	int td = 0;
	simulator->data = QSS_Data(32,42,32,10,23,"microgrid");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Cza = 6.092000000000000000000000e+06;
	__PAR_kout = 4.625000000000000000000000e+02;
	__PAR_kiz = 5.000000000000000104083409e-04;
	__PAR_kpz = 1.0;
	__PAR_Ccw = (1.041800000000000000000000e+07+3.334000000000000074606987e+00*1.000000000000000000000000e+06)*10;
	__PAR_kcw = 2.633910000000000195541361e+02;
	__PAR_Tcwsp = 10.0;
	__PAR_kicw = 5.000000000000000000000000e-01*10;
	__PAR_kpcw = 6000.0*10;
	__PAR_Qcmax = 3000.0*10;
	__PAR_p1 = -2.199000000000000121236354e-01;
	__PAR_p2 = 5.059700000000000308375547e+00;
	__PAR_p3 = 8.491679999999999495230441e+01;
	__PAR_Tzasp = 23.0;
	for(i0 = 0; i0 <= 9;i0++)
	{
		modelData->x[(i0+10) * 3] = 0.0;
	}
	modelData->x[93] = __PAR_Tcwsp;
	for(i0 = 0; i0 <= 9;i0++)
	{
		modelData->x[(i0+20) * 3] = 23.0;
	}
	for(i0 = 0; i0 <= 9;i0++)
	{
		modelData->x[i0 * 3] = 0.0;
	}
	modelData->x[90] = 0.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->d[(i0)] = __fede__get_npeople_qss((i0+1));
		modelData->d[(i0+10)] = __fede__get_nextpeople_qss((i0+1));
	}
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nDS[i0] = 2;
		modelData->nDS[i0+10] = 3;
		modelData->nDS[i0+20] = 2;
	}
	modelData->nDS[30] = 1;
		modelData->nDS[31] += 10;
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->nDS[i0+10]++;
	}
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	modelData->nDS[30]++;
	modelData->nDS[31]++;
	modelData->nDS[31]++;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nSD[i0]++;
		modelData->nSD[i0+20]++;
		modelData->nSD[i0+10]++;
		modelData->nSD[i0+20]++;
		modelData->nSD[31]++;
		modelData->nSD[i0+10]++;
		modelData->nSD[i0+20]++;
	}
	modelData->nSD[30]++;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nSD[i0+10]++;
	}
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->nSD[i0]++;
	}
	modelData->nSD[29] += 10;
	modelData->nSD[31]++;
	modelData->nSD[30]++;
	modelData->nSD[31]++;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nZS[i0+10] = 2;
		modelData->nZS[i0+20] = 2;
	}
	modelData->nZS[30] = 2;
	modelData->nZS[31] = 2;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nSZ[i0]++;
		modelData->nSZ[i0+20]++;
		modelData->nSZ[i0]++;
		modelData->nSZ[i0+20]++;
	}
	modelData->nSZ[30]++;
	modelData->nSZ[31]++;
	modelData->nSZ[30]++;
	modelData->nSZ[31]++;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 10; i0 <= 19; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 20; i0 <= 29; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 10; i0 <= 19; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 20; i0 <= 29; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->nHD[i0]++;
	}
	modelData->nHD[30] = 2;
	modelData->nHD[31] = 2;
	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].nLHSDsc = 2;
		modelData->event[i0+10].nLHSDsc = 1;
		modelData->event[i0+20].nLHSDsc = 1;
	}
	modelData->event[30].nLHSDsc = 1;
	modelData->event[31].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		for (i0 = 0; i0 <= 9;i0++)
	{
		modelData->TD[td++] = i0+20;
	}
		cleanVector(states,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = i0+20;
		modelData->DS[i0+10][states[i0+10]++] = i0+10;
		modelData->DS[i0+10][states[i0+10]++] = i0+20;
		modelData->DS[i0+10][states[i0+10]++] = 31;
		modelData->DS[i0+20][states[i0+20]++] = i0+10;
		modelData->DS[i0+20][states[i0+20]++] = i0+20;
	}
	modelData->DS[30][states[30]++] = 30;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->DS[31][states[31]++] = i0+10;
	}
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->DS[i0+10][states[i0+10]++] = i0;
	}
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = 29;
	}
	modelData->DS[30][states[30]++] = 31;
	modelData->DS[31][states[31]++] = 30;
	modelData->DS[31][states[31]++] = 31;
		cleanVector(states,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+20][states[i0+20]++] = i0;
		modelData->SD[i0+10][states[i0+10]++] = i0+10;
		modelData->SD[i0+20][states[i0+20]++] = i0+10;
		modelData->SD[31][states[31]++] = i0+10;
		modelData->SD[i0+10][states[i0+10]++] = i0+20;
		modelData->SD[i0+20][states[i0+20]++] = i0+20;
	}
	modelData->SD[30][states[30]++] = 30;
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->SD[i0+10][states[i0+10]++] = 31;
	}
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0+10;
	}
	for ( i0 = 0; i0 <= 9; i0++) 
	{
	modelData->SD[29][states[29]++] = i0+20;
	}
	modelData->SD[31][states[31]++] = 30;
	modelData->SD[30][states[30]++] = 31;
	modelData->SD[31][states[31]++] = 31;
		cleanVector(events,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->ZS[i0+10][events[i0+10]++] = i0;
		modelData->ZS[i0+10][events[i0+10]++] = i0+20;
		modelData->ZS[i0+20][events[i0+20]++] = i0;
		modelData->ZS[i0+20][events[i0+20]++] = i0+20;
	}
	modelData->ZS[30][events[30]++] = 30;
	modelData->ZS[30][events[30]++] = 31;
	modelData->ZS[31][events[31]++] = 30;
	modelData->ZS[31][events[31]++] = 31;
		cleanVector(states,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0+10;
		modelData->SZ[i0+20][states[i0+20]++] = i0+10;
		modelData->SZ[i0][states[i0]++] = i0+20;
		modelData->SZ[i0+20][states[i0+20]++] = i0+20;
	}
	modelData->SZ[30][states[30]++] = 30;
	modelData->SZ[31][states[31]++] = 30;
	modelData->SZ[30][states[30]++] = 31;
	modelData->SZ[31][states[31]++] = 31;
		cleanVector(events,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,32);

	for(i0 = 10; i0 <= 19; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 20; i0 <= 29; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-20;
	}
	for(i0 = 10; i0 <= 19; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 20; i0 <= 29; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+20;
	}
	modelData->HD[30][events[30]++] = 30;
	modelData->HD[31][events[31]++] = 30;
	modelData->HD[30][events[30]++] = 31;
	modelData->HD[31][events[31]++] = 31;
		cleanVector(events,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].LHSDsc[events[i0]++] = i0;
	modelData->event[i0].LHSDsc[events[i0]++] = i0+10;
		modelData->event[i0+10].LHSDsc[events[i0+10]++] = i0+22;
		modelData->event[i0+20].LHSDsc[events[i0+20]++] = i0+32;
	}
	modelData->event[30].LHSDsc[events[30]++] = 20;
	modelData->event[31].LHSDsc[events[31]++] = 21;
		cleanVector(events,0,32);

	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].direction = 1;
	modelData->event[i0].relation = 2;
		modelData->event[i0+10].direction = 0;
		modelData->event[i0+10].relation = 0;
		modelData->event[i0+20].direction = 0;
		modelData->event[i0+20].relation = 2;
	}
	modelData->event[30].direction = 0;
	modelData->event[30].relation = 0;
	modelData->event[31].direction = 0;
	modelData->event[31].relation = 2;
	simulator->time = QSS_Time(32,32,10,0,ST_Binary,NULL);

		double period[1];
	period[0] = 72;
	simulator->output = SD_Output("microgrid",1,42,32,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[29]++;
	SD_allocOutputMatrix(modelOutput,32,42);
		cleanVector(states,0,32);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"Tza[10]");
		cleanVector(outputs,0,1);

		modelOutput->SO[29][states[29]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 29;
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
