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
	double tmp0[4];
	int j = 0;
	int j17;
	switch(i)
	{
		case 10:
		tmp0[0] = 0;
	for(j17 = 0; j17 <= 9; j17++)
	{
		tmp0[0] += __PAR_P[j17];
	}
			dx[1] = tmp0[0];
			dx[2] = (tmp0[1])/2;
			dx[3] = (tmp0[2])/6;
			return;
		default:
			j = i;
			if(j >=0 && j <= 9)
			{
				dx[1] = (j+1);
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
	double tmp18[4];
	double tmp20[4];
	double tmp22[4];
	double tmp24[4];
	double tmp26[4];
	double tmp29[4];
	double tmp32[4];
	double tmp35[4];
	double tmp38[4];
	double tmp41[4];
	double tmp44[4];
	double tmp46[4];
	double tmp48[4];
	double tmp50[4];
	double tmp52[4];
	int j19;
	int j21;
	int j23;
	int j25;
	int j27= 0;
	int j28;
	int j30= 0;
	int j31;
	int j33= 0;
	int j34;
	int j36= 0;
	int j37;
	int j39= 0;
	int j40;
	int j42= 0;
	int j43;
	int j45;
	int j47;
	int j49;
	int j51;
	int j53= 0;
	int j54;
	switch(i)
	{
		case 0:
		tmp18[0] = __PAR_P[0];
	for(j19 = 0; j19 <= 9; j19++)
	{
		 if (tmp18[0] < __PAR_P[j19])
	 	tmp18[0] = __PAR_P[j19];
	}
			d[(0)] = tmp18[0];
		tmp20[0] = __PAR_P[0];
	for(j21 = 0; j21 <= 9; j21++)
	{
		 if (tmp20[0] > __PAR_P[j21])
	 	tmp20[0] = __PAR_P[j21];
	}
			d[(0)] = tmp20[0];
		tmp22[0] = 0;
	for(j23 = 0; j23 <= 9; j23++)
	{
		tmp22[0] += __PAR_P[j23];
	}
			d[(0)] = tmp22[0];
		tmp24[0] = 0;
	for(j25 = 0; j25 <= 9; j25++)
	{
		tmp24[0] *= __PAR_P[j25];
	}
			d[(0)] = tmp24[0];
			for(j27 = 0; j27 <= 9; j27++)
			{
					alg[(j27) * 4] = j27;
			}
	for(j28 = 0;j28 < 4; j28++)
	{
		tmp26[j28] = 0;
	}
	for(j28 = 0; j28 <= 9; j28++)
	{
		tmp26[0] += __PAR_P[j28]*alg[(j28) * 4 + 0];
		tmp26[1] += __PAR_P[j28]*alg[(j28) * 4 + 1];
		tmp26[2] += __PAR_P[j28]*alg[(j28) * 4 + 2];
		tmp26[3] += __PAR_P[j28]*alg[(j28) * 4 + 3];
	}
			d[(0)] = tmp26[0];
			for(j30 = 0; j30 <= 9; j30++)
			{
					alg[(j30) * 4] = j30;
			}
	for(j31 = 0;j31 < 4; j31++)
	{
		tmp29[j31] = alg[0 * 4 + 0];
	}
	for(j31 = 0; j31 <= 9; j31++)
	{
		 if (tmp29[0] < alg[(j31) * 4 + 0])
	 	tmp29[0] = alg[(j31) * 4 + 0];
		 if (tmp29[1] < alg[(j31) * 4 + 0])
	 	tmp29[1] = alg[(j31) * 4 + 0];
		 if (tmp29[2] < alg[(j31) * 4 + 0])
	 	tmp29[2] = alg[(j31) * 4 + 0];
		 if (tmp29[3] < alg[(j31) * 4 + 0])
	 	tmp29[3] = alg[(j31) * 4 + 0];
	}
			d[(0)] = tmp29[0];
			for(j33 = 0; j33 <= 9; j33++)
			{
					alg[(j33) * 4] = j33;
			}
	for(j34 = 0;j34 < 4; j34++)
	{
		tmp32[j34] = alg[0 * 4 + 0];
	}
	for(j34 = 0; j34 <= 9; j34++)
	{
		 if (tmp32[0] > alg[(j34) * 4 + 0])
	 	tmp32[0] = alg[(j34) * 4 + 0];
		 if (tmp32[1] > alg[(j34) * 4 + 0])
	 	tmp32[1] = alg[(j34) * 4 + 0];
		 if (tmp32[2] > alg[(j34) * 4 + 0])
	 	tmp32[2] = alg[(j34) * 4 + 0];
		 if (tmp32[3] > alg[(j34) * 4 + 0])
	 	tmp32[3] = alg[(j34) * 4 + 0];
	}
			d[(0)] = tmp32[0];
			for(j36 = 0; j36 <= 9; j36++)
			{
					alg[(j36) * 4] = j36;
			}
	for(j37 = 0;j37 < 4; j37++)
	{
		tmp35[j37] = 0;
	}
	for(j37 = 0; j37 <= 9; j37++)
	{
		tmp35[0] += alg[(j37) * 4 + 0];
		tmp35[1] += alg[(j37) * 4 + 1];
		tmp35[2] += alg[(j37) * 4 + 2];
		tmp35[3] += alg[(j37) * 4 + 3];
	}
			d[(0)] = tmp35[0];
			for(j39 = 0; j39 <= 9; j39++)
			{
					alg[(j39) * 4] = j39;
			}
	for(j40 = 0;j40 < 4; j40++)
	{
		tmp38[j40] = 0;
	}
	for(j40 = 0; j40 <= 9; j40++)
	{
		tmp38[0] *= alg[(j40) * 4 + 0];
		tmp38[1] *= alg[(j40) * 4 + 1];
		tmp38[2] *= alg[(j40) * 4 + 2];
		tmp38[3] *= alg[(j40) * 4 + 3];
	}
			d[(0)] = tmp38[0];
			for(j42 = 0; j42 <= 9; j42++)
			{
					alg[(j42) * 4] = j42;
			}
	for(j43 = 0;j43 < 4; j43++)
	{
		tmp41[j43] = 0;
	}
	for(j43 = 0; j43 <= 9; j43++)
	{
		tmp41[0] += alg[(j43) * 4 + 0] * __PAR_P[j43];
		tmp41[1] += alg[(j43) * 4 + 1] * __PAR_P[j43];
		tmp41[2] += alg[(j43) * 4 + 2] * __PAR_P[j43];
		tmp41[3] += alg[(j43) * 4 + 3] * __PAR_P[j43];
	}
			d[(0)] = tmp41[0];
	for(j45 = 0;j45 < 4; j45++)
	{
		tmp44[j45] = x[0 * 4 + 0];
	}
	for(j45 = 0; j45 <= 9; j45++)
	{
		 if (tmp44[0] < x[(j45) * 4 + 0])
	 	tmp44[0] = x[(j45) * 4 + 0];
		 if (tmp44[1] < x[(j45) * 4 + 0])
	 	tmp44[1] = x[(j45) * 4 + 0];
		 if (tmp44[2] < x[(j45) * 4 + 0])
	 	tmp44[2] = x[(j45) * 4 + 0];
		 if (tmp44[3] < x[(j45) * 4 + 0])
	 	tmp44[3] = x[(j45) * 4 + 0];
	}
			d[(0)] = tmp44[0];
	for(j47 = 0;j47 < 4; j47++)
	{
		tmp46[j47] = x[0 * 4 + 0];
	}
	for(j47 = 0; j47 <= 9; j47++)
	{
		 if (tmp46[0] > x[(j47) * 4 + 0])
	 	tmp46[0] = x[(j47) * 4 + 0];
		 if (tmp46[1] > x[(j47) * 4 + 0])
	 	tmp46[1] = x[(j47) * 4 + 0];
		 if (tmp46[2] > x[(j47) * 4 + 0])
	 	tmp46[2] = x[(j47) * 4 + 0];
		 if (tmp46[3] > x[(j47) * 4 + 0])
	 	tmp46[3] = x[(j47) * 4 + 0];
	}
			d[(0)] = tmp46[0];
	for(j49 = 0;j49 < 4; j49++)
	{
		tmp48[j49] = 0;
	}
	for(j49 = 0; j49 <= 9; j49++)
	{
		tmp48[0] += x[(j49) * 4 + 0];
		tmp48[1] += x[(j49) * 4 + 1];
		tmp48[2] += x[(j49) * 4 + 2];
		tmp48[3] += x[(j49) * 4 + 3];
	}
			d[(0)] = tmp48[0];
	for(j51 = 0;j51 < 4; j51++)
	{
		tmp50[j51] = 0;
	}
	for(j51 = 0; j51 <= 9; j51++)
	{
		tmp50[0] *= x[(j51) * 4 + 0];
		tmp50[1] *= x[(j51) * 4 + 1];
		tmp50[2] *= x[(j51) * 4 + 2];
		tmp50[3] *= x[(j51) * 4 + 3];
	}
			d[(0)] = tmp50[0];
			for(j53 = 0; j53 <= 9; j53++)
			{
					alg[(j53) * 4] = j53;
			}
	for(j54 = 0;j54 < 4; j54++)
	{
		tmp52[j54] = 0;
	}
	for(j54 = 0; j54 <= 9; j54++)
	{
		tmp52[0] += alg[(j54) * 4 + 0] * x[(j54) * 4 + 0];
		tmp52[1] += alg[(j54) * 4 + 1 ]* x[(j54) * 4 + 0 ] + alg[(j54) * 4 + 0 ] * x[(j54) * 4 + 1];
		tmp52[2] += alg[(j54) * 4 + 2] * x[(j54) * 4 + 0] + 2 * alg[(j54) * 4 + 1 ] * x[(j54) * 4 + 1] + alg[(j54) * 4 + 0] * x[(j54) * 4 + 2];
		tmp52[3] += alg[(j54) * 4 + 3] * x[(j54) * 4 + 0] + 3 * alg[(j54) * 4 + 2] * x[(j54) * 4 + 1] + 3 * alg[(j54) * 4 + 1] * x[(j54) * 4 + 2] + alg[(j54) * 4 + 0] * x[(j54) * 4 + 3];
	}
			d[(0)] = tmp52[0];
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 9)
	{
		out[0] = x[(j) * 4];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1*sizeof(int));
	int *events = (int*)malloc(1*sizeof(int));
	int *outputs = (int*)malloc(10*sizeof(int));
	int *states = (int*)malloc(11*sizeof(int));
	int i16 = 0;
	int i55;
	int i;
	int j = 0;
	simulator->data = QSS_Data(11,1,1,0,10,"par_sum");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	// Initialize model code.
	for(i55 = 0; i55 <= 9; i55++)
	{
		__PAR_P[(i55)] = (i55+1);
	}
	modelData->event[0].nRHSSt += 10;
	modelData->event[0].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(events,0,1);

	for(i16 = 0; i16 <= 9; i16++)
	{
	modelData->event[0].RHSSt[events[0]++] = i16;
	}
		cleanVector(events,0,1);

	modelData->event[0].LHSDsc[events[0]++] = 0;
		cleanVector(events,0,1);

	modelData->event[0].direction = 1;
	modelData->event[0].relation = 2;
	simulator->time = QSS_Time(11,1,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("par_sum",10,1,11,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

	for(i = 0; i <= 9; i++)
	{
		modelOutput->nOS[i] = 1;
		modelOutput->nSO[i]++;
	}
	SD_allocOutputMatrix(modelOutput,11,1);
		cleanVector(states,0,11);

		cleanVector(outputs,0,10);

	for(i = 0; i <= 9; i++)
	{
		sprintf(modelOutput->variable[i].name,"c[%d]",i+1);
	}
		cleanVector(outputs,0,10);

	for(i = 0; i <= 9; i++)
	{
		modelOutput->SO[i][states[i]++] = i;
		modelOutput->OS[i][outputs[i]++] = i;
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
