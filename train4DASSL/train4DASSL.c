#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;

double __PAR_g = 0;
double __PAR_m = 0;
double __PAR_L = 0;
double __PAR_kc = 0;
double __PAR_bc = 0;
double __PAR_R = 0;
double __PAR_J = 0;
double __PAR_bslip = 0;
double __PAR_bnoslip = 0;
double __PAR_baxle = 0;
double __PAR_vslipmax = 0;
double __PAR_fric_coef_dyn = 0;
double __PAR_r = 0;
double __PAR_phi = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 8;
}

void
MOD_definition(double *x, double *d, double *alg, double t, double *dx)
{
	int i0 = 0;
	int i00 = 0;
	modelData->scalarEvaluations++;
		alg[0] = __PAR_r*sin(x[10]/__PAR_r)*cos(__PAR_phi);
		alg[10] = __PAR_r*cos(x[10]/__PAR_r);
		alg[20] = __PAR_r*sin(x[10]/__PAR_r)*sin(__PAR_phi);
		alg[30] = 0.0;
		alg[40] = 0.0;
		alg[50] = -__PAR_m*__PAR_g;
		alg[60] = cos(x[10]/__PAR_r)*cos(__PAR_phi);
		alg[70] = -sin(x[10]/__PAR_r);
		alg[80] = cos(x[10]/__PAR_r)*sin(__PAR_phi);
		alg[90] = __PAR_R*x[0]-x[20];
		alg[100] = d[0]*__PAR_bnoslip*alg[90]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[90]*__PAR_m*__PAR_g+__PAR_bslip*alg[90]*__PAR_m*__PAR_g);
		alg[110] = alg[60]*alg[30]+alg[70]*alg[40]+alg[80]*alg[50]+alg[100]+__PAR_kc*(x[11]-x[10]-__PAR_L)+__PAR_bc*(x[21]-x[20]);
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0] = __PAR_r*sin(x[(i0+10) * 1]/__PAR_r)*cos(__PAR_phi);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+10] = __PAR_r*cos(x[(i0+10) * 1]/__PAR_r);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+20] = __PAR_r*sin(x[(i0+10) * 1]/__PAR_r)*sin(__PAR_phi);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+30] = 0.0;
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+40] = 0.0;
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+50] = -__PAR_m*__PAR_g;
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+60] = cos(x[(i0+10) * 1]/__PAR_r)*cos(__PAR_phi);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+70] = -sin(x[(i0+10) * 1]/__PAR_r);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+80] = cos(x[(i0+10) * 1]/__PAR_r)*sin(__PAR_phi);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+90] = __PAR_R*x[(i0) * 1]-x[(i0+20) * 1];
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+100] = d[(i0)]*__PAR_bnoslip*alg[(i0+90) * 1]*__PAR_m*__PAR_g+(1.0-d[(i0)])*(__PAR_fric_coef_dyn*alg[(i0+90) * 1]*__PAR_m*__PAR_g+__PAR_bslip*alg[(i0+90) * 1]*__PAR_m*__PAR_g);
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		alg[i0+110] = alg[(i0+60) * 1]*alg[(i0+30) * 1]+alg[(i0+70) * 1]*alg[(i0+40) * 1]+alg[(i0+80) * 1]*alg[(i0+50) * 1]+alg[(i0+100) * 1]+__PAR_kc*(x[(i0+11) * 1]+x[(i0+9) * 1]-2.0*x[(i0+10) * 1])+__PAR_bc*(x[(i0+21) * 1]+x[(i0+19) * 1]-2.0*x[(i0+20) * 1]);
	}
		alg[9] = __PAR_r*sin(x[19]/__PAR_r)*cos(__PAR_phi);
		alg[19] = __PAR_r*cos(x[19]/__PAR_r);
		alg[29] = __PAR_r*sin(x[19]/__PAR_r)*sin(__PAR_phi);
		alg[39] = 0.0;
		alg[49] = 0.0;
		alg[59] = -__PAR_m*__PAR_g;
		alg[69] = cos(x[19]/__PAR_r)*cos(__PAR_phi);
		alg[79] = -sin(x[19]/__PAR_r);
		alg[89] = cos(x[19]/__PAR_r)*sin(__PAR_phi);
		alg[99] = __PAR_R*x[9]-x[29];
		alg[109] = d[9]*__PAR_bnoslip*alg[99]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[99]*__PAR_m*__PAR_g+__PAR_bslip*alg[99]*__PAR_m*__PAR_g);
		alg[119] = alg[69]*alg[39]+alg[79]*alg[49]+alg[89]*alg[59]+alg[109]+__PAR_kc*(x[18]-x[19]+__PAR_L)+__PAR_bc*(x[28]-x[29]);
		dx[0] = (d[(10)]-__PAR_R*alg[100]-__PAR_baxle*__PAR_m*__PAR_g*x[0])/__PAR_J;
		dx[10] = x[20];
		dx[20] = alg[110]/__PAR_m;
	for(i0 = 1; i0 <= 8; i0++)
	{
		dx[i0] = (-__PAR_R*alg[(i0+100) * 1]-__PAR_baxle*__PAR_m*__PAR_g*x[(i0) * 1])/__PAR_J;
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		dx[i0+10] = x[(i0+20) * 1];
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		dx[i0+20] = alg[(i0+110) * 1]/__PAR_m;
	}
		dx[9] = (-__PAR_R*alg[109]-__PAR_baxle*__PAR_m*__PAR_g*x[9])/__PAR_J;
		dx[19] = x[29];
		dx[29] = alg[119]/__PAR_m;
}

void
MOD_jacobian(double *x, double *d, double *alg, double t, double *jac)
{
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	int j880 = 0;
	int j890 = 0;
	modelData->zeroCrossings++;
	switch(i)
	{
		case 20:
			zc[0] = t-(30.0);
			return;
		default:
			if(i0 >= 0 && i0 <= 9)
			{
				alg[90] = __PAR_R*x[0]-x[20];
				j880 = i0;
	if (j880 >= 1 && j880 <= 8)
	{
				alg[(i0+90) * 1] = __PAR_R*x[(j880) * 1]-x[(j880+20) * 1];
				}
				alg[99] = __PAR_R*x[9]-x[29];
				zc[0] = alg[(i0+90) * 1]-(__PAR_vslipmax);
			}
			if(i0 >= 10 && i0 <= 19)
			{
				alg[90] = __PAR_R*x[0]-x[20];
				j890 = i0;
	if (j890 >= 1 && j890 <= 8)
	{
				alg[(i0+80) * 1] = __PAR_R*x[(j890-10) * 1]-x[(j890+10) * 1];
				}
				alg[99] = __PAR_R*x[9]-x[29];
				zc[0] = alg[(i0+80) * 1]-(__PAR_vslipmax);
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 20:
			d[(10)] = 0.0;
			return;
		default:
			if(i0 >= 0 && i0 <= 9)
			{
				d[(i0)] = 0.0;
			}
			if(i0 >= 10 && i0 <= 19)
			{
				d[(i0-10)] = 1.0;
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 9)
	{
		d[(i0)] = 1.0;
	}
	if(i0 >= 10 && i0 <= 19)
	{
		d[(i0-10)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[20];
			return;
	}
}

void
CLC_initializeDataStructs(CLC_simulator simulator)
{
	int *discretes = (int*)malloc(11*sizeof(int));
	int *events = (int*)malloc(21*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(30*sizeof(int));
	int i0 = 0;
	int j0 = 0;
	simulator->data = CLC_Data(30,11,21,0,120,"train4DASSL");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_g = 9.810000000000000497379915e+00;
	__PAR_m = 126000.0;
	__PAR_L = 20.0;
	__PAR_kc = 1.000000000000000000000000e+07;
	__PAR_bc = 1.000000000000000000000000e+07;
	__PAR_R = 1.0;
	__PAR_J = 100.0;
	__PAR_bslip = 1.000000000000000020816682e-03;
	__PAR_bnoslip = 1.0;
	__PAR_baxle = 1.000000000000000047921736e-04;
	__PAR_vslipmax = 1.000000000000000055511151e-01;
	__PAR_fric_coef_dyn = 5.999999999999999777955395e-01;
	__PAR_r = 1.000000000000000000000000e+04;
	__PAR_phi = 1.000000000000000047921736e-04;
	for(i0 = 0; i0 <= 9;i0++)
	{
		modelData->d[i0] = 1.0;
	}
	modelData->d[(10)] = 190000.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->x[(i0+10) * 1] = ((i0+1)-1.0)*__PAR_L;
	}
	CLC_allocDataMatrix(modelData);
	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].direction = 0;
	modelData->event[i0+10].direction = 0;
	}
	modelData->event[20].direction = 1;
		double period[1];
	period[0] = 0.2;
	simulator->output = SD_Output("train4DASSL",1,11,30,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[20]++;
	SD_allocOutputMatrix(modelOutput,30,11);
		cleanVector(states,0,30);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"vl[1]");
		cleanVector(outputs,0,1);

		modelOutput->SO[20][states[20]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 20;
	simulator->model = CLC_Model(MOD_definition,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg, MOD_jacobian);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
