/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "qss_lp.h"

#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"

LP_partitionInfo
LP_PartitionInfo (int states, int handlers, int out)
{
    LP_partitionInfo p = checkedMalloc (sizeof(*p));
    p->stateInput = (int*) checkedMalloc (states * sizeof(int));
    p->handlerInput = (int*) checkedMalloc (handlers * sizeof(int));
    p->output = (int*) checkedMalloc (out * sizeof(int));
    p->stateReinits = (int*) checkedMalloc (states * sizeof(int));
    cleanVector (p->stateInput, NOT_ASSIGNED, states);
    cleanVector (p->handlerInput, NOT_ASSIGNED, handlers);
    cleanVector (p->output, NOT_ASSIGNED, out);
    cleanVector (p->stateReinits, 0, states);
    p->states = 0;
    p->handlers = 0;
    p->outputs = 0;
    p->stateInputs = 0;
    p->handlerInputs = 0;
    p->lastHandler = 0;
    p->lastState = 0;
    p->nLPS = 0;
    p->lps = 0;
    return (p);
}

void
LP_freePartitionInfo (LP_partitionInfo pi)
{
    free (pi->stateInput);
    free (pi->handlerInput);
    free (pi->output);
    free (pi->stateReinits);
    free (pi);
}

LP_partitionInfo *
LP_PartitionInfoArray (int lps, int states, int handlers, int out)
{
    LP_partitionInfo *pi = (LP_partitionInfo*) checkedMalloc (lps * (sizeof(*pi)));
    int i;
    for (i = 0; i < lps; i++)
        {
            pi[i] = LP_PartitionInfo (states, handlers, out);
        }
    return (pi);
}

void
LP_freePartitionInfoArray (LP_partitionInfo *pi, int lps)
{
    int i;
    for (i = 0; i < lps; i++)
        {
            LP_freePartitionInfo (pi[i]);
        }
    free (pi);
}

bool
LP_find (int *vector, int size, int value)
{
    int i;
    for (i = 0; i < size; i++)
        {
            if (vector[i] == value)
                {
                    return (TRUE);
                }
        }
    return (FALSE);
}

LP_partitionInfo *
LP_processPartition (QSS_simulator simulator, PRT_partition partition)
{
    /* Local mappings */
    QSS_data simulatorData = simulator->data;
    int outputs = simulator->output->outputs;
    int lps = simulator->data->params->lps;
    int beginStates = partition->beginStates;
    int beginHandlers = partition->beginHandlers;
    int endStates = partition->endStates;
    int endHandlers = partition->endHandlers;
    LP_partitionInfo *pi = LP_PartitionInfoArray (lps, endStates - beginStates, endHandlers - beginHandlers, outputs);
    int i;
    for (i = beginStates; i < endStates; i++)
        {
            int partitionNumber = partition->values[i];
            LP_partitionInfo p = pi[partitionNumber];
            p->states++;
            int nSD = simulatorData->nSD[i];
            int j;
            for (j = 0; j < nSD; j++)
                {
                    int k = simulatorData->SD[i][j];
                    int derPartitionNumber = partition->values[k];
                    LP_partitionInfo derivativePartition = pi[derPartitionNumber];
                    if (derPartitionNumber != partitionNumber && derivativePartition->stateInput[derivativePartition->lastState] != i)
                        {
                            derivativePartition->lastState = derivativePartition->stateInputs;
                            derivativePartition->stateInput[derivativePartition->stateInputs++] = i;
                            partition->outputs[i][partition->nOutputs[i]++] = derPartitionNumber;
                        }
                }
            if (simulatorData->nSZ != NULL)
                {
                    int nSZ = simulatorData->nSZ[i];
                    for (j = 0; j < nSZ; j++)
                        {
                            int k = simulatorData->SZ[i][j];
                            int zcPartitionNumber = partition->values[beginHandlers + k];
                            LP_partitionInfo zcPartition = pi[zcPartitionNumber];
                            if (zcPartitionNumber != partitionNumber && zcPartition->stateInput[zcPartition->lastState] != i)
                                {
                                    zcPartition->lastState = zcPartition->stateInputs;
                                    zcPartition->stateInput[zcPartition->stateInputs++] = i;
                                    partition->outputs[i][partition->nOutputs[i]++] = zcPartitionNumber;
                                }
                        }
                }
            if (partition->nOutputs[i] > 0)
                {
                    p->nLPS++;
                    p->lps += partition->nOutputs[i];
                }
        }
    int handlerNumber = 0;
    bool evalRHS = FALSE;
    for (i = beginHandlers; i < endHandlers; i++)
        {
            int partitionNumber = partition->values[i];
            LP_partitionInfo p = pi[partitionNumber];
            p->handlers++;
            int j;
            int nHD = simulatorData->nHD[handlerNumber];
            for (j = 0; j < nHD; j++)
                {
                    int k = simulatorData->HD[handlerNumber][j];
                    int derPartitionNumber = partition->values[k];
                    LP_partitionInfo derivativePartition = pi[derPartitionNumber];
                    if (derPartitionNumber != partitionNumber && derivativePartition->handlerInput[derivativePartition->lastHandler] != handlerNumber)
                        {
                            derivativePartition->lastHandler = derivativePartition->handlerInputs;
                            derivativePartition->handlerInput[derivativePartition->handlerInputs++] = handlerNumber;
                            partition->outputs[i][partition->nOutputs[i]++] = derPartitionNumber;
                        }
                }
            int nDD = simulatorData->nDD[handlerNumber];
            for (j = 0; j < nDD; j++)
                {
                    int k = simulatorData->DD[handlerNumber][j];
                    int hndPartitionNumber = partition->values[beginHandlers + k];
                    LP_partitionInfo hndPartition = pi[hndPartitionNumber];
                    if (hndPartitionNumber != partitionNumber && hndPartition->handlerInput[hndPartition->lastHandler] != handlerNumber)
                        {
                            hndPartition->lastHandler = hndPartition->handlerInputs;
                            hndPartition->handlerInput[hndPartition->handlerInputs++] = handlerNumber;
                            partition->outputs[i][partition->nOutputs[i]++] = hndPartitionNumber;
                        }
                }
            int nHZ = simulatorData->nHZ[handlerNumber];
            for (j = 0; j < nHZ; j++)
                {
                    int k = simulatorData->HZ[handlerNumber][j];
                    int zcPartitionNumber = partition->values[beginHandlers + k];
                    LP_partitionInfo zcPartition = pi[zcPartitionNumber];
                    if (zcPartitionNumber != partitionNumber && zcPartition->handlerInput[zcPartition->lastHandler] != handlerNumber)
                        {
                            zcPartition->lastHandler = zcPartition->handlerInputs;
                            zcPartition->handlerInput[zcPartition->handlerInputs++] = handlerNumber;
                            partition->outputs[i][partition->nOutputs[i]++] = zcPartitionNumber;
                            int t;
                            int inZS = simulatorData->nZS[handlerNumber];
                            for (t = 0; t < inZS; t++)
                                {
                                    int ik = simulatorData->ZS[handlerNumber][t];
                                    if (partition->asgDscInf[partitionNumber][ik] <= NOT_ASSIGNED)
                                        {
                                            partition->dscInf[partitionNumber][partition->nDsc[partitionNumber]++] = ik;
                                            partition->asgDscInf[partitionNumber][ik] = ik;
                                        }
                                }
                        }
                }
            int nLHSSt = simulatorData->event[handlerNumber].nLHSSt;
            for (j = 0; j < nLHSSt; j++)
                {
                    int k = simulatorData->event[handlerNumber].LHSSt[j];
                    int derPartitionNumber = partition->values[k];
                    LP_partitionInfo derivativePartition = pi[derPartitionNumber];
                    if (derPartitionNumber != partitionNumber && derivativePartition->handlerInput[derivativePartition->lastHandler] != handlerNumber)
                        {
                            derivativePartition->lastHandler = derivativePartition->handlerInputs;
                            derivativePartition->handlerInput[derivativePartition->handlerInputs++] = handlerNumber;
                            partition->outputs[i][partition->nOutputs[i]++] = derPartitionNumber;
                        }
                    else if (derPartitionNumber == partitionNumber)
                        {
                            int nSD = simulatorData->nSD[k];
                            int h;
                            for (h = 0; h < nSD; h++)
                                {
                                    int l = simulatorData->SD[k][h];
                                    int derPartitionNumber = partition->values[l];
                                    LP_partitionInfo derivativePartition = pi[derPartitionNumber];
                                    if (derPartitionNumber != partitionNumber
                                            && derivativePartition->handlerInput[derivativePartition->lastHandler] != handlerNumber)
                                        {
                                            derivativePartition->lastHandler = derivativePartition->handlerInputs;
                                            derivativePartition->handlerInput[derivativePartition->handlerInputs++] = handlerNumber;
                                            partition->outputs[i][partition->nOutputs[i]++] = derPartitionNumber;
                                        }
                                }
                        }
                }
            int nRHSSt = simulatorData->event[handlerNumber].nRHSSt;
            for (j = 0; j < nRHSSt; j++)
                {
                    int k = simulatorData->event[handlerNumber].RHSSt[j];
                    int derPartitionNumber = partition->values[k];
                    LP_partitionInfo derivativePartition = pi[derPartitionNumber];
                    if (derPartitionNumber != partitionNumber)
                        {
                            if (!LP_find (p->stateInput, p->stateInputs, k))
                                {
                                    p->lastState = p->stateInputs;
                                    p->stateInput[p->stateInputs++] = k;
                                    if (partition->nOutputs[k] == 0)
                                        {
                                            derivativePartition->nLPS++;
                                        }
                                    derivativePartition->lps++;
                                    partition->outputs[k][partition->nOutputs[k]++] = partitionNumber;
                                }
                            evalRHS = TRUE;
                        }
                }
            if (partition->nOutputs[i] > 0)
                {
                    p->nLPS++;
                    p->lps += partition->nOutputs[i];
                }
            handlerNumber++;
        }
    if (evalRHS)
        {
            handlerNumber = 0;
            int j;
            for (i = beginHandlers; i < endHandlers; i++)
                {
                    int partitionNumber = partition->values[i];
                    LP_partitionInfo p = pi[partitionNumber];
                    int nRHSSt = simulatorData->event[handlerNumber].nRHSSt;
                    for (j = 0; j < nRHSSt; j++)
                        {
                            int k = simulatorData->event[handlerNumber].RHSSt[j];
                            int derPartitionNumber = partition->values[k];
                            if (derPartitionNumber != partitionNumber)
                                {
                                    int nSH = simulatorData->nSH[k], it;
                                    for (it = 0; it < nSH; it++)
                                        {
                                            int hr = simulatorData->SH[k][it];
                                            int handlerPartitionNumber = partition->values[beginHandlers + hr];
                                            if (handlerPartitionNumber != partitionNumber && !LP_find (p->handlerInput, p->handlerInputs, hr))
                                                {
                                                    LP_partitionInfo handlerPartition = pi[handlerPartitionNumber];
                                                    p->lastHandler = p->handlerInputs;
                                                    p->handlerInput[p->handlerInputs++] = hr;
                                                    if (partition->nOutputs[beginHandlers + hr] == 0)
                                                        {
                                                            handlerPartition->nLPS++;
                                                        }
                                                    handlerPartition->lps++;
                                                    partition->outputs[beginHandlers + hr][partition->nOutputs[beginHandlers + hr]++] = partitionNumber;
                                                }
                                        }
                                }
                        }
                    handlerNumber++;
                }
        }
    for (i = 0; i < outputs; i++)
        {
            int nOS = simulator->output->nOS[i];
            int j;
            for (j = 0; j < nOS; j++)
                {
                    int k = simulator->output->OS[i][j];
                    LP_partitionInfo derivativePartition = pi[partition->values[k]];
                    derivativePartition->output[derivativePartition->outputs++] = i;
                }
            int nOD = simulator->output->nOD[i];
            for (j = 0; j < nOD; j++)
                {
                    int k = simulator->output->OD[i][j];
                    int g, nDH = simulator->data->nDH[k];
                    for (g = 0; g < nDH; g++)
                        {
                            int h = simulator->data->DH[k][g];
                            LP_partitionInfo handlerPartition = pi[partition->values[partition->beginHandlers + h]];
                            if (!LP_find (handlerPartition->output, handlerPartition->outputs, i))
                                {
                                    handlerPartition->output[handlerPartition->outputs++] = i;
                                }

                        }
                }
        }
#ifdef DEBUG
    for (i = 0; i < lps; i++)
        {
            int j;
            printf("Partition %d\n", i);
            printf("States %d\n", pi[i]->states);
            printf("State Inputs %d\n", pi[i]->stateInputs);
            for (j = 0; j < pi[i]->stateInputs; j++)
                {
                    printf("State Input %d = %d\n",j, pi[i]->stateInput[j]);
                }
            printf("Handlers %d\n", pi[i]->handlers);
            printf("Handler Inputs %d\n", pi[i]->handlerInputs);
            for (j = 0; j < pi[i]->handlerInputs; j++)
                {
                    printf("Handler Input %d = %d\n",j, pi[i]->handlerInput[j]);
                }
            printf("Outputs %d\n", pi[i]->outputs);
            printf("\n");
        }
#endif
    return (pi);
}

void
LP_initializeDataStructs (QSS_simulator simulator, PRT_partition partition)
{
    /* Local mappings */
    QSS_data simulatorData = simulator->data;
    int lps = simulatorData->params->lps;
    int outputs = simulator->output->outputs;
    int states = simulatorData->states;
    int events = simulatorData->events;
    int beginStates = partition->beginStates;
    int beginHandlers = partition->beginHandlers;
    int endStates = partition->endStates;
    int endHandlers = partition->endHandlers;
    int i;
    int *outputMap = (int*) checkedMalloc (lps * sizeof(int));
    int *nLPSMap = (int*) checkedMalloc (lps * sizeof(int));
    cleanVector (outputMap, 0, lps);
    cleanVector (nLPSMap, 0, lps);
    LP_partitionInfo *pi = LP_processPartition (simulator, partition);
    bool autonomous;
    for (i = 0; i < lps; i++)
        {
            simulator->lps->lp[i] = QSS_LP_Data (pi[i]->states, pi[i]->handlers, 0, pi[i]->outputs, pi[i]->stateInputs, pi[i]->handlerInputs, states, events,
                                                 outputs, pi[i]->nLPS, pi[i]->lps, i);
        }

    for (i = beginStates; i < endStates; i++)
        {
            int partitionNumber = partition->values[i];
            int nOutputs = partition->nOutputs[i];
            if (nOutputs > 0)
                {
                    int lpsInit = simulator->lps->lp[partitionNumber]->nLPS[nLPSMap[partitionNumber]];
                    simulator->lps->lp[partitionNumber]->qMap[i] = nLPSMap[partitionNumber];
                    simulator->lps->lp[partitionNumber]->nLPS[nLPSMap[partitionNumber] + 1] = lpsInit + nOutputs;
                    int j;
                    for (j = 0; j < nOutputs; j++)
                        {
                            int partitionOutput = partition->outputs[i][j];
                            simulator->lps->lp[partitionNumber]->lps[lpsInit + j] = partitionOutput;
                        }
                    nLPSMap[partitionNumber]++;
                }
            else
                {
                    simulator->lps->lp[partitionNumber]->qMap[i] = ASSIGNED;
                }
            simulator->lps->lp[partitionNumber]->qOutMap[outputMap[partitionNumber]++] = i;
        }
    autonomous = TRUE;
    for (i = 0; i < lps; i++)
        {
            simulator->lps->lp[i]->outStates = nLPSMap[i];
            if (nLPSMap[i] > 0)
                {
                    autonomous = FALSE;
                }
        }
    if (autonomous == TRUE && simulatorData->params->dtSynch == SD_DT_Adaptive)
        {
            simulatorData->params->dtSynch = SD_DT_AdaptiveDiscrete;
        }
    cleanVector (outputMap, 0, lps);
    int handlerNumber = 0;
    for (i = beginHandlers; i < endHandlers; i++)
        {
            int nOutputs = partition->nOutputs[i];
            int partitionNumber = partition->values[i];
            if (nOutputs > 0)
                {
                    int lpsInit = simulator->lps->lp[partitionNumber]->nLPS[nLPSMap[partitionNumber]];
                    simulator->lps->lp[partitionNumber]->eMap[handlerNumber] = nLPSMap[partitionNumber];
                    simulator->lps->lp[partitionNumber]->nLPS[nLPSMap[partitionNumber] + 1] = lpsInit + nOutputs;
                    int j;
                    for (j = 0; j < nOutputs; j++)
                        {
                            int partitionOutput = partition->outputs[i][j];
                            simulator->lps->lp[partitionNumber]->lps[lpsInit + j] = partitionOutput;
                        }
                    nLPSMap[partitionNumber]++;
                }
            else
                {
                    simulator->lps->lp[partitionNumber]->eMap[handlerNumber] =
                    ASSIGNED;
                }
            simulator->lps->lp[partitionNumber]->eOutMap[outputMap[partitionNumber]++] = handlerNumber;
            handlerNumber++;
        }
    if (simulatorData->params->dtSynch == SD_DT_AdaptiveDiscrete)
        {
            bool noComm[MAX_LPS];
            for (i = 0; i < lps; i++)
                {
                    simulator->lps->lp[i]->dscMap = (QSS_idxMap) checkedMalloc (simulatorData->states * sizeof(int));
                    cleanVector (simulator->lps->lp[i]->dscMap, NOT_ASSIGNED, simulatorData->states);
                    int nDscInf = partition->nDsc[i];
                    noComm[i] = FALSE;
                    if (nDscInf > 0)
                        {
                            noComm[i] = TRUE;
                        }
                    simulator->lps->lp[i]->dscInf = nDscInf;
                    int t;
                    for (t = 0; t < nDscInf; t++)
                        {
                            simulator->lps->lp[i]->dscMap[partition->dscInf[i][t]] = t;
                        }
                }
            for (i = 0; i < lps; i++)
                {
                    if (noComm[i] == TRUE)
                        {
                            simulator->lps->lp[i]->dscInf = 1;
                            int asg = 0;
                            while (simulator->lps->lp[i]->qMap[asg] <= NOT_ASSIGNED)
                                {
                                    asg++;
                                }
                            simulator->lps->lp[i]->dscMap[0] = asg;
                            SD_print (simulator->simulationLog, "LP%d: Controlling state variable: %d", i, asg);
                        }
                }
        }
    autonomous = TRUE;
    for (i = 0; i < lps; i++)
        {
            LP_partitionInfo p = pi[i];
            QSS_LP_data lp = simulator->lps->lp[i];
            int stateInputs = p->stateInputs;
            int handlerInputs = p->handlerInputs;
            int partitionOutput = p->outputs;
            int j;
            if (lp->inStates || lp->inEvents)
                {
                    autonomous = FALSE;
                }
            for (j = 0; j < stateInputs; j++)
                {
                    lp->qInMap[j] = partition->values[p->stateInput[j]];
                    lp->qMap[p->stateInput[j]] = ASSIGNED_INPUT - j;
                }
            for (j = 0; j < handlerInputs; j++)
                {
                    lp->eInMap[j] = partition->values[beginHandlers + p->handlerInput[j]];
                }
            for (j = 0; j < partitionOutput; j++)
                {
                    lp->oMap[p->output[j]] = j;
                }
        }
    for (i = 0; i < lps; i++)
        {
            QSS_LP_data lp = simulator->lps->lp[i];
            if (autonomous == TRUE)
                {
                    lp->initDt = INF;
                }
            else if (simulatorData->params->dtSynch == SD_DT_Fixed)
                {
                    lp->initDt = simulatorData->params->dt;
                }
            else
                {
                    lp->initDt = 0;
                }
        }
    free (outputMap);
    free (nLPSMap);
    LP_freePartitionInfoArray (pi, lps);
}
