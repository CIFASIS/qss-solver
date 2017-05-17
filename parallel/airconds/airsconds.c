#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_math.h"
#include <common/utils.h>

#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_CAP[2000];
double __PAR_RES[2000];
double __PAR_POT[2000];
double __PAR_THA = 0;
double __PAR_pmax = 0;
double __PAR_Kp = 0;
double __PAR_Ki = 0;
double __PAR_tref = 0;

void
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 9;
    settings->parallel = TRUE;
    settings->hybrid = TRUE;
    settings->method = 5;
}

void
MOD_definition (int i, double *x, double *d, double *alg, double t, double *dx)
{
    int j = 0;
    switch (i)
    {
        case 2000:
            dx[1] = d[(4002)] - d[(0)] / __PAR_pmax;
            dx[2] = (0.0) / 2;
            dx[3] = (0.0) / 6;
            return;
        default:
            j = i;
            if (j >= 0 && j <= 1999)
            {
                dx[1] = (__PAR_THA / __PAR_RES[(j)] - __PAR_POT[(j)] * d[(j + 2001)] - x[(j) * 4] / __PAR_RES[(j)] + d[(j + 4004)] / __PAR_RES[(j)])
                        / __PAR_CAP[(j)];
                dx[2] = (-(1.0 / (__PAR_CAP[(j)])) * x[(j) * 4 + 1] * (1.0 / (__PAR_RES[(j)]))) / 2;
                dx[3] = (-(1.0 / (__PAR_CAP[(j)])) * (1.0 / (__PAR_RES[(j)])) * x[(j) * 4 + 2] * 2) / 6;
            }
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    int j = 0;
    j = i;
    if (j >= 0 && j <= 1999)
    {
        if (map[j] > NOT_ASSIGNED)
        {
            der[(j) * 4 + 1] = (__PAR_THA / __PAR_RES[(j)] - __PAR_POT[(j)] * d[(j + 2001)] - x[(j) * 4] / __PAR_RES[(j)]
                    + d[(j + 4004)] / __PAR_RES[(j)]) / __PAR_CAP[(j)];
            der[(j) * 4 + 2] = (-(1.0 / (__PAR_CAP[(j)])) * x[(j) * 4 + 1] * (1.0 / (__PAR_RES[(j)]))) / 2;
            der[(j) * 4 + 3] = (-(1.0 / (__PAR_CAP[(j)])) * (1.0 / (__PAR_RES[(j)])) * x[(j) * 4 + 2] * 2) / 6;
        }
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    switch (i)
    {
        case 0:
            zc[0] = t - (1000.0);
            zc[1] = 1.0;
            zc[2] = (0.0) / 2;
            return;
        case 1:
            zc[0] = t - (2000.0);
            zc[1] = 1.0;
            zc[2] = (0.0) / 2;
            return;
        case 2:
            zc[0] = t - (d[(4003)]);
            zc[1] = 1.0;
            zc[2] = (0.0) / 2;
            return;
        default:
            if (i >= 3 && i <= 2002)
            {
                zc[0] = x[(i - 3) * 4] - __PAR_tref - d[(4001)] + d[(i + 1998)] - 5.000000000000000000000000e-01 - (0.0);
                zc[1] = x[(i - 3) * 4 + 1];
                zc[2] = (x[(i - 3) * 4 + 2] * 2) / 2;
            }
            if (i >= 2003 && i <= 4002)
            {
                zc[0] = t - (d[(i + 4001)]);
                zc[1] = 1.0;
                zc[2] = (0.0) / 2;
            }
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    double tmp3[4];
    int j4;
    switch (i)
    {
        case 0:
            d[(4002)] = 4.000000000000000222044605e-01;
            return;
        case 1:
            d[(4002)] = 5.000000000000000000000000e-01;
            return;
        case 2:
            d[(4003)] = d[(4003)] + 1.0;
            tmp3[0] = 0;
            for (j4 = 0; j4 <= 1999; j4++)
            {
                tmp3[0] += d[j4 + 1];
            }
            d[(0)] = tmp3[0];
            d[(4001)] = __PAR_Kp * (d[(0)] / __PAR_pmax - d[(4002)]) - __PAR_Ki * x[8000];
            return;
        default:
            if (i >= 3 && i <= 2002)
            {
                d[(i + 1998)] = 1.0;
                d[(i - 2)] = __PAR_POT[(i - 3)];
            }
            if (i >= 2003 && i <= 4002)
            {
                d[(i + 4001)] = d[(i + 4001)] + 1.0;
                d[(i + 6001)] = d[(i + 6001)] + 1.0;
                d[(i + 2001)] = 2.0 * pow (sin ((i - 2002) * d[(i + 6001)]), 2.0);
            }
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 3 && i <= 2002)
    {
        d[(i + 1998)] = 0.0;
        d[(i - 2)] = 0.0;
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = x[8000];
            return;
        case 1:
            out[0] = d[(0)];
            return;
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    double tmp14[4];
    int *discretes = (int*) malloc (10004 * sizeof(int));
    int *events = (int*) malloc (4003 * sizeof(int));
    int *outputs = (int*) malloc (2 * sizeof(int));
    int *states = (int*) malloc (2001 * sizeof(int));
    int i13;
    int i6;
    int i;
    int j = 0;
    int j15;
    int tmp2 = 0;
    simulator->data = QSS_Data (2001, 10004, 4003, 0, 0, "airsconds");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_THA = 32.0;
    __PAR_pmax = 0.0;
    __PAR_Kp = 1.0;
    __PAR_Ki = 1.0;
    __PAR_tref = 20.0;
    modelData->d[(4003)] = 1.0;
    modelData->d[(4002)] = 5.000000000000000000000000e-01;
    // Initialize model code.
    for (i6 = 0; i6 <= 1999; i6++)
    {
        modelData->x[(i6) * 4] = __math__rand (4.0) + 18.0;
        __PAR_CAP[(i6)] = __math__rand (100.0) + 550.0;
        __PAR_RES[(i6)] = __math__rand (4.000000000000000222044605e-01) + 1.800000000000000044408921e+00;
        __PAR_POT[(i6)] = __math__rand (2.000000000000000111022302e-01) + 13.0;
        __PAR_pmax = __PAR_pmax + __PAR_POT[(i6)];
        modelData->d[(i6 + 4004)] = 2.0 * pow (sin ((i6 + 1)), 2.0);
        modelData->d[(i6 + 6004)] = __math__rand (2.0);
    }
    for (i13 = 0; i13 <= 1999; i13++)
    {
        if (modelData->x[(i13) * 4] - __PAR_tref - 5.000000000000000000000000e-01 > 0.0)
        {
            modelData->d[(i13 + 2001)] = 1.0;
            modelData->d[(i13 + 1)] = __PAR_POT[(i13)];
        }
    }
    tmp14[0] = 0;
    for (j15 = 0; j15 <= 1999; j15++)
    {
        tmp14[0] += modelData->d[j15 + 1];
    }
    modelData->d[(0)] = tmp14[0];
    for (i = 0; i <= 1999; i++)
    {
        modelData->nDS[i] = 1;
    }
    for (i = 0; i <= 1999; i++)
    {
        modelData->nSD[i]++;
    }
    for (i = 0; i <= 1999; i++)
    {
        modelData->nZS[i + 3] = 1;
    }
    for (i = 0; i <= 1999; i++)
    {
        modelData->nSZ[i]++;
    }
    modelData->nHZ[2] += 2001;
    for (i = 3; i <= 2002; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2003; i <= 4002; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 3; i <= 2002; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 2003; i <= 4002; i++)
    {
        modelData->nHD[i]++;
    }
    modelData->nHD[0] = 1;
    modelData->nHD[1] = 1;
    modelData->nHD[2] = 1;
    modelData->nDD[0] += 1;
    modelData->nDD[1] += 1;
    modelData->nDD[2] += 1;
    for (tmp2 = 0; tmp2 <= 1999; tmp2++)
    {
        modelData->nDD[tmp2 + 3]++;
    }
    modelData->nDH[4002]++;
    modelData->nDH[4002]++;
    modelData->nDH[0]++;
    modelData->nDH[4001]++;
    modelData->nDH[4003]++;
    for (i = 0; i <= 1999; i++)
    {
        modelData->nDH[i + 1]++;
        modelData->nDH[i + 2001]++;
        modelData->nDH[i + 4004]++;
        modelData->nDH[i + 6004]++;
        modelData->nDH[i + 8004]++;
    }
    modelData->event[2].nRHSSt = 1;
    modelData->event[0].nLHSDsc = 1;
    modelData->event[1].nLHSDsc = 1;
    modelData->event[2].nLHSDsc = 3;
    for (i = 0; i <= 1999; i++)
    {
        modelData->event[i + 3].nLHSDsc = 2;
        modelData->event[i + 2003].nLHSDsc = 3;
    }
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    cleanVector (states, 0, 2001);

    for (i = 0; i <= 1999; i++)
    {
        modelData->DS[i][states[i]++] = i;
    }
    cleanVector (states, 0, 2001);

    for (i = 0; i <= 1999; i++)
    {
        modelData->SD[i][states[i]++] = i;
    }
    cleanVector (events, 0, 4003);

    for (i = 0; i <= 1999; i++)
    {
        modelData->ZS[i + 3][events[i + 3]++] = i;
    }
    cleanVector (states, 0, 2001);

    for (i = 0; i <= 1999; i++)
    {
        modelData->SZ[i][states[i]++] = i + 3;
    }
    cleanVector (events, 0, 4003);

    for (i = 0; i <= 1999; i++)
    {
        modelData->HZ[2][events[2]++] = i + 3;
    }
    modelData->HZ[2][events[2]++] = 2;
    for (i = 3; i <= 2002; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 2003; i <= 4002; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    cleanVector (events, 0, 4003);

    for (i = 3; i <= 2002; i++)
    {
        modelData->HD[i][events[i]++] = i - 3;
    }
    for (i = 2003; i <= 4002; i++)
    {
        modelData->HD[i][events[i]++] = i - 2003;
    }
    modelData->HD[2][events[2]++] = 2000;
    modelData->HD[0][events[0]++] = 2000;
    modelData->HD[1][events[1]++] = 2000;
    cleanVector (events, 0, 4003);

    modelData->DD[0][events[0]++] = 2;
    modelData->DD[1][events[1]++] = 2;
    modelData->DD[2][events[2]++] = 2;
    for (tmp2 = 0; tmp2 <= 1999; tmp2++)
    {
        modelData->DD[tmp2 + 3][events[tmp2 + 3]++] = 2;
    }
    cleanVector (discretes, 0, 10004);

    modelData->DH[4002][discretes[4002]++] = 0;
    modelData->DH[4002][discretes[4002]++] = 1;
    modelData->DH[0][discretes[0]++] = 2;
    modelData->DH[4001][discretes[4001]++] = 2;
    modelData->DH[4003][discretes[4003]++] = 2;
    for (i = 0; i <= 1999; i++)
    {
        modelData->DH[i + 1][discretes[i + 1]++] = i + 3;
        modelData->DH[i + 2001][discretes[i + 2001]++] = i + 3;
        modelData->DH[i + 4004][discretes[i + 4004]++] = i + 2003;
        modelData->DH[i + 6004][discretes[i + 6004]++] = i + 2003;
        modelData->DH[i + 8004][discretes[i + 8004]++] = i + 2003;
    }
    cleanVector (events, 0, 4003);

    modelData->event[2].RHSSt[events[2]++] = 2000;
    cleanVector (events, 0, 4003);

    modelData->event[0].LHSDsc[events[0]++] = 4002;
    modelData->event[1].LHSDsc[events[1]++] = 4002;
    modelData->event[2].LHSDsc[events[2]++] = 0;
    modelData->event[2].LHSDsc[events[2]++] = 4001;
    modelData->event[2].LHSDsc[events[2]++] = 4003;
    for (i = 0; i <= 1999; i++)
    {
        modelData->event[i + 3].LHSDsc[events[i + 3]++] = i + 1;
        modelData->event[i + 3].LHSDsc[events[i + 3]++] = i + 2001;
        modelData->event[i + 2003].LHSDsc[events[i + 2003]++] = i + 4004;
        modelData->event[i + 2003].LHSDsc[events[i + 2003]++] = i + 6004;
        modelData->event[i + 2003].LHSDsc[events[i + 2003]++] = i + 8004;
    }
    cleanVector (events, 0, 4003);

    modelData->event[0].direction = 1;
    modelData->event[0].relation = 2;
    modelData->event[1].direction = 1;
    modelData->event[1].relation = 2;
    modelData->event[2].direction = 1;
    modelData->event[2].relation = 2;
    for (i = 0; i <= 1999; i++)
    {
        modelData->event[i + 3].direction = 0;
        modelData->event[i + 3].relation = 2;
        modelData->event[i + 2003].direction = 1;
        modelData->event[i + 2003].relation = 2;
    }
    simulator->time = QSS_Time (2001, 4003, 0, 0, ST_Binary, NULL);

    simulator->output = SD_Output ("airsconds", 2, 10004, 2001, NULL, 0, 0, CI_Step, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[2000]++;
    modelOutput->nOD[1] = 1;
    modelOutput->nDO[0]++;
    SD_allocOutputMatrix (modelOutput, 2001, 10004);
    cleanVector (states, 0, 2001);

    cleanVector (discretes, 0, 10004);

    cleanVector (outputs, 0, 2);

    sprintf (modelOutput->variable[0].name, "ierr");
    sprintf (modelOutput->variable[1].name, "ptotals");
    cleanVector (outputs, 0, 2);

    modelOutput->SO[2000][states[2000]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 2000;
    cleanVector (outputs, 0, 2);

    modelOutput->DO[0][discretes[0]++] = 1;
    modelOutput->OD[1][outputs[1]++] = 0;
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
