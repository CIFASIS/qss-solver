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

#include <qss/qss_binary_random.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/random.h"
#include "../common/tree.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_scheduler.h"

void BTR_initTree(SC_scheduler scheduler, BTR_tree tree)
{
  if (tree->size == 1) {
    tree->nodes[0][0].value = 0;
    tree->nodes[0][0].father = 0;
    tree->nodes[0][0].size = 1;
    tree->nodes[0][0].visit[0] = NOT_EQUAL;
    tree->nodes[0][0].visit[1] = NOT_EQUAL;
    tree->nodes[0][0].visit[2] = NOT_EQUAL;
    tree->father[0] = 0;
    return;
  }
  unsigned int j, i, size, rightChild, leftChild;
  int k;
  int *numberOfNodes;
  if (tree->size == 2 || tree->size == 3) {
    numberOfNodes = checkedMalloc(sizeof(int));
    numberOfNodes[0] = 0;
  } else {
    numberOfNodes = checkedMalloc((tree->height + 1) * sizeof(int));
  }
  size = DIV2(tree->size);
  for (j = 0; j < tree->size; j++) {
    tree->father[j] = DIV2(j);
  }
  if (tree->size & ODD) {
    tree->father[tree->size - 1]--;
  }
  if (tree->height) {
    for (j = 0; j <= tree->height; j++) {
      numberOfNodes[j] = size - 1;
      size = DIV2(size);
    }
  }
  for (j = 0; j <= numberOfNodes[0]; j++) {
    leftChild = MUL2(j);
    rightChild = MUL2(j) + 1;
    tree->nodes[0][j].size = 2;
    tree->nodes[0][j].father = DIV2(j);
    tree->nodes[0][j].value = MIN(tree->leaves[leftChild], leftChild, tree->leaves[rightChild], rightChild);
    tree->nodes[0][j].visit[0] = tree->nodes[0][j].value;
    if (tree->leaves[leftChild] == tree->leaves[rightChild]) {
      tree->nodes[0][j].visit[1] = rightChild;
    } else {
      tree->nodes[0][j].visit[1] = NOT_EQUAL;
    }
  }
  if ((numberOfNodes[0] + 1) & ODD) {
    tree->nodes[0][numberOfNodes[0]].father--;
  }
  if (tree->size & ODD) {
    j = numberOfNodes[0];
    tree->nodes[0][numberOfNodes[0]].size++;
    rightChild = MUL2(numberOfNodes[0]) + 2;
    k = tree->nodes[0][numberOfNodes[0]].value;
    if (tree->leaves[k] == tree->leaves[rightChild]) {
      if (tree->nodes[0][j].visit[1] != NOT_EQUAL) {
        tree->nodes[0][j].visit[2] = rightChild;
      } else {
        tree->nodes[0][j].visit[1] = rightChild;
        tree->nodes[0][j].visit[2] = NOT_EQUAL;
      }
    } else {
      tree->nodes[0][j].visit[2] = NOT_EQUAL;
      tree->nodes[0][numberOfNodes[0]].value = MIN(tree->leaves[k], k, tree->leaves[rightChild], rightChild);
      if (tree->nodes[0][numberOfNodes[0]].value == rightChild) {
        tree->nodes[0][j].visit[0] = rightChild;
        tree->nodes[0][j].visit[1] = NOT_EQUAL;
      }
    }
  }
  if (tree->height) {
    for (i = 1; i <= tree->height; i++) {
      for (j = 0; j <= numberOfNodes[i]; j++) {
        leftChild = tree->nodes[i - 1][MUL2(j)].value;
        rightChild = tree->nodes[i - 1][MUL2(j) + 1].value;
        tree->nodes[i][j].size = 2;
        tree->nodes[i][j].father = DIV2(j);
        tree->nodes[i][j].value = MIN(tree->leaves[leftChild], leftChild, tree->leaves[rightChild], rightChild);
        tree->nodes[i][j].visit[0] = MIN(tree->leaves[leftChild], MUL2(j), tree->leaves[rightChild], MUL2(j) + 1);
        if (tree->leaves[leftChild] == tree->leaves[rightChild]) {
          tree->nodes[i][j].visit[1] = MUL2(j) + 1;
        } else {
          tree->nodes[i][j].visit[1] = NOT_EQUAL;
        }
      }
      if ((numberOfNodes[i] + 1) & ODD) {
        tree->nodes[i][numberOfNodes[i]].father--;
      }
      if ((numberOfNodes[i - 1] + 1) & ODD) {
        j = numberOfNodes[i];
        tree->nodes[i][numberOfNodes[i]].size++;
        rightChild = tree->nodes[i - 1][MUL2(numberOfNodes[i]) + 2].value;
        k = tree->nodes[i][numberOfNodes[i]].value;
        if (tree->leaves[k] == tree->leaves[rightChild]) {
          if (tree->nodes[i][j].visit[1] != NOT_EQUAL) {
            tree->nodes[i][j].visit[2] = MUL2(numberOfNodes[i]) + 2;
          } else {
            tree->nodes[i][j].visit[1] = MUL2(numberOfNodes[i]) + 2;
            tree->nodes[i][j].visit[2] = NOT_EQUAL;
          }
        } else {
          tree->nodes[i][j].visit[2] = NOT_EQUAL;
          tree->nodes[i][numberOfNodes[i]].value = MIN(tree->leaves[k], k, tree->leaves[rightChild], rightChild);
          if (tree->nodes[i][numberOfNodes[i]].value == rightChild) {
            tree->nodes[i][j].visit[0] = MUL2(numberOfNodes[i]) + 2;
            tree->nodes[i][j].visit[1] = NOT_EQUAL;
          }
        }
      }
    }
    tree->numEquals = 0;
    tree->randomRange = 0;
    scheduler->state->visit->fathers[tree->height][0] = 0;
    int visitNodes = 1;
    for (i = tree->height; i >= 1; i--) {
      int nNodes = visitNodes;
      visitNodes = 0;
      for (j = 0; j < nNodes; j++) {
        int upd = scheduler->state->visit->fathers[i][j];
        int cChilds = tree->nodes[i][upd].size;
        for (k = 0; k < cChilds; k++) {
          int add = tree->nodes[i][upd].visit[k];
          if (add != NOT_EQUAL) {
            scheduler->state->visit->fathers[i - 1][visitNodes++] = add;
          } else {
            break;
          }
        }
      }
    }
    for (i = 0; i < visitNodes; i++) {
      int upd = scheduler->state->visit->fathers[0][i];
      int cChilds = tree->nodes[0][upd].size;
      for (j = 0; j < cChilds; j++) {
        int add = tree->nodes[0][upd].visit[j];
        if (add != NOT_EQUAL) {
          tree->equals[add] = add;
          tree->numEquals++;
          int g, w = tree->randomRange + tree->weights[add];
          for (g = tree->randomRange; g < w; g++) {
            tree->weightedEquals[g] = add;
          }
          tree->randomRange += tree->weights[add];
        }
      }
    }
    if (tree->numEquals > 1) {
      shuffle(tree->weightedEquals, tree->randomRange);
      int selected = tree->weightedEquals[0];
      tree->nodes[tree->height][0].value = selected;
      tree->equals[selected] = NOT_ASSIGNED;
      tree->num = 1;
      tree->numEquals--;
    } else {
      tree->numEquals = 0;
    }
  }
  free(numberOfNodes);
}

void BTR_updateTree(SC_scheduler scheduler, BTR_tree tree, int *inf, int cant, int idx, double *times)
{
  if (tree->size == 1) return;
  int vars, nodes;
  unsigned int j, updateVar, minIdx;
  int i;
  nodes = 1;
  if (idx >= 0) {
    scheduler->state->visit->fathers[0][0] = tree->nodes[0][tree->father[idx]].father;
    updateVar = tree->father[idx];
    minIdx = MUL2(updateVar);
    if (tree->leaves[minIdx] == tree->leaves[minIdx + 1]) {
      tree->nodes[0][updateVar].visit[1] = minIdx + 1;
    } else {
      tree->nodes[0][updateVar].visit[1] = NOT_EQUAL;
    }
    minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[minIdx + 1], minIdx + 1);
    tree->nodes[0][updateVar].visit[0] = minIdx;
    if (tree->nodes[0][updateVar].size & ODD) {
      int last = MUL2(updateVar) + 2;
      if (tree->leaves[minIdx] == tree->leaves[last]) {
        if (tree->nodes[0][updateVar].visit[1] != NOT_EQUAL) {
          tree->nodes[0][updateVar].visit[2] = last;
        } else {
          tree->nodes[0][updateVar].visit[1] = last;
          tree->nodes[0][updateVar].visit[2] = NOT_EQUAL;
        }
      } else {
        tree->nodes[0][updateVar].visit[2] = NOT_EQUAL;
        minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[last], last);
        if (minIdx == last) {
          tree->nodes[0][updateVar].visit[0] = last;
          tree->nodes[0][updateVar].visit[1] = NOT_EQUAL;
        }
      }
    }
    tree->nodes[0][updateVar].value = minIdx;
  } else {
    scheduler->state->visit->fathers[0][0] = tree->nodes[0][tree->father[inf[0]]].father;
  }
  vars = cant;
  for (i = 0; i < vars; i++) {
    updateVar = tree->father[inf[i]];
    minIdx = MUL2(updateVar);
    if (tree->leaves[minIdx] == tree->leaves[minIdx + 1]) {
      tree->nodes[0][updateVar].visit[1] = minIdx + 1;
    } else {
      tree->nodes[0][updateVar].visit[1] = NOT_EQUAL;
    }
    minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[minIdx + 1], minIdx + 1);
    tree->nodes[0][updateVar].visit[0] = minIdx;
    if (tree->nodes[0][updateVar].size & ODD) {
      int last = MUL2(updateVar) + 2;
      if (tree->leaves[minIdx] == tree->leaves[last]) {
        if (tree->nodes[0][updateVar].visit[1] != NOT_EQUAL) {
          tree->nodes[0][updateVar].visit[2] = last;
        } else {
          tree->nodes[0][updateVar].visit[1] = last;
          tree->nodes[0][updateVar].visit[2] = NOT_EQUAL;
        }
      } else {
        tree->nodes[0][updateVar].visit[2] = NOT_EQUAL;
        minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[last], last);
        if (minIdx == last) {
          tree->nodes[0][updateVar].visit[0] = last;
          tree->nodes[0][updateVar].visit[1] = NOT_EQUAL;
        }
      }
    }
    tree->nodes[0][updateVar].value = minIdx;
    if (tree->nodes[0][updateVar].father != scheduler->state->visit->fathers[0][nodes - 1]) {
      scheduler->state->visit->fathers[0][nodes] = tree->nodes[0][updateVar].father;
      nodes++;
    }
  }
  for (j = 1; j <= tree->height; j++) {
    vars = nodes;
    nodes = 1;
    scheduler->state->visit->fathers[j][0] = tree->nodes[j][scheduler->state->visit->fathers[j - 1][0]].father;
    for (i = 0; i < vars; i++) {
      updateVar = scheduler->state->visit->fathers[j - 1][i];
      minIdx = MUL2(updateVar);
      if (tree->leaves[tree->nodes[j - 1][minIdx].value] == tree->leaves[tree->nodes[j - 1][minIdx + 1].value]) {
        tree->nodes[j][updateVar].visit[1] = minIdx + 1;
      } else {
        tree->nodes[j][updateVar].visit[1] = NOT_EQUAL;
      }
      tree->nodes[j][updateVar].visit[0] =
          MIN(tree->leaves[tree->nodes[j - 1][minIdx].value], minIdx, tree->leaves[tree->nodes[j - 1][minIdx + 1].value], minIdx + 1);
      minIdx = MIN(tree->leaves[tree->nodes[j - 1][minIdx].value], tree->nodes[j - 1][minIdx].value,
                   tree->leaves[tree->nodes[j - 1][minIdx + 1].value], tree->nodes[j - 1][minIdx + 1].value);
      if (tree->nodes[j][updateVar].size & ODD) {
        int last = MUL2(updateVar) + 2;
        if (tree->leaves[minIdx] == tree->leaves[tree->nodes[j - 1][last].value]) {
          if (tree->nodes[j][updateVar].visit[1] != NOT_EQUAL) {
            tree->nodes[j][updateVar].visit[2] = last;
          } else {
            tree->nodes[j][updateVar].visit[1] = last;
            tree->nodes[j][updateVar].visit[2] = NOT_EQUAL;
          }
        } else {
          tree->nodes[j][updateVar].visit[2] = NOT_EQUAL;
          minIdx = MIN(tree->leaves[minIdx], minIdx, tree->leaves[tree->nodes[j - 1][last].value], tree->nodes[j - 1][last].value);
          if (minIdx == tree->nodes[j - 1][last].value) {
            tree->nodes[j][updateVar].visit[0] = last;
            tree->nodes[j][updateVar].visit[1] = NOT_EQUAL;
          }
        }
      }
      tree->nodes[j][updateVar].value = minIdx;
      if (tree->nodes[j][updateVar].father != scheduler->state->visit->fathers[j][nodes - 1]) {
        scheduler->state->visit->fathers[j][nodes] = tree->nodes[j][updateVar].father;
        nodes++;
      }
    }
  }
  if (tree->numEquals) {
    int var = tree->weightedEquals[tree->num++];
    while (tree->equals[var] <= NOT_ASSIGNED) {
      var = tree->weightedEquals[tree->num++];
    }
    tree->equals[var] = NOT_ASSIGNED;
    tree->nodes[tree->height][0].value = var;
    tree->numEquals--;
  } else {
    tree->minimum = tree->leaves[tree->nodes[tree->height][0].value];
    tree->numEquals = 0;
    int nNodes;
    tree->randomRange = 0;
    int visitNodes = 1;
    scheduler->state->visit->fathers[tree->height][0] = 0;
    for (i = tree->height; i >= 1; i--) {
      int k;
      nNodes = visitNodes;
      visitNodes = 0;
      for (j = 0; j < nNodes; j++) {
        int upd = scheduler->state->visit->fathers[i][j];
        int cChilds = tree->nodes[i][upd].size;
        for (k = 0; k < cChilds; k++) {
          int add = tree->nodes[i][upd].visit[k];
          if (add != NOT_EQUAL) {
            scheduler->state->visit->fathers[i - 1][visitNodes++] = add;
          } else {
            break;
          }
        }
      }
    }
    for (i = 0; i < visitNodes; i++) {
      int upd = scheduler->state->visit->fathers[0][i];
      int cChilds = tree->nodes[0][upd].size;
      for (j = 0; j < cChilds; j++) {
        int add = tree->nodes[0][upd].visit[j];
        if (add != NOT_EQUAL) {
          tree->equals[add] = add;
          tree->numEquals++;
          int g, w = tree->randomRange + tree->weights[add];
          for (g = tree->randomRange; g < w; g++) {
            tree->weightedEquals[g] = add;
          }
          tree->randomRange += tree->weights[add];
        } else {
          break;
        }
      }
    }
    if (tree->numEquals > 1) {
      shuffle(tree->weightedEquals, tree->randomRange);
      int selected = tree->weightedEquals[0];
      tree->nodes[tree->height][0].value = selected;
      tree->equals[selected] = NOT_ASSIGNED;
      tree->num = 1;
      tree->numEquals--;
    } else {
      tree->numEquals = 0;
    }
  }
}

void BTR_updateS(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  int w = ST_State, minIdx = -1, stateVar;
  double minVal = INF;
  stateVar = simTime->minIndex;
  BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->SD[stateVar], simData->nSD[stateVar], stateVar, simTime->nextStateTime);
  minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
  minVal = simTime->nextStateTime[minIdx];
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

void BTR_updateH(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  int w = ST_State, minIdx = -1, stateVar;
  double minVal = INF;
  if (simTime->type == ST_State) {
    stateVar = simTime->minIndex;
    BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->SD[stateVar], simData->nSD[stateVar], stateVar,
                   simTime->nextStateTime);
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    if (simData->nSZ[stateVar] && scheduler->state->BTR_events->size != 1)
      BTR_updateTree(scheduler, scheduler->state->BTR_events, simData->SZ[stateVar], simData->nSZ[stateVar], -1, simTime->nextEventTime);
    if (simTime->nextEventTime[scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
  } else if (simTime->type == ST_Event) {
    stateVar = simTime->minIndex;
    if (simData->nHD[stateVar])
      BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->HD[stateVar], simData->nHD[stateVar], -1, simTime->nextStateTime);
    if (simData->event[stateVar].nLHSSt) {
      int i, j;
      for (i = 0; i < simData->event[stateVar].nLHSSt; i++) {
        j = simData->event[stateVar].LHSSt[i];
        BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->SD[j], simData->nSD[j], j, simTime->nextStateTime);
      }
    }
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    BTR_updateTree(scheduler, scheduler->state->BTR_events, simData->HZ[stateVar], simData->nHZ[stateVar], stateVar,
                   simTime->nextEventTime);
    if (simTime->nextEventTime[scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
  }
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

void BTR_updateI(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  int w = ST_State, minIdx = -1, stateVar;
  double minVal = INF;
  if (simTime->type == ST_State) {
    stateVar = simTime->minIndex;
    BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->SD[stateVar], simData->nSD[stateVar], stateVar,
                   simTime->nextStateTime);
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    if (simTime->nextInputTime[scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  } else if (simTime->type == ST_Input) {
    BTR_updateTree(scheduler, scheduler->state->BTR_states, NULL, 0, simData->TD[simTime->minIndex], simTime->nextStateTime);
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    BTR_updateTree(scheduler, scheduler->state->BTR_inputs, NULL, 0, simTime->minIndex, simTime->nextInputTime);
    if (simTime->nextInputTime[scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

void BTR_updateHI(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  int w = ST_State, minIdx = -1, stateVar;
  double minVal = INF;
  if (simTime->type == ST_State) {
    stateVar = simTime->minIndex;
    BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->SD[stateVar], simData->nSD[stateVar], stateVar,
                   simTime->nextStateTime);
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    if (simData->nSZ[stateVar] && scheduler->state->BTR_events->size != 1)
      BTR_updateTree(scheduler, scheduler->state->BTR_events, simData->SZ[stateVar], simData->nSZ[stateVar], -1, simTime->nextEventTime);
    if (simTime->nextEventTime[scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
    if (simTime->nextInputTime[scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  } else if (simTime->type == ST_Event) {
    stateVar = simTime->minIndex;
    if (simData->HD[stateVar])
      BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->HD[stateVar], simData->nHD[stateVar], -1, simTime->nextStateTime);
    if (simData->event[stateVar].nLHSSt) {
      int i, j;
      for (i = 0; i < simData->event[stateVar].nLHSSt; i++) {
        j = simData->event[stateVar].LHSSt[i];
        BTR_updateTree(scheduler, scheduler->state->BTR_states, simData->SD[j], simData->nSD[j], j, simTime->nextStateTime);
      }
    }
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    BTR_updateTree(scheduler, scheduler->state->BTR_events, simData->HZ[stateVar], simData->nHZ[stateVar], stateVar,
                   simTime->nextEventTime);
    if (simTime->nextEventTime[scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
    if (simTime->nextInputTime[scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  } else if (simTime->type == ST_Input) {
    BTR_updateTree(scheduler, scheduler->state->BTR_states, NULL, 0, simData->TD[simTime->minIndex], simTime->nextStateTime);
    minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
    minVal = simTime->nextStateTime[minIdx];
    BTR_updateTree(scheduler, scheduler->state->BTR_inputs, NULL, 0, simTime->minIndex, simTime->nextInputTime);
    if (simTime->nextEventTime[scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
    if (simTime->nextInputTime[scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
}

void BTR_update(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  scheduler->state->update(scheduler, simData, simTime);
#ifdef QSS_PARALLEL
  if (simTime->time == INF) {
    simTime->time = simData->ft;
  }
#endif
  double tElapsed = (simTime->time - simData->it);
  if (tElapsed >= scheduler->state->tOut) {
    scheduler->state->tOut += scheduler->state->tUnit;
    fprintf(stderr, "*%.8f", tElapsed);
  }
}

void BTR_init(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  int minIdx, w = ST_State, max_h, max_n;
  double minVal;
  int stHeight = (int)LOG2(simData->states) - 1;
  if (stHeight < 0) stHeight = 0;
  int dscHeight = (int)LOG2(simData->events) - 1;
  if (dscHeight < 0) dscHeight = 0;
  int inHeight = (int)LOG2(simData->inputs) - 1;
  if (inHeight < 0) inHeight = 0;
  max_h = stHeight;
  max_n = simData->states;
  if (simData->events) {
    if (dscHeight > max_h) {
      max_h = dscHeight;
      max_n = simData->events;
    }
  }
  if (simData->inputs) {
    if (inHeight > max_h) {
      max_h = inHeight;
      max_n = simData->inputs;
    }
  }
  scheduler->state->visit = BT_Info(max_h, max_n);
  scheduler->state->BTR_states = BTR_Tree(simData->states, simTime->nextStateTime, NULL);
  BTR_initTree(scheduler, scheduler->state->BTR_states);
  minIdx = scheduler->state->BTR_states->nodes[scheduler->state->BTR_states->height][0].value;
  minVal = simTime->nextStateTime[minIdx];
  if (simData->events) {
    scheduler->state->BTR_events = BTR_Tree(simData->events, simTime->nextEventTime, simTime->weights);
    BTR_initTree(scheduler, scheduler->state->BTR_events);
    if (simTime->nextEventTime[scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_events->nodes[scheduler->state->BTR_events->height][0].value;
      minVal = simTime->nextEventTime[minIdx];
      w = ST_Event;
    }
  }
  if (simData->inputs) {
    scheduler->state->BTR_inputs = BTR_Tree(simData->inputs, simTime->nextInputTime, NULL);
    BTR_initTree(scheduler, scheduler->state->BTR_inputs);
    if (simTime->nextInputTime[scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value] < minVal) {
      minIdx = scheduler->state->BTR_inputs->nodes[scheduler->state->BTR_inputs->height][0].value;
      minVal = simTime->nextInputTime[minIdx];
      w = ST_Input;
    }
  }
  simTime->minValue = minVal;
  simTime->minIndex = minIdx;
  simTime->type = w;
  simTime->time += simTime->minValue;
  if (simData->events && simData->inputs) {
    scheduler->state->update = BTR_updateHI;
  } else if (simData->events) {
    scheduler->state->update = BTR_updateH;
  } else if (simData->inputs) {
    scheduler->state->update = BTR_updateI;
  } else {
    scheduler->state->update = BTR_updateS;
  }
  scheduler->state->tUnit = (simData->ft - simData->it) / 100.0;
  scheduler->state->tOut = 0;
  scheduler->ops->update = BTR_update;
}
