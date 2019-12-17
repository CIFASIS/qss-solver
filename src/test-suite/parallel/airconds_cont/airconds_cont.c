#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "airconds_cont_functions.h"
#include "pkg_math.h"
#include <common/utils.h>

#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_CAP[20000];
double __PAR_RES[20000];
double __PAR_POT[20000];
double __PAR_THA = 0;
double __PAR_pmax = 0;
double __PAR_Kp = 0;
double __PAR_Ki = 0;
double __PAR_tref = 0;
int __reverse__PAR_sections[20000];
int __PAR_sections[20000];

void
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 0;
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
        case 20000:
            dx[1] = d[(20042)] - d[(0)] / __PAR_pmax;
            dx[2] = (0.0) / 2;
            dx[3] = (0.0) / 6;
            return;
        case 20001:
            dx[1] = 0.0;
            dx[2] = (0.0) / 2;
            dx[3] = (0.0) / 6;
            return;
        default:
            j = i;
            if (j >= 0 && j <= 19999)
            {
                dx[1] = (__PAR_THA / __PAR_RES[(j)] - __PAR_POT[(j)] * d[(j + 41)] - x[(j) * 4] / __PAR_RES[(j)] + d[(j + 20064)] / __PAR_RES[(j)])
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
    if (j >= 0 && j <= 19999)
    {
        if (map[j] > NOT_ASSIGNED)
        {
            der[(j) * 4 + 1] = (__PAR_THA / __PAR_RES[(j)] - __PAR_POT[(j)] * d[(j + 41)] - x[(j) * 4] / __PAR_RES[(j)]
                    + d[(j + 20064)] / __PAR_RES[(j)]) / __PAR_CAP[(j)];
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
        case 40:
            zc[0] = t - (1000.0);
            zc[1] = 1.0;
            zc[2] = (0.0) / 2;
            return;
        case 41:
            zc[0] = t - (2000.0);
            zc[1] = 1.0;
            zc[2] = (0.0) / 2;
            return;
        case 42:
            zc[0] = t - (d[(20043)]);
            zc[1] = 1.0;
            zc[2] = (0.0) / 2;
            return;
        default:
            if (i >= 0 && i <= 19)
            {
                zc[0] = t - (d[(i + 20044)]);
                zc[1] = 1.0;
                zc[2] = (0.0) / 2;
            }
            if (i >= 20 && i <= 39)
            {
                zc[0] = d[(i + 40044)] - (5.000000000000000000000000e-01);
                zc[1] = 0.0;
                zc[2] = (0.0) / 2;
            }
            if (i >= 43 && i <= 20042)
            {
                zc[0] = x[(i - 43) * 4] - __PAR_tref - d[(20041)] + d[(i - 2)] - 5.000000000000000000000000e-01 - (0.0);
                zc[1] = x[(i - 43) * 4 + 1];
                zc[2] = (x[(i - 43) * 4 + 2] * 2) / 2;
            }
            if (i >= 20043 && i <= 40042)
            {
                zc[0] = t - (d[(i + 20041)]);
                zc[1] = 1.0;
                zc[2] = (0.0) / 2;
            }
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 40:
            d[(20042)] = 4.000000000000000222044605e-01;
            return;
        case 41:
            d[(20042)] = 5.000000000000000000000000e-01;
            return;
        case 42:
            d[(20043)] = d[(20043)] + 1.0;
            d[(0)] = x[80004];
            d[(20041)] = __PAR_Kp * (d[(0)] / __PAR_pmax - d[(20042)]) - __PAR_Ki * x[80000];
            return;
        default:
            if (i >= 0 && i <= 19)
            {
                d[(i + 20044)] = d[(i + 20044)] + 1.0;
                d[(i + 40064)] = 1.0;
                d[(i + 21)] = d[(i + 1)];
                d[(i + 1)] = 0.0;
            }
            if (i >= 20 && i <= 39)
            {
                x[80004] = x[80004] + d[(i + 1)];
                d[(i + 40044)] = 0.0;
            }
            if (i >= 43 && i <= 20042)
            {
                d[(i - 2)] = 1.0;
                d[(__PAR_sections[i - 43])] = d[(__PAR_sections[i - 43])] + __PAR_POT[(i - 43)];
            }
            if (i >= 20043 && i <= 40042)
            {
                d[(i + 20041)] = d[(i + 20041)] + 1.0;
                d[(i + 40041)] = d[(i + 40041)] + 1.0;
                d[(i + 21)] = 2.0 * pow (sin ((i - 20042) * d[(i + 40041)]), 2.0);
            }
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 43 && i <= 20042)
    {
        d[(i - 2)] = 0.0;
        d[(__PAR_sections[i - 43])] = d[(__PAR_sections[i - 43])] - __PAR_POT[(i - 43)];
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = x[80004];
            return;
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *discretes = (int*) malloc (80084 * sizeof(int));
    int *events = (int*) malloc (40043 * sizeof(int));
    int *outputs = (int*) malloc (1 * sizeof(int));
    int *states = (int*) malloc (20002 * sizeof(int));
    int i10;
    int i2;
    int i;
    int j = 0;
    simulator->data = QSS_Data (20002, 80084, 40043, 0, 0, "airconds_cont");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_THA = 32.0;
    __PAR_pmax = 0.0;
    __PAR_Kp = 1.0;
    __PAR_Ki = 1.0;
    __PAR_tref = 20.0;
    modelData->d[(20043)] = 1.0;
    for (i = 0; i <= 19; i++)
    {
        modelData->d[(i + 20044)] = 1.0;
    }
    modelData->d[(20042)] = 5.000000000000000000000000e-01;
    // Initialize model code.
    for (i2 = 0; i2 <= 19999; i2++)
    {
        modelData->x[(i2) * 4] = __math__rand (4.0) + 18.0;
        __PAR_CAP[(i2)] = __math__rand (100.0) + 550.0;
        __PAR_RES[(i2)] = __math__rand (4.000000000000000222044605e-01) + 1.800000000000000044408921e+00;
        __PAR_POT[(i2)] = __math__rand (2.000000000000000111022302e-01) + 13.0;
        __PAR_pmax = __PAR_pmax + __PAR_POT[(i2)];
        modelData->d[(i2 + 20064)] = 2.0 * pow (sin ((i2 + 1)), 2.0);
        modelData->d[(i2 + 40084)] = __math__rand (2.0);
        __PAR_sections[(i2)] = __getSection ((i2 + 1));
        __reverse__PAR_sections[__PAR_sections[(i2)] - 1] = i2 + 1;
    }
    for (i10 = 0; i10 <= 19999; i10++)
    {
        if (modelData->x[(i10) * 4] - __PAR_tref - 5.000000000000000000000000e-01 > 0.0)
        {
            modelData->d[(i10 + 41)] = 1.0;
            modelData->x[80004] = modelData->x[80004] + __PAR_POT[(i10)];
        }
    }
    for (i = 0; i <= 19999; i++)
    {
        modelData->nDS[i] = 1;
    }
    for (i = 0; i <= 19999; i++)
    {
        modelData->nSD[i]++;
    }
    for (i = 0; i <= 19999; i++)
    {
        modelData->nZS[i + 43] = 1;
    }
    for (i = 0; i <= 19999; i++)
    {
        modelData->nSZ[i]++;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 20; i <= 39; i++)
    {
        modelData->nHZ[i]++;
    }
    modelData->nHZ[42] += 20001;
    for (i = 43; i <= 20042; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 43; i <= 20042; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 20043; i <= 40042; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 43; i <= 20042; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 43; i <= 20042; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 20043; i <= 40042; i++)
    {
        modelData->nHD[i]++;
    }
    modelData->nHD[40] = 1;
    modelData->nHD[41] = 1;
    modelData->nHD[42] = 1;
    for (i = 0; i <= 19; i++)
    {
        modelData->nSH[20001]++;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->nDD[i]++;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->nDD[i]++;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->nDD[i]++;
    }
    for (i = 0; i <= 19; i++)
    {
    }
    modelData->nDD[40] += 1;
    modelData->nDD[41] += 1;
    modelData->nDD[42] += 1;
    for (i = 43; i <= 62; i++)
    {
        modelData->nDD[i]++;
    }
    for (i = 63; i <= 82; i++)
    {
        modelData->nDD[i]++;
    }
    for (i = 0; i <= 19999; i++)
    {
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->nDH[i + 1]++;
        modelData->nDH[i + 21]++;
        modelData->nDH[i + 20044]++;
        modelData->nDH[i + 40064]++;
        modelData->nDH[i + 40064]++;
    }
    modelData->nDH[20042]++;
    modelData->nDH[20042]++;
    modelData->nDH[0]++;
    modelData->nDH[20041]++;
    modelData->nDH[20043]++;
    for (i = 0; i <= 19999; i++)
    {
        modelData->nDH[__PAR_sections[i]]++;
        modelData->nDH[i + 41]++;
        modelData->nDH[i + 20064]++;
        modelData->nDH[i + 40084]++;
        modelData->nDH[i + 60084]++;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->event[i + 20].nLHSSt = 1;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->event[i + 20].nRHSSt = 1;
    }
    modelData->event[42].nRHSSt = 2;
    for (i = 0; i <= 19; i++)
    {
        modelData->event[i].nLHSDsc = 4;
        modelData->event[i + 20].nLHSDsc = 1;
    }
    modelData->event[40].nLHSDsc = 1;
    modelData->event[41].nLHSDsc = 1;
    modelData->event[42].nLHSDsc = 3;
    for (i = 0; i <= 19999; i++)
    {
        modelData->event[i + 43].nLHSDsc = 2;
        modelData->event[i + 20043].nLHSDsc = 3;
    }
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    cleanVector (states, 0, 20002);

    for (i = 0; i <= 19999; i++)
    {
        modelData->DS[i][states[i]++] = i;
    }
    cleanVector (states, 0, 20002);

    for (i = 0; i <= 19999; i++)
    {
        modelData->SD[i][states[i]++] = i;
    }
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19999; i++)
    {
        modelData->ZS[i + 43][events[i + 43]++] = i;
    }
    cleanVector (states, 0, 20002);

    for (i = 0; i <= 19999; i++)
    {
        modelData->SZ[i][states[i]++] = i + 43;
    }
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->HZ[i][events[i]++] = i + 20;
    }
    for (i = 20; i <= 39; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 0; i <= 19999; i++)
    {
        modelData->HZ[42][events[42]++] = i + 43;
    }
    modelData->HZ[42][events[42]++] = 42;
    for (i = 43; i <= 20042; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 43; i <= 20042; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 20043; i <= 40042; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    cleanVector (events, 0, 40043);

    for (i = 43; i <= 20042; i++)
    {
        modelData->HD[i][events[i]++] = i - 43;
    }
    for (i = 43; i <= 20042; i++)
    {
        modelData->HD[i][events[i]++] = i - 43;
    }
    for (i = 20043; i <= 40042; i++)
    {
        modelData->HD[i][events[i]++] = i - 20043;
    }
    modelData->HD[42][events[42]++] = 20000;
    modelData->HD[40][events[40]++] = 20000;
    modelData->HD[41][events[41]++] = 20000;
    cleanVector (states, 0, 20002);

    for (i = 0; i <= 19; i++)
    {
        modelData->SH[20001][states[20001]++] = i + 20;
    }
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19; i++)
    {
        modelData->DD[i][events[i]++] = i;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->DD[i][events[i]++] = i + 43;
    }
    for (i = 0; i <= 19; i++)
    {
        modelData->DD[i][events[i]++] = i + 20;
    }
    for (i = 0; i <= 19; i++)
    {
    }
    modelData->DD[40][events[40]++] = 42;
    modelData->DD[41][events[41]++] = 42;
    modelData->DD[42][events[42]++] = 42;
    for (i = 43; i <= 62; i++)
    {
        modelData->DD[i][events[i]++] = __PAR_sections[i - 43] - 1;
    }
    for (i = 63; i <= 82; i++)
    {
        modelData->DD[i][events[i]++] = __PAR_sections[i - 63] + 19;
    }
    for (i = 0; i <= 19999; i++)
    {
    }
    cleanVector (discretes, 0, 80084);

    for (i = 0; i <= 19; i++)
    {
        modelData->DH[i + 1][discretes[i + 1]++] = i;
        modelData->DH[i + 21][discretes[i + 21]++] = i;
        modelData->DH[i + 20044][discretes[i + 20044]++] = i;
        modelData->DH[i + 40064][discretes[i + 40064]++] = i;
        modelData->DH[i + 40064][discretes[i + 40064]++] = i + 20;
    }
    modelData->DH[20042][discretes[20042]++] = 40;
    modelData->DH[20042][discretes[20042]++] = 41;
    modelData->DH[0][discretes[0]++] = 42;
    modelData->DH[20041][discretes[20041]++] = 42;
    modelData->DH[20043][discretes[20043]++] = 42;
    for (i = 0; i <= 19999; i++)
    {
        modelData->DH[__PAR_sections[i]][discretes[__PAR_sections[i]]++] = i + 43;
        modelData->DH[i + 41][discretes[i + 41]++] = i + 43;
        modelData->DH[i + 20064][discretes[i + 20064]++] = i + 20043;
        modelData->DH[i + 40084][discretes[i + 40084]++] = i + 20043;
        modelData->DH[i + 60084][discretes[i + 60084]++] = i + 20043;
    }
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19; i++)
    {
        modelData->event[i + 20].LHSSt[events[i + 20]++] = 20001;
    }
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19; i++)
    {
        modelData->event[i + 20].RHSSt[events[i + 20]++] = 20001;
    }
    modelData->event[42].RHSSt[events[42]++] = 20000;
    modelData->event[42].RHSSt[events[42]++] = 20001;
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19; i++)
    {
        modelData->event[i].LHSDsc[events[i]++] = i + 1;
        modelData->event[i].LHSDsc[events[i]++] = i + 21;
        modelData->event[i].LHSDsc[events[i]++] = i + 20044;
        modelData->event[i].LHSDsc[events[i]++] = i + 40064;
        modelData->event[i + 20].LHSDsc[events[i + 20]++] = i + 40064;
    }
    modelData->event[40].LHSDsc[events[40]++] = 20042;
    modelData->event[41].LHSDsc[events[41]++] = 20042;
    modelData->event[42].LHSDsc[events[42]++] = 0;
    modelData->event[42].LHSDsc[events[42]++] = 20041;
    modelData->event[42].LHSDsc[events[42]++] = 20043;
    for (i = 0; i <= 19999; i++)
    {
        modelData->event[i + 43].LHSDsc[events[i + 43]++] = __PAR_sections[i];
        modelData->event[i + 43].LHSDsc[events[i + 43]++] = i + 41;
        modelData->event[i + 20043].LHSDsc[events[i + 20043]++] = i + 20064;
        modelData->event[i + 20043].LHSDsc[events[i + 20043]++] = i + 40084;
        modelData->event[i + 20043].LHSDsc[events[i + 20043]++] = i + 60084;
    }
    cleanVector (events, 0, 40043);

    for (i = 0; i <= 19; i++)
    {
        modelData->event[i].direction = 1;
        modelData->event[i].relation = 2;
        modelData->event[i + 20].direction = 1;
        modelData->event[i + 20].relation = 2;
    }
    modelData->event[40].direction = 1;
    modelData->event[40].relation = 2;
    modelData->event[41].direction = 1;
    modelData->event[41].relation = 2;
    modelData->event[42].direction = 1;
    modelData->event[42].relation = 2;
    for (i = 0; i <= 19999; i++)
    {
        modelData->event[i + 43].direction = 0;
        modelData->event[i + 43].relation = 2;
        modelData->event[i + 20043].direction = 1;
        modelData->event[i + 20043].relation = 2;
    }
    simulator->time = QSS_Time (20002, 40043, 0, 0, ST_Binary, NULL);

    double period[1];
    period[0] = 0.6;
    simulator->output = SD_Output ("airconds_cont", 1, 80084, 20002, period, 1, 0, CI_Sampled, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[20001]++;
    SD_allocOutputMatrix (modelOutput, 20002, 80084);
    cleanVector (states, 0, 20002);

    cleanVector (outputs, 0, 1);

    sprintf (modelOutput->variable[0].name, "ptotal");
    cleanVector (outputs, 0, 1);

    modelOutput->SO[20001][states[20001]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 20001;
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
