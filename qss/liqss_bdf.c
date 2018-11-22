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

#include <common/utils.h>
#include <math.h>
#include <qss/liqss_bdf.h>
#include <stdio.h>
#include <stdlib.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_splinalg.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_vector.h>

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_init(QA_quantizer quantizer, QSS_data simData,
                        QSS_time simTime)
#else
void LIQSS_BDF_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int i;
  int states = simData->states;

  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->a = (double *)malloc(states * sizeof(double));
  quantizer->state->u0 = (double *)malloc(states * sizeof(double));
  quantizer->state->u1 = (double *)malloc(states * sizeof(double));
  quantizer->state->tx = (double *)malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *)malloc(states * sizeof(double));
  quantizer->state->finTime = simData->ft;
  quantizer->state->qss_bdf = QSS_BDF_Hybrid(simData);
  for (i = 0; i < states; i++) {
    int cf0 = i * 3;
    simData->x[cf0 + 2] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->flag2[i] = 0;
    quantizer->state->tx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->u1[i] = 0;
  }
  QSS_BDF_partition(quantizer->state->qss_bdf, simData);
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = LIQSS_BDF_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS_BDF_PAR_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS_BDF_PAR_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS_BDF_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = LIQSS_BDF_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS_BDF_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS_BDF_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS_BDF_updateQuantizedState;
#endif
  quantizer->state->finTime = simData->ft;
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
}

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_setGSLVector(QA_quantizer quantizer, double *x, double *q,
                                double *xprev)
#else
void LIQSS_BDF_setGSLVector(QA_quantizer quantizer, double *x, double *q,
                            double *xprev)
#endif
{
  int i, j, jacIt;
  double *J = quantizer->state->qss_bdf->jac;
  int *BES = quantizer->state->qss_bdf->BES;
  int nBE = quantizer->state->qss_bdf->nBE;
  int *BE = quantizer->state->qss_bdf->BE;
  int *Jac = quantizer->state->qss_bdf->Jac;
  double h = quantizer->state->qss_bdf->h;
  double B, f, B1 = 0.0;
  gsl_spmatrix *As = quantizer->state->qss_bdf->As;
  gsl_vector *fs = quantizer->state->qss_bdf->fs;
  gsl_vector *us = quantizer->state->qss_bdf->us;
  bool setMatrix = FALSE;
  if (quantizer->state->qss_bdf->hprev != h) {
    setMatrix = TRUE;
    B1 = -1 / 3.0;
  }
  for (i = 0, jacIt = 0; i < nBE; i++) {
    f = 0;
    int row = BES[i];
    int nSD = quantizer->state->qss_bdf->nSD[row];
    bool diag = FALSE;
    for (j = 0; j < nSD; j++, jacIt++) {
      int sd = quantizer->state->qss_bdf->SD[row][j];
      int col = BE[sd];
      if (col != NOT_ASSIGNED) {
        if (i == col) {
          diag = TRUE;
          B = (4 / 3.0 - (2 / 3.0) * h * J[Jac[jacIt]]);
          if (setMatrix) {
            gsl_spmatrix_set(As, i, col, B + B1);
          }
        } else {
          B = -(2 / 3.0) * h * J[Jac[jacIt]];
          if (setMatrix) {
            gsl_spmatrix_set(As, i, col, B);
          }
        }
        f += B * q[3 * BES[col]];
      }
    }
    if (!diag) {
      B = 4 / 3.0;
      if (setMatrix) {
        gsl_spmatrix_set(As, i, i, B);
      }
      f += B * q[3 * BES[i]];
    }
    gsl_vector_set(fs, i, f - xprev[i] / 3.0 + 2 / 3.0 * h * x[3 * BES[i] + 1]);
    gsl_vector_set(us, i, q[3 * BES[i]]);
  }
}

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q,
                               double *lqu, int var)
#else
void LIQSS_BDF_solver2x2_h(QA_quantizer quantizer, double *x, double *q,
                           double *lqu, int var)
#endif
{
  double *J = quantizer->state->qss_bdf->jac;
  double h = quantizer->state->qss_bdf->h;
  int *BES = quantizer->state->qss_bdf->BES;
  int nBE = quantizer->state->qss_bdf->nBE;
  int *BE = quantizer->state->qss_bdf->BE;
  int *Jac = quantizer->state->qss_bdf->Jac;
  int i, j;
  double H3;
  double hmin = quantizer->state->qss_bdf->hmin;
  double hmax = quantizer->state->qss_bdf->hmax;
  double RTE;
  double *xprev = quantizer->state->qss_bdf->xprev;
  double *dxprev = quantizer->state->oldDx;
  double tol = 1.0e-6;
  double B, f;

  gsl_spmatrix *As = quantizer->state->qss_bdf->As;
  gsl_vector *fs = quantizer->state->qss_bdf->fs;
  gsl_vector *us = quantizer->state->qss_bdf->us;
  gsl_spmatrix *Cs = quantizer->state->qss_bdf->Cs;
  gsl_splinalg_itersolve *work = quantizer->state->qss_bdf->work;

  if (quantizer->state->qss_bdf->band) {
    LIQSS_BDF_setGSLVector(quantizer, x, q, xprev);
    // solver
    Cs = gsl_spmatrix_ccs(As);
    size_t iter = 0;
    int status;
    do
      status = gsl_splinalg_itersolve_iterate(Cs, fs, tol, us, work);
    while (status == GSL_CONTINUE && ++iter < 1);
    double AB2, err, e = 0;
    tol = lqu[BES[var]] * 10;
    for (i = 0; i < nBE; i++) {
      xprev[i] = q[3 * BES[i]]; 
      AB2 = q[3 * BES[i]] + h * (3 * x[3 * BES[i] + 1] - dxprev[BES[i]]) / 2;
      q[3 * BES[i]] = gsl_vector_get(us, i);
      err = fabs(q[3 * BES[i]] - AB2);
      if (err > e) e = err;
      if (lqu[BES[i]] * 10 < tol) tol = lqu[BES[i]] * 10;
    }
    quantizer->state->qss_bdf->hprev = h;
    int SZC_LIBDF = 1;
    RTE = 3 * pow(tol / e, 1 / 3.0);
    if (RTE < 1 && SZC_LIBDF && h > hmin) {
      quantizer->state->qss_bdf->dec_step++;
      if (quantizer->state->qss_bdf->dec_step > 1) {
        h = h * RTE * 0.8;  
        if (h < hmin)
          h = hmin;  
        quantizer->state->qss_bdf->dec_step = 0;
        quantizer->state->qss_bdf->inc_step = 0;
        H3 = h / quantizer->state->qss_bdf->h;
        for (i = 0; i < nBE; i++) {
          xprev[i] = (1 - H3) * q[3 * BES[i]] + H3 * xprev[i];
        }
      }
    }
    if (RTE > 10 && SZC_LIBDF && h < hmax) {
      quantizer->state->qss_bdf->inc_step++;
      if (quantizer->state->qss_bdf->inc_step > 10) { 
        h = h * RTE * 0.8;
        if (h > hmax)
          h = hmax;  
        quantizer->state->qss_bdf->inc_step = 0;
        quantizer->state->qss_bdf->dec_step = 0;
        H3 = h / quantizer->state->qss_bdf->h;
        quantizer->state->qss_bdf->h = h;
        for (i = 0; i < nBE; i++)
          xprev[i] = (1 - H3) * q[3 * BES[i]] + H3 * xprev[i];
      }
    }
  } else {
    bool setMatrix = FALSE;
    int jacIt;
    if (quantizer->state->qss_bdf->hprev != h) {
      setMatrix = TRUE;
    }
    for (i = 0, jacIt = 0; i < nBE; i++) {
      int row = BES[i];
      int nSD = quantizer->state->qss_bdf->nSD[row];
      bool diag = FALSE;
      f = 0;
      for (j = 0; j < nSD; j++, jacIt++) {
        int sd = quantizer->state->qss_bdf->SD[row][j];
        int col = BE[sd];
        if (col != NOT_ASSIGNED) {
          if (i == col) {
            diag = TRUE;
            B = 1 - h * J[Jac[jacIt]];
          } else {
            B = -h * J[Jac[jacIt]];
          }
          if (setMatrix) {
            gsl_spmatrix_set(As, i, col, B);
          }
          f += B * q[3 * BES[col]];
        }
      }
      if (!diag) {
        if (setMatrix) {
          gsl_spmatrix_set(As, i, i, 1);
        }
        f += q[3 * BES[i]];
      }
      gsl_vector_set(fs, i, f + h * x[3 * BES[i] + 1]);
      gsl_vector_set(us, i, q[3 * BES[i]]);
    }
    // solver
    Cs = gsl_spmatrix_ccs(As);
    size_t iter = 0;
    int status;
    do
      status = gsl_splinalg_itersolve_iterate(Cs, fs, tol, us, work);
    while (status == GSL_CONTINUE && ++iter < nBE);

    for (i = 0; i < nBE; i++) {
      xprev[i] = q[3 * BES[i]]; 
      q[3 * BES[i]] = gsl_vector_get(us, i);
    }
    quantizer->state->qss_bdf->band = TRUE;
  }
  for (i = 0; i < nBE; i++) {
    dxprev[BES[i]] = x[3 * BES[i] + 1];
    q[3 * BES[i] + 1] = 0;  
  }
}

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_recomputeNextTime(QA_quantizer quantizer, int var, double t,
                                     double *nTime, double *x, double *lqu,
                                     double *q)
#else
void LIQSS_BDF_recomputeNextTime(QA_quantizer quantizer, int var, double t,
                                 double *nTime, double *x, double *lqu,
                                 double *q)
#endif
{

  int *BE = quantizer->state->qss_bdf->BE;

  if (BE[var] == NOT_ASSIGNED) {
    int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1;
    double *a = quantizer->state->a;
    double *u0 = quantizer->state->u0;
    double *u1 = quantizer->state->u1;
    bool self = quantizer->state->lSimTime->minIndex == var &&
                quantizer->state->lSimTime->type == ST_State;
    double diffQ, timeaux;
    double coeff[3];
    double *tu = quantizer->state->tx;
    int *flag2 = quantizer->state->flag2;

    if (self) {
      if (t > 0) {
        diffQ = q[cf0] - quantizer->state->qAux[var];
        if (diffQ) a[var] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
        if (a[var] > 0) a[var] = 0;
      } else {
        a[var] = 0;
      }
    }
    u0[var] = x[cf1] - q[cf0] * a[var];
    u1[var] = 2 * x[cf2] - q[cf1] * a[var];
    tu[var] = t;

    coeff[1] = q[cf1] - x[cf1];
    coeff[2] = -x[cf2];
    if (flag2[var] != 1) {
      if ((quantizer->state->lSimTime->type == ST_Event) && (a[var] < 0) &&
          (quantizer->state->nSZ[var] >
           0)) {  // we check if var is involved in the zero crossing function
                  // that produced the current event
        int i;
        for (i = 0; i < quantizer->state->nSZ[var]; i++) {
          if (quantizer->state->SZ[var][i] ==
              quantizer->state->lSimTime->minIndex) {
            nTime[var] = t;
            flag2[var] = 1;  // it does, so we restart the quantized state
                             // q[var]
          }
        }
      }
      if (flag2[var] != 1) {
        if (a[var] < 0 && self && x[cf2] != 0) {
          nTime[var] = t + fabs((q[cf1] - x[cf1]) / x[cf2]);
        } else {
          nTime[var] = INF;
        }
        coeff[0] = q[cf0] + 2 * lqu[var] - x[cf0];
        timeaux = t + minPosRoot(coeff, 2);
        if (timeaux < nTime[var]) {
          nTime[var] = timeaux;
        }
        coeff[0] = q[cf0] - 2 * lqu[var] - x[cf0];
        timeaux = t + minPosRoot(coeff, 2);
        if (timeaux < nTime[var]) {
          nTime[var] = timeaux;
        }

        if (q[cf0] * q[cf1] < 0 && fabs(q[cf0]) > 10 * lqu[var]) {
          timeaux = -q[cf0] / q[cf1] - 2 * fabs(lqu[var] / q[cf1]);
          if (nTime[var] > t + timeaux) nTime[var] = t + timeaux;
        }
        if (flag2[var] == 2 && self) flag2[var] = 0;
        double err1 = q[cf0] - x[cf0] + coeff[1] * (nTime[var] - t) / 2 +
                      coeff[2] * pow((nTime[var] - t) / 2, 2);
        if (fabs(err1) > 3 * fabs(lqu[var]))
          nTime[var] =
              t + quantizer->state->finTime * quantizer->state->minStep;
      }
    } else {
      if (self) {
        flag2[var] = 2;
        nTime[var] = t;
      }
    }
  } else {
    bool *change = quantizer->state->qss_bdf->change;
    double h = quantizer->state->qss_bdf->h;
    if (change[var] == TRUE)
      nTime[var] = t;
    else
      nTime[var] = t + h;
  }
}

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_recomputeNextTimes(QA_quantizer quantizer, int vars,
                                      int *inf, double t, double *nTime,
                                      double *x, double *lqu, double *q)
#else
void LIQSS_BDF_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf,
                                  double t, double *nTime, double *x,
                                  double *lqu, double *q)
#endif
{
  int i;
#ifdef QSS_PARALLEL
  int *map = quantizer->state->qMap;
#endif
  for (i = 0; i < vars; i++) {
#ifdef QSS_PARALLEL
    if (map[inf[i]] != NOT_ASSIGNED) {
#endif
      LIQSS_BDF_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
    }
#endif
  }
}

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_nextTime(QA_quantizer quantizer, int var, double t,
                            double *nTime, double *x, double *lqu)
#else
void LIQSS_BDF_nextTime(QA_quantizer quantizer, int var, double t,
                        double *nTime, double *x, double *lqu)
#endif
{
  int *BE = quantizer->state->qss_bdf->BE;
  if (BE[var] != NOT_ASSIGNED) {
    nTime[var] = INF;
  } else {
    int cf2 = var * 3 + 2;
    if (x[cf2] == 0) {
      nTime[var] = INF;
    } else {
      nTime[var] = t + sqrt(fabs(lqu[var] / x[cf2]));
    }
  }
}

#ifdef QSS_PARALLEL
void LIQSS_BDF_PAR_updateQuantizedState(QA_quantizer quantizer, int var,
                                        double *q, double *x, double *lqu)
#else
void LIQSS_BDF_updateQuantizedState(QA_quantizer quantizer, int var, double *q,
                                    double *x, double *lqu)
#endif
{
  int *BE = quantizer->state->qss_bdf->BE;
  if (BE[var] != NOT_ASSIGNED) {  // LIDF2
    bool *change = quantizer->state->qss_bdf->change;
    int nBE = quantizer->state->qss_bdf->nBE;
    int *BES = quantizer->state->qss_bdf->BES;

    if (change[var] == TRUE) {
      change[var] = FALSE;
    } else {
      int j;
      for (j = 0; j < nBE; j++) {
        if (BES[j] != var) {
          change[BES[j]] = TRUE;
        }
      }
#ifdef QSS_PARALLEL
      LIQSS_BDF_PAR_solver2x2_h(quantizer, x, q, lqu, var);
#else
      LIQSS_BDF_solver2x2_h(quantizer, x, q, lqu, var);
#endif
    }
  } else {  // LIQSS2
    double t = quantizer->state->lSimTime->time;
    double *a = quantizer->state->a;
    double *u0 = quantizer->state->u0;
    double *u1 = quantizer->state->u1;
    double *tu = quantizer->state->tx;
    int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1;
    double elapsed;
    double h;
    int *flag2 = quantizer->state->flag2;

    elapsed = t - quantizer->state->lSimTime->tq[var];
    quantizer->state->qAux[var] = q[cf0] + elapsed * q[cf1];
    quantizer->state->oldDx[var] = x[cf1];
    elapsed = t - tu[var];
    u0[var] = u0[var] + elapsed * u1[var];
    tu[var] = t;
    double ddx = x[cf2] * 2;
    double oldq1 = q[cf1];

    if (a[var] < 0) {
      if (ddx == 0) {
        ddx = a[var] * a[var] * q[cf0] + a[var] * u0[var] + u1[var];
        if (ddx == 0) ddx = 1e-40;
      }

      h = (quantizer->state->finTime - t);
      q[cf0] =
          ((x[cf0] + h * u0[var] + h * h / 2 * u1[var]) * (1 - h * a[var]) +
           (h * h / 2 * a[var] - h) * (u0[var] + h * u1[var])) /
          (1 - h * a[var] + h * h * a[var] * a[var] / 2);
      if (fabs(q[cf0] - x[cf0]) > 2 * lqu[var]) {
        h = sqrt(fabs(2 * lqu[var] / ddx));
        q[cf0] =
            ((x[cf0] + h * u0[var] + h * h / 2 * u1[var]) * (1 - h * a[var]) +
             (h * h / 2 * a[var] - h) * (u0[var] + h * u1[var])) /
            (1 - h * a[var] + h * h * a[var] * a[var] / 2);
      }
      while (fabs(q[cf0] - x[cf0]) > 2 * lqu[var]) {
        h = h * sqrt(lqu[var] / fabs(q[cf0] - x[cf0]));
        q[cf0] =
            ((x[cf0] + h * u0[var] + h * h / 2 * u1[var]) * (1 - h * a[var]) +
             (h * h / 2 * a[var] - h) * (u0[var] + h * u1[var])) /
            (1 - h * a[var] + h * h * a[var] * a[var] / 2);
      }
      q[cf1] = (a[var] * q[cf0] + u0[var] + h * u1[var]) / (1 - h * a[var]);

      if (x[cf2] * ddx < 0)  // no hace falta
      {
        q[cf1] = -u1[var] / a[var];
        q[cf0] = (q[cf1] - u0[var]) / a[var];
        if (fabs(q[cf0] - x[cf0]) > 2 * lqu[var]) {
          if (q[cf0] > x[cf0])
            q[cf0] = x[cf0] + lqu[var];
          else
            q[cf0] = x[cf0] - lqu[var];
        }
      }
    } else {
      ddx = u1[var];
      if (ddx > 0)
        q[cf0] = x[cf0] - lqu[var];
      else
        q[cf0] = x[cf0] + lqu[var];
      if (ddx != 0) {
        h = sqrt(2 * lqu[var] / fabs(ddx));
        q[cf1] = u0[var] + h * u1[var];
      } else {
        q[cf1] = u0[var];
      }
    }
    if (fabs(q[cf0] - x[cf0]) > 2 * lqu[var]) q[cf0] = x[cf0];
    if (flag2[var] >= 1) {
      if (oldq1 > 0)
        q[cf0] = quantizer->state->qAux[var] + lqu[var] / 100;
      else
        q[cf0] = quantizer->state->qAux[var] - lqu[var] / 100;
      q[cf1] = 0;
    }
  }
}
