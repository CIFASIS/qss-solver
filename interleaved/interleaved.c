#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>

#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_C = 0;
double __PAR_L = 0;
double __PAR_R = 0;
double __PAR_U = 0;
double __PAR_T = 0;
double __PAR_DC = 0;
double __PAR_ROn = 0;
double __PAR_ROff = 0;

void
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 0;
    settings->parallel = FALSE;
    settings->hybrid = TRUE;
    settings->method = 4;
}

void
MOD_definition (int i, double *x, double *d, double *alg, double t, double *dx)
{
    double tmp0[3];
    int j = 0;
    int j1;
    switch (i)
    {
        case 0:
            for (j1 = 0; j1 < 3; j1++)
            {
                tmp0[j1] = 0;
            }
            for (j1 = 0; j1 <= 3; j1++)
            {
                tmp0[0] += x[(j1 + 1) * 3 + 0];
                tmp0[1] += x[(j1 + 1) * 3 + 1];
                tmp0[2] += x[(j1 + 1) * 3 + 2];
            }
            dx[1] = (tmp0[0] - x[0] / __PAR_R) / __PAR_C;
            dx[2] = (-(1.0 / (__PAR_C)) * ((1.0 / (__PAR_R)) * x[1] - tmp0[1])) / 2;
            return;
        default:
            j = i - 1;
            if (j >= 0 && j <= 3)
            {
                dx[1] = (((__PAR_U / d[(j + 4)]) - x[(j + 1) * 3]) * (d[(j + 4)] * d[(j)] / (d[(j + 4)] + d[(j)])) - x[0]) / __PAR_L;
                dx[2] = (-(x[1] + d[(j + 4)] * (1.0 / (d[(j + 4)] + d[(j)])) * x[(j + 1) * 3 + 1] * d[(j)]) * (1.0 / (__PAR_L))) / 2;
            }
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    double tmp0[3];
    int j = 0;
    int j1;
    switch (i)
    {
        case 0:
            for (j1 = 0; j1 < 3; j1++)
            {
                tmp0[j1] = 0;
            }
            for (j1 = 0; j1 <= 3; j1++)
            {
                tmp0[0] += x[(j1 + 1) * 3 + 0];
                tmp0[1] += x[(j1 + 1) * 3 + 1];
                tmp0[2] += x[(j1 + 1) * 3 + 2];
            }
            der[0 + 1] = (tmp0[0] - x[0] / __PAR_R) / __PAR_C;
            der[0 + 2] = (-(1.0 / (__PAR_C)) * ((1.0 / (__PAR_R)) * x[1] - tmp0[1])) / 2;
            for (j = 0; j <= 3; j++)
            {
                der[(j + 1) * 3 + 1] = (((__PAR_U / d[(j + 4)]) - x[(j + 1) * 3]) * (d[(j + 4)] * d[(j)] / (d[(j + 4)] + d[(j)])) - x[0]) / __PAR_L;
                der[(j + 1) * 3 + 2] = (-(x[1] + d[(j + 4)] * (1.0 / (d[(j + 4)] + d[(j)])) * x[(j + 1) * 3 + 1] * d[(j)]) * (1.0 / (__PAR_L))) / 2;
            }
            break;
    }
    j = i - 1;
    if (j >= 0 && j <= 3)
    {
        der[(j + 1) * 3 + 1] = (((__PAR_U / d[(j + 4)]) - x[(j + 1) * 3]) * (d[(j + 4)] * d[(j)] / (d[(j + 4)] + d[(j)])) - x[0]) / __PAR_L;
        der[(j + 1) * 3 + 2] = (-(x[1] + d[(j + 4)] * (1.0 / (d[(j + 4)] + d[(j)])) * x[(j + 1) * 3 + 1] * d[(j)]) * (1.0 / (__PAR_L))) / 2;
    }
    j = i - 1;
    if (j >= 0 && j <= 3)
    {
        for (j1 = 0; j1 < 3; j1++)
        {
            tmp0[j1] = 0;
        }
        for (j1 = 0; j1 <= 3; j1++)
        {
            tmp0[0] += x[(j1 + 1) * 3 + 0];
            tmp0[1] += x[(j1 + 1) * 3 + 1];
            tmp0[2] += x[(j1 + 1) * 3 + 2];
        }
        der[0 + 1] = (tmp0[0] - x[0] / __PAR_R) / __PAR_C;
        der[0 + 2] = (-(1.0 / (__PAR_C)) * ((1.0 / (__PAR_R)) * x[1] - tmp0[1])) / 2;
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    switch (i)
    {
        case 0:
            zc[0] = t - (d[(8)]);
            zc[1] = 1.0;
            return;
        default:
            if (i >= 1 && i <= 4)
            {
                zc[0] = t - d[(9)] - __PAR_T * (i - 1.0) / 4 - 1.000000000000000020816682e-02 * __PAR_T - (0.0);
                zc[1] = 1.0;
            }
            if (i >= 5 && i <= 8)
            {
                zc[0] = t - d[(9)] - __PAR_T * ((i - 4) - 1.0) / 4 - __PAR_DC * __PAR_T / 4 - 1.000000000000000020816682e-02 * __PAR_T - (0.0);
                zc[1] = 1.0;
            }
            if (i >= 9 && i <= 12)
            {
                zc[0] = x[(i - 8) * 3] - (0.0);
                zc[1] = x[(i - 8) * 3 + 1];
            }
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 0:
            d[(9)] = d[(8)];
            d[(8)] = d[(8)] + __PAR_T;
            return;
        default:
            if (i >= 1 && i <= 4)
            {
                d[(i + 3)] = __PAR_ROn;
                d[(i - 1)] = __PAR_ROff;
            }
            if (i >= 5 && i <= 8)
            {
                d[(i - 1)] = __PAR_ROff;
                d[(i - 5)] = __PAR_ROn;
            }
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 9 && i <= 12)
    {
        d[(i - 9)] = __PAR_ROff;
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    int j = 0;
    switch (i)
    {
        case 0:
            out[0] = x[0];
            return;
        default:
            j = i - 1;
            if (j >= 0 && j <= 3)
            {
                out[0] = x[(j + 1) * 3];
            }
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *discretes = (int*) malloc (10 * sizeof(int));
    int *events = (int*) malloc (13 * sizeof(int));
    int *outputs = (int*) malloc (5 * sizeof(int));
    int *states = (int*) malloc (5 * sizeof(int));
    int i;
    int j = 0;
    simulator->data = QSS_Data (5, 10, 13, 0, 0, "interleaved");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_C = 1.000000000000000047921736e-04;
    __PAR_L = 1.000000000000000047921736e-04;
    __PAR_R = 10.0;
    __PAR_U = 24.0;
    __PAR_T = 1.000000000000000047921736e-04;
    __PAR_DC = 5.000000000000000000000000e-01;
    __PAR_ROn = 1.000000000000000081803054e-05;
    __PAR_ROff = 1.000000000000000000000000e+05;
    for (i = 0; i <= 3; i++)
    {
        modelData->d[i] = 1.000000000000000000000000e+05;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->d[(i + 4)] = 1.000000000000000000000000e+05;
    }
    modelData->d[(8)] = __PAR_T;
    // Initialize model code.
    modelData->nDS[0] = 1;
    modelData->nDS[0] += 4;
    for (i = 0; i <= 3; i++)
    {
        modelData->nDS[i + 1] = 2;
    }
    modelData->nSD[0]++;
    for (i = 0; i <= 3; i++)
    {
        modelData->nSD[i + 1]++;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->nSD[0]++;
        modelData->nSD[i + 1]++;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->nZS[i + 9] = 1;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->nSZ[i + 1]++;
    }
    modelData->nHZ[0] += 9;
    for (i = 1; i <= 4; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 5; i <= 8; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 9; i <= 12; i++)
    {
        modelData->nHD[i]++;
    }
    modelData->event[0].nLHSDsc = 2;
    for (i = 0; i <= 3; i++)
    {
        modelData->event[i + 1].nLHSDsc = 2;
        modelData->event[i + 5].nLHSDsc = 2;
        modelData->event[i + 9].nLHSDsc = 1;
    }
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    cleanVector (states, 0, 5);

    modelData->DS[0][states[0]++] = 0;
    for (i = 0; i <= 3; i++)
    {
        modelData->DS[0][states[0]++] = i + 1;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->DS[i + 1][states[i + 1]++] = 0;
        modelData->DS[i + 1][states[i + 1]++] = i + 1;
    }
    cleanVector (states, 0, 5);

    modelData->SD[0][states[0]++] = 0;
    for (i = 0; i <= 3; i++)
    {
        modelData->SD[i + 1][states[i + 1]++] = 0;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->SD[0][states[0]++] = i + 1;
        modelData->SD[i + 1][states[i + 1]++] = i + 1;
    }
    cleanVector (events, 0, 13);

    for (i = 0; i <= 3; i++)
    {
        modelData->ZS[i + 9][events[i + 9]++] = i + 1;
    }
    cleanVector (states, 0, 5);

    for (i = 0; i <= 3; i++)
    {
        modelData->SZ[i + 1][states[i + 1]++] = i + 9;
    }
    cleanVector (events, 0, 13);

    modelData->HZ[0][events[0]++] = 0;
    for (i = 0; i <= 3; i++)
    {
        modelData->HZ[0][events[0]++] = i + 1;
    }
    for (i = 0; i <= 3; i++)
    {
        modelData->HZ[0][events[0]++] = i + 5;
    }
    cleanVector (events, 0, 13);

    for (i = 1; i <= 4; i++)
    {
        modelData->HD[i][events[i]++] = i;
    }
    for (i = 5; i <= 8; i++)
    {
        modelData->HD[i][events[i]++] = i - 4;
    }
    for (i = 9; i <= 12; i++)
    {
        modelData->HD[i][events[i]++] = i - 8;
    }
    cleanVector (events, 0, 13);

    modelData->event[0].LHSDsc[events[0]++] = 8;
    modelData->event[0].LHSDsc[events[0]++] = 9;
    for (i = 0; i <= 3; i++)
    {
        modelData->event[i + 1].LHSDsc[events[i + 1]++] = i;
        modelData->event[i + 1].LHSDsc[events[i + 1]++] = i + 4;
        modelData->event[i + 5].LHSDsc[events[i + 5]++] = i;
        modelData->event[i + 5].LHSDsc[events[i + 5]++] = i + 4;
        modelData->event[i + 9].LHSDsc[events[i + 9]++] = i;
    }
    cleanVector (events, 0, 13);

    modelData->event[0].direction = 1;
    modelData->event[0].relation = 3;
    for (i = 0; i <= 3; i++)
    {
        modelData->event[i + 1].direction = 1;
        modelData->event[i + 1].relation = 2;
        modelData->event[i + 5].direction = 1;
        modelData->event[i + 5].relation = 2;
        modelData->event[i + 9].direction = -1;
        modelData->event[i + 9].relation = 0;
    }
    simulator->time = QSS_Time (5, 13, 0, 0, ST_Binary, NULL);

    simulator->output = SD_Output ("interleaved", 5, 10, 5, NULL, 0, 0, CI_Step, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[0]++;
    for (i = 0; i <= 3; i++)
    {
        modelOutput->nOS[i + 1] = 1;
        modelOutput->nSO[i + 1]++;
    }
    SD_allocOutputMatrix (modelOutput, 5, 10);
    cleanVector (states, 0, 5);

    cleanVector (outputs, 0, 5);

    sprintf (modelOutput->variable[0].name, "uC");
    for (i = 0; i <= 3; i++)
    {
        sprintf (modelOutput->variable[i + 1].name, "iL[%d]", i + 1);
    }
    cleanVector (outputs, 0, 5);

    modelOutput->SO[0][states[0]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 0;
    for (i = 0; i <= 3; i++)
    {
        modelOutput->SO[i + 1][states[i + 1]++] = i + 1;
        modelOutput->OS[i + 1][outputs[i + 1]++] = i + 1;
    }
    simulator->model = QSS_Model (MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg);
    free (discretes);
    free (events);
    free (outputs);
    free (states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
