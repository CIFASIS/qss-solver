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
    double tmp22[3];
    int j = 0;
    int j25;
    switch (i)
    {
        case 30:
            alg[66] = 0.000000000000000000000000e+00
                    + (1.0 - d[(20)]) * (d[(21)] * __PAR_Qcmax + (1.0 - d[(21)]) * (__PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90]));
            alg[66 + 1] = (__PAR_kpcw * x[94] + x[91]) * (-1.0 + d[(20)]) * (-1.0 + d[(21)]);
            dx[1] = __PAR_kicw / __PAR_kpcw * (alg[66] - x[90]);
            dx[2] = (-(1.0 / (__PAR_kpcw)) * __PAR_kicw * (x[91] - alg[67])) / 2;
            return;
        case 31:
            alg[66] = 0.000000000000000000000000e+00
                    + (1.0 - d[(20)]) * (d[(21)] * __PAR_Qcmax + (1.0 - d[(21)]) * (__PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90]));
            alg[66 + 1] = (__PAR_kpcw * x[94] + x[91]) * (-1.0 + d[(20)]) * (-1.0 + d[(21)]);
            for (j25 = 0; j25 < 3; j25++)
            {
                tmp22[j25] = 0;
            }
            for (j25 = 0; j25 <= 9; j25++)
            {
                tmp22[0] += x[(j25 + 10) * 3 + 0];
                tmp22[1] += x[(j25 + 10) * 3 + 1];
                tmp22[2] += x[(j25 + 10) * 3 + 2];
            }
            dx[1] = (tmp22[0] - alg[66]) / __PAR_Ccw;
            dx[2] = ((1.0 / (__PAR_Ccw)) * (tmp22[1] - alg[67])) / 2;
            return;
        default:
            j = i;
            if (j >= 0 && j <= 9)
            {
                dx[1] = -__PAR_kiz / __PAR_kpz * x[(j) * 3]
                        + __PAR_kiz / __PAR_kpz
                                * (d[(j + 32)]
                                        + (1.0 - d[(j + 32)])
                                                * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3])));
                dx[2] = (-x[(j) * 3 + 1] * __PAR_kiz * (1.0 / (__PAR_kpz))
                        + (-1.0 + d[(j + 32)]) * __PAR_kiz * (x[(j + 20) * 3 + 1] * __PAR_kpz + x[(j) * 3 + 1]) * (1.0 / (__PAR_kpz))
                                * (-1.0 + d[(j + 22)])) / 2;
            }
            j = i - 10;
            if (j >= 0 && j <= 9)
            {
                alg[(j + 1) * 3] = d[(j + 32)]
                        + (1.0 - d[(j + 32)]) * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3]));
                alg[(j + 1) * 3 + 1] = (-1.0 + d[(j + 22)]) * (__PAR_kpz * x[(j + 20) * 3 + 1] + x[(j) * 3 + 1]) * (-1.0 + d[(j + 32)]);
                dx[1] = alg[(j + 1) * 3] * __PAR_kcw * (x[(j + 20) * 3] - x[93]) - x[(j + 10) * 3];
                dx[2] = (-__PAR_kcw * (x[94] - x[(j + 20) * 3 + 1]) * alg[(j + 1) * 3] - x[(j + 10) * 3 + 1]
                        + alg[(j + 1) * 3 + 1] * __PAR_kcw * (x[(j + 20) * 3] - x[93])) / 2;
            }
            j = i - 20;
            if (j >= 0 && j <= 9)
            {
                alg[0] = 26.0 + 4.0 * sin (t / 3600.0 / 24.0 * 6.280000000000000248689958e+00) + 9.999999999999999451532715e-21 * x[87];
                alg[0 + 1] = (9.999999999999999451532715e-21) * x[88] + (2.907407407407407704848235e-04) * cos ((7.268518518518519262120586e-05) * t);
                alg[(j + 11) * 3] = (__PAR_p1 * pow (x[(j + 20) * 3], 2.0) + __PAR_p2 * x[(j + 20) * 3] + __PAR_p3) * d[(j)];
                alg[(j + 11) * 3 + 1] = d[(j)] * (2.0 * x[(j + 20) * 3 + 1] * __PAR_p1 * x[(j + 20) * 3] + x[(j + 20) * 3 + 1] * __PAR_p2);
                dx[1] = (-x[(j + 10) * 3] + __PAR_kout * (alg[0] - x[(j + 20) * 3]) + alg[(j + 11) * 3]) / __PAR_Cza;
                dx[2] = ((alg[(j + 11) * 3 + 1] + __PAR_kout * (alg[1] - x[(j + 20) * 3 + 1]) - x[(j + 10) * 3 + 1]) * (1.0 / (__PAR_Cza))) / 2;
            }
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    double tmp22[3];
    int j = 0;
    int j25;
    int j26 = 0;
    switch (i)
    {
        case 29:
            alg[0] = 26.0 + 4.0 * sin (t / 3600.0 / 24.0 * 6.280000000000000248689958e+00) + 9.999999999999999451532715e-21 * x[87];
            alg[0 + 1] = (9.999999999999999451532715e-21) * x[88] + (2.907407407407407704848235e-04) * cos ((7.268518518518519262120586e-05) * t);
            for (j = 0; j <= 9; j++)
            {
                alg[(j + 11) * 3] = (__PAR_p1 * pow (x[(j + 20) * 3], 2.0) + __PAR_p2 * x[(j + 20) * 3] + __PAR_p3) * d[(j)];
                alg[(j + 11) * 3 + 1] = d[(j)] * (2.0 * x[(j + 20) * 3 + 1] * __PAR_p1 * x[(j + 20) * 3] + x[(j + 20) * 3 + 1] * __PAR_p2);
            }
            for (j = 0; j <= 9; j++)
            {
                der[(j + 20) * 3 + 1] = (-x[(j + 10) * 3] + __PAR_kout * (alg[0] - x[(j + 20) * 3]) + alg[(j + 11) * 3]) / __PAR_Cza;
                der[(j + 20) * 3 + 2] = ((alg[(j + 11) * 3 + 1] + __PAR_kout * (alg[1] - x[(j + 20) * 3 + 1]) - x[(j + 10) * 3 + 1])
                        * (1.0 / (__PAR_Cza))) / 2;
            }
            break;
        case 30:
            alg[66] = 0.000000000000000000000000e+00
                    + (1.0 - d[(20)]) * (d[(21)] * __PAR_Qcmax + (1.0 - d[(21)]) * (__PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90]));
            alg[66 + 1] = (__PAR_kpcw * x[94] + x[91]) * (-1.0 + d[(20)]) * (-1.0 + d[(21)]);
            der[90 + 1] = __PAR_kicw / __PAR_kpcw * (alg[66] - x[90]);
            der[90 + 2] = (-(1.0 / (__PAR_kpcw)) * __PAR_kicw * (x[91] - alg[67])) / 2;
            for (j25 = 0; j25 < 3; j25++)
            {
                tmp22[j25] = 0;
            }
            for (j25 = 0; j25 <= 9; j25++)
            {
                tmp22[0] += x[(j25 + 10) * 3 + 0];
                tmp22[1] += x[(j25 + 10) * 3 + 1];
                tmp22[2] += x[(j25 + 10) * 3 + 2];
            }
            der[93 + 1] = (tmp22[0] - alg[66]) / __PAR_Ccw;
            der[93 + 2] = ((1.0 / (__PAR_Ccw)) * (tmp22[1] - alg[67])) / 2;
            return;
        case 31:
            for (j = 0; j <= 9; j++)
            {
                j26 = j;
                if (j26 >= 0 && j26 <= 9)
                {
                    alg[(j + 1) * 3] = d[(j26 + 32)]
                            + (1.0 - d[(j26 + 32)]) * (0.0 + (1.0 - d[(j26 + 22)]) * (__PAR_kpz * (x[(j26 + 20) * 3] - __PAR_Tzasp) + x[(j26) * 3]));
                    alg[(j + 1) * 3 + 1] = (-1.0 + d[(j26 + 22)]) * (__PAR_kpz * x[(j26 + 20) * 3 + 1] + x[(j26) * 3 + 1]) * (-1.0 + d[(j26 + 32)]);
                }
            }
            alg[66] = 0.000000000000000000000000e+00
                    + (1.0 - d[(20)]) * (d[(21)] * __PAR_Qcmax + (1.0 - d[(21)]) * (__PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90]));
            alg[66 + 1] = (__PAR_kpcw * x[94] + x[91]) * (-1.0 + d[(20)]) * (-1.0 + d[(21)]);
            for (j = 0; j <= 9; j++)
            {
                der[(j + 10) * 3 + 1] = alg[(j + 1) * 3] * __PAR_kcw * (x[(j + 20) * 3] - x[93]) - x[(j + 10) * 3];
                der[(j + 10) * 3 + 2] = (-__PAR_kcw * (x[94] - x[(j + 20) * 3 + 1]) * alg[(j + 1) * 3] - x[(j + 10) * 3 + 1]
                        + alg[(j + 1) * 3 + 1] * __PAR_kcw * (x[(j + 20) * 3] - x[93])) / 2;
            }
            der[90 + 1] = __PAR_kicw / __PAR_kpcw * (alg[66] - x[90]);
            der[90 + 2] = (-(1.0 / (__PAR_kpcw)) * __PAR_kicw * (x[91] - alg[67])) / 2;
            for (j25 = 0; j25 < 3; j25++)
            {
                tmp22[j25] = 0;
            }
            for (j25 = 0; j25 <= 9; j25++)
            {
                tmp22[0] += x[(j25 + 10) * 3 + 0];
                tmp22[1] += x[(j25 + 10) * 3 + 1];
                tmp22[2] += x[(j25 + 10) * 3 + 2];
            }
            der[93 + 1] = (tmp22[0] - alg[66]) / __PAR_Ccw;
            der[93 + 2] = ((1.0 / (__PAR_Ccw)) * (tmp22[1] - alg[67])) / 2;
            break;
    }
    j = i;
    if (j >= 0 && j <= 9)
    {
        alg[(j + 1) * 3] = d[(j + 32)]
                + (1.0 - d[(j + 32)]) * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3]));
        alg[(j + 1) * 3 + 1] = (-1.0 + d[(j + 22)]) * (__PAR_kpz * x[(j + 20) * 3 + 1] + x[(j) * 3 + 1]) * (-1.0 + d[(j + 32)]);
        der[(j) * 3 + 1] = -__PAR_kiz / __PAR_kpz * x[(j) * 3]
                + __PAR_kiz / __PAR_kpz
                        * (d[(j + 32)]
                                + (1.0 - d[(j + 32)]) * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3])));
        der[(j) * 3 + 2] = (-x[(j) * 3 + 1] * __PAR_kiz * (1.0 / (__PAR_kpz))
                + (-1.0 + d[(j + 32)]) * __PAR_kiz * (x[(j + 20) * 3 + 1] * __PAR_kpz + x[(j) * 3 + 1]) * (1.0 / (__PAR_kpz)) * (-1.0 + d[(j + 22)]))
                / 2;
        der[(j + 10) * 3 + 1] = alg[(j + 1) * 3] * __PAR_kcw * (x[(j + 20) * 3] - x[93]) - x[(j + 10) * 3];
        der[(j + 10) * 3 + 2] = (-__PAR_kcw * (x[94] - x[(j + 20) * 3 + 1]) * alg[(j + 1) * 3] - x[(j + 10) * 3 + 1]
                + alg[(j + 1) * 3 + 1] * __PAR_kcw * (x[(j + 20) * 3] - x[93])) / 2;
    }
    j = i - 10;
    if (j >= 0 && j <= 9)
    {
        alg[0] = 26.0 + 4.0 * sin (t / 3600.0 / 24.0 * 6.280000000000000248689958e+00) + 9.999999999999999451532715e-21 * x[87];
        alg[0 + 1] = (9.999999999999999451532715e-21) * x[88] + (2.907407407407407704848235e-04) * cos ((7.268518518518519262120586e-05) * t);
        alg[(j + 1) * 3] = d[(j + 32)]
                + (1.0 - d[(j + 32)]) * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3]));
        alg[(j + 1) * 3 + 1] = (-1.0 + d[(j + 22)]) * (__PAR_kpz * x[(j + 20) * 3 + 1] + x[(j) * 3 + 1]) * (-1.0 + d[(j + 32)]);
        alg[(j + 11) * 3] = (__PAR_p1 * pow (x[(j + 20) * 3], 2.0) + __PAR_p2 * x[(j + 20) * 3] + __PAR_p3) * d[(j)];
        alg[(j + 11) * 3 + 1] = d[(j)] * (2.0 * x[(j + 20) * 3 + 1] * __PAR_p1 * x[(j + 20) * 3] + x[(j + 20) * 3 + 1] * __PAR_p2);
        der[(j + 10) * 3 + 1] = alg[(j + 1) * 3] * __PAR_kcw * (x[(j + 20) * 3] - x[93]) - x[(j + 10) * 3];
        der[(j + 10) * 3 + 2] = (-__PAR_kcw * (x[94] - x[(j + 20) * 3 + 1]) * alg[(j + 1) * 3] - x[(j + 10) * 3 + 1]
                + alg[(j + 1) * 3 + 1] * __PAR_kcw * (x[(j + 20) * 3] - x[93])) / 2;
        der[(j + 20) * 3 + 1] = (-x[(j + 10) * 3] + __PAR_kout * (alg[0] - x[(j + 20) * 3]) + alg[(j + 11) * 3]) / __PAR_Cza;
        der[(j + 20) * 3 + 2] = ((alg[(j + 11) * 3 + 1] + __PAR_kout * (alg[1] - x[(j + 20) * 3 + 1]) - x[(j + 10) * 3 + 1]) * (1.0 / (__PAR_Cza)))
                / 2;
    }
    j = i - 20;
    if (j >= 0 && j <= 9)
    {
        alg[0] = 26.0 + 4.0 * sin (t / 3600.0 / 24.0 * 6.280000000000000248689958e+00) + 9.999999999999999451532715e-21 * x[87];
        alg[0 + 1] = (9.999999999999999451532715e-21) * x[88] + (2.907407407407407704848235e-04) * cos ((7.268518518518519262120586e-05) * t);
        alg[(j + 1) * 3] = d[(j + 32)]
                + (1.0 - d[(j + 32)]) * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3]));
        alg[(j + 1) * 3 + 1] = (-1.0 + d[(j + 22)]) * (__PAR_kpz * x[(j + 20) * 3 + 1] + x[(j) * 3 + 1]) * (-1.0 + d[(j + 32)]);
        alg[(j + 11) * 3] = (__PAR_p1 * pow (x[(j + 20) * 3], 2.0) + __PAR_p2 * x[(j + 20) * 3] + __PAR_p3) * d[(j)];
        alg[(j + 11) * 3 + 1] = d[(j)] * (2.0 * x[(j + 20) * 3 + 1] * __PAR_p1 * x[(j + 20) * 3] + x[(j + 20) * 3 + 1] * __PAR_p2);
        der[(j) * 3 + 1] = -__PAR_kiz / __PAR_kpz * x[(j) * 3]
                + __PAR_kiz / __PAR_kpz
                        * (d[(j + 32)]
                                + (1.0 - d[(j + 32)]) * (0.0 + (1.0 - d[(j + 22)]) * (__PAR_kpz * (x[(j + 20) * 3] - __PAR_Tzasp) + x[(j) * 3])));
        der[(j) * 3 + 2] = (-x[(j) * 3 + 1] * __PAR_kiz * (1.0 / (__PAR_kpz))
                + (-1.0 + d[(j + 32)]) * __PAR_kiz * (x[(j + 20) * 3 + 1] * __PAR_kpz + x[(j) * 3 + 1]) * (1.0 / (__PAR_kpz)) * (-1.0 + d[(j + 22)]))
                / 2;
        der[(j + 10) * 3 + 1] = alg[(j + 1) * 3] * __PAR_kcw * (x[(j + 20) * 3] - x[93]) - x[(j + 10) * 3];
        der[(j + 10) * 3 + 2] = (-__PAR_kcw * (x[94] - x[(j + 20) * 3 + 1]) * alg[(j + 1) * 3] - x[(j + 10) * 3 + 1]
                + alg[(j + 1) * 3 + 1] * __PAR_kcw * (x[(j + 20) * 3] - x[93])) / 2;
        der[(j + 20) * 3 + 1] = (-x[(j + 10) * 3] + __PAR_kout * (alg[0] - x[(j + 20) * 3]) + alg[(j + 11) * 3]) / __PAR_Cza;
        der[(j + 20) * 3 + 2] = ((alg[(j + 11) * 3 + 1] + __PAR_kout * (alg[1] - x[(j + 20) * 3 + 1]) - x[(j + 10) * 3 + 1]) * (1.0 / (__PAR_Cza)))
                / 2;
    }
    j = i - 10;
    if (j >= 0 && j <= 9)
    {
        alg[66] = 0.000000000000000000000000e+00
                + (1.0 - d[(20)]) * (d[(21)] * __PAR_Qcmax + (1.0 - d[(21)]) * (__PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90]));
        alg[66 + 1] = (__PAR_kpcw * x[94] + x[91]) * (-1.0 + d[(20)]) * (-1.0 + d[(21)]);
        for (j25 = 0; j25 < 3; j25++)
        {
            tmp22[j25] = 0;
        }
        for (j25 = 0; j25 <= 9; j25++)
        {
            tmp22[0] += x[(j25 + 10) * 3 + 0];
            tmp22[1] += x[(j25 + 10) * 3 + 1];
            tmp22[2] += x[(j25 + 10) * 3 + 2];
        }
        der[93 + 1] = (tmp22[0] - alg[66]) / __PAR_Ccw;
        der[93 + 2] = ((1.0 / (__PAR_Ccw)) * (tmp22[1] - alg[67])) / 2;
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    switch (i)
    {
        case 30:
            zc[0] = __PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90] - (0.0);
            zc[1] = x[91] + __PAR_kpcw * x[94];
            return;
        case 31:
            zc[0] = __PAR_kpcw * (x[93] - __PAR_Tcwsp) + x[90] - (__PAR_Qcmax);
            zc[1] = __PAR_kpcw * x[94] + x[91];
            return;
        default:
            if (i >= 0 && i <= 9)
            {
                zc[0] = t - (d[(i + 10)]);
                zc[1] = 1.0;
            }
            if (i >= 10 && i <= 19)
            {
                zc[0] = __PAR_kpz * (x[(i + 10) * 3] - __PAR_Tzasp) + x[(i - 10) * 3] - (0.0);
                zc[1] = x[(i + 10) * 3 + 1] * __PAR_kpz + x[(i - 10) * 3 + 1];
            }
            if (i >= 20 && i <= 29)
            {
                zc[0] = __PAR_kpz * (x[(i) * 3] - __PAR_Tzasp) + x[(i - 20) * 3] - (1.0);
                zc[1] = __PAR_kpz * x[(i) * 3 + 1] + x[(i - 20) * 3 + 1];
            }
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 30:
            d[(20)] = 0.0;
            return;
        case 31:
            d[(21)] = 1.0;
            return;
        default:
            if (i >= 0 && i <= 9)
            {
                d[(i)] = __fede__get_npeople_qss ((i + 1));
                d[(i + 10)] = t + __fede__get_nextpeople_qss ((i + 1));
            }
            if (i >= 10 && i <= 19)
            {
                d[(i + 12)] = 0.0;
            }
            if (i >= 20 && i <= 29)
            {
                d[(i + 12)] = 1.0;
            }
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 30:
            d[(20)] = 1.0;
            return;
        case 31:
            d[(21)] = 0.0;
            return;
        default:
            if (i >= 10 && i <= 19)
            {
                d[(i + 12)] = 1.0;
            }
            if (i >= 20 && i <= 29)
            {
                d[(i + 12)] = 0.0;
            }
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = x[87];
            return;
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *discretes = (int*) malloc (42 * sizeof(int));
    int *events = (int*) malloc (32 * sizeof(int));
    int *outputs = (int*) malloc (1 * sizeof(int));
    int *states = (int*) malloc (32 * sizeof(int));
    int i29;
    int i;
    int j = 0;
    int td = 0;
    simulator->data = QSS_Data (32, 42, 32, 10, 23, "microgrid");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_Cza = 6.092000000000000000000000e+06;
    __PAR_kout = 4.625000000000000000000000e+02;
    __PAR_kiz = 5.000000000000000104083409e-04;
    __PAR_kpz = 1.0;
    __PAR_Ccw = (1.041800000000000000000000e+07 + 3.334000000000000074606987e+00 * 1.000000000000000000000000e+06) * 10;
    __PAR_kcw = 2.633910000000000195541361e+02;
    __PAR_Tcwsp = 10.0;
    __PAR_kicw = 5.000000000000000000000000e-01 * 10;
    __PAR_kpcw = 6000.0 * 10;
    __PAR_Qcmax = 3000.0 * 10;
    __PAR_p1 = -2.199000000000000121236354e-01;
    __PAR_p2 = 5.059700000000000308375547e+00;
    __PAR_p3 = 8.491679999999999495230441e+01;
    __PAR_Tzasp = 23.0;
    for (i = 0; i <= 9; i++)
    {
        modelData->x[(i + 10) * 3] = 0.0;
    }
    modelData->x[93] = __PAR_Tcwsp;
    for (i = 0; i <= 9; i++)
    {
        modelData->x[(i + 20) * 3] = 23.0;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->x[i * 3] = 0.0;
    }
    modelData->x[90] = 0.0;
    // Initialize model code.
    for (i29 = 0; i29 <= 9; i29++)
    {
        modelData->d[(i29)] = __fede__get_npeople_qss ((i29 + 1));
        modelData->d[(i29 + 10)] = __fede__get_nextpeople_qss ((i29 + 1));
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->nDS[i] = 2;
        modelData->nDS[i + 10] = 3;
        modelData->nDS[i + 20] = 2;
    }
    modelData->nDS[30] = 1;
    modelData->nDS[31] += 10;
    for (i = 0; i <= 9; i++)
    {
        modelData->nDS[i + 10]++;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->nDS[i + 20]++;
    }
    modelData->nDS[30]++;
    modelData->nDS[31]++;
    modelData->nDS[31]++;
    for (i = 0; i <= 9; i++)
    {
        modelData->nSD[i]++;
        modelData->nSD[i + 20]++;
        modelData->nSD[i + 10]++;
        modelData->nSD[i + 20]++;
        modelData->nSD[31]++;
        modelData->nSD[i + 10]++;
        modelData->nSD[i + 20]++;
    }
    modelData->nSD[30]++;
    for (i = 0; i <= 9; i++)
    {
        modelData->nSD[i + 10]++;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->nSD[i]++;
    }
    modelData->nSD[29] += 10;
    modelData->nSD[31]++;
    modelData->nSD[30]++;
    modelData->nSD[31]++;
    for (i = 0; i <= 9; i++)
    {
        modelData->nZS[i + 10] = 2;
        modelData->nZS[i + 20] = 2;
    }
    modelData->nZS[30] = 2;
    modelData->nZS[31] = 2;
    for (i = 0; i <= 9; i++)
    {
        modelData->nSZ[i]++;
        modelData->nSZ[i + 20]++;
        modelData->nSZ[i]++;
        modelData->nSZ[i + 20]++;
    }
    modelData->nSZ[30]++;
    modelData->nSZ[31]++;
    modelData->nSZ[30]++;
    modelData->nSZ[31]++;
    for (i = 0; i <= 9; i++)
    {
        modelData->nHZ[i]++;
    }
    for (i = 10; i <= 19; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 20; i <= 29; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 10; i <= 19; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 20; i <= 29; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->nHD[i]++;
    }
    modelData->nHD[30] = 2;
    modelData->nHD[31] = 2;
    for (i = 0; i <= 9; i++)
    {
        modelData->event[i].nLHSDsc = 2;
        modelData->event[i + 10].nLHSDsc = 1;
        modelData->event[i + 20].nLHSDsc = 1;
    }
    modelData->event[30].nLHSDsc = 1;
    modelData->event[31].nLHSDsc = 1;
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    for (i = 0; i <= 9; i++)
    {
        modelData->TD[td++] = i + 20;
    }
    cleanVector (states, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->DS[i][states[i]++] = i;
        modelData->DS[i][states[i]++] = i + 20;
        modelData->DS[i + 10][states[i + 10]++] = i + 10;
        modelData->DS[i + 10][states[i + 10]++] = i + 20;
        modelData->DS[i + 10][states[i + 10]++] = 31;
        modelData->DS[i + 20][states[i + 20]++] = i + 10;
        modelData->DS[i + 20][states[i + 20]++] = i + 20;
    }
    modelData->DS[30][states[30]++] = 30;
    for (i = 0; i <= 9; i++)
    {
        modelData->DS[31][states[31]++] = i + 10;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->DS[i + 10][states[i + 10]++] = i;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->DS[i + 20][states[i + 20]++] = 29;
    }
    modelData->DS[30][states[30]++] = 31;
    modelData->DS[31][states[31]++] = 30;
    modelData->DS[31][states[31]++] = 31;
    cleanVector (states, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->SD[i][states[i]++] = i;
        modelData->SD[i + 20][states[i + 20]++] = i;
        modelData->SD[i + 10][states[i + 10]++] = i + 10;
        modelData->SD[i + 20][states[i + 20]++] = i + 10;
        modelData->SD[31][states[31]++] = i + 10;
        modelData->SD[i + 10][states[i + 10]++] = i + 20;
        modelData->SD[i + 20][states[i + 20]++] = i + 20;
    }
    modelData->SD[30][states[30]++] = 30;
    for (i = 0; i <= 9; i++)
    {
        modelData->SD[i + 10][states[i + 10]++] = 31;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->SD[i][states[i]++] = i + 10;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->SD[29][states[29]++] = i + 20;
    }
    modelData->SD[31][states[31]++] = 30;
    modelData->SD[30][states[30]++] = 31;
    modelData->SD[31][states[31]++] = 31;
    cleanVector (events, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->ZS[i + 10][events[i + 10]++] = i;
        modelData->ZS[i + 10][events[i + 10]++] = i + 20;
        modelData->ZS[i + 20][events[i + 20]++] = i;
        modelData->ZS[i + 20][events[i + 20]++] = i + 20;
    }
    modelData->ZS[30][events[30]++] = 30;
    modelData->ZS[30][events[30]++] = 31;
    modelData->ZS[31][events[31]++] = 30;
    modelData->ZS[31][events[31]++] = 31;
    cleanVector (states, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->SZ[i][states[i]++] = i + 10;
        modelData->SZ[i + 20][states[i + 20]++] = i + 10;
        modelData->SZ[i][states[i]++] = i + 20;
        modelData->SZ[i + 20][states[i + 20]++] = i + 20;
    }
    modelData->SZ[30][states[30]++] = 30;
    modelData->SZ[31][states[31]++] = 30;
    modelData->SZ[30][states[30]++] = 31;
    modelData->SZ[31][states[31]++] = 31;
    cleanVector (events, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->HZ[i][events[i]++] = i;
    }
    cleanVector (events, 0, 32);

    for (i = 10; i <= 19; i++)
    {
        modelData->HD[i][events[i]++] = i - 10;
    }
    for (i = 20; i <= 29; i++)
    {
        modelData->HD[i][events[i]++] = i - 20;
    }
    for (i = 10; i <= 19; i++)
    {
        modelData->HD[i][events[i]++] = i;
    }
    for (i = 20; i <= 29; i++)
    {
        modelData->HD[i][events[i]++] = i - 10;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->HD[i][events[i]++] = i + 20;
    }
    modelData->HD[30][events[30]++] = 30;
    modelData->HD[31][events[31]++] = 30;
    modelData->HD[30][events[30]++] = 31;
    modelData->HD[31][events[31]++] = 31;
    cleanVector (events, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->event[i].LHSDsc[events[i]++] = i;
        modelData->event[i].LHSDsc[events[i]++] = i + 10;
        modelData->event[i + 10].LHSDsc[events[i + 10]++] = i + 22;
        modelData->event[i + 20].LHSDsc[events[i + 20]++] = i + 32;
    }
    modelData->event[30].LHSDsc[events[30]++] = 20;
    modelData->event[31].LHSDsc[events[31]++] = 21;
    cleanVector (events, 0, 32);

    for (i = 0; i <= 9; i++)
    {
        modelData->event[i].direction = 1;
        modelData->event[i].relation = 2;
        modelData->event[i + 10].direction = 0;
        modelData->event[i + 10].relation = 0;
        modelData->event[i + 20].direction = 0;
        modelData->event[i + 20].relation = 2;
    }
    modelData->event[30].direction = 0;
    modelData->event[30].relation = 0;
    modelData->event[31].direction = 0;
    modelData->event[31].relation = 2;
    simulator->time = QSS_Time (32, 32, 10, 0, ST_Binary, NULL);

    double period[1];
    period[0] = 72;
    simulator->output = SD_Output ("microgrid", 1, 42, 32, period, 1, 0, CI_Sampled, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[29]++;
    SD_allocOutputMatrix (modelOutput, 32, 42);
    cleanVector (states, 0, 32);

    cleanVector (outputs, 0, 1);

    sprintf (modelOutput->variable[0].name, "Tza[10]");
    cleanVector (outputs, 0, 1);

    modelOutput->SO[29][states[29]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 29;
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
