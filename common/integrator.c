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

#include "integrator.h"

#include <stdlib.h>

#include "../classic/classic_dassl_integrator.h"
#include "../classic/classic_dopri_integrator.h"
#include "../classic/classic_cvode_integrator.h"
#include "../classic/classic_ida_integrator.h"
#include "../classic/classic_integrator.h"
#include "../qss/qss_integrator.h"
#include "../qss/qss_par_integrator.h"
#include "../qss/qss_seq_integrator.h"
#include "data.h"
#include "utils.h"

INT_integrator
INT_Integrator(SIM_simulator simulator)
{
  INT_integrator p = checkedMalloc(sizeof(*p));
  p->ops = INT_IntegratorOps();
  switch(simulator->state->settings->method)
  {
    case SD_DOPRI:
      p->ops->initiliaze = CLC_initialize;
      p->ops->integrate = DOPRI_integrate;
      break;
    case SD_DASSL:
      p->ops->initiliaze = CLC_initialize;
      p->ops->integrate = DASSL_integrate;
      break;
    case SD_CVODE_BDF:
    case SD_CVODE_AM:
      p->ops->initiliaze = CLC_initialize;
      p->ops->integrate = CVODE_integrate;
      break;
    case SD_IDA:
      p->ops->initiliaze = CLC_initialize;
      p->ops->integrate = IDA_integrate;
      break;
    default:
      {
        if(simulator->state->settings->parallel == TRUE)
        {
          p->ops->initiliaze = QSS_PAR_initialize;
          p->ops->integrate = QSS_PAR_integrate;
        }
        else
        {
          p->ops->initiliaze = QSS_SEQ_initialize;
          p->ops->integrate = QSS_SEQ_integrate;
        }
      }
  }
  return p;
}

void
INT_freeIntegrator(INT_integrator integrator)
{
  INT_freeIntegratorOps(integrator->ops);
  free(integrator);
}

INT_integratorOps
INT_IntegratorOps()
{
  INT_integratorOps p = checkedMalloc(sizeof(*p));
  p->initiliaze = NULL;
  p->integrate = NULL;
  return p;
}

void
INT_freeIntegratorOps(INT_integratorOps ops)
{
  free(ops);
}

void
INT_initialize(INT_integrator integrator, SIM_simulator simulator)
{
  integrator->ops->initiliaze(simulator);
}

void
INT_integrate(INT_integrator integrator, SIM_simulator simulator)
{
  integrator->ops->integrate(simulator);
}
