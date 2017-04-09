#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_VecInt_1_p[5];
double __PAR_VecInt_1_x0 = 0;
double __PAR_VecInt_2_p[5];
double __PAR_VecInt_2_x0 = 0;
double __PAR_VectorSum_3_p[10];
double __PAR_VectorSum_3_w[2];
double __PAR_IndexShift_4_p[2];
double __PAR_VectorSum_5_p[10];
double __PAR_VectorSum_5_w[3];
double __PAR_IndexShift_6_p[2];
double __PAR_Scalar2Vector_7_p[2];
double __PAR_pulse_sci_8_p[4];
double __PAR_pulse_sci_8_low = 0;
double __PAR_pulse_sci_8_amplitude = 0;
double __PAR_pulse_sci_8_ti = 0;
double __PAR_pulse_sci_8_tf = 0;
double __PAR_Vec2Scalar_10_p[2];
int __reverse__PAR_Vec2Scalar_10_Index = 0;
int __PAR_Vec2Scalar_10_Index = 0;
double __PAR_Vec2Scalar_11_p[2];
int __reverse__PAR_Vec2Scalar_11_Index = 0;
int __PAR_Vec2Scalar_11_Index = 0;

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
	int j = 0;
	int j0 = 0;
	int j1 = 0;
	int j2 = 0;
	int j3 = 0;
	j = i;
	if(j >=0 && j <= 49)
	{
		alg[0] = d[(0)];
		alg[(j+101) * 4] = x[(j+50) * 4];
		j0 = j+1;
	if (j >= 0 && j <= 48)
	{
		alg[(j+1+151) * 4] = x[(j0+50) * 4];
		}
		alg[804] = alg[0];
		alg[1208] = alg[804];
		j1 = j;
	if (j1 >= 1 && j1 <= 49)
	{
		alg[(j+302) * 4] = 0.0;
		}
		j1 = j;
	if (j1 >= 0 && j1 <= 48)
	{
		alg[(j+352) * 4] = alg[(j1+152) * 4];
		}
		j1 = j;
	if (j1 >= 49 && j1 <= 49)
	{
		alg[(j+352) * 4] = 0.0;
		}
		alg[(j+402) * 4] = alg[(j+352) * 4];
		alg[(j+452) * 4] = alg[(j+302) * 4];
		alg[(j+602) * 4] = alg[(j+452) * 4]*__PAR_VectorSum_5_w[(0)]+alg[(j+402) * 4]*__PAR_VectorSum_5_w[(1)]+alg[(j+101) * 4]*__PAR_VectorSum_5_w[(2)];
		alg[(j+702) * 4] = alg[(j+602) * 4];
		dx[1] = alg[(j+702) * 4];
	}
	j = i-50;
	if(j >=0 && j <= 49)
	{
		alg[(j+1) * 4] = x[(j) * 4];
		j2 = j-1;
	if (j >= 1 && j <= 49)
	{
		alg[(j-1+51) * 4] = x[(j2) * 4];
		}
		j3 = j;
	if (j3 >= 0 && j3 <= 0)
	{
		alg[(j+502) * 4] = 0.0;
		}
		j3 = j-1;
	if (j3 >= 0 && j3 <= 48)
	{
		alg[(j+502) * 4] = alg[(j3+51) * 4];
		}
		alg[(j+552) * 4] = alg[(j+502) * 4];
		alg[(j+652) * 4] = alg[(j+1) * 4]*__PAR_VectorSum_3_w[(0)]+alg[(j+552) * 4]*__PAR_VectorSum_3_w[(1)];
		alg[(j+752) * 4] = alg[(j+652) * 4];
		dx[1] = alg[(j+752) * 4];
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	int j0 = 0;
	int j1 = 0;
	int j10 = 0;
	int j11 = 0;
	int j2 = 0;
	int j3 = 0;
	int j4 = 0;
	int j5 = 0;
	int j6 = 0;
	int j7 = 0;
	int j8 = 0;
	int j9 = 0;
	j = i+1;
	if(j >=0 && j <= 49)
	{
		alg[(j+1) * 4] = x[(j) * 4];
		j4 = j-1;
	if (j >= 1 && j <= 49)
	{
		alg[(j-1+51) * 4] = x[(j4) * 4];
		}
		j5 = j;
	if (j5 >= 0 && j5 <= 0)
	{
		alg[(j+502) * 4] = 0.0;
		}
		j5 = j-1;
	if (j5 >= 0 && j5 <= 48)
	{
		alg[(j+502) * 4] = alg[(j5+51) * 4];
		}
		alg[(j+552) * 4] = alg[(j+502) * 4];
		alg[(j+652) * 4] = alg[(j+1) * 4]*__PAR_VectorSum_3_w[(0)]+alg[(j+552) * 4]*__PAR_VectorSum_3_w[(1)];
		alg[(j+752) * 4] = alg[(j+652) * 4];
		der[(j+50) * 4 + 1] = alg[(j+752) * 4];
	}
	j = i;
	if(j >=0 && j <= 49)
	{
		alg[(j+1) * 4] = x[(j) * 4];
		j6 = j-1;
	if (j >= 1 && j <= 49)
	{
		alg[(j-1+51) * 4] = x[(j6) * 4];
		}
		j7 = j;
	if (j7 >= 0 && j7 <= 0)
	{
		alg[(j+502) * 4] = 0.0;
		}
		j7 = j-1;
	if (j7 >= 0 && j7 <= 48)
	{
		alg[(j+502) * 4] = alg[(j7+51) * 4];
		}
		alg[(j+552) * 4] = alg[(j+502) * 4];
		alg[(j+652) * 4] = alg[(j+1) * 4]*__PAR_VectorSum_3_w[(0)]+alg[(j+552) * 4]*__PAR_VectorSum_3_w[(1)];
		alg[(j+752) * 4] = alg[(j+652) * 4];
		der[(j+50) * 4 + 1] = alg[(j+752) * 4];
	}
	j = i-50;
	if(j >=0 && j <= 49)
	{
		alg[0] = d[(0)];
		alg[(j+101) * 4] = x[(j+50) * 4];
		j8 = j+1;
	if (j >= 0 && j <= 48)
	{
		alg[(j+1+151) * 4] = x[(j8+50) * 4];
		}
		alg[804] = alg[0];
		alg[1208] = alg[804];
		j9 = j;
	if (j9 >= 1 && j9 <= 49)
	{
		alg[(j+302) * 4] = 0.0;
		}
		j9 = j;
	if (j9 >= 0 && j9 <= 48)
	{
		alg[(j+352) * 4] = alg[(j9+152) * 4];
		}
		j9 = j;
	if (j9 >= 49 && j9 <= 49)
	{
		alg[(j+352) * 4] = 0.0;
		}
		alg[(j+402) * 4] = alg[(j+352) * 4];
		alg[(j+452) * 4] = alg[(j+302) * 4];
		alg[(j+602) * 4] = alg[(j+452) * 4]*__PAR_VectorSum_5_w[(0)]+alg[(j+402) * 4]*__PAR_VectorSum_5_w[(1)]+alg[(j+101) * 4]*__PAR_VectorSum_5_w[(2)];
		alg[(j+702) * 4] = alg[(j+602) * 4];
		der[(j) * 4 + 1] = alg[(j+702) * 4];
	}
	j = i-51;
	if(j >=0 && j <= 49)
	{
		alg[0] = d[(0)];
		alg[(j+101) * 4] = x[(j+50) * 4];
		j10 = j+1;
	if (j >= 0 && j <= 48)
	{
		alg[(j+1+151) * 4] = x[(j10+50) * 4];
		}
		alg[804] = alg[0];
		alg[1208] = alg[804];
		j11 = j;
	if (j11 >= 1 && j11 <= 49)
	{
		alg[(j+302) * 4] = 0.0;
		}
		j11 = j;
	if (j11 >= 0 && j11 <= 48)
	{
		alg[(j+352) * 4] = alg[(j11+152) * 4];
		}
		j11 = j;
	if (j11 >= 49 && j11 <= 49)
	{
		alg[(j+352) * 4] = 0.0;
		}
		alg[(j+402) * 4] = alg[(j+352) * 4];
		alg[(j+452) * 4] = alg[(j+302) * 4];
		alg[(j+602) * 4] = alg[(j+452) * 4]*__PAR_VectorSum_5_w[(0)]+alg[(j+402) * 4]*__PAR_VectorSum_5_w[(1)]+alg[(j+101) * 4]*__PAR_VectorSum_5_w[(2)];
		alg[(j+702) * 4] = alg[(j+602) * 4];
		der[(j) * 4 + 1] = alg[(j+702) * 4];
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	switch(i)
	{
		case 0:
			zc[0] = t-(0.0);
			return;
		case 1:
			zc[0] = t-(1.0);
			return;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	switch(i)
	{
		case 0:
			d[(0)] = 1.0;
			return;
		case 1:
			d[(0)] = 0.0;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1*sizeof(int));
	int *events = (int*)malloc(2*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(100*sizeof(int));
	int i12;
	int i13;
	int i14;
	int i15;
	int i;
	int j = 0;
	simulator->data = QSS_Data(100,1,2,0,804,"lcline_qss");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_VecInt_1_x0 = 0.0;
	__PAR_VecInt_2_x0 = 0.0;
	__PAR_pulse_sci_8_low = 0.0;
	__PAR_pulse_sci_8_amplitude = 1.0;
	__PAR_pulse_sci_8_ti = 0.0;
	__PAR_pulse_sci_8_tf = 1.0;
	__PAR_Vec2Scalar_10_Index = 1.0;
	__PAR_Vec2Scalar_11_Index = 50.0;
	modelData->d[(0)] = 0.0;
	// Initialize model code.
	for(i12 = 0; i12 <= 49; i12++)
	{
		modelData->x[(i12+50) * 4] = 0.0;
	}
	for(i13 = 0; i13 <= 49; i13++)
	{
		modelData->x[(i13) * 4] = 0.0;
	}
		__PAR_VecInt_1_p[(0)] = 0.0;
		__PAR_VecInt_1_p[(1)] = 9.999999999999999547481118e-07;
		__PAR_VecInt_1_p[(2)] = 1.000000000000000020816682e-03;
		__PAR_VecInt_1_p[(3)] = 0.0;
		__PAR_VecInt_1_p[(4)] = 50.0;
		__PAR_VecInt_2_p[(0)] = 0.0;
		__PAR_VecInt_2_p[(1)] = 9.999999999999999547481118e-07;
		__PAR_VecInt_2_p[(2)] = 1.000000000000000020816682e-03;
		__PAR_VecInt_2_p[(3)] = 0.0;
		__PAR_VecInt_2_p[(4)] = 50.0;
		__PAR_VectorSum_3_p[(0)] = 1.0;
		__PAR_VectorSum_3_p[(1)] = -1.0;
		__PAR_VectorSum_3_p[(2)] = 0.0;
		__PAR_VectorSum_3_p[(3)] = 0.0;
		__PAR_VectorSum_3_p[(4)] = 0.0;
		__PAR_VectorSum_3_p[(5)] = 0.0;
		__PAR_VectorSum_3_p[(6)] = 0.0;
		__PAR_VectorSum_3_p[(7)] = 0.0;
		__PAR_VectorSum_3_p[(8)] = 2.0;
		__PAR_VectorSum_3_p[(9)] = 50.0;
	for(i14 = 0; i14 <= 1; i14++)
	{
		__PAR_VectorSum_3_w[(i14)] = __PAR_VectorSum_3_p[(i14)];
	}
		__PAR_IndexShift_4_p[(0)] = 1.0;
		__PAR_IndexShift_4_p[(1)] = 50.0;
		__PAR_VectorSum_5_p[(0)] = 1.0;
		__PAR_VectorSum_5_p[(1)] = 1.0;
		__PAR_VectorSum_5_p[(2)] = -1.0;
		__PAR_VectorSum_5_p[(3)] = 0.0;
		__PAR_VectorSum_5_p[(4)] = 0.0;
		__PAR_VectorSum_5_p[(5)] = 0.0;
		__PAR_VectorSum_5_p[(6)] = 0.0;
		__PAR_VectorSum_5_p[(7)] = 0.0;
		__PAR_VectorSum_5_p[(8)] = 3.0;
		__PAR_VectorSum_5_p[(9)] = 50.0;
	for(i15 = 0; i15 <= 2; i15++)
	{
		__PAR_VectorSum_5_w[(i15)] = __PAR_VectorSum_5_p[(i15)];
	}
		__PAR_IndexShift_6_p[(0)] = -1.0;
		__PAR_IndexShift_6_p[(1)] = 50.0;
		__PAR_Scalar2Vector_7_p[(0)] = 0.0;
		__PAR_Scalar2Vector_7_p[(1)] = 50.0;
		__PAR_pulse_sci_8_p[(0)] = 0.0;
		__PAR_pulse_sci_8_p[(1)] = 1.0;
		__PAR_pulse_sci_8_p[(2)] = 0.0;
		__PAR_pulse_sci_8_p[(3)] = 1.0;
		__PAR_Vec2Scalar_10_p[(0)] = 0.0;
		__PAR_Vec2Scalar_10_p[(1)] = 50.0;
		__PAR_Vec2Scalar_11_p[(0)] = 49.0;
		__PAR_Vec2Scalar_11_p[(1)] = 50.0;
		modelData->d[(0)] = 1.0;
	for ( i = 0; i <= 49; i++) 
	{
	modelData->nDS[i]++;
	}
	for ( i = 0; i <= 48; i++) 
	{
	modelData->nDS[i]++;
	}
	for ( i = 1; i <= 49; i++) 
	{
	modelData->nDS[i+50]++;
	}
	for ( i = 0; i <= 49; i++) 
	{
	modelData->nDS[i+50]++;
	}
	for ( i = 0; i <= 49; i++) 
	{
	modelData->nSD[i+50]++;
	}
	for ( i = 0; i <= 48; i++) 
	{
	modelData->nSD[i+51]++;
	}
	for ( i = 1; i <= 49; i++) 
	{
	modelData->nSD[i-1]++;
	}
	for ( i = 0; i <= 49; i++) 
	{
	modelData->nSD[i]++;
	}
	modelData->nHD[0] = 1;
	modelData->nHD[1] = 1;
	modelData->event[0].nLHSDsc = 1;
	modelData->event[1].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,100);

	for ( i = 0; i <= 49; i++) 
	{
	modelData->DS[i][states[i]++] = i+50;
	}
	for ( i = 0; i <= 48; i++) 
	{
	modelData->DS[i][states[i]++] = i+51;
	}
	for ( i = 1; i <= 49; i++) 
	{
	modelData->DS[i+50][states[i+50]++] = i-1;
	}
	for ( i = 0; i <= 49; i++) 
	{
	modelData->DS[i+50][states[i+50]++] = i;
	}
		cleanVector(states,0,100);

	for ( i = 0; i <= 49; i++) 
	{
	modelData->SD[i+50][states[i+50]++] = i;
	}
	for ( i = 0; i <= 48; i++) 
	{
	modelData->SD[i+51][states[i+51]++] = i;
	}
	for ( i = 1; i <= 49; i++) 
	{
	modelData->SD[i-1][states[i-1]++] = i+50;
	}
	for ( i = 0; i <= 49; i++) 
	{
	modelData->SD[i][states[i]++] = i+50;
	}
		cleanVector(events,0,2);

	modelData->HD[0][events[0]++] = 0;
	modelData->HD[1][events[1]++] = 0;
		cleanVector(events,0,2);

	modelData->event[0].LHSDsc[events[0]++] = 0;
	modelData->event[1].LHSDsc[events[1]++] = 0;
		cleanVector(events,0,2);

	modelData->event[0].direction = 1;
	modelData->event[0].relation = 2;
	modelData->event[1].direction = 1;
	modelData->event[1].relation = 2;
	simulator->time = QSS_Time(100,2,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.01;
	simulator->output = SD_Output("lcline_qss",1,1,100,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,100,1);
		cleanVector(states,0,100);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"VecInt_2_y_1[1]");
		cleanVector(outputs,0,1);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
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
