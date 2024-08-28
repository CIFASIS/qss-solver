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

#include <common/data.h>
#include <common/macros.h>
#include <qss/qss_data.h>
#include <qss/qss_log.h>

#define QSS_DEFINE_OPUTPUT_OPS(module)       \
  output->ops->write = module##_write;       \
  output->ops->getSteps = module##_getSteps; \
  output->ops->save = module##_save;

#ifdef QSS_PARALLEL
#define QSS_ASSIGN_OUTPUT_OPS(module) QSS_DEFINE_OPUTPUT_OPS(module##_PAR)
#else
#define QSS_ASSIGN_OUTPUT_OPS(module) QSS_DEFINE_OPUTPUT_OPS(module)
#endif

#define QSS_DEFINE_OUTPUT_INTERFACE(module)                                                                             \
  void module##_write(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output); \
  int module##_getSteps(const_OUT_output output, int var);                                                              \
  void module##_save(OUT_output output);                                                                                \
  void module##_init(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output);

#define QSS_DECLARE_OUTPUT_INTERFACE(module) \
  QSS_DEFINE_OUTPUT_INTERFACE(module)        \
  QSS_DEFINE_OUTPUT_INTERFACE(module##_PAR)

TYPE_DEF(OUT_outputOps)
TYPE_DEF(OUT_outputState)
TYPE_DEF(OUT_output)

typedef void (*OUT_writeFn)(OUT_output, const_QSS_data, const_QSS_time, const_SD_output);

typedef int (*OUT_getStepsFn)(const_OUT_output, int);

typedef void (*OUT_saveFn)(OUT_output);

struct OUT_outputState_ {
  LG_log log;
  int *steps;
  double *values;
  double *output_var;
  int order;
};

struct OUT_outputOps_ {
  OUT_writeFn write;
  OUT_getStepsFn getSteps;
  OUT_saveFn save;
};

struct OUT_output_ {
  OUT_outputState state;
  OUT_outputOps ops;
};

OUT_output OUT_Output(QSS_data sim_data, QSS_time sim_time, SD_output sim_output);

OUT_outputState OUT_OutputState();

OUT_outputOps OUT_OutputOps();

void OUT_freeOutput(OUT_output o);

void OUT_freeOutputState(OUT_outputState o);

void OUT_freeOutputOps(OUT_outputOps o);

void OUT_write(OUT_output output, QSS_data sim_data, QSS_time sim_time, SD_output sim_output);

int OUT_getSteps(const_OUT_output output, int var);

void OUT_save(OUT_output output);
