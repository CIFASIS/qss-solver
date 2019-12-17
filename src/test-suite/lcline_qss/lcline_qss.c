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
	int i0 = i;
	int j0 = 0;
	int j10 = 0;
	int j20 = 0;
	int j30 = 0;
	int j40 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 49)
	{
		alg[0] = d[(0)];
		alg[(j0+101) * 4] = x[(j0+50) * 4];
		j10 = j0+1;
	if (j0 >= 0 && j0 <= 48)
	{
		alg[(j0+1+151) * 4] = x[(j10+50) * 4];
		}
		alg[804] = alg[0];
		alg[1208] = alg[804];
		j20 = j0;
	if (j20 >= 1 && j20 <= 49)
	{
		alg[(j0+302) * 4] = 0.0;
		}
		j20 = j0;
	if (j20 >= 0 && j20 <= 48)
	{
		alg[(j0+352) * 4] = alg[(j20+152) * 4];
		}
		j20 = j0;
	if (j20 >= 49 && j20 <= 49)
	{
		alg[(j0+352) * 4] = 0.0;
		}
		alg[(j0+402) * 4] = alg[(j0+352) * 4];
		alg[(j0+452) * 4] = alg[(j0+302) * 4];
		alg[(j0+602) * 4] = alg[(j0+452) * 4]*__PAR_VectorSum_5_w[(0)]+alg[(j0+402) * 4]*__PAR_VectorSum_5_w[(1)]+alg[(j0+101) * 4]*__PAR_VectorSum_5_w[(2)];
		alg[(j0+702) * 4] = alg[(j0+602) * 4];
		dx[1] = alg[(j0+702) * 4];
	}
	j0 = i0-50;
	if(j0 >=0 && j0 <= 49)
	{
		alg[(j0+1) * 4] = x[(j0) * 4];
		j30 = j0-1;
	if (j0 >= 1 && j0 <= 49)
	{
		alg[(j0-1+51) * 4] = x[(j30) * 4];
		}
		j40 = j0;
	if (j40 >= 0 && j40 <= 0)
	{
		alg[(j0+502) * 4] = 0.0;
		}
		j40 = j0-1;
	if (j40 >= 0 && j40 <= 48)
	{
		alg[(j0+502) * 4] = alg[(j40+51) * 4];
		}
		alg[(j0+552) * 4] = alg[(j0+502) * 4];
		alg[(j0+652) * 4] = alg[(j0+1) * 4]*__PAR_VectorSum_3_w[(0)]+alg[(j0+552) * 4]*__PAR_VectorSum_3_w[(1)];
		alg[(j0+752) * 4] = alg[(j0+652) * 4];
		dx[1] = alg[(j0+752) * 4];
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	int j0100 = 0;
	int j0120 = 0;
	int j060 = 0;
	int j080 = 0;
	int j10 = 0;
	int j110 = 0;
	int j20 = 0;
	int j30 = 0;
	int j40 = 0;
	int j50 = 0;
	int j70 = 0;
	int j90 = 0;
	j0 = i0+1;
	if(j0 >=1 && j0 <= 49)
	{
		alg[(j0+1) * 4] = x[(j0) * 4];
		j50 = j0-1;
	if (j0 >= 1 && j0 <= 49)
	{
		alg[(j0-1+51) * 4] = x[(j50) * 4];
		}
		j060 = j0;
	if (j060 >= 0 && j060 <= 0)
	{
		alg[(j0+502) * 4] = 0.0;
		}
		j060 = j0-1;
	if (j060 >= 0 && j060 <= 48)
	{
		alg[(j0+502) * 4] = alg[(j060+51) * 4];
		}
		alg[(j0+552) * 4] = alg[(j0+502) * 4];
		alg[(j0+652) * 4] = alg[(j0+1) * 4]*__PAR_VectorSum_3_w[(0)]+alg[(j0+552) * 4]*__PAR_VectorSum_3_w[(1)];
		alg[(j0+752) * 4] = alg[(j0+652) * 4];
		der[(j0+50) * 4 + 1] = alg[(j0+752) * 4];
	}
	j0 = i0;
	if(j0 >=0 && j0 <= 49)
	{
		alg[(j0+1) * 4] = x[(j0) * 4];
		j70 = j0-1;
	if (j0 >= 1 && j0 <= 49)
	{
		alg[(j0-1+51) * 4] = x[(j70) * 4];
		}
		j080 = j0;
	if (j080 >= 0 && j080 <= 0)
	{
		alg[(j0+502) * 4] = 0.0;
		}
		j080 = j0-1;
	if (j080 >= 0 && j080 <= 48)
	{
		alg[(j0+502) * 4] = alg[(j080+51) * 4];
		}
		alg[(j0+552) * 4] = alg[(j0+502) * 4];
		alg[(j0+652) * 4] = alg[(j0+1) * 4]*__PAR_VectorSum_3_w[(0)]+alg[(j0+552) * 4]*__PAR_VectorSum_3_w[(1)];
		alg[(j0+752) * 4] = alg[(j0+652) * 4];
		der[(j0+50) * 4 + 1] = alg[(j0+752) * 4];
	}
	j0 = i0-50;
	if(j0 >=0 && j0 <= 49)
	{
		alg[0] = d[(0)];
		alg[(j0+101) * 4] = x[(j0+50) * 4];
		j90 = j0+1;
	if (j0 >= 0 && j0 <= 48)
	{
		alg[(j0+1+151) * 4] = x[(j90+50) * 4];
		}
		alg[804] = alg[0];
		alg[1208] = alg[804];
		j0100 = j0;
	if (j0100 >= 1 && j0100 <= 49)
	{
		alg[(j0+302) * 4] = 0.0;
		}
		j0100 = j0;
	if (j0100 >= 0 && j0100 <= 48)
	{
		alg[(j0+352) * 4] = alg[(j0100+152) * 4];
		}
		j0100 = j0;
	if (j0100 >= 49 && j0100 <= 49)
	{
		alg[(j0+352) * 4] = 0.0;
		}
		alg[(j0+402) * 4] = alg[(j0+352) * 4];
		alg[(j0+452) * 4] = alg[(j0+302) * 4];
		alg[(j0+602) * 4] = alg[(j0+452) * 4]*__PAR_VectorSum_5_w[(0)]+alg[(j0+402) * 4]*__PAR_VectorSum_5_w[(1)]+alg[(j0+101) * 4]*__PAR_VectorSum_5_w[(2)];
		alg[(j0+702) * 4] = alg[(j0+602) * 4];
		der[(j0) * 4 + 1] = alg[(j0+702) * 4];
	}
	j0 = i0-51;
	if(j0 >=0 && j0 <= 48)
	{
		alg[0] = d[(0)];
		alg[(j0+101) * 4] = x[(j0+50) * 4];
		j110 = j0+1;
	if (j0 >= 0 && j0 <= 48)
	{
		alg[(j0+1+151) * 4] = x[(j110+50) * 4];
		}
		alg[804] = alg[0];
		alg[1208] = alg[804];
		j0120 = j0;
	if (j0120 >= 1 && j0120 <= 49)
	{
		alg[(j0+302) * 4] = 0.0;
		}
		j0120 = j0;
	if (j0120 >= 0 && j0120 <= 48)
	{
		alg[(j0+352) * 4] = alg[(j0120+152) * 4];
		}
		j0120 = j0;
	if (j0120 >= 49 && j0120 <= 49)
	{
		alg[(j0+352) * 4] = 0.0;
		}
		alg[(j0+402) * 4] = alg[(j0+352) * 4];
		alg[(j0+452) * 4] = alg[(j0+302) * 4];
		alg[(j0+602) * 4] = alg[(j0+452) * 4]*__PAR_VectorSum_5_w[(0)]+alg[(j0+402) * 4]*__PAR_VectorSum_5_w[(1)]+alg[(j0+101) * 4]*__PAR_VectorSum_5_w[(2)];
		alg[(j0+702) * 4] = alg[(j0+602) * 4];
		der[(j0) * 4 + 1] = alg[(j0+702) * 4];
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
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
	int i0 = i;
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
	int i0;
	int j0 = 0;
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
	for(i0 = 0; i0 <= 49; i0++)
	{
		modelData->x[(i0+50) * 4] = 0.0;
	}
	for(i0 = 0; i0 <= 49; i0++)
	{
		modelData->x[(i0) * 4] = 0.0;
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
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_VectorSum_3_w[(i0)] = __PAR_VectorSum_3_p[(i0)];
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
	for(i0 = 0; i0 <= 2; i0++)
	{
		__PAR_VectorSum_5_w[(i0)] = __PAR_VectorSum_5_p[(i0)];
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
	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->nDS[i0]++;
	}
	for ( i0 = 0; i0 <= 48; i0++) 
	{
	modelData->nDS[i0]++;
	}
	for ( i0 = 1; i0 <= 49; i0++) 
	{
	modelData->nDS[i0+50]++;
	}
	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->nDS[i0+50]++;
	}
	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->nSD[i0+50]++;
	}
	for ( i0 = 0; i0 <= 48; i0++) 
	{
	modelData->nSD[i0+51]++;
	}
	for ( i0 = 1; i0 <= 49; i0++) 
	{
	modelData->nSD[i0-1]++;
	}
	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->nSD[i0]++;
	}
	modelData->nHD[0] = 1;
	modelData->nHD[1] = 1;
	modelData->event[0].nLHSDsc = 1;
	modelData->event[1].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,100);

	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0+50;
	}
	for ( i0 = 0; i0 <= 48; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0+51;
	}
	for ( i0 = 1; i0 <= 49; i0++) 
	{
	modelData->DS[i0+50][states[i0+50]++] = i0-1;
	}
	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->DS[i0+50][states[i0+50]++] = i0;
	}
		cleanVector(states,0,100);

	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->SD[i0+50][states[i0+50]++] = i0;
	}
	for ( i0 = 0; i0 <= 48; i0++) 
	{
	modelData->SD[i0+51][states[i0+51]++] = i0;
	}
	for ( i0 = 1; i0 <= 49; i0++) 
	{
	modelData->SD[i0-1][states[i0-1]++] = i0+50;
	}
	for ( i0 = 0; i0 <= 49; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0+50;
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
