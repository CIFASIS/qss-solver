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
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 0;
    settings->parallel = FALSE;
    settings->hybrid = TRUE;
    settings->method = 7;
}

void
MOD_definition (double *x, double *d, double *alg, double t, double *dx)
{
    int i = 0;
    modelData->scalarEvaluations++;
    alg[90] = __PAR_R * x[0] - x[20];
    alg[100] = d[0] * __PAR_bnoslip * alg[90] * __PAR_m * __PAR_g
            + (1.0 - d[0]) * (__PAR_fric_coef_dyn * alg[90] * __PAR_m * __PAR_g + __PAR_bslip * alg[90] * __PAR_m * __PAR_g);
    dx[0] = (d[(10)] - __PAR_R * alg[100] - __PAR_baxle * __PAR_m * __PAR_g * x[0]) / __PAR_J;
    dx[10] = x[20];
    alg[30] = 0.0;
    alg[40] = 0.0;
    alg[50] = -__PAR_m * __PAR_g;
    alg[60] = cos (x[10] / __PAR_r) * cos (__PAR_phi);
    alg[70] = -sin (x[10] / __PAR_r);
    alg[80] = cos (x[10] / __PAR_r) * sin (__PAR_phi);
    alg[90] = __PAR_R * x[0] - x[20];
    alg[100] = d[0] * __PAR_bnoslip * alg[90] * __PAR_m * __PAR_g
            + (1.0 - d[0]) * (__PAR_fric_coef_dyn * alg[90] * __PAR_m * __PAR_g + __PAR_bslip * alg[90] * __PAR_m * __PAR_g);
    alg[110] = alg[60] * alg[30] + alg[70] * alg[40] + alg[80] * alg[50] + alg[100] + __PAR_kc * (x[11] - x[10] - __PAR_L)
            + __PAR_bc * (x[21] - x[20]);
    dx[20] = alg[110] / __PAR_m;
    alg[99] = __PAR_R * x[9] - x[29];
    alg[109] = d[9] * __PAR_bnoslip * alg[99] * __PAR_m * __PAR_g
            + (1.0 - d[9]) * (__PAR_fric_coef_dyn * alg[99] * __PAR_m * __PAR_g + __PAR_bslip * alg[99] * __PAR_m * __PAR_g);
    dx[9] = (-__PAR_R * alg[109] - __PAR_baxle * __PAR_m * __PAR_g * x[9]) / __PAR_J;
    dx[19] = x[29];
    alg[39] = 0.0;
    alg[49] = 0.0;
    alg[59] = -__PAR_m * __PAR_g;
    alg[69] = cos (x[19] / __PAR_r) * cos (__PAR_phi);
    alg[79] = -sin (x[19] / __PAR_r);
    alg[89] = cos (x[19] / __PAR_r) * sin (__PAR_phi);
    alg[99] = __PAR_R * x[9] - x[29];
    alg[109] = d[9] * __PAR_bnoslip * alg[99] * __PAR_m * __PAR_g
            + (1.0 - d[9]) * (__PAR_fric_coef_dyn * alg[99] * __PAR_m * __PAR_g + __PAR_bslip * alg[99] * __PAR_m * __PAR_g);
    alg[119] = alg[69] * alg[39] + alg[79] * alg[49] + alg[89] * alg[59] + alg[109] + __PAR_kc * (x[18] - x[19] + __PAR_L)
            + __PAR_bc * (x[28] - x[29]);
    dx[29] = alg[119] / __PAR_m;
    for (i = 1; i <= 8; i++)
    {
        alg[(i + 90) * 1] = __PAR_R * x[(i) * 1] - x[(i + 20) * 1];
        alg[(i + 100) * 1] = d[(i)] * __PAR_bnoslip * alg[(i + 90) * 1] * __PAR_m * __PAR_g
                + (1.0 - d[(i)])
                        * (__PAR_fric_coef_dyn * alg[(i + 90) * 1] * __PAR_m * __PAR_g + __PAR_bslip * alg[(i + 90) * 1] * __PAR_m * __PAR_g);
        dx[i] = (-__PAR_R * alg[(i + 100) * 1] - __PAR_baxle * __PAR_m * __PAR_g * x[(i) * 1]) / __PAR_J;
    }
    for (i = 1; i <= 8; i++)
    {
        dx[i + 10] = x[(i + 20) * 1];
    }
    for (i = 1; i <= 8; i++)
    {
        alg[(i + 30) * 1] = 0.0;
        alg[(i + 40) * 1] = 0.0;
        alg[(i + 50) * 1] = -__PAR_m * __PAR_g;
        alg[(i + 60) * 1] = cos (x[(i + 10) * 1] / __PAR_r) * cos (__PAR_phi);
        alg[(i + 70) * 1] = -sin (x[(i + 10) * 1] / __PAR_r);
        alg[(i + 80) * 1] = cos (x[(i + 10) * 1] / __PAR_r) * sin (__PAR_phi);
        alg[(i + 90) * 1] = __PAR_R * x[(i) * 1] - x[(i + 20) * 1];
        alg[(i + 100) * 1] = d[(i)] * __PAR_bnoslip * alg[(i + 90) * 1] * __PAR_m * __PAR_g
                + (1.0 - d[(i)])
                        * (__PAR_fric_coef_dyn * alg[(i + 90) * 1] * __PAR_m * __PAR_g + __PAR_bslip * alg[(i + 90) * 1] * __PAR_m * __PAR_g);
        alg[(i + 110) * 1] = alg[(i + 60) * 1] * alg[(i + 30) * 1] + alg[(i + 70) * 1] * alg[(i + 40) * 1] + alg[(i + 80) * 1] * alg[(i + 50) * 1]
                + alg[(i + 100) * 1] + __PAR_kc * (x[(i + 11) * 1] + x[(i + 9) * 1] - 2.0 * x[(i + 10) * 1])
                + __PAR_bc * (x[(i + 21) * 1] + x[(i + 19) * 1] - 2.0 * x[(i + 20) * 1]);
        dx[i + 20] = alg[(i + 110) * 1] / __PAR_m;
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    int j30 = 0;
    int j31 = 0;
    modelData->zeroCrossings++;
    switch (i)
    {
        case 20:
            zc[0] = t - (30.0);
            return;
        default:
            if (i >= 0 && i <= 9)
            {
                alg[90] = __PAR_R * x[0] - x[20];
                j30 = i;
                if (j30 >= 1 && j30 <= 8)
                {
                    alg[(i + 90) * 1] = __PAR_R * x[(j30) * 1] - x[(j30 + 20) * 1];
                }
                alg[99] = __PAR_R * x[9] - x[29];
                zc[0] = alg[(i + 90) * 1] - (__PAR_vslipmax);
            }
            if (i >= 10 && i <= 19)
            {
                alg[90] = __PAR_R * x[0] - x[20];
                j31 = i;
                if (j31 >= 1 && j31 <= 8)
                {
                    alg[(i + 80) * 1] = __PAR_R * x[(j31 - 10) * 1] - x[(j31 + 10) * 1];
                }
                alg[99] = __PAR_R * x[9] - x[29];
                zc[0] = alg[(i + 80) * 1] - (__PAR_vslipmax);
            }
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 20:
            d[(10)] = 0.0;
            return;
        default:
            if (i >= 0 && i <= 9)
            {
                d[(i)] = 0.0;
            }
            if (i >= 10 && i <= 19)
            {
                d[(i - 10)] = 1.0;
            }
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 0 && i <= 9)
    {
        d[(i)] = 1.0;
    }
    if (i >= 10 && i <= 19)
    {
        d[(i - 10)] = 0.0;
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = x[20];
            return;
    }
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
    int discretes[11];
    int i = 0;
    int i32;
    int outputs[1];
    int states[30];
    simulator->data = CLC_Data (30, 11, 21, 0, 120, "train4DASSL");
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
    for (i = 0; i <= 9; i++)
    {
        modelData->d[i] = 1.0;
    }
    modelData->d[(10)] = 190000.0;
    // Initialize model code.
    for (i32 = 0; i32 <= 9; i32++)
    {
        modelData->x[(i32 + 10) * 1] = ((i32 + 1) - 1.0) * __PAR_L;
    }
    for (i = 0; i <= 9; i++)
    {
        modelData->event[i].direction = 0;
        modelData->event[i + 10].direction = 0;
    }
    modelData->event[20].direction = 1;
    double period[1];
    period[0] = 0.2;
    simulator->output = SD_Output ("train4DASSL", 1, 11, 30, period, 1, 0, CI_Sampled, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[20]++;
    SD_allocOutputMatrix (modelOutput, 30, 11);
    cleanVector (states, 0, 30);

    cleanVector (outputs, 0, 1);

    sprintf (modelOutput->variable[0].name, "vl[1]");
    cleanVector (outputs, 0, 1);

    modelOutput->SO[20][states[20]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 20;
    simulator->model = CLC_Model (MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
