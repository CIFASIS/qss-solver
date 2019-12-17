#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_P[10];

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 5;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	double tmp10[4];
	int i0 = i;
	int j0 = 0;
	int j180;
	switch(i)
	{
		case 10:
		tmp10[0] = 0;
	for(j180 = 0; j180 <= 9; j180++)
	{
		tmp10[0] += __PAR_P[j180];
	}
			dx[1] = tmp10[0];
			dx[2] = (tmp10[1])/2;
			dx[3] = (tmp10[2])/6;
			return;
		default:
			j0 = i0;
			if(j0 >=0 && j0 <= 9)
			{
				dx[1] = (j0+1);
				dx[2] = (0.0)/2;
				dx[3] = (0.0)/6;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			zc[0] = t-(0.0);
			zc[1] = 1.0;
			zc[2] = (0.0)/2;
			return;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	double tmp190[4];
	double tmp210[4];
	double tmp230[4];
	double tmp250[4];
	double tmp270[4];
	double tmp300[4];
	double tmp330[4];
	double tmp360[4];
	double tmp390[4];
	double tmp420[4];
	double tmp450[4];
	double tmp470[4];
	double tmp490[4];
	double tmp510[4];
	double tmp530[4];
	int i0 = i;
	int j200;
	int j220;
	int j240;
	int j260;
	int j280= 0;
	int j290;
	int j310= 0;
	int j320;
	int j340= 0;
	int j350;
	int j370= 0;
	int j380;
	int j400= 0;
	int j410;
	int j430= 0;
	int j440;
	int j460;
	int j480;
	int j500;
	int j520;
	int j540= 0;
	int j550;
	switch(i)
	{
		case 0:
		tmp190[0] = __PAR_P[0];
	for(j200 = 0; j200 <= 9; j200++)
	{
		 if (tmp190[0] < __PAR_P[j200])
	 	tmp190[0] = __PAR_P[j200];
	}
			d[(0)] = tmp190[0];
		tmp210[0] = __PAR_P[0];
	for(j220 = 0; j220 <= 9; j220++)
	{
		 if (tmp210[0] > __PAR_P[j220])
	 	tmp210[0] = __PAR_P[j220];
	}
			d[(0)] = tmp210[0];
		tmp230[0] = 0;
	for(j240 = 0; j240 <= 9; j240++)
	{
		tmp230[0] += __PAR_P[j240];
	}
			d[(0)] = tmp230[0];
		tmp250[0] = 0;
	for(j260 = 0; j260 <= 9; j260++)
	{
		tmp250[0] *= __PAR_P[j260];
	}
			d[(0)] = tmp250[0];
			for(j280 = 0; j280 <= 9; j280++)
			{
					alg[(j280) * 4] = j280;
			}
	for(j290 = 0;j290 < 4; j290++)
	{
		tmp270[j290] = 0;
	}
	for(j290 = 0; j290 <= 9; j290++)
	{
		tmp270[0] += __PAR_P[j290]*alg[(j290) * 4 + 0];
		tmp270[1] += __PAR_P[j290]*alg[(j290) * 4 + 1];
		tmp270[2] += __PAR_P[j290]*alg[(j290) * 4 + 2];
		tmp270[3] += __PAR_P[j290]*alg[(j290) * 4 + 3];
	}
			d[(0)] = tmp270[0];
			for(j310 = 0; j310 <= 9; j310++)
			{
					alg[(j310) * 4] = j310;
			}
	for(j320 = 0;j320 < 4; j320++)
	{
		tmp300[j320] = alg[0 * 4 + 0];
	}
	for(j320 = 0; j320 <= 9; j320++)
	{
		 if (tmp300[0] < alg[(j320) * 4 + 0])
	 	tmp300[0] = alg[(j320) * 4 + 0];
		 if (tmp300[1] < alg[(j320) * 4 + 0])
	 	tmp300[1] = alg[(j320) * 4 + 0];
		 if (tmp300[2] < alg[(j320) * 4 + 0])
	 	tmp300[2] = alg[(j320) * 4 + 0];
		 if (tmp300[3] < alg[(j320) * 4 + 0])
	 	tmp300[3] = alg[(j320) * 4 + 0];
	}
			d[(0)] = tmp300[0];
			for(j340 = 0; j340 <= 9; j340++)
			{
					alg[(j340) * 4] = j340;
			}
	for(j350 = 0;j350 < 4; j350++)
	{
		tmp330[j350] = alg[0 * 4 + 0];
	}
	for(j350 = 0; j350 <= 9; j350++)
	{
		 if (tmp330[0] > alg[(j350) * 4 + 0])
	 	tmp330[0] = alg[(j350) * 4 + 0];
		 if (tmp330[1] > alg[(j350) * 4 + 0])
	 	tmp330[1] = alg[(j350) * 4 + 0];
		 if (tmp330[2] > alg[(j350) * 4 + 0])
	 	tmp330[2] = alg[(j350) * 4 + 0];
		 if (tmp330[3] > alg[(j350) * 4 + 0])
	 	tmp330[3] = alg[(j350) * 4 + 0];
	}
			d[(0)] = tmp330[0];
			for(j370 = 0; j370 <= 9; j370++)
			{
					alg[(j370) * 4] = j370;
			}
	for(j380 = 0;j380 < 4; j380++)
	{
		tmp360[j380] = 0;
	}
	for(j380 = 0; j380 <= 9; j380++)
	{
		tmp360[0] += alg[(j380) * 4 + 0];
		tmp360[1] += alg[(j380) * 4 + 1];
		tmp360[2] += alg[(j380) * 4 + 2];
		tmp360[3] += alg[(j380) * 4 + 3];
	}
			d[(0)] = tmp360[0];
			for(j400 = 0; j400 <= 9; j400++)
			{
					alg[(j400) * 4] = j400;
			}
	for(j410 = 0;j410 < 4; j410++)
	{
		tmp390[j410] = 0;
	}
	for(j410 = 0; j410 <= 9; j410++)
	{
		tmp390[0] *= alg[(j410) * 4 + 0];
		tmp390[1] *= alg[(j410) * 4 + 1];
		tmp390[2] *= alg[(j410) * 4 + 2];
		tmp390[3] *= alg[(j410) * 4 + 3];
	}
			d[(0)] = tmp390[0];
			for(j430 = 0; j430 <= 9; j430++)
			{
					alg[(j430) * 4] = j430;
			}
	for(j440 = 0;j440 < 4; j440++)
	{
		tmp420[j440] = 0;
	}
	for(j440 = 0; j440 <= 9; j440++)
	{
		tmp420[0] += alg[(j440) * 4 + 0] * __PAR_P[j440];
		tmp420[1] += alg[(j440) * 4 + 1] * __PAR_P[j440];
		tmp420[2] += alg[(j440) * 4 + 2] * __PAR_P[j440];
		tmp420[3] += alg[(j440) * 4 + 3] * __PAR_P[j440];
	}
			d[(0)] = tmp420[0];
	for(j460 = 0;j460 < 4; j460++)
	{
		tmp450[j460] = x[0 * 4 + 0];
	}
	for(j460 = 0; j460 <= 9; j460++)
	{
		 if (tmp450[0] < x[(j460) * 4 + 0])
	 	tmp450[0] = x[(j460) * 4 + 0];
		 if (tmp450[1] < x[(j460) * 4 + 0])
	 	tmp450[1] = x[(j460) * 4 + 0];
		 if (tmp450[2] < x[(j460) * 4 + 0])
	 	tmp450[2] = x[(j460) * 4 + 0];
		 if (tmp450[3] < x[(j460) * 4 + 0])
	 	tmp450[3] = x[(j460) * 4 + 0];
	}
			d[(0)] = tmp450[0];
	for(j480 = 0;j480 < 4; j480++)
	{
		tmp470[j480] = x[0 * 4 + 0];
	}
	for(j480 = 0; j480 <= 9; j480++)
	{
		 if (tmp470[0] > x[(j480) * 4 + 0])
	 	tmp470[0] = x[(j480) * 4 + 0];
		 if (tmp470[1] > x[(j480) * 4 + 0])
	 	tmp470[1] = x[(j480) * 4 + 0];
		 if (tmp470[2] > x[(j480) * 4 + 0])
	 	tmp470[2] = x[(j480) * 4 + 0];
		 if (tmp470[3] > x[(j480) * 4 + 0])
	 	tmp470[3] = x[(j480) * 4 + 0];
	}
			d[(0)] = tmp470[0];
	for(j500 = 0;j500 < 4; j500++)
	{
		tmp490[j500] = 0;
	}
	for(j500 = 0; j500 <= 9; j500++)
	{
		tmp490[0] += x[(j500) * 4 + 0];
		tmp490[1] += x[(j500) * 4 + 1];
		tmp490[2] += x[(j500) * 4 + 2];
		tmp490[3] += x[(j500) * 4 + 3];
	}
			d[(0)] = tmp490[0];
	for(j520 = 0;j520 < 4; j520++)
	{
		tmp510[j520] = 0;
	}
	for(j520 = 0; j520 <= 9; j520++)
	{
		tmp510[0] *= x[(j520) * 4 + 0];
		tmp510[1] *= x[(j520) * 4 + 1];
		tmp510[2] *= x[(j520) * 4 + 2];
		tmp510[3] *= x[(j520) * 4 + 3];
	}
			d[(0)] = tmp510[0];
			for(j540 = 0; j540 <= 9; j540++)
			{
					alg[(j540) * 4] = j540;
			}
	for(j550 = 0;j550 < 4; j550++)
	{
		tmp530[j550] = 0;
	}
	for(j550 = 0; j550 <= 9; j550++)
	{
		tmp530[0] += alg[(j550) * 4 + 0] * x[(j550) * 4 + 0];
		tmp530[1] += alg[(j550) * 4 + 1 ]* x[(j550) * 4 + 0 ] + alg[(j550) * 4 + 0 ] * x[(j550) * 4 + 1];
		tmp530[2] += alg[(j550) * 4 + 2] * x[(j550) * 4 + 0] + 2 * alg[(j550) * 4 + 1 ] * x[(j550) * 4 + 1] + alg[(j550) * 4 + 0] * x[(j550) * 4 + 2];
		tmp530[3] += alg[(j550) * 4 + 3] * x[(j550) * 4 + 0] + 3 * alg[(j550) * 4 + 2] * x[(j550) * 4 + 1] + 3 * alg[(j550) * 4 + 1] * x[(j550) * 4 + 2] + alg[(j550) * 4 + 0] * x[(j550) * 4 + 3];
	}
			d[(0)] = tmp530[0];
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 9)
	{
		out[0] = x[(j0) * 4];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1*sizeof(int));
	int *events = (int*)malloc(1*sizeof(int));
	int *outputs = (int*)malloc(10*sizeof(int));
	int *states = (int*)malloc(11*sizeof(int));
	int i0;
	int i170 = 0;
	int j0 = 0;
	simulator->data = QSS_Data(11,1,1,0,10,"par_sum");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	// Initialize model code.
	for(i0 = 0; i0 <= 9; i0++)
	{
		__PAR_P[(i0)] = (i0+1);
	}
	modelData->event[0].nRHSSt += 10;
	modelData->event[0].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(events,0,1);

	for(i170 = 0; i170 <= 9; i170++)
	{
	modelData->event[0].RHSSt[events[0]++] = i170;
	}
		cleanVector(events,0,1);

	modelData->event[0].LHSDsc[events[0]++] = 0;
		cleanVector(events,0,1);

	modelData->event[0].direction = 1;
	modelData->event[0].relation = 2;
	simulator->time = QSS_Time(11,1,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("par_sum",10,1,11,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[i0]++;
	}
	SD_allocOutputMatrix(modelOutput,11,1);
		cleanVector(states,0,11);

		cleanVector(outputs,0,10);

	for(i0 = 0; i0 <= 9; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"c[%d]",i0+1);
	}
		cleanVector(outputs,0,10);

	for(i0 = 0; i0 <= 9; i0++)
	{
		modelOutput->SO[i0][states[i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = i0;
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
