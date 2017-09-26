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

#include <qss/qss_binary.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/tree.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_scheduler.h"

#ifdef QSS_PARALLEL
#define BT_UPDATE_FN BT_PAR_update
#define BT_UPDATE_HI_FN BT_PAR_updateHI
#define BT_UPDATE_H_FN BT_PAR_updateH
#define BT_UPDATE_I_FN BT_PAR_updateI
#define BT_UPDATE_C_FN BT_PAR_updateC
#define BT_INIT_TREE(t) (BT_PAR_initTree(t))
#define BT_UPDATE_TREE(s, t, i, c,idx) (BT_PAR_updateTree (s, t, i, c,idx))
#else
#define BT_UPDATE_FN BT_update
#define BT_UPDATE_HI_FN BT_updateHI
#define BT_UPDATE_H_FN BT_updateH
#define BT_UPDATE_I_FN BT_updateI
#define BT_UPDATE_C_FN BT_updateC
#define BT_INIT_TREE(t) (BT_initTree(t))
#define BT_UPDATE_TREE(s, t, i, c,idx) (BT_updateTree (s, t, i, c,idx))
#endif

/**
 * @brief Initialize the BT_tree data structure and calculate the minimum.
 *
 * This procedure initializes each node of the tree.
 *
 * @param tree BT_tree data structure
 */

#ifdef QSS_PARALLEL
void
BT_PAR_initTree (BT_tree tree)
#else
void
BT_initTree(BT_tree tree)
#endif
{
  if(tree->size == 1)
  {
#ifdef QSS_PARALLEL
    tree->nodes[0][0].value = tree->map[0];
#else
    tree->nodes[0][0].value = 0;
#endif
    tree->nodes[0][0].father = 0;
    tree->nodes[0][0].size = 1;
    tree->father[0] = 0;
    return;
  }
  unsigned int j, i, size, rightChild, leftChild;
  int k;
  int *numberOfNodes;
  if(tree->size == 2 || tree->size == 3)
  {
    numberOfNodes = checkedMalloc(sizeof(int));
    numberOfNodes[0] = 0;
  }
  else
  {
    numberOfNodes = checkedMalloc((tree->height + 1) * sizeof(int));
  }
  size = DIV2(tree->size);
  for(j = 0; j < tree->size; j++)
  {
#ifdef QSS_PARALLEL
    tree->father[tree->map[j]] = DIV2(j);
#else
    tree->father[j] = DIV2(j);
#endif
  }
  if(tree->size & ODD)
  {
#ifdef QSS_PARALLEL
    tree->father[tree->map[tree->size - 1]]--;
#else
    tree->father[tree->size - 1]--;
#endif
  }
  if(tree->height)
  {
    for(j = 0; j <= tree->height; j++)
    {
      numberOfNodes[j] = size - 1;
      size = DIV2(size);
    }
  }
  for(j = 0; j <= numberOfNodes[0]; j++)
  {
#ifdef QSS_PARALLEL
    leftChild = tree->map[MUL2(j)];
    rightChild = tree->map[MUL2(j) + 1];
#else
    leftChild = MUL2(j);
    rightChild = MUL2(j) + 1;
#endif
    tree->nodes[0][j].size = 2;
    tree->nodes[0][j].father = DIV2(j);
    tree->nodes[0][j].value = MIN(tree->leaves[leftChild], leftChild,
        tree->leaves[rightChild], rightChild);
  }
  if((numberOfNodes[0] + 1) & ODD)
  {
    tree->nodes[0][numberOfNodes[0]].father--;
  }
  if(tree->size & ODD)
  {
    tree->nodes[0][numberOfNodes[0]].size++;
#ifdef QSS_PARALLEL
    rightChild = tree->map[MUL2(numberOfNodes[0]) + 2];
#else
    rightChild = MUL2(numberOfNodes[0]) + 2;
#endif
    k = tree->nodes[0][numberOfNodes[0]].value;
    tree->nodes[0][numberOfNodes[0]].value = MIN(tree->leaves[k], k,
        tree->leaves[rightChild], rightChild);
  }
  if(tree->height)
  {
    for(i = 1; i <= tree->height; i++)
    {
      for(j = 0; j <= numberOfNodes[i]; j++)
      {
        leftChild = tree->nodes[i - 1][MUL2(j)].value;
        rightChild = tree->nodes[i - 1][MUL2(j) + 1].value;
        tree->nodes[i][j].size = 2;
        tree->nodes[i][j].father = DIV2(j);
        tree->nodes[i][j].value = MIN(tree->leaves[leftChild], leftChild,
            tree->leaves[rightChild], rightChild);
      }
      if((numberOfNodes[i] + 1) & ODD)
      {
        tree->nodes[i][numberOfNodes[i]].father--;
      }
      if((numberOfNodes[i - 1] + 1) & ODD)
      {
        tree->nodes[i][numberOfNodes[i]].size++;
        rightChild = tree->nodes[i - 1][MUL2(numberOfNodes[i]) + 2].value;
        k = tree->nodes[i][numberOfNodes[i]].value;
        tree->nodes[i][numberOfNodes[i]].value = MIN(tree->leaves[k], k,
            tree->leaves[rightChild], rightChild);
      }
    }
  }
  free(numberOfNodes);
}

/**
 * @brief Update a given BT_tree dtata structure.
 *
 * @param scheduler
 * @param tree
 * @param inf
 * @param cant
 * @param idx
 */

#ifdef QSS_PARALLEL
void
BT_PAR_updateTree (SC_scheduler scheduler, BT_tree tree, int *inf, int cant,
    int idx)
#else
void
BT_updateTree(SC_scheduler scheduler, BT_tree tree, int *inf, int cant, int idx)
#endif
{
  if(tree->size == 1)
  {
    return;
  }
  int vars, nodes;
  unsigned int j, i, updateVar, minIdx;
  int **fathers = scheduler->state->info->fathers;
#ifdef QSS_PARALLEL
  nodes = 0;
  fathers[0][0] = NOT_ASSIGNED;
  QSS_LP_data lp = scheduler->state->lp;
#else
  nodes = 1;
#endif
  if(idx >= 0)
  {
    fathers[0][0] = tree->nodes[0][tree->father[idx]].father;
    updateVar = tree->father[idx];
#ifdef QSS_PARALLEL
    minIdx = tree->map[MUL2(updateVar)];
    int right = tree->map[MUL2(updateVar)+1];
#else
    minIdx = MUL2(updateVar);
    int right = minIdx + 1;
#endif
    minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[right], right);
    if(tree->nodes[0][updateVar].size & ODD)
    {
#ifdef QSS_PARALLEL
      int last = tree->map[MUL2(updateVar) + 2];
#else
      int last = MUL2(updateVar) + 2;
#endif
      minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[last], last);
    }
    tree->nodes[0][updateVar].value = minIdx;
  }
#ifdef QSS_PARALLEL
  else if (cant > 0)
  {
    switch (scheduler->state->stepType)
    {
      case ST_State:
      if (lp->qMap[inf[0]] > NOT_ASSIGNED)
      {
        fathers[0][0] = tree->nodes[0][tree->father[inf[0]]].father;
      }
      break;
      case ST_Event:
      if (lp->eMap[inf[0]] > NOT_ASSIGNED)
      {
        fathers[0][0] = tree->nodes[0][tree->father[inf[0]]].father;
      }
      break;
      default:
      break;
    }
  }
#else
  else
  {
    fathers[0][0] = tree->nodes[0][tree->father[inf[0]]].father;
  }
#endif
  vars = cant;
  for(i = 0; i < vars; i++)
  {
#ifdef QSS_PARALLEL
    bool eval = FALSE;
    switch (scheduler->state->stepType)
    {
      case ST_State:
      if (lp->qMap[inf[i]] > NOT_ASSIGNED)
      {
        eval = TRUE;
      }
      break;
      case ST_Event:
      if (lp->eMap[inf[i]] > NOT_ASSIGNED)
      {
        eval = TRUE;
      }
      break;
      default:
      break;
    }
    if (eval)
    {
#endif
    updateVar = tree->father[inf[i]];
#ifdef QSS_PARALLEL
    minIdx = tree->map[MUL2(updateVar)];
    int right = tree->map[MUL2(updateVar)+1];
#else
    minIdx = MUL2(updateVar);
    int right = minIdx + 1;
#endif
    minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[right], right);
    if(tree->nodes[0][updateVar].size & ODD)
    {
#ifdef QSS_PARALLEL
      int last = tree->map[MUL2(updateVar) + 2];
#else
      int last = MUL2(updateVar) + 2;
#endif
      minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[last], last);
    }
    tree->nodes[0][updateVar].value = minIdx;
#ifdef QSS_PARALLEL
    if (fathers[0][0] <= NOT_ASSIGNED)
    {
      fathers[0][0] = tree->nodes[0][updateVar].father;
    }
    else if (tree->nodes[0][updateVar].father != fathers[0][nodes])
    {
      fathers[0][++nodes] = tree->nodes[0][updateVar].father;
    }
  }
#else
    if(tree->nodes[0][updateVar].father != fathers[0][nodes - 1])
    {
      fathers[0][nodes] = tree->nodes[0][updateVar].father;
      nodes++;
    }
#endif
  }
#ifdef QSS_PARALLEL
  if (fathers[0][0] <= NOT_ASSIGNED)
  {
    return;
  }
  nodes++;
#endif
  for(j = 1; j <= tree->height; j++)
  {
    vars = nodes;
    nodes = 1;
    fathers[j][0] = tree->nodes[j][fathers[j - 1][0]].father;
    for(i = 0; i < vars; i++)
    {
      updateVar = fathers[j - 1][i];
      minIdx = MUL2(updateVar);
      minIdx = MIN(tree->leaves[tree->nodes[j - 1][minIdx].value],
          tree->nodes[j - 1][minIdx].value,
          tree->leaves[tree->nodes[j - 1][minIdx + 1].value],
          tree->nodes[j - 1][minIdx + 1].value);
      if(tree->nodes[j][updateVar].size & ODD)
      {
        int last = MUL2(updateVar) + 2;
        minIdx = MIN(tree->leaves[minIdx], minIdx,
            tree->leaves[tree->nodes[j - 1][last].value],
            tree->nodes[j - 1][last].value);
      }
      tree->nodes[j][updateVar].value = minIdx;
      if(tree->nodes[j][updateVar].father != fathers[j][nodes - 1])
      {
        fathers[j][nodes] = tree->nodes[j][updateVar].father;
        nodes++;
      }
    }
  }
}

#ifdef QSS_PARALLEL
void
BT_PAR_updateC (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
BT_updateC(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  int w = ST_State, minIdx = -1, stateVar;
  double minVal = INF;
  stateVar = simTime->minIndex;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = scheduler->state->lp;
  scheduler->state->stepType = ST_State;
  if (simTime->type == ST_State)
  {
    int update = stateVar;
    if (lp->qMap[stateVar] <= NOT_ASSIGNED)
    {
      update = UPDATE_DEPS;
    }
    BT_UPDATE_TREE (scheduler, scheduler->state->states, simData->SD[stateVar],
        simData->nSD[stateVar], update);
    minIdx =
    scheduler->state->states->nodes[scheduler->state->states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
  }
  else if (simTime->type == ST_Event)
  {
    if (simData->nHD[stateVar])
    {
      BT_UPDATE_TREE (scheduler, scheduler->state->states, simData->HD[stateVar],
          simData->nHD[stateVar], UPDATE_DEPS);
    }
    if (simData->event[stateVar].nLHSSt)
    {
      int i,j, nLHSSt = simData->event[stateVar].nLHSSt;
      for (i = 0; i < nLHSSt; i++)
      {
        j = simData->event[stateVar].LHSSt[i];
        if (lp->qMap[j] > NOT_ASSIGNED)
        {
          BT_UPDATE_TREE (scheduler, scheduler->state->states, simData->SD[j],
              simData->nSD[j], j);
        }
      }
    }
  }
#else
  BT_UPDATE_TREE(scheduler, scheduler->state->states, simData->SD[stateVar],
      simData->nSD[stateVar], stateVar);
  minIdx =
      scheduler->state->states->nodes[scheduler->state->states->height][0].value;
  minVal = simTime->nextStateTime[minIdx];
#endif
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

#ifdef QSS_PARALLEL
void
BT_PAR_updateH (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
BT_updateH(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  int w = ST_State, minIdx = -1, stateVar, update, nStates = simData->states;
  double minVal = INF;
  BT_tree states = scheduler->state->states;
  BT_tree events = scheduler->state->events;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = scheduler->state->lp;
  nStates = lp->states;
  simTime->noReinit = 1;
#endif
  if(simTime->type == ST_State)
  {
    stateVar = simTime->minIndex;
    update = simTime->minIndex;
#ifdef QSS_PARALLEL
    if (nStates > 0)
    {
      if (lp->qMap[stateVar] <= NOT_ASSIGNED)
      {
        update = UPDATE_DEPS;
      }
      scheduler->state->stepType = ST_State;
#endif
    BT_UPDATE_TREE(scheduler, states, simData->SD[stateVar],
        simData->nSD[stateVar], update);
    minIdx = states->nodes[states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
#ifdef QSS_PARALLEL
  }
#endif
    if(simData->nSZ[stateVar] && events->size != 1)
    {
#ifdef QSS_PARALLEL
      scheduler->state->stepType = ST_Event;
#endif
      BT_UPDATE_TREE(scheduler, events, simData->SZ[stateVar],
          simData->nSZ[stateVar], UPDATE_DEPS);
    }
    if(simTime->nextEventTime[events->nodes[events->height][0].value] < minVal)
    {
      minIdx = events->nodes[events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
  }
  else if(simTime->type == ST_Event)
  {
    stateVar = simTime->minIndex;
    if(nStates > 0)
    {
      if(simData->nHD[stateVar])
      {
#ifdef QSS_PARALLEL
        scheduler->state->stepType = ST_State;
#endif
        BT_UPDATE_TREE(scheduler, states, simData->HD[stateVar],
            simData->nHD[stateVar], UPDATE_DEPS);
      }
      if(simData->event[stateVar].nLHSSt)
      {
        int i, j;
        int nLHSSt = simData->event[stateVar].nLHSSt;
        for(i = 0; i < nLHSSt; i++)
        {
          j = simData->event[stateVar].LHSSt[i];
          update = j;
#ifdef QSS_PARALLEL
          if (lp->qMap[j] <= NOT_ASSIGNED)
          {
            update = UPDATE_DEPS;
          }
          if (lp->states > 0)
          {
            scheduler->state->stepType = ST_State;
#endif
          BT_UPDATE_TREE(scheduler, states, simData->SD[j], simData->nSD[j],
              update);
#ifdef QSS_PARALLEL
        }
#endif
        }
      }
      minIdx = states->nodes[states->height][0].value;
      minVal = simTime->nextStateTime[minIdx];
    }
    update = stateVar;
#ifdef QSS_PARALLEL
    if (lp->eMap[stateVar] <= NOT_ASSIGNED)
    {
      update = UPDATE_DEPS;
    }
    scheduler->state->stepType = ST_Event;
#endif
    BT_UPDATE_TREE(scheduler, events, simData->HZ[stateVar],
        simData->nHZ[stateVar], update);
    if(simTime->nextEventTime[events->nodes[events->height][0].value] < minVal)
    {
      minIdx = events->nodes[events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
  }
#ifdef QSS_PARALLEL
  if (simTime->reinits->counter > 0)
  {
    simTime->reinits->counter--;
    simTime->minValue = simTime->reinits->time;
    simTime->minIndex = simTime->reinits->variable[simTime->reinits->counter];
    simTime->type = ST_State;
    simTime->time = simTime->minValue;
    simTime->noReinit = 0;
    return;
  }
#endif
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

#ifdef QSS_PARALLEL
void
BT_PAR_updateI (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
BT_updateI(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  BT_tree states = scheduler->state->states;
  BT_tree inputs = scheduler->state->inputs;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = scheduler->state->lp;
#endif
  int w = ST_State, minIdx = -1, stateVar, update;
  double minVal = INF;
  if(simTime->type == ST_State)
  {
    stateVar = simTime->minIndex;
    update = stateVar;
#ifdef QSS_PARALLEL
    if (lp->qMap[stateVar] <= NOT_ASSIGNED)
    {
      update = UPDATE_DEPS;
    }
    scheduler->state->stepType = ST_State;
#endif
    BT_UPDATE_TREE(scheduler, states, simData->SD[stateVar],
        simData->nSD[stateVar], update);
    minIdx = states->nodes[states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    if(simTime->nextInputTime[inputs->nodes[inputs->height][0].value] < minVal)
    {
      minIdx = inputs->nodes[inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  else if(simTime->type == ST_Input)
  {
#ifdef QSS_PARALLEL
    scheduler->state->stepType = ST_State;
#endif
    BT_UPDATE_TREE(scheduler, states, NULL, 0, simData->TD[simTime->minIndex]);
    minIdx = states->nodes[states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    BT_UPDATE_TREE(scheduler, inputs, NULL, 0, simTime->minIndex);
    if(simTime->nextInputTime[inputs->nodes[inputs->height][0].value] < minVal)
    {
      minIdx = inputs->nodes[inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

#ifdef QSS_PARALLEL
void
BT_PAR_updateHI (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
BT_updateHI(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  int w = ST_State, minIdx = -1, stateVar, update, nStates = simData->states;
  double minVal = INF;
  BT_tree states = scheduler->state->states;
  BT_tree events = scheduler->state->events;
  BT_tree inputs = scheduler->state->inputs;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = scheduler->state->lp;
  nStates = lp->states;
  simTime->noReinit = 1;
#endif
  if(simTime->type == ST_State)
  {
    stateVar = simTime->minIndex;
    update = stateVar;
#ifdef QSS_PARALLEL
    if (nStates > 0)
    {
      if (lp->qMap[stateVar] <= NOT_ASSIGNED)
      {
        update = UPDATE_DEPS;
      }
      scheduler->state->stepType = ST_State;
#endif
    BT_UPDATE_TREE(scheduler, states, simData->SD[stateVar],
        simData->nSD[stateVar], update);
    minIdx = states->nodes[states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
#ifdef QSS_PARALLEL
  }
#endif
    if(simData->nSZ[stateVar] && events->size != 1)
    {
#ifdef QSS_PARALLEL
      scheduler->state->stepType = ST_Event;
#endif
      BT_UPDATE_TREE(scheduler, events, simData->SZ[stateVar],
          simData->nSZ[stateVar], UPDATE_DEPS);
    }
    if(simTime->nextEventTime[events->nodes[events->height][0].value] < minVal)
    {
      minIdx = events->nodes[events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
    if(simTime->nextInputTime[inputs->nodes[inputs->height][0].value] < minVal)
    {
      minIdx = inputs->nodes[inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  else if(simTime->type == ST_Event)
  {
    stateVar = simTime->minIndex;
    if(nStates > 0)
    {
      if(simData->nHD[stateVar])
      {
#ifdef QSS_PARALLEL
        scheduler->state->stepType = ST_State;
#endif
        BT_UPDATE_TREE(scheduler, states, simData->HD[stateVar],
            simData->nHD[stateVar], UPDATE_DEPS);
      }
      int nLHSSt = simData->event[stateVar].nLHSSt;
      int i, j;
      for(i = 0; i < nLHSSt; i++)
      {
        j = simData->event[stateVar].LHSSt[i];
        update = j;
#ifdef QSS_PARALLEL
        if (lp->qMap[j] <= NOT_ASSIGNED)
        {
          update = UPDATE_DEPS;
        }
        if (lp->states > 0)
        {
          scheduler->state->stepType = ST_State;
#endif
        BT_UPDATE_TREE(scheduler, states, simData->SD[j], simData->nSD[j],
            update);
#ifdef QSS_PARALLEL
      }
#endif
      }
      minIdx = states->nodes[states->height][0].value;
      minVal = simTime->nextStateTime[minIdx];
    }
    update = stateVar;
#ifdef QSS_PARALLEL
    if (lp->eMap[stateVar] <= NOT_ASSIGNED)
    {
      update = UPDATE_DEPS;
    }
    scheduler->state->stepType = ST_Event;
#endif
    BT_UPDATE_TREE(scheduler, events, simData->HZ[stateVar],
        simData->nHZ[stateVar], update);
    if(simTime->nextEventTime[events->nodes[events->height][0].value] < minVal)
    {
      minIdx = events->nodes[events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
    if(simTime->nextInputTime[inputs->nodes[inputs->height][0].value] < minVal)
    {
      minIdx = inputs->nodes[inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  else if(simTime->type == ST_Input)
  {
#ifdef QSS_PARALLEL
    scheduler->state->stepType = ST_State;
#endif
    BT_UPDATE_TREE(scheduler, states, NULL, 0, simData->TD[simTime->minIndex]);
    minIdx = states->nodes[states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    BT_UPDATE_TREE(scheduler, inputs, NULL, 0, simTime->minIndex);
    if(simTime->nextEventTime[events->nodes[events->height][0].value] < minVal)
    {
      minIdx = events->nodes[events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
    if(simTime->nextInputTime[inputs->nodes[inputs->height][0].value] < minVal)
    {
      minIdx = inputs->nodes[inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
#ifdef QSS_PARALLEL
  if (simTime->reinits->counter > 0)
  {
    simTime->reinits->counter--;
    simTime->minValue = simTime->reinits->time;
    simTime->minIndex = simTime->reinits->variable[simTime->reinits->counter];
    simTime->type = ST_State;
    simTime->time = simTime->minValue;
    simTime->noReinit = 0;
    return;
  }
#endif
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

#ifdef QSS_PARALLEL
void
BT_PAR_update (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
BT_update(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  scheduler->state->update(scheduler, simData, simTime);
#ifdef QSS_PARALLEL
  if (simTime->time == INF)
  {
    simTime->time = simData->ft;
  }
#endif
  double tElapsed = (simTime->time - simData->it);
  if(tElapsed >= scheduler->state->tOut)
  {
    scheduler->state->tOut += scheduler->state->tUnit;
    fprintf(stderr, "*%.8f", tElapsed);
  }
}

#ifdef QSS_PARALLEL
void
BT_PAR_init (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
BT_init(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  int minIdx, w = ST_State, max_h, max_n, nStates, nEvents, nInputs;
  double minVal;
#ifdef QSS_PARALLEL
  scheduler->state->lp = simData->lp;
  QSS_LP_data lp = scheduler->state->lp;
  nEvents = lp->events;
  nInputs = lp->inputs;
  nStates = lp->states;
#else
  nEvents = simData->events;
  nInputs = simData->inputs;
  nStates = simData->states;
#endif
#ifdef QSS_PARALLEL
  scheduler->state->states = BT_Tree (lp->states, simTime->nextStateTime,
      simData->states, lp->qOutMap);
#else
  scheduler->state->states = BT_Tree(simData->states, simTime->nextStateTime,
      simData->states,
      NULL);
#endif
  if(nStates)
  {
    BT_INIT_TREE(scheduler->state->states);
    minIdx =
        scheduler->state->states->nodes[scheduler->state->states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
  }
  else
  {
    minIdx = 0;
    minVal = INF;
  }
  if(nEvents)
  {
#ifdef QSS_PARALLEL
    scheduler->state->events = BT_Tree (lp->events,simTime->nextEventTime,
        simData->events, lp->eOutMap);
#else
    scheduler->state->events = BT_Tree(simData->events, simTime->nextEventTime,
        simData->events, NULL);
#endif
    BT_INIT_TREE(scheduler->state->events);
    if(simTime->nextEventTime[scheduler->state->events->nodes[scheduler->state->events->height][0].value]
        < minVal)
    {
      minIdx =
          scheduler->state->events->nodes[scheduler->state->events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
  }
  if(nInputs)
  {
#ifdef QSS_PARALLEL
    scheduler->state->inputs = BT_Tree (lp->inputs,
        simTime->nextInputTime,
        simData->inputs, lp->iMap);
#else
    scheduler->state->inputs = BT_Tree(simData->inputs, simTime->nextInputTime,
        simData->inputs, NULL);
#endif
    BT_INIT_TREE(scheduler->state->inputs);
    if(simTime->nextInputTime[scheduler->state->inputs->nodes[scheduler->state->inputs->height][0].value]
        < minVal)
    {
      minIdx =
          scheduler->state->inputs->nodes[scheduler->state->inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
#ifdef QSS_PARALLEL
  if (nStates)
  {
#endif
  max_h = scheduler->state->states->height;
  max_n = simData->states;
#ifdef QSS_PARALLEL
}
else
{
  max_h = 0;
  max_n = 0;
}
#endif
  if(nEvents)
  {
    if(scheduler->state->events->height > max_h)
    {
      max_h = scheduler->state->events->height;
      max_n = nEvents;
    }
  }
  if(nInputs)
  {
    if(scheduler->state->inputs->height > max_h)
    {
      max_h = scheduler->state->inputs->height;
      max_n = nInputs;
    }
  }
  scheduler->state->info = BT_Info(max_h, max_n);
  if(nEvents && nInputs)
  {
    scheduler->state->update = BT_UPDATE_HI_FN;
  }
  else if(nEvents)
  {
    scheduler->state->update = BT_UPDATE_H_FN;
  }
  else if(nInputs)
  {
    scheduler->state->update = BT_UPDATE_I_FN;
  }
  else
  {
    scheduler->state->update = BT_UPDATE_C_FN;
  }
  scheduler->state->tUnit = (simData->ft - simData->it) / 100.0;
  scheduler->state->tOut = 0;
  scheduler->ops->update = BT_UPDATE_FN;
#ifdef QSS_PARALLEL
  if (simTime->time == INF)
  {
    simTime->time = simData->ft;
  }
#endif
}
