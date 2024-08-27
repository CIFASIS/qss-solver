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

QSS_DECLARE_QUANTIZER_INTERFACE(mLIQSS2)

QSS_INT_FUNC_DECL(void, mLIQSS2, AxB, double A[2][2], double B[2][2], double AB[2][2]);

QSS_INT_FUNC_DECL(void, mLIQSS2, invA, double A[2][2], double invA[2][2]);

QSS_INT_FUNC_DECL(void, mLIQSS2, BEStep, QA_quantizer quantizer, double *x, double *q, double *next, int i, int j, double h, double xj0);

QSS_INT_FUNC_DECL(void, mLIQSS2, dxPrev, QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx);

QSS_INT_FUNC_DECL(void, mLIQSS2, BEStepSingle, QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *h2);
