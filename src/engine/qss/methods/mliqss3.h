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

#include <common/macros.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

QSS_DECLARE_QUANTIZER_INTERFACE(mLIQSS3)

QSS_INT_FUNC_DECL(void, mLIQSS3, AxB, double A[2][2], double B[2][2], double AB[2][2]);

QSS_INT_FUNC_DECL(void, mLIQSS3, invA, double A[2][2], double invA[2][2]);

QSS_INT_FUNC_DECL(void, mLIQSS3, solver2x2_h, QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0,
                  double *lqu);

QSS_INT_FUNC_DECL(void, mLIQSS3, solve_single, QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *h2);
