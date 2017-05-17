#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>

#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_alpha = 0;
double __PAR_mu = 0;

void
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 0;
    settings->parallel = FALSE;
    settings->hybrid = FALSE;
    settings->method = 4;
}

void
MOD_definition (int i, double *x, double *d, double *alg, double t, double *dx)
{
    int j = 0;
    switch (i)
    {
        case 0:
            dx[1] = (-x[0] + 1.0) * 1000 - __PAR_mu * x[0] * (x[0] - __PAR_alpha) * (x[0] - 1.0);
            return;
        default:
            j = i;
            if (j >= 1 && j <= 999)
            {
                dx[1] = (-x[(j) * 3] + x[(j - 1) * 3]) * 1000 - __PAR_mu * x[(j) * 3] * (x[(j) * 3] - __PAR_alpha) * (x[(j) * 3] - 1.0);
            }
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    int j = 0;
    switch (i)
    {
        case 0:
            der[0 + 1] = (-x[0] + 1.0) * 1000 - __PAR_mu * x[0] * (x[0] - __PAR_alpha) * (x[0] - 1.0);
            break;
    }
    j = i + 1;
    if (j >= 1 && j <= 999)
    {
        der[(j) * 3 + 1] = (-x[(j) * 3] + x[(j - 1) * 3]) * 1000 - __PAR_mu * x[(j) * 3] * (x[(j) * 3] - __PAR_alpha) * (x[(j) * 3] - 1.0);
    }
    j = i;
    if (j >= 1 && j <= 999)
    {
        der[(j) * 3 + 1] = (-x[(j) * 3] + x[(j - 1) * 3]) * 1000 - __PAR_mu * x[(j) * 3] * (x[(j) * 3] - __PAR_alpha) * (x[(j) * 3] - 1.0);
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = x[2997];
            return;
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *outputs = (int*) malloc (1 * sizeof(int));
    int *states = (int*) malloc (1000 * sizeof(int));
    int i0;
    int i;
    int j = 0;
    simulator->data = QSS_Data (1000, 0, 0, 0, 0, "advection");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_alpha = 5.000000000000000000000000e-01;
    __PAR_mu = 1000.0;
    // Initialize model code.
    for (i0 = 0; i0 <= 332; i0++)
    {
        modelData->x[(i0) * 3] = 1.0;
    }
    modelData->nDS[0] = 1;
    for (i = 1; i <= 999; i++)
    {
        modelData->nDS[i] = 2;
    }
    modelData->nSD[0]++;
    for (i = 1; i <= 999; i++)
    {
        modelData->nSD[i - 1]++;
        modelData->nSD[i]++;
    }
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    cleanVector (states, 0, 1000);

    modelData->DS[0][states[0]++] = 0;
    for (i = 1; i <= 999; i++)
    {
        modelData->DS[i][states[i]++] = i - 1;
        modelData->DS[i][states[i]++] = i;
    }
    cleanVector (states, 0, 1000);

    modelData->SD[0][states[0]++] = 0;
    for (i = 1; i <= 999; i++)
    {
        modelData->SD[i - 1][states[i - 1]++] = i;
        modelData->SD[i][states[i]++] = i;
    }
    simulator->time = QSS_Time (1000, 0, 0, 0, ST_Binary, NULL);

    simulator->output = SD_Output ("advection", 1, 0, 1000, NULL, 0, 0, CI_Step, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[999]++;
    SD_allocOutputMatrix (modelOutput, 1000, 0);
    cleanVector (states, 0, 1000);

    cleanVector (outputs, 0, 1);

    sprintf (modelOutput->variable[0].name, "u[1000]");
    cleanVector (outputs, 0, 1);

    modelOutput->SO[999][states[999]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 999;
    simulator->model = QSS_Model (MOD_definition, MOD_dependencies, NULL, NULL, NULL);
    free (outputs);
    free (states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
