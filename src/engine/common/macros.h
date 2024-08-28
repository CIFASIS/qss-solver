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

#define QSS_MODULE_INIT(module, args...) \
  if (sim_data->params->lps > 0) {       \
    module##_PAR_init(args);             \
  } else {                               \
    module##_init(args);                 \
  }

#define TYPE_DEF(type)          \
  typedef struct type##_ *type; \
  typedef const struct type##_ *const_##type;

/// Define a minimum approximation of DQ used in QSS methods when
/// recomputing state variables next times, to avoid numerical
/// errors. @see https://github.com/CIFASIS/qss-solver/issues/258
#define DQ_APPROX 0.999999999

#ifdef QSS_PARALLEL

#define QSS_FUNC_DECL(module, name) module##_##PAR_##name

#define QSS_FUNC_INVK(module, name) module##_##PAR_##name

#define QSS_PARALLEL_EXP(exp) exp

#define QSS_PARALLEL_IF(map) if (map > NOT_ASSIGNED) {
#define QSS_PARALLEL_IF_END }

#else

#define QSS_FUNC_DECL(module, name) module##_##name

#define QSS_FUNC_INVK(module, name) module##_##name

#define QSS_PARALLEL_EXP(exp)

#define QSS_PARALLEL_IF(map)

#define QSS_PARALLEL_IF_END

#endif

#define QSS_INT_FUNC_DECL(ret_type, module, name, args...) \
  extern ret_type module##_##name(args);                   \
  extern ret_type module##_PAR_##name(args);
