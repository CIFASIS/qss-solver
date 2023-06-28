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

#pragma once

#ifdef QSS_PARALLEL

#define QSS_FUNC_DECL(module, name) module##_##PAR_##name

#define QSS_FUNC_INVK(module, name) module##_##PAR_##name

#define QSS_ASSIGN_QUANTIZER_OPS(module)                                  \
  quantizer->state->qMap = simData->lp->qMap;                             \
  quantizer->ops->recomputeNextTimes = module##_##PAR_recomputeNextTimes; \
  quantizer->ops->recomputeNextTime = module##_##PAR_recomputeNextTime;   \
  quantizer->ops->nextTime = module##_##PAR_nextTime;                     \
  quantizer->ops->updateQuantizedState = module##_##PAR_updateQuantizedState;

#define QSS_PARALLEL_EXP(exp) exp

#else

#define QSS_FUNC_DECL(module, name) module##_##name

#define QSS_FUNC_INVK(module, name) module##_##name

#define QSS_PARALLEL_EXP(exp)

#define QSS_ASSIGN_QUANTIZER_OPS(module)                              \
  quantizer->ops->recomputeNextTimes = module##_##recomputeNextTimes; \
  quantizer->ops->recomputeNextTime = module##_##recomputeNextTime;   \
  quantizer->ops->nextTime = module##_##nextTime;                     \
  quantizer->ops->updateQuantizedState = module##_##updateQuantizedState;

#endif

#define QSS_DECLARE_QUANTIZER_INTERFACE(module)                                                                                           \
  extern void module##_##init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime);                                                \
  extern void module##_##recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,  \
                                            double *q) __attribute__((hot));                                                              \
  extern void module##_##recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)   \
      __attribute__((hot));                                                                                                               \
  extern void module##_##nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu) __attribute__((hot)); \
  extern void module##_##updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu) __attribute__((hot));     \
  extern void module##_##PAR_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime);                                            \
  extern void module##_##PAR_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x,           \
                                                double *lqu, double *q) __attribute__((hot));                                             \
  extern void module##_##PAR_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu,          \
                                               double *q) __attribute__((hot));                                                           \
  extern void module##_##PAR_nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)                   \
      __attribute__((hot));                                                                                                               \
  extern void module##_##PAR_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu) __attribute__((hot));

#define QSS_INT_FUNC_DECL(ret_type, module, name, args...) \
  extern ret_type module##_##name(args);                   \
  extern ret_type module##_PAR_##name(args);
