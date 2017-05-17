#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>

#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;

void
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 0;
    settings->parallel = FALSE;
    settings->hybrid = FALSE;
    settings->method = 8;
}

void
MOD_definition (double *x, double *d, double *alg, double t, double *dx)
{
    modelData->scalarEvaluations++;
    dx[0] = 1.000000000000000055511151e-01 * x[0] - 1.000000000000000055511151e-01 * x[0] * x[1];
    dx[1] = 1.000000000000000055511151e-01 * x[0] * x[1] - 1.000000000000000055511151e-01 * x[1];
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    int j = 0;
    j = i;
    if (j >= 0 && j <= 1)
    {
        out[0] = x[(j) * 1];
    }
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
    int i = 0;
    int outputs[2];
    int states[2];
    simulator->data = CLC_Data (2, 0, 0, 0, 0, "lotka_volterraDOPRI");
    modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    // Initialize model code.
    modelData->x[0] = 5.000000000000000000000000e-01;
    modelData->x[1] = 5.000000000000000000000000e-01;
    double period[1];
    period[0] = 0.06;
    simulator->output = SD_Output ("lotka_volterraDOPRI", 2, 0, 2, period, 1, 0, CI_Sampled, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    for (i = 0; i <= 1; i++)
    {
        modelOutput->nOS[i] = 1;
        modelOutput->nSO[i]++;
    }
    SD_allocOutputMatrix (modelOutput, 2, 0);
    cleanVector (states, 0, 2);

    cleanVector (outputs, 0, 2);

    for (i = 0; i <= 1; i++)
    {
        sprintf (modelOutput->variable[i].name, "x[%d]", i + 1);
    }
    cleanVector (outputs, 0, 2);

    for (i = 0; i <= 1; i++)
    {
        modelOutput->SO[i][states[i]++] = i;
        modelOutput->OS[i][outputs[i]++] = i;
    }
    simulator->model = CLC_Model (MOD_definition, NULL, NULL, NULL);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
