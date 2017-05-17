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
    double tmp1[3];
    int j = 0;
    int j2 = 0;
    int j3;
    switch (i)
    {
        case 16:
            for (j2 = 0; j2 <= 1; j2++)
            {
                alg[(j2 + 8) * 3] = (x[(j2 + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j2 + 18)]);
            }
            alg[36] = -(__PAR_VlineaRef - x[48]) * 1.000000000000000055511151e-01 - x[51];
            for (j3 = 0; j3 < 3; j3++)
            {
                tmp1[j3] = 0;
            }
            for (j3 = 0; j3 <= 1; j3++)
            {
                tmp1[0] += alg[(j3 + 8) * 3 + 0];
                tmp1[1] += alg[(j3 + 8) * 3 + 1];
                tmp1[2] += alg[(j3 + 8) * 3 + 2];
            }
            dx[1] = (tmp1[0] - __PAR_iLoad - alg[36]) / __PAR_Cbatery;
            return;
        case 17:
            dx[1] = __PAR_VlineaRef - x[48];
            return;
        default:
            j = i;
            if (j >= 0 && j <= 1)
            {
                alg[(j) * 3] = __ipvsolver (x[(j) * 3], 36, 36, d[(10)], d[(j + 14)], 1.000000000000000081803054e-05, 6.0, d[(j + 20)]);
                dx[1] = (alg[(j) * 3] - x[(j + 4) * 3]) / __PAR_Cpvin;
            }
            j = i - 2;
            if (j >= 0 && j <= 1)
            {
                alg[(j + 4) * 3] = ((d[(j + 4)]) * x[(j + 4) * 3] - x[(j + 2) * 3]) / ((d[(j + 2)]) + (d[(j + 4)]));
                dx[1] = (alg[(j + 4) * 3] - x[(j + 2) * 3] / __PAR_R - (x[(j + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j + 18)])) / __PAR_Cboost;
            }
            j = i - 4;
            if (j >= 0 && j <= 1)
            {
                alg[(j + 2) * 3] = x[(j) * 3];
                alg[(j + 4) * 3] = ((d[(j + 4)]) * x[(j + 4) * 3] - x[(j + 2) * 3]) / ((d[(j + 2)]) + (d[(j + 4)]));
                dx[1] = (alg[(j + 2) * 3] - d[(j + 4)] * (x[(j + 4) * 3] - alg[(j + 4) * 3])) / __PAR_Lboost;
            }
            j = i - 6;
            if (j >= 0 && j <= 1)
            {
                dx[1] = d[(j + 30)] - x[(j) * 3];
            }
            j = i - 8;
            if (j >= 0 && j <= 1)
            {
                alg[(j) * 3] = __ipvsolver (x[(j) * 3], 36, 36, d[(10)], d[(j + 14)], 1.000000000000000081803054e-05, 6.0, d[(j + 20)]);
                alg[(j + 10) * 3] = x[(j) * 3] * alg[(j) * 3];
                dx[1] = alg[(j + 10) * 3] / __PAR_TContMaxPot;
            }
            j = i - 10;
            if (j >= 0 && j <= 1)
            {
                dx[1] = x[(j) * 3] / __PAR_Tboost[(j)];
            }
            j = i - 12;
            if (j >= 0 && j <= 1)
            {
                dx[1] = (x[(j) * 3] - x[(j + 12) * 3]) * 100.0;
            }
            j = i - 14;
            if (j >= 0 && j <= 1)
            {
                alg[(j + 8) * 3] = (x[(j + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j + 18)]);
                dx[1] = (alg[(j + 8) * 3] - x[(j + 14) * 3]);
            }
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    double tmp1[3];
    int j = 0;
    int j2 = 0;
    int j3;
    int j4 = 0;
    int j5 = 0;
    int j6 = 0;
    int j7 = 0;
    switch (i)
    {
        case 16:
            for (j = 0; j <= 1; j++)
            {
                j5 = j;
                if (j5 >= 0 && j5 <= 1)
                {
                    alg[(j + 4) * 3] = ((d[(j5 + 4)]) * x[(j5 + 4) * 3] - x[(j5 + 2) * 3]) / ((d[(j5 + 2)]) + (d[(j5 + 4)]));
                }
            }
            for (j6 = 0; j6 <= 1; j6++)
            {
                alg[(j6 + 8) * 3] = (x[(j6 + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j6 + 18)]);
            }
            alg[36] = -(__PAR_VlineaRef - x[48]) * 1.000000000000000055511151e-01 - x[51];
            for (j = 0; j <= 1; j++)
            {
                der[(j + 2) * 3 + 1] = (alg[(j + 4) * 3] - x[(j + 2) * 3] / __PAR_R - (x[(j + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j + 18)]))
                        / __PAR_Cboost;
            }
            for (j = 0; j <= 1; j++)
            {
                der[(j + 14) * 3 + 1] = (alg[(j + 8) * 3] - x[(j + 14) * 3]);
            }
            for (j3 = 0; j3 < 3; j3++)
            {
                tmp1[j3] = 0;
            }
            for (j3 = 0; j3 <= 1; j3++)
            {
                tmp1[0] += alg[(j3 + 8) * 3 + 0];
                tmp1[1] += alg[(j3 + 8) * 3 + 1];
                tmp1[2] += alg[(j3 + 8) * 3 + 2];
            }
            der[48 + 1] = (tmp1[0] - __PAR_iLoad - alg[36]) / __PAR_Cbatery;
            der[51 + 1] = __PAR_VlineaRef - x[48];
            break;
        case 17:
            for (j7 = 0; j7 <= 1; j7++)
            {
                alg[(j7 + 8) * 3] = (x[(j7 + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j7 + 18)]);
            }
            alg[36] = -(__PAR_VlineaRef - x[48]) * 1.000000000000000055511151e-01 - x[51];
            for (j3 = 0; j3 < 3; j3++)
            {
                tmp1[j3] = 0;
            }
            for (j3 = 0; j3 <= 1; j3++)
            {
                tmp1[0] += alg[(j3 + 8) * 3 + 0];
                tmp1[1] += alg[(j3 + 8) * 3 + 1];
                tmp1[2] += alg[(j3 + 8) * 3 + 2];
            }
            der[48 + 1] = (tmp1[0] - __PAR_iLoad - alg[36]) / __PAR_Cbatery;
            return;
    }
    j = i;
    if (j >= 0 && j <= 1)
    {
        alg[(j) * 3] = __ipvsolver (x[(j) * 3], 36, 36, d[(10)], d[(j + 14)], 1.000000000000000081803054e-05, 6.0, d[(j + 20)]);
        alg[(j + 2) * 3] = x[(j) * 3];
        alg[(j + 4) * 3] = ((d[(j + 4)]) * x[(j + 4) * 3] - x[(j + 2) * 3]) / ((d[(j + 2)]) + (d[(j + 4)]));
        alg[(j + 10) * 3] = x[(j) * 3] * alg[(j) * 3];
        der[(j) * 3 + 1] = (alg[(j) * 3] - x[(j + 4) * 3]) / __PAR_Cpvin;
        der[(j + 4) * 3 + 1] = (alg[(j + 2) * 3] - d[(j + 4)] * (x[(j + 4) * 3] - alg[(j + 4) * 3])) / __PAR_Lboost;
        der[(j + 6) * 3 + 1] = d[(j + 30)] - x[(j) * 3];
        der[(j + 8) * 3 + 1] = alg[(j + 10) * 3] / __PAR_TContMaxPot;
        der[(j + 10) * 3 + 1] = x[(j) * 3] / __PAR_Tboost[(j)];
        der[(j + 12) * 3 + 1] = (x[(j) * 3] - x[(j + 12) * 3]) * 100.0;
    }
    j = i - 2;
    if (j >= 0 && j <= 1)
    {
        alg[(j + 2) * 3] = x[(j) * 3];
        alg[(j + 4) * 3] = ((d[(j + 4)]) * x[(j + 4) * 3] - x[(j + 2) * 3]) / ((d[(j + 2)]) + (d[(j + 4)]));
        for (j4 = 0; j4 <= 1; j4++)
        {
            alg[(j4 + 8) * 3] = (x[(j4 + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j4 + 18)]);
        }
        alg[36] = -(__PAR_VlineaRef - x[48]) * 1.000000000000000055511151e-01 - x[51];
        der[(j + 2) * 3 + 1] = (alg[(j + 4) * 3] - x[(j + 2) * 3] / __PAR_R - (x[(j + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j + 18)])) / __PAR_Cboost;
        der[(j + 4) * 3 + 1] = (alg[(j + 2) * 3] - d[(j + 4)] * (x[(j + 4) * 3] - alg[(j + 4) * 3])) / __PAR_Lboost;
        der[(j + 14) * 3 + 1] = (alg[(j + 8) * 3] - x[(j + 14) * 3]);
        for (j3 = 0; j3 < 3; j3++)
        {
            tmp1[j3] = 0;
        }
        for (j3 = 0; j3 <= 1; j3++)
        {
            tmp1[0] += alg[(j3 + 8) * 3 + 0];
            tmp1[1] += alg[(j3 + 8) * 3 + 1];
            tmp1[2] += alg[(j3 + 8) * 3 + 2];
        }
        der[48 + 1] = (tmp1[0] - __PAR_iLoad - alg[36]) / __PAR_Cbatery;
    }
    j = i - 4;
    if (j >= 0 && j <= 1)
    {
        alg[(j) * 3] = __ipvsolver (x[(j) * 3], 36, 36, d[(10)], d[(j + 14)], 1.000000000000000081803054e-05, 6.0, d[(j + 20)]);
        alg[(j + 2) * 3] = x[(j) * 3];
        alg[(j + 4) * 3] = ((d[(j + 4)]) * x[(j + 4) * 3] - x[(j + 2) * 3]) / ((d[(j + 2)]) + (d[(j + 4)]));
        der[(j) * 3 + 1] = (alg[(j) * 3] - x[(j + 4) * 3]) / __PAR_Cpvin;
        der[(j + 2) * 3 + 1] = (alg[(j + 4) * 3] - x[(j + 2) * 3] / __PAR_R - (x[(j + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j + 18)])) / __PAR_Cboost;
        der[(j + 4) * 3 + 1] = (alg[(j + 2) * 3] - d[(j + 4)] * (x[(j + 4) * 3] - alg[(j + 4) * 3])) / __PAR_Lboost;
    }
    j = i - 12;
    if (j >= 0 && j <= 1)
    {
        der[(j + 12) * 3 + 1] = (x[(j) * 3] - x[(j + 12) * 3]) * 100.0;
    }
    j = i - 14;
    if (j >= 0 && j <= 1)
    {
        alg[(j + 8) * 3] = (x[(j + 2) * 3] - x[48]) / (__PAR_Rshunt + d[(j + 18)]);
        der[(j + 14) * 3 + 1] = (alg[(j + 8) * 3] - x[(j + 14) * 3]);
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    if (i >= 0 && i <= 1)
    {
        zc[0] = t - (d[(i + 32)]);
    }
    if (i >= 2 && i <= 3)
    {
        zc[0] = t - (d[(i + 4)]);
    }
    if (i >= 4 && i <= 5)
    {
        zc[0] = t - d[(i + 4)] - d[(i - 4)] * __PAR_Tboost[(i - 4)] - (0.0);
    }
    if (i >= 6 && i <= 7)
    {
        alg[(i - 6) * 3] = __ipvsolver (x[(i - 6) * 3], 36, 36, d[(10)], d[(i + 8)], 1.000000000000000081803054e-05, 6.0, d[(i + 14)]);
        zc[0] = 36 * d[(i + 5)] - alg[(i - 6) * 3] - (1.000000000000000020816682e-03);
    }
    if (i >= 8 && i <= 9)
    {
        alg[(i - 8) * 3] = __ipvsolver (x[(i - 8) * 3], 36, 36, d[(10)], d[(i + 6)], 1.000000000000000081803054e-05, 6.0, d[(i + 12)]);
        zc[0] = 36 * d[(i + 3)] - alg[(i - 8) * 3] - (1.000000000000000020816682e-03);
    }
    if (i >= 10 && i <= 11)
    {
        alg[(i - 6) * 3] = ((d[(i - 6)]) * x[(i - 6) * 3] - x[(i - 8) * 3]) / ((d[(i - 8)]) + (d[(i - 6)]));
        zc[0] = alg[(i - 6) * 3] - (0.0);
    }
    if (i >= 12 && i <= 13)
    {
        alg[(i - 8) * 3] = ((d[(i - 8)]) * x[(i - 8) * 3] - x[(i - 10) * 3]) / ((d[(i - 10)]) + (d[(i - 8)]));
        alg[(i - 6) * 3] = alg[(i - 8) * 3] * d[(i - 10)];
        zc[0] = alg[(i - 6) * 3] - (0.0);
    }
    if (i >= 14 && i <= 15)
    {
        zc[0] = x[(i - 12) * 3] - x[48] - (1.000000000000000055511151e-01);
    }
    if (i >= 16 && i <= 17)
    {
        zc[0] = (x[(i - 14) * 3] - x[48]) / d[(i + 2)] - (1.000000000000000055511151e-01);
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 0 && i <= 1)
    {
        d[(i + 32)] = d[(i + 32)] + __PAR_TContMaxPot;
        d[(i + 22)] = d[(i + 24)];
        d[(i + 24)] = x[(i + 8) * 3];
        d[(i + 26)] = d[(i + 24)] - d[(i + 22)];
        x[(i + 8) * 3] = 0.0;
        if ((d[(i + 26)] > 0.0 && d[(i + 30)] < 10.0))
        {
            d[(i + 30)] = d[(i + 30)] + 5.000000000000000000000000e-01;
        }
        else
        {
            if ((d[(i + 30)] > 1.0))
            {
                d[(i + 30)] = d[(i + 30)] - 5.000000000000000000000000e-01;
            }
        }
    }
    if (i >= 2 && i <= 3)
    {
        d[(i + 6)] = d[(i + 4)];
        d[(i + 4)] = d[(i + 4)] + __PAR_Tboost[(i - 2)];
        d[(i + 2)] = __PAR_ROn;
        alg[(i - 2) * 3] = __ipvsolver (x[(i - 2) * 3], 36, 36, d[(10)], d[(i + 12)], 1.000000000000000081803054e-05, 6.0, d[(i + 18)]);
        d[(i + 18)] = alg[(i - 2) * 3];
        d[(i + 26)] = x[(i + 8) * 3];
        x[(i + 8) * 3] = 0.0;
        d[(i - 2)] = -1.000000000000000020816682e-02 * (d[(i + 28)] - x[(i - 2) * 3]) - x[(i + 4) * 3] * 6.999999999999999555910790e-01;
        if ((d[(i - 2)] > 9.799999999999999822364316e-01))
        {
            d[(i - 2)] = 9.799999999999999822364316e-01;
        }
        if ((d[(i - 2)] < 2.000000000000000041633363e-02))
        {
            d[(i - 2)] = 2.000000000000000041633363e-02;
        }
    }
    if (i >= 4 && i <= 5)
    {
        d[(i)] = __PAR_ROff;
    }
    if (i >= 8 && i <= 9)
    {
        d[(i + 8)] = 1.0;
    }
    if (i >= 12 && i <= 13)
    {
        d[(i - 10)] = __PAR_ROn;
    }
    if (i >= 14 && i <= 15)
    {
        d[(i + 4)] = __PAR_ROn;
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 6 && i <= 7)
    {
        d[(i + 10)] = 0.0;
    }
    if (i >= 10 && i <= 11)
    {
        d[(i - 8)] = __PAR_ROff;
    }
    if (i >= 16 && i <= 17)
    {
        d[(i + 2)] = __PAR_ROff;
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    int j = 0;
    j = i;
    if (j >= 0 && j <= 1)
    {
        out[0] = x[(j) * 3];
    }
    j = i - 2;
    if (j >= 0 && j <= 1)
    {
        out[0] = d[(j + 30)];
    }
    j = i - 4;
    if (j >= 0 && j <= 1)
    {
        alg[(j) * 3] = __ipvsolver (x[(j) * 3], 36, 36, d[(10)], d[(j + 14)], 1.000000000000000081803054e-05, 6.0, d[(j + 20)]);
        alg[(j + 10) * 3] = x[(j) * 3] * alg[(j) * 3];
        out[0] = alg[(j + 10) * 3];
    }
    j = i - 6;
    if (j >= 0 && j <= 1)
    {
        alg[(j) * 3] = __ipvsolver (x[(j) * 3], 36, 36, d[(10)], d[(j + 14)], 1.000000000000000081803054e-05, 6.0, d[(j + 20)]);
        out[0] = alg[(j) * 3];
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *discretes = (int*) malloc (34 * sizeof(int));
    int *events = (int*) malloc (18 * sizeof(int));
    int *outputs = (int*) malloc (8 * sizeof(int));
    int *states = (int*) malloc (18 * sizeof(int));
    int i10;
    int i8;
    int i;
    int j = 0;
    simulator->data = QSS_Data (18, 34, 18, 0, 13, "sisthibgen");
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
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 24)] = 0.0;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 16)] = 1.0;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 20)] = 0.0;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 22)] = 0.0;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 2)] = 1.000000000000000000000000e+05;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 18)] = 1.000000000000000000000000e+05;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 4)] = 1.000000000000000000000000e+05;
    }
    modelData->x[48] = 20.0;
    for (i = 0; i <= 1; i++)
    {
        modelData->x[i * 3] = 2.0;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->d[(i + 30)] = 3.0;
    }
    // Initialize model code.
    modelData->d[(10)] = 273.0 + 5.000000000000000000000000e+01;
    for (i8 = 0; i8 <= 1; i8++)
    {
        __PAR_TContMaxPot = 1.000000000000000055511151e-01;
        modelData->d[(i8 + 32)] = 0.0;
        modelData->d[(i8 + 6)] = 0.000000000000000000000000e+00;
        modelData->d[(i8 + 4)] = 1.000000000000000000000000e+05;
        modelData->d[(i8 + 2)] = 1.000000000000000000000000e+05;
        __PAR_Tboost[(i8)] = 1.000000000000000047921736e-04;
        modelData->d[(i8)] = 9.000000000000000222044605e-01;
        modelData->d[(i8 + 14)] = 8.0;
        modelData->d[(i8 + 18)] = 1.000000000000000000000000e+05;
    }
    modelData->d[(0)] = 6.999999999999999555910790e-01;
    modelData->d[(1)] = 2.999999999999999888977698e-01;
    modelData->d[(30)] = 1.0;
    modelData->d[(31)] = 2.0;
    __PAR_Rspv = 1.000000000000000000000000e+00;
    modelData->d[(13)] = __PAR_Ior * pow ((modelData->d[(10)] / __PAR_Tref), 3.0)
            * exp (__PAR_q * __PAR_Eg * (1.0 / __PAR_Tref - 1.0 / modelData->d[(10)]) / __PAR_K / __PAR_Ac);
    for (i10 = 0; i10 <= 1; i10++)
    {
        modelData->d[(i10 + 11)] = (__PAR_Isc + __PAR_K1 * (modelData->d[(10)] - __PAR_Tref)) * modelData->d[(i10 + 14)] / 100.0;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i] = 1;
        modelData->nDS[i + 2] = 2;
        modelData->nDS[i + 4] = 1;
        modelData->nDS[i + 6] = 1;
        modelData->nDS[i + 10] = 1;
        modelData->nDS[i + 12] = 2;
        modelData->nDS[i + 14] = 1;
    }
    modelData->nDS[17] = 1;
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i + 2]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i + 8]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i + 14]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[i + 14]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nDS[16]++;
    }
    modelData->nDS[16]++;
    modelData->nDS[16]++;
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i + 4]++;
        modelData->nSD[i + 2]++;
        modelData->nSD[16]++;
        modelData->nSD[i + 4]++;
        modelData->nSD[i]++;
        modelData->nSD[i]++;
        modelData->nSD[i]++;
        modelData->nSD[i + 12]++;
        modelData->nSD[i + 14]++;
    }
    modelData->nSD[16]++;
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i + 2]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i + 2]++;
    }
    modelData->nSD[16] += 2;
    for (i = 0; i <= 1; i++)
    {
        modelData->nSD[i + 2]++;
    }
    modelData->nSD[16]++;
    modelData->nSD[17]++;
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 14] = 2;
        modelData->nZS[i + 16] = 2;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 6]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 8]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 10]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 10]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 12]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nZS[i + 12]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i + 2]++;
        modelData->nSZ[16]++;
        modelData->nSZ[i + 2]++;
        modelData->nSZ[16]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i + 2]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i + 2]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nSZ[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 14; i <= 15; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 16; i <= 17; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 14; i <= 15; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 16; i <= 17; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 14; i <= 15; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 16; i <= 17; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].nLHSSt = 1;
        modelData->event[i + 2].nLHSSt = 1;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].nRHSSt = 1;
        modelData->event[i + 2].nRHSSt = 3;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].nLHSDsc = 5;
        modelData->event[i + 2].nLHSDsc = 6;
        modelData->event[i + 4].nLHSDsc = 1;
        modelData->event[i + 6].nLHSDsc = 1;
        modelData->event[i + 8].nLHSDsc = 1;
        modelData->event[i + 10].nLHSDsc = 1;
        modelData->event[i + 12].nLHSDsc = 1;
        modelData->event[i + 14].nLHSDsc = 1;
        modelData->event[i + 16].nLHSDsc = 1;
    }
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    cleanVector (states, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i][states[i]++] = i + 4;
        modelData->DS[i + 2][states[i + 2]++] = i + 2;
        modelData->DS[i + 2][states[i + 2]++] = 16;
        modelData->DS[i + 4][states[i + 4]++] = i + 4;
        modelData->DS[i + 6][states[i + 6]++] = i;
        modelData->DS[i + 10][states[i + 10]++] = i;
        modelData->DS[i + 12][states[i + 12]++] = i;
        modelData->DS[i + 12][states[i + 12]++] = i + 12;
        modelData->DS[i + 14][states[i + 14]++] = i + 14;
    }
    modelData->DS[17][states[17]++] = 16;
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i][states[i]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i + 2][states[i + 2]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i + 4][states[i + 4]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i + 4][states[i + 4]++] = i + 2;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i + 8][states[i + 8]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i + 14][states[i + 14]++] = i + 2;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[i + 14][states[i + 14]++] = 16;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->DS[16][states[16]++] = i + 2;
    }
    modelData->DS[16][states[16]++] = 16;
    modelData->DS[16][states[16]++] = 17;
    cleanVector (states, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i + 4][states[i + 4]++] = i;
        modelData->SD[i + 2][states[i + 2]++] = i + 2;
        modelData->SD[16][states[16]++] = i + 2;
        modelData->SD[i + 4][states[i + 4]++] = i + 4;
        modelData->SD[i][states[i]++] = i + 6;
        modelData->SD[i][states[i]++] = i + 10;
        modelData->SD[i][states[i]++] = i + 12;
        modelData->SD[i + 12][states[i + 12]++] = i + 12;
        modelData->SD[i + 14][states[i + 14]++] = i + 14;
    }
    modelData->SD[16][states[16]++] = 17;
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i][states[i]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i + 4][states[i + 4]++] = i + 2;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i][states[i]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i + 2][states[i + 2]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i][states[i]++] = i + 8;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i + 2][states[i + 2]++] = i + 14;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[16][states[16]++] = i + 14;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SD[i + 2][states[i + 2]++] = 16;
    }
    modelData->SD[16][states[16]++] = 16;
    modelData->SD[17][states[17]++] = 16;
    cleanVector (events, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 14][events[i + 14]++] = i + 2;
        modelData->ZS[i + 14][events[i + 14]++] = 16;
        modelData->ZS[i + 16][events[i + 16]++] = i + 2;
        modelData->ZS[i + 16][events[i + 16]++] = 16;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 6][events[i + 6]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 8][events[i + 8]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 10][events[i + 10]++] = i + 2;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 10][events[i + 10]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 12][events[i + 12]++] = i + 2;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->ZS[i + 12][events[i + 12]++] = i + 4;
    }
    cleanVector (states, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i + 2][states[i + 2]++] = i + 14;
        modelData->SZ[16][states[16]++] = i + 14;
        modelData->SZ[i + 2][states[i + 2]++] = i + 16;
        modelData->SZ[16][states[16]++] = i + 16;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i][states[i]++] = i + 6;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i][states[i]++] = i + 8;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i + 2][states[i + 2]++] = i + 10;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i + 4][states[i + 4]++] = i + 10;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i + 2][states[i + 2]++] = i + 12;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->SZ[i + 4][states[i + 4]++] = i + 12;
    }
    cleanVector (events, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i + 2;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i + 2;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i + 8;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i + 10;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i + 4;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HZ[i][events[i]++] = i + 6;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->HZ[i][events[i]++] = i + 6;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->HZ[i][events[i]++] = i + 8;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->HZ[i][events[i]++] = i + 2;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->HZ[i][events[i]++] = i - 2;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    cleanVector (events, 0, 18);

    for (i = 2; i <= 3; i++)
    {
        modelData->HD[i][events[i]++] = i - 2;
    }
    for (i = 14; i <= 15; i++)
    {
        modelData->HD[i][events[i]++] = i - 12;
    }
    for (i = 16; i <= 17; i++)
    {
        modelData->HD[i][events[i]++] = i - 14;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->HD[i][events[i]++] = i - 8;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->HD[i][events[i]++] = i - 10;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HD[i][events[i]++] = i;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->HD[i][events[i]++] = i - 2;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HD[i][events[i]++] = i + 2;
    }
    for (i = 4; i <= 5; i++)
    {
        modelData->HD[i][events[i]++] = i;
    }
    for (i = 10; i <= 11; i++)
    {
        modelData->HD[i][events[i]++] = i - 6;
    }
    for (i = 12; i <= 13; i++)
    {
        modelData->HD[i][events[i]++] = i - 8;
    }
    for (i = 0; i <= 1; i++)
    {
        modelData->HD[i][events[i]++] = i + 6;
    }
    for (i = 2; i <= 3; i++)
    {
        modelData->HD[i][events[i]++] = i + 6;
    }
    for (i = 14; i <= 15; i++)
    {
        modelData->HD[i][events[i]++] = i;
    }
    for (i = 16; i <= 17; i++)
    {
        modelData->HD[i][events[i]++] = i - 2;
    }
    for (i = 14; i <= 15; i++)
    {
        modelData->HD[i][events[i]++] = 16;
    }
    for (i = 16; i <= 17; i++)
    {
        modelData->HD[i][events[i]++] = 16;
    }
    cleanVector (events, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].LHSSt[events[i]++] = i + 8;
        modelData->event[i + 2].LHSSt[events[i + 2]++] = i + 10;
    }
    cleanVector (events, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].RHSSt[events[i]++] = i + 8;
        modelData->event[i + 2].RHSSt[events[i + 2]++] = i;
        modelData->event[i + 2].RHSSt[events[i + 2]++] = i + 6;
        modelData->event[i + 2].RHSSt[events[i + 2]++] = i + 10;
    }
    cleanVector (events, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].LHSDsc[events[i]++] = i + 22;
        modelData->event[i].LHSDsc[events[i]++] = i + 24;
        modelData->event[i].LHSDsc[events[i]++] = i + 26;
        modelData->event[i].LHSDsc[events[i]++] = i + 30;
        modelData->event[i].LHSDsc[events[i]++] = i + 32;
        modelData->event[i + 2].LHSDsc[events[i + 2]++] = i;
        modelData->event[i + 2].LHSDsc[events[i + 2]++] = i + 4;
        modelData->event[i + 2].LHSDsc[events[i + 2]++] = i + 6;
        modelData->event[i + 2].LHSDsc[events[i + 2]++] = i + 8;
        modelData->event[i + 2].LHSDsc[events[i + 2]++] = i + 20;
        modelData->event[i + 2].LHSDsc[events[i + 2]++] = i + 28;
        modelData->event[i + 4].LHSDsc[events[i + 4]++] = i + 4;
        modelData->event[i + 6].LHSDsc[events[i + 6]++] = i + 16;
        modelData->event[i + 8].LHSDsc[events[i + 8]++] = i + 16;
        modelData->event[i + 10].LHSDsc[events[i + 10]++] = i + 2;
        modelData->event[i + 12].LHSDsc[events[i + 12]++] = i + 2;
        modelData->event[i + 14].LHSDsc[events[i + 14]++] = i + 18;
        modelData->event[i + 16].LHSDsc[events[i + 16]++] = i + 18;
    }
    cleanVector (events, 0, 18);

    for (i = 0; i <= 1; i++)
    {
        modelData->event[i].direction = 1;
        modelData->event[i].relation = 2;
        modelData->event[i + 2].direction = 1;
        modelData->event[i + 2].relation = 2;
        modelData->event[i + 4].direction = 1;
        modelData->event[i + 4].relation = 2;
        modelData->event[i + 6].direction = -1;
        modelData->event[i + 6].relation = 0;
        modelData->event[i + 8].direction = 1;
        modelData->event[i + 8].relation = 2;
        modelData->event[i + 10].direction = -1;
        modelData->event[i + 10].relation = 0;
        modelData->event[i + 12].direction = 1;
        modelData->event[i + 12].relation = 2;
        modelData->event[i + 14].direction = 1;
        modelData->event[i + 14].relation = 2;
        modelData->event[i + 16].direction = -1;
        modelData->event[i + 16].relation = 0;
    }
    simulator->time = QSS_Time (18, 18, 0, 0, ST_Binary, NULL);

    simulator->output = SD_Output ("sisthibgen", 8, 34, 18, NULL, 0, 0, CI_Step, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOS[i] = 1;
        modelOutput->nSO[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOS[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nSO[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOS[i + 6]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nSO[i]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 2] = 1;
        modelOutput->nDO[i + 30]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 4]++;
    }
    modelOutput->nDO[10] += 2;
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nDO[i + 14]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 4]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nDO[i + 20]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 6]++;
    }
    modelOutput->nDO[10] += 2;
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 6]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nDO[i + 14]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOD[i + 6]++;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->nDO[i + 20]++;
    }
    SD_allocOutputMatrix (modelOutput, 18, 34);
    cleanVector (states, 0, 18);

    cleanVector (discretes, 0, 34);

    cleanVector (outputs, 0, 8);

    for (i = 0; i <= 1; i++)
    {
        sprintf (modelOutput->variable[i].name, "Vpv[%d]", i + 1);
        sprintf (modelOutput->variable[i + 2].name, "VpvRef[%d]", i + 1);
        sprintf (modelOutput->variable[i + 4].name, "PotPanel[%d]", i + 1);
        sprintf (modelOutput->variable[i + 6].name, "Ipv[%d]", i + 1);
    }
    cleanVector (outputs, 0, 8);

    for (i = 0; i <= 1; i++)
    {
        modelOutput->SO[i][states[i]++] = i;
        modelOutput->OS[i][outputs[i]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OS[i + 4][outputs[i + 4]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->SO[i][states[i]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OS[i + 6][outputs[i + 6]++] = i;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->SO[i][states[i]++] = i + 6;
    }
    cleanVector (outputs, 0, 8);

    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[i + 30][discretes[i + 30]++] = i + 2;
        modelOutput->OD[i + 2][outputs[i + 2]++] = i + 30;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OD[i + 4][outputs[i + 4]++] = 10;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[10][discretes[10]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OD[i + 4][outputs[i + 4]++] = i + 14;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[i + 14][discretes[i + 14]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OD[i + 4][outputs[i + 4]++] = i + 20;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[i + 20][discretes[i + 20]++] = i + 4;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OD[i + 6][outputs[i + 6]++] = 10;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[10][discretes[10]++] = i + 6;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OD[i + 6][outputs[i + 6]++] = i + 14;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[i + 14][discretes[i + 14]++] = i + 6;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->OD[i + 6][outputs[i + 6]++] = i + 20;
    }
    for (i = 0; i <= 1; i++)
    {
        modelOutput->DO[i + 20][discretes[i + 20]++] = i + 6;
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
