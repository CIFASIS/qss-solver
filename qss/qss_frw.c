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

#include "qss_frw.h"

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_frw_imp.h"

/*! \brief Initializes the \f$ \Delta \f$ value that the different functions use to
 * advance the time variable (\f$ t' = t + \Delta t\f$)
 *
 * \f$ \Delta \f$ is proportional to the final simulation time.
 */

void
FRW_initDelta(FRW_framework f, double ft, double param)
{
  f->state->delta = param * ft;
}

FRW_framework
FRW_Framework(QSS_data simData)
{
  FRW_framework p = checkedMalloc(sizeof(*p));
  p->state = FRW_FrameworkState();
  p->ops = FRW_FrameworkOps();
  if(simData->params->symDiff)
  {
    if(simData->params->lps > 0)
    {
      p->ops->recomputeDerivatives = SYM_PAR_recomputeDerivatives;
      p->ops->recomputeDerivative = SYM_PAR_recomputeDerivative;
      p->ops->nextEventTime = SYM_PAR_nextEventTime;
    }
    else
    {
      p->ops->recomputeDerivatives = SYM_recomputeDerivatives;
      p->ops->recomputeDerivative = SYM_recomputeDerivative;
      p->ops->nextEventTime = SYM_nextEventTime;
    }
    switch(simData->order)
    {
      case 1:
        if(simData->params->lps > 0)
        {
          p->ops->recomputeDerivatives = FO_PAR_recomputeDerivatives;
          p->ops->recomputeDerivative = FO_PAR_recomputeDerivative;
          p->ops->nextEventTime = FO_PAR_nextEventTime;
          p->ops->nextInputTime = FO_PAR_nextInputTime;
        }
        else
        {
          p->ops->recomputeDerivatives = FO_recomputeDerivatives;
          p->ops->recomputeDerivative = FO_recomputeDerivative;
          p->ops->nextEventTime = FO_nextEventTime;
          p->ops->nextInputTime = FO_nextInputTime;
        }
        break;
      case 2:
        if(simData->params->lps > 0)
        {
          p->ops->nextInputTime = SO_PAR_nextInputTime;
        }
        else
        {
          p->ops->nextInputTime = SO_nextInputTime;
        }
        break;
      case 3:
        if(simData->params->lps > 0)
        {
          p->ops->nextInputTime = TO_PAR_nextInputTime;
        }
        else
        {
          p->ops->nextInputTime = TO_nextInputTime;
        }
        break;
      case 4:
        if(simData->params->lps > 0)
        {
          p->ops->nextInputTime = TO_PAR_nextInputTime;
        }
        else
        {
          p->ops->nextInputTime = TO_nextInputTime;
        }
        break;
    }
  }
  else
  {
    switch(simData->order)
    {
      case 1:
        if(simData->params->lps > 0)
        {
          p->ops->recomputeDerivatives = FO_PAR_recomputeDerivatives;
          p->ops->recomputeDerivative = FO_PAR_recomputeDerivative;
          p->ops->nextEventTime = FO_PAR_nextEventTime;
          p->ops->nextInputTime = FO_PAR_nextInputTime;
        }
        else
        {
          p->ops->recomputeDerivatives = FO_recomputeDerivatives;
          p->ops->recomputeDerivative = FO_recomputeDerivative;
          p->ops->nextEventTime = FO_nextEventTime;
          p->ops->nextInputTime = FO_nextInputTime;
        }

        break;
      case 2:
        if(simData->params->lps > 0)
        {
          p->ops->recomputeDerivatives = SO_PAR_recomputeDerivatives;
          p->ops->recomputeDerivative = SO_PAR_recomputeDerivative;
          p->ops->nextEventTime = SO_PAR_nextEventTime;
          p->ops->nextInputTime = SO_PAR_nextInputTime;
        }
        else
        {
          p->ops->recomputeDerivatives = SO_recomputeDerivatives;
          p->ops->recomputeDerivative = SO_recomputeDerivative;
          p->ops->nextEventTime = SO_nextEventTime;
          p->ops->nextInputTime = SO_nextInputTime;
        }
        break;
      case 3:
        if(simData->params->lps > 0)
        {
          p->ops->recomputeDerivatives = TO_PAR_recomputeDerivatives;
          p->ops->recomputeDerivative = TO_PAR_recomputeDerivative;
          p->ops->nextEventTime = TO_PAR_nextEventTime;
          p->ops->nextInputTime = TO_PAR_nextInputTime;
        }
        else
        {
          p->ops->recomputeDerivatives = TO_recomputeDerivatives;
          p->ops->recomputeDerivative = TO_recomputeDerivative;
          p->ops->nextEventTime = TO_nextEventTime;
          p->ops->nextInputTime = TO_nextInputTime;
        }
        break;
      case 4:
        printf("TODO!!\n");
        break;
      default:
        return NULL;
    }
  }
  FRW_initDelta(p, simData->ft, simData->params->derDelta);
  return p;
}

void
FRW_freeFramework(FRW_framework f)
{
  FRW_freeFrameworkState(f->state);
  FRW_freeFrameworkOps(f->ops);
  free(f);
}

FRW_frameworkState
FRW_FrameworkState()
{
  FRW_frameworkState p = checkedMalloc(sizeof(*p));
  p->delta = 0;
  p->dxnOld = 0;
  return p;
}

void
FRW_freeFrameworkState(FRW_frameworkState f)
{
  free(f);
}

FRW_frameworkOps
FRW_FrameworkOps()
{
  FRW_frameworkOps p = checkedMalloc(sizeof(*p));
  p->nextEventTime = NULL;
  p->nextInputTime = NULL;
  p->recomputeDerivative = NULL;
  p->recomputeDerivatives = NULL;
  return p;
}

void
FRW_freeFrameworkOps(FRW_frameworkOps o)
{
  free(o);
}

void
FRW_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
{
  f->ops->recomputeDerivative(f, simModel, simData, simTime, index);
}

void
FRW_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
{
  f->ops->recomputeDerivatives(f, simModel, simData, simTime, index);
}

void
FRW_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
{
  f->ops->nextEventTime(f, simModel, simData, simTime, index);
}

void
FRW_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
{
  f->ops->nextInputTime(f, simModel, simData, simTime, elapsed, var, index);
}

