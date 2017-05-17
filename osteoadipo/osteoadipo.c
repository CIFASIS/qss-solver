#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_math.h"
#include <common/utils.h>

#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_cytoplasm = 0;
double __PAR_nucleus = 0;
double __PAR_k1 = 0;
double __PAR_k2 = 0;
double __PAR_k3 = 0;
double __PAR_k4 = 0;
double __PAR_k5 = 0;
double __PAR_k_plus6 = 0;
double __PAR_k_minus6 = 0;
double __PAR_k_plus7 = 0;
double __PAR_k_minus7 = 0;
double __PAR_k_plus8 = 0;
double __PAR_k_minus8 = 0;
double __PAR_k9 = 0;
double __PAR_k10 = 0;
double __PAR_k11 = 0;
double __PAR_V12 = 0;
double __PAR_k13 = 0;
double __PAR_k14 = 0;
double __PAR_k21 = 0;
double __PAR_k15 = 0;
double __PAR_k_plus16 = 0;
double __PAR_k_minus16 = 0;
double __PAR_k_plus17 = 0;
double __PAR_k_minus17 = 0;
double __PAR_Vmax1 = 0;
double __PAR_Km1 = 0;
double __PAR_Ki = 0;
double __PAR_Vmax2 = 0;
double __PAR_Km2 = 0;
double __PAR_kcat1 = 0;
double __PAR_Km3 = 0;
double __PAR_Vmax3 = 0;
double __PAR_Km4 = 0;
double __PAR_kcat2 = 0;
double __PAR_Km5 = 0;
double __PAR_Vmax4 = 0;
double __PAR_Km6 = 0;
double __PAR_kcat3 = 0;
double __PAR_Km7 = 0;
double __PAR_Vmax5 = 0;
double __PAR_Km8 = 0;
double __PAR_kcat4 = 0;
double __PAR_Km9 = 0;
double __PAR_k18 = 0;
double __PAR_k19 = 0;
double __PAR_Vmax6 = 0;
double __PAR_Km10 = 0;
double __PAR_kcat5 = 0;
double __PAR_n1 = 0;
double __PAR_Km11 = 0;
double __PAR_k20 = 0;
double __PAR_kcat6 = 0;
double __PAR_Km12 = 0;
double __PAR_kcat7 = 0;
double __PAR_Km13 = 0;
double __PAR_Vmax7 = 0;
double __PAR_Km14 = 0;
double __PAR_normal_bCatenin_TCF = 0;
double __PAR_n = 0;
double __PAR_ap = 0;
double __PAR_bp = 0;
double __PAR_mp = 0;
double __PAR_cpp = 0;
double __PAR_kp = 0;
double __PAR_ao = 0;
double __PAR_aa = 0;
double __PAR_bo = 0;
double __PAR_ba = 0;
double __PAR_mo = 0;
double __PAR_ma = 0;
double __PAR_coo = 0;
double __PAR_caa = 0;
double __PAR_coa = 0;
double __PAR_cao = 0;
double __PAR_cop = 0;
double __PAR_cap = 0;
double __PAR_ko = 0;
double __PAR_ka = 0;

void
MOD_settings (SD_simulationSettings settings)
{
    settings->debug = 0;
    settings->parallel = FALSE;
    settings->hybrid = TRUE;
    settings->method = 4;
}

void
MOD_definition (int i, double *x, double *d, double *alg, double t, double *dx)
{
    switch (i)
    {
        case 0:
            alg[21] = __PAR_cytoplasm * __PAR_k1 * x[0] * d[(3)];
            alg[27] = __PAR_cytoplasm * __PAR_k2 * x[3];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[21]) + (1.000000000000000000000000e+00 * alg[27]));
            return;
        case 1:
            alg[21] = __PAR_cytoplasm * __PAR_k1 * x[0] * d[(3)];
            alg[27] = __PAR_cytoplasm * __PAR_k2 * x[3];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[21]) + (-1.000000000000000000000000e+00 * alg[27]));
            return;
        case 2:
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[39]) + (-1.000000000000000000000000e+00 * alg[33])
                            + (-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[57]));
            return;
        case 3:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[39])
                            + (1.000000000000000000000000e+00 * alg[33]) + (1.000000000000000000000000e+00 * alg[42]));
            return;
        case 4:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            return;
        case 5:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            return;
        case 6:
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (-1.000000000000000000000000e+00 * alg[51]));
            return;
        case 7:
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[54] = __PAR_cytoplasm * __PAR_k9 * x[21];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[36]) + (-1.000000000000000000000000e+00 * alg[54]));
            return;
        case 8:
            alg[54] = __PAR_cytoplasm * __PAR_k9 * x[21];
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[54]) + (-1.000000000000000000000000e+00 * alg[57]));
            return;
        case 9:
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[60] = __PAR_cytoplasm * __PAR_k11 * x[27];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[57]) + (-1.000000000000000000000000e+00 * alg[60]));
            return;
        case 10:
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            return;
        case 11:
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[66] = __PAR_nucleus * (__PAR_k14 + __PAR_k21 * (x[30] + x[39]));
            alg[72] = __PAR_cytoplasm * __PAR_k15 * x[33];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (1.000000000000000000000000e+00 * alg[66])
                            + (-1.000000000000000000000000e+00 * alg[72]));
            return;
        case 12:
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((-1.000000000000000000000000e+00 * alg[69]));
            return;
        case 13:
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((1.000000000000000000000000e+00 * alg[69]));
            return;
        case 14:
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm)) * ((1.000000000000000000000000e+00 * alg[51]));
            return;
        case 15:
            alg[24] = __PAR_cytoplasm * __PAR_Vmax1 * x[45] * d[(3)] * 1.000000000000000000000000e+00 / (__PAR_Km1 + x[45]) * __PAR_Ki
                    * 1.000000000000000000000000e+00 / (__PAR_Ki + x[66]);
            alg[78] = __PAR_cytoplasm * __PAR_Vmax2 * x[48] * 1.000000000000000000000000e+00 / (__PAR_Km2 + x[48]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[24]) + (1.000000000000000000000000e+00 * alg[78]));
            return;
        case 16:
            alg[24] = __PAR_cytoplasm * __PAR_Vmax1 * x[45] * d[(3)] * 1.000000000000000000000000e+00 / (__PAR_Km1 + x[45]) * __PAR_Ki
                    * 1.000000000000000000000000e+00 / (__PAR_Ki + x[66]);
            alg[78] = __PAR_cytoplasm * __PAR_Vmax2 * x[48] * 1.000000000000000000000000e+00 / (__PAR_Km2 + x[48]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[24]) + (-1.000000000000000000000000e+00 * alg[78]));
            return;
        case 17:
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            return;
        case 18:
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[81]) + (-1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[87]));
            return;
        case 19:
            alg[93] = __PAR_cytoplasm * __PAR_kcat2 * x[54] * x[57] * 1.000000000000000000000000e+00 / (__PAR_Km5 + x[57]);
            alg[96] = __PAR_cytoplasm * __PAR_Vmax4 * x[60] * 1.000000000000000000000000e+00 / (__PAR_Km6 + x[60]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[93]) + (1.000000000000000000000000e+00 * alg[96]));
            return;
        case 20:
            alg[93] = __PAR_cytoplasm * __PAR_kcat2 * x[54] * x[57] * 1.000000000000000000000000e+00 / (__PAR_Km5 + x[57]);
            alg[96] = __PAR_cytoplasm * __PAR_Vmax4 * x[60] * 1.000000000000000000000000e+00 / (__PAR_Km6 + x[60]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[93]) + (-1.000000000000000000000000e+00 * alg[96]));
            return;
        case 21:
            alg[99] = __PAR_cytoplasm * __PAR_kcat3 * x[60] * x[63] * 1.000000000000000000000000e+00 / (__PAR_Km7 + x[63]);
            alg[102] = __PAR_cytoplasm * __PAR_Vmax5 * x[66] * 1.000000000000000000000000e+00 / (__PAR_Km8 + x[66]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[99]) + (1.000000000000000000000000e+00 * alg[102]));
            return;
        case 22:
            alg[99] = __PAR_cytoplasm * __PAR_kcat3 * x[60] * x[63] * 1.000000000000000000000000e+00 / (__PAR_Km7 + x[63]);
            alg[102] = __PAR_cytoplasm * __PAR_Vmax5 * x[66] * 1.000000000000000000000000e+00 / (__PAR_Km8 + x[66]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[99]) + (-1.000000000000000000000000e+00 * alg[102]));
            return;
        case 23:
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[105]) + (1.000000000000000000000000e+00 * alg[84]));
            return;
        case 24:
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[84]) + (1.000000000000000000000000e+00 * alg[108]));
            return;
        case 25:
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[108]));
            return;
        case 26:
            alg[75] = __PAR_cytoplasm * __PAR_kcat5 * __math__pow (x[39], (__PAR_n1)) * 1.000000000000000000000000e+00
                    / (__math__pow (__PAR_Km11, (__PAR_n1)) + __math__pow (x[39], (__PAR_n1)));
            alg[111] = __PAR_cytoplasm * __PAR_k20 * x[78];
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[75]) + (-1.000000000000000000000000e+00 * alg[111]));
            return;
        case 27:
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            dx[1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[45]) + (-1.000000000000000000000000e+00 * alg[114]));
            return;
        case 28:
            alg[0] = d[(2)];
            alg[3] = alg[0];
            dx[1] = (__PAR_ap * __math__pow (x[84], __PAR_n) + __PAR_bp) * 1.000000000000000000000000e+00
                    / (__PAR_mp + alg[3] + __PAR_cpp * __math__pow (x[84], __PAR_n)) - __PAR_kp * x[84];
            return;
        case 29:
            alg[6] = d[(0)];
            alg[9] = alg[6];
            alg[12] = alg[9];
            dx[1] = (__PAR_ao * __math__pow (x[87], __PAR_n) + __PAR_bo + alg[12]) * 1.000000000000000000000000e+00
                    / (__PAR_mo + __PAR_coo * __math__pow (x[87], __PAR_n) + __PAR_coa * __math__pow (x[90], __PAR_n)
                            + __PAR_cop * __math__pow (x[84], __PAR_n)) - __PAR_ko * x[87];
            return;
        case 30:
            alg[15] = d[(6)];
            alg[18] = alg[15];
            dx[1] = (__PAR_aa * __math__pow (x[90], __PAR_n) + __PAR_ba + alg[18]) * 1.000000000000000000000000e+00
                    / (__PAR_ma + __PAR_caa * __math__pow (x[90], __PAR_n) + __PAR_cao * __math__pow (x[87], __PAR_n)
                            + __PAR_cap * __math__pow (x[84], __PAR_n)) - __PAR_ka * x[90];
            return;
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    switch (i)
    {
        case 0:
            alg[21] = __PAR_cytoplasm * __PAR_k1 * x[0] * d[(3)];
            alg[27] = __PAR_cytoplasm * __PAR_k2 * x[3];
            der[0 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[21]) + (1.000000000000000000000000e+00 * alg[27]));
            der[3 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[21]) + (-1.000000000000000000000000e+00 * alg[27]));
            return;
        case 1:
            alg[21] = __PAR_cytoplasm * __PAR_k1 * x[0] * d[(3)];
            alg[27] = __PAR_cytoplasm * __PAR_k2 * x[3];
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            der[0 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[21]) + (1.000000000000000000000000e+00 * alg[27]));
            der[3 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[21]) + (-1.000000000000000000000000e+00 * alg[27]));
            der[9 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[39])
                            + (1.000000000000000000000000e+00 * alg[33]) + (1.000000000000000000000000e+00 * alg[42]));
            der[12 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            der[15 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            return;
        case 2:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[54] = __PAR_cytoplasm * __PAR_k9 * x[21];
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[6 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[39]) + (-1.000000000000000000000000e+00 * alg[33])
                            + (-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[57]));
            der[9 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[39])
                            + (1.000000000000000000000000e+00 * alg[33]) + (1.000000000000000000000000e+00 * alg[42]));
            der[21 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[36]) + (-1.000000000000000000000000e+00 * alg[54]));
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            return;
        case 3:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            der[6 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[39]) + (-1.000000000000000000000000e+00 * alg[33])
                            + (-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[57]));
            der[9 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[39])
                            + (1.000000000000000000000000e+00 * alg[33]) + (1.000000000000000000000000e+00 * alg[42]));
            der[12 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            der[15 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            return;
        case 4:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            der[9 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[39])
                            + (1.000000000000000000000000e+00 * alg[33]) + (1.000000000000000000000000e+00 * alg[42]));
            der[12 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            der[15 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            der[81 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[45]) + (-1.000000000000000000000000e+00 * alg[114]));
            return;
        case 5:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[66] = __PAR_nucleus * (__PAR_k14 + __PAR_k21 * (x[30] + x[39]));
            alg[72] = __PAR_cytoplasm * __PAR_k15 * x[33];
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            der[9 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[39])
                            + (1.000000000000000000000000e+00 * alg[33]) + (1.000000000000000000000000e+00 * alg[42]));
            der[12 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            der[15 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            der[18 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (-1.000000000000000000000000e+00 * alg[51]));
            der[33 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (1.000000000000000000000000e+00 * alg[66])
                            + (-1.000000000000000000000000e+00 * alg[72]));
            return;
        case 6:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[66] = __PAR_nucleus * (__PAR_k14 + __PAR_k21 * (x[30] + x[39]));
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[72] = __PAR_cytoplasm * __PAR_k15 * x[33];
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[15 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            der[18 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (-1.000000000000000000000000e+00 * alg[51]));
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            der[33 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (1.000000000000000000000000e+00 * alg[66])
                            + (-1.000000000000000000000000e+00 * alg[72]));
            der[42 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm)) * ((1.000000000000000000000000e+00 * alg[51]));
            return;
        case 7:
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[54] = __PAR_cytoplasm * __PAR_k9 * x[21];
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[6 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[39]) + (-1.000000000000000000000000e+00 * alg[33])
                            + (-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[57]));
            der[21 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[36]) + (-1.000000000000000000000000e+00 * alg[54]));
            der[24 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[54]) + (-1.000000000000000000000000e+00 * alg[57]));
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            return;
        case 8:
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[54] = __PAR_cytoplasm * __PAR_k9 * x[21];
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[60] = __PAR_cytoplasm * __PAR_k11 * x[27];
            der[6 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[39]) + (-1.000000000000000000000000e+00 * alg[33])
                            + (-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[57]));
            der[24 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[54]) + (-1.000000000000000000000000e+00 * alg[57]));
            der[27 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[57]) + (-1.000000000000000000000000e+00 * alg[60]));
            return;
        case 9:
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[60] = __PAR_cytoplasm * __PAR_k11 * x[27];
            der[27 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[57]) + (-1.000000000000000000000000e+00 * alg[60]));
            return;
        case 10:
            alg[33] = __PAR_cytoplasm * __PAR_k5 * x[6];
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[39] = __PAR_cytoplasm * __PAR_k4 * x[9];
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[54] = __PAR_cytoplasm * __PAR_k9 * x[21];
            alg[57] = __PAR_cytoplasm * __PAR_k10 * x[24];
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[66] = __PAR_nucleus * (__PAR_k14 + __PAR_k21 * (x[30] + x[39]));
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[72] = __PAR_cytoplasm * __PAR_k15 * x[33];
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[6 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[39]) + (-1.000000000000000000000000e+00 * alg[33])
                            + (-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[57]));
            der[18 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (-1.000000000000000000000000e+00 * alg[51]));
            der[21 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[36]) + (-1.000000000000000000000000e+00 * alg[54]));
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            der[33 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (1.000000000000000000000000e+00 * alg[66])
                            + (-1.000000000000000000000000e+00 * alg[72]));
            der[36 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((-1.000000000000000000000000e+00 * alg[69]));
            der[39 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((1.000000000000000000000000e+00 * alg[69]));
            der[42 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm)) * ((1.000000000000000000000000e+00 * alg[51]));
            return;
        case 11:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[66] = __PAR_nucleus * (__PAR_k14 + __PAR_k21 * (x[30] + x[39]));
            alg[72] = __PAR_cytoplasm * __PAR_k15 * x[33];
            der[15 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (1.000000000000000000000000e+00 * alg[48]));
            der[18 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (-1.000000000000000000000000e+00 * alg[51]));
            der[33 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (1.000000000000000000000000e+00 * alg[66])
                            + (-1.000000000000000000000000e+00 * alg[72]));
            return;
        case 12:
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            der[36 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((-1.000000000000000000000000e+00 * alg[69]));
            der[39 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((1.000000000000000000000000e+00 * alg[69]));
            return;
        case 13:
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[66] = __PAR_nucleus * (__PAR_k14 + __PAR_k21 * (x[30] + x[39]));
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[72] = __PAR_cytoplasm * __PAR_k15 * x[33];
            alg[75] = __PAR_cytoplasm * __PAR_kcat5 * __math__pow (x[39], (__PAR_n1)) * 1.000000000000000000000000e+00
                    / (__math__pow (__PAR_Km11, (__PAR_n1)) + __math__pow (x[39], (__PAR_n1)));
            alg[111] = __PAR_cytoplasm * __PAR_k20 * x[78];
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            der[33 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (1.000000000000000000000000e+00 * alg[66])
                            + (-1.000000000000000000000000e+00 * alg[72]));
            der[36 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((-1.000000000000000000000000e+00 * alg[69]));
            der[39 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus)) * ((1.000000000000000000000000e+00 * alg[69]));
            der[78 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[75]) + (-1.000000000000000000000000e+00 * alg[111]));
            return;
        case 14:
            alg[36] = __PAR_cytoplasm * (__PAR_k_plus8 * x[6] * x[30] - __PAR_k_minus8 * x[21]);
            alg[48] = __PAR_cytoplasm * (__PAR_k_plus7 * x[18] * x[33] - __PAR_k_minus7 * x[15]);
            alg[51] = __PAR_cytoplasm * (__PAR_k_plus17 * x[18] * x[30] - __PAR_k_minus17 * x[42]);
            alg[63] = __PAR_nucleus * __PAR_k13 * x[30];
            alg[69] = __PAR_nucleus * (__PAR_k_plus16 * x[30] * x[36] - __PAR_k_minus16 * x[39]);
            alg[120] = __PAR_cytoplasm * __PAR_V12;
            der[18 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[48]) + (-1.000000000000000000000000e+00 * alg[51]));
            der[30 + 1] = (1.000000000000000000000000e+00 / (__PAR_nucleus))
                    * ((-1.000000000000000000000000e+00 * alg[36]) + (1.000000000000000000000000e+00 * alg[120])
                            + (-1.000000000000000000000000e+00 * alg[63]) + (-1.000000000000000000000000e+00 * alg[69])
                            + (-1.000000000000000000000000e+00 * alg[51]));
            der[42 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm)) * ((1.000000000000000000000000e+00 * alg[51]));
            return;
        case 15:
            alg[24] = __PAR_cytoplasm * __PAR_Vmax1 * x[45] * d[(3)] * 1.000000000000000000000000e+00 / (__PAR_Km1 + x[45]) * __PAR_Ki
                    * 1.000000000000000000000000e+00 / (__PAR_Ki + x[66]);
            alg[78] = __PAR_cytoplasm * __PAR_Vmax2 * x[48] * 1.000000000000000000000000e+00 / (__PAR_Km2 + x[48]);
            der[45 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[24]) + (1.000000000000000000000000e+00 * alg[78]));
            der[48 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[24]) + (-1.000000000000000000000000e+00 * alg[78]));
            return;
        case 16:
            alg[24] = __PAR_cytoplasm * __PAR_Vmax1 * x[45] * d[(3)] * 1.000000000000000000000000e+00 / (__PAR_Km1 + x[45]) * __PAR_Ki
                    * 1.000000000000000000000000e+00 / (__PAR_Ki + x[66]);
            alg[78] = __PAR_cytoplasm * __PAR_Vmax2 * x[48] * 1.000000000000000000000000e+00 / (__PAR_Km2 + x[48]);
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            der[45 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[24]) + (1.000000000000000000000000e+00 * alg[78]));
            der[48 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[24]) + (-1.000000000000000000000000e+00 * alg[78]));
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[54 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[81]) + (-1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[87]));
            return;
        case 17:
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[54 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[81]) + (-1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[87]));
            der[69 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[105]) + (1.000000000000000000000000e+00 * alg[84]));
            der[72 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[84]) + (1.000000000000000000000000e+00 * alg[108]));
            return;
        case 18:
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[93] = __PAR_cytoplasm * __PAR_kcat2 * x[54] * x[57] * 1.000000000000000000000000e+00 / (__PAR_Km5 + x[57]);
            alg[96] = __PAR_cytoplasm * __PAR_Vmax4 * x[60] * 1.000000000000000000000000e+00 / (__PAR_Km6 + x[60]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[54 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[81]) + (-1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[87]));
            der[57 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[93]) + (1.000000000000000000000000e+00 * alg[96]));
            der[60 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[93]) + (-1.000000000000000000000000e+00 * alg[96]));
            return;
        case 19:
            alg[93] = __PAR_cytoplasm * __PAR_kcat2 * x[54] * x[57] * 1.000000000000000000000000e+00 / (__PAR_Km5 + x[57]);
            alg[96] = __PAR_cytoplasm * __PAR_Vmax4 * x[60] * 1.000000000000000000000000e+00 / (__PAR_Km6 + x[60]);
            der[57 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[93]) + (1.000000000000000000000000e+00 * alg[96]));
            der[60 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[93]) + (-1.000000000000000000000000e+00 * alg[96]));
            return;
        case 20:
            alg[93] = __PAR_cytoplasm * __PAR_kcat2 * x[54] * x[57] * 1.000000000000000000000000e+00 / (__PAR_Km5 + x[57]);
            alg[96] = __PAR_cytoplasm * __PAR_Vmax4 * x[60] * 1.000000000000000000000000e+00 / (__PAR_Km6 + x[60]);
            alg[99] = __PAR_cytoplasm * __PAR_kcat3 * x[60] * x[63] * 1.000000000000000000000000e+00 / (__PAR_Km7 + x[63]);
            alg[102] = __PAR_cytoplasm * __PAR_Vmax5 * x[66] * 1.000000000000000000000000e+00 / (__PAR_Km8 + x[66]);
            der[57 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[93]) + (1.000000000000000000000000e+00 * alg[96]));
            der[60 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[93]) + (-1.000000000000000000000000e+00 * alg[96]));
            der[63 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[99]) + (1.000000000000000000000000e+00 * alg[102]));
            der[66 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[99]) + (-1.000000000000000000000000e+00 * alg[102]));
            return;
        case 21:
            alg[99] = __PAR_cytoplasm * __PAR_kcat3 * x[60] * x[63] * 1.000000000000000000000000e+00 / (__PAR_Km7 + x[63]);
            alg[102] = __PAR_cytoplasm * __PAR_Vmax5 * x[66] * 1.000000000000000000000000e+00 / (__PAR_Km8 + x[66]);
            der[63 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[99]) + (1.000000000000000000000000e+00 * alg[102]));
            der[66 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[99]) + (-1.000000000000000000000000e+00 * alg[102]));
            return;
        case 22:
            alg[24] = __PAR_cytoplasm * __PAR_Vmax1 * x[45] * d[(3)] * 1.000000000000000000000000e+00 / (__PAR_Km1 + x[45]) * __PAR_Ki
                    * 1.000000000000000000000000e+00 / (__PAR_Ki + x[66]);
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[78] = __PAR_cytoplasm * __PAR_Vmax2 * x[48] * 1.000000000000000000000000e+00 / (__PAR_Km2 + x[48]);
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[99] = __PAR_cytoplasm * __PAR_kcat3 * x[60] * x[63] * 1.000000000000000000000000e+00 / (__PAR_Km7 + x[63]);
            alg[102] = __PAR_cytoplasm * __PAR_Vmax5 * x[66] * 1.000000000000000000000000e+00 / (__PAR_Km8 + x[66]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            der[12 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            der[45 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[24]) + (1.000000000000000000000000e+00 * alg[78]));
            der[48 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[24]) + (-1.000000000000000000000000e+00 * alg[78]));
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[63 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[99]) + (1.000000000000000000000000e+00 * alg[102]));
            der[66 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[99]) + (-1.000000000000000000000000e+00 * alg[102]));
            der[69 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[105]) + (1.000000000000000000000000e+00 * alg[84]));
            der[75 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[108]));
            der[81 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[45]) + (-1.000000000000000000000000e+00 * alg[114]));
            return;
        case 23:
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[69 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[105]) + (1.000000000000000000000000e+00 * alg[84]));
            der[72 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[84]) + (1.000000000000000000000000e+00 * alg[108]));
            der[75 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[108]));
            return;
        case 24:
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[69 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[105]) + (1.000000000000000000000000e+00 * alg[84]));
            der[72 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[84]) + (1.000000000000000000000000e+00 * alg[108]));
            return;
        case 25:
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[108] = __PAR_cytoplasm * __PAR_Vmax6 * x[75] * 1.000000000000000000000000e+00 / (__PAR_Km10 + x[75]);
            der[72 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[84]) + (1.000000000000000000000000e+00 * alg[108]));
            der[75 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[108]));
            return;
        case 26:
            alg[75] = __PAR_cytoplasm * __PAR_kcat5 * __math__pow (x[39], (__PAR_n1)) * 1.000000000000000000000000e+00
                    / (__math__pow (__PAR_Km11, (__PAR_n1)) + __math__pow (x[39], (__PAR_n1)));
            alg[81] = __PAR_cytoplasm * __PAR_kcat1 * x[48] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km3 + x[51]);
            alg[84] = __PAR_cytoplasm * (__PAR_k18 * x[51] * x[72] - __PAR_k19 * x[69]);
            alg[87] = __PAR_cytoplasm * __PAR_kcat6 * x[78] * x[51] * 1.000000000000000000000000e+00 / (__PAR_Km12 + x[51]);
            alg[90] = __PAR_cytoplasm * __PAR_Vmax3 * x[54] * 1.000000000000000000000000e+00 / (__PAR_Km4 + x[54]);
            alg[105] = __PAR_cytoplasm * __PAR_kcat4 * x[66] * x[69] * 1.000000000000000000000000e+00 / (__PAR_Km9 + x[69]);
            alg[111] = __PAR_cytoplasm * __PAR_k20 * x[78];
            der[51 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((-1.000000000000000000000000e+00 * alg[81]) + (1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[105]) + (-1.000000000000000000000000e+00 * alg[84])
                            + (-1.000000000000000000000000e+00 * alg[87]));
            der[54 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[81]) + (-1.000000000000000000000000e+00 * alg[90])
                            + (1.000000000000000000000000e+00 * alg[87]));
            der[78 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[75]) + (-1.000000000000000000000000e+00 * alg[111]));
            return;
        case 27:
            alg[30] = __PAR_cytoplasm * __PAR_k3 * x[3] * x[9];
            alg[42] = __PAR_cytoplasm * (__PAR_k_plus6 * x[12] * x[15] - __PAR_k_minus6 * x[9]);
            alg[45] = __PAR_cytoplasm * __PAR_kcat7 * x[66] * x[12] * 1.000000000000000000000000e+00 / (__PAR_Km13 + x[12]);
            alg[114] = __PAR_cytoplasm * __PAR_Vmax7 * x[81] * 1.000000000000000000000000e+00 / (__PAR_Km14 + x[81]);
            der[12 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[30]) + (-1.000000000000000000000000e+00 * alg[42])
                            + (-1.000000000000000000000000e+00 * alg[45]) + (1.000000000000000000000000e+00 * alg[114]));
            der[81 + 1] = (1.000000000000000000000000e+00 / (__PAR_cytoplasm))
                    * ((1.000000000000000000000000e+00 * alg[45]) + (-1.000000000000000000000000e+00 * alg[114]));
            return;
        case 28:
            alg[0] = d[(2)];
            alg[3] = alg[0];
            alg[6] = d[(0)];
            alg[9] = alg[6];
            alg[12] = alg[9];
            alg[15] = d[(6)];
            alg[18] = alg[15];
            der[84 + 1] = (__PAR_ap * __math__pow (x[84], __PAR_n) + __PAR_bp) * 1.000000000000000000000000e+00
                    / (__PAR_mp + alg[3] + __PAR_cpp * __math__pow (x[84], __PAR_n)) - __PAR_kp * x[84];
            der[87 + 1] = (__PAR_ao * __math__pow (x[87], __PAR_n) + __PAR_bo + alg[12]) * 1.000000000000000000000000e+00
                    / (__PAR_mo + __PAR_coo * __math__pow (x[87], __PAR_n) + __PAR_coa * __math__pow (x[90], __PAR_n)
                            + __PAR_cop * __math__pow (x[84], __PAR_n)) - __PAR_ko * x[87];
            der[90 + 1] = (__PAR_aa * __math__pow (x[90], __PAR_n) + __PAR_ba + alg[18]) * 1.000000000000000000000000e+00
                    / (__PAR_ma + __PAR_caa * __math__pow (x[90], __PAR_n) + __PAR_cao * __math__pow (x[87], __PAR_n)
                            + __PAR_cap * __math__pow (x[84], __PAR_n)) - __PAR_ka * x[90];
            return;
        case 29:
            alg[6] = d[(0)];
            alg[9] = alg[6];
            alg[12] = alg[9];
            alg[15] = d[(6)];
            alg[18] = alg[15];
            der[87 + 1] = (__PAR_ao * __math__pow (x[87], __PAR_n) + __PAR_bo + alg[12]) * 1.000000000000000000000000e+00
                    / (__PAR_mo + __PAR_coo * __math__pow (x[87], __PAR_n) + __PAR_coa * __math__pow (x[90], __PAR_n)
                            + __PAR_cop * __math__pow (x[84], __PAR_n)) - __PAR_ko * x[87];
            der[90 + 1] = (__PAR_aa * __math__pow (x[90], __PAR_n) + __PAR_ba + alg[18]) * 1.000000000000000000000000e+00
                    / (__PAR_ma + __PAR_caa * __math__pow (x[90], __PAR_n) + __PAR_cao * __math__pow (x[87], __PAR_n)
                            + __PAR_cap * __math__pow (x[84], __PAR_n)) - __PAR_ka * x[90];
            return;
        case 30:
            alg[6] = d[(0)];
            alg[9] = alg[6];
            alg[12] = alg[9];
            alg[15] = d[(6)];
            alg[18] = alg[15];
            der[87 + 1] = (__PAR_ao * __math__pow (x[87], __PAR_n) + __PAR_bo + alg[12]) * 1.000000000000000000000000e+00
                    / (__PAR_mo + __PAR_coo * __math__pow (x[87], __PAR_n) + __PAR_coa * __math__pow (x[90], __PAR_n)
                            + __PAR_cop * __math__pow (x[84], __PAR_n)) - __PAR_ko * x[87];
            der[90 + 1] = (__PAR_aa * __math__pow (x[90], __PAR_n) + __PAR_ba + alg[18]) * 1.000000000000000000000000e+00
                    / (__PAR_ma + __PAR_caa * __math__pow (x[90], __PAR_n) + __PAR_cao * __math__pow (x[87], __PAR_n)
                            + __PAR_cap * __math__pow (x[84], __PAR_n)) - __PAR_ka * x[90];
            return;
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    switch (i)
    {
        case 0:
            zc[0] = t - (d[(5)]);
            return;
        case 1:
            zc[0] = t - (d[(4)]);
            return;
        case 2:
            zc[0] = x[39] - (1.100000000000000088817842e+00 * __PAR_normal_bCatenin_TCF);
            return;
        case 3:
            zc[0] = d[(3)] - (0.0);
            return;
        case 4:
            zc[0] = d[(3)] - (0.0);
            return;
        case 5:
            zc[0] = t - (500.0);
            return;
        case 6:
            zc[0] = t - (1000.0);
            return;
        case 7:
            zc[0] = d[(3)] - (1.0);
            return;
        case 8:
            zc[0] = d[(3)] - (1.0);
            return;
        case 9:
            zc[0] = t - (1000.0);
            return;
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 0:
            if (d[(2)] == 0.0)
            {
                d[(2)] = 1.0;
            }
            if (d[(2)] == 0.0)
            {
                d[(5)] = t + 1.000000000000000020816682e-02;
            }
            else
            {
                d[(5)] = d[(1)];
            }
            return;
        case 1:
            if (d[(6)] == 0.0)
            {
                d[(6)] = 8.000000000000000444089210e-01;
            }
            if (d[(6)] == 0.0)
            {
                d[(4)] = t + 1.000000000000000020816682e-03;
            }
            else
            {
                d[(4)] = d[(1)];
            }
            return;
        case 2:
            d[(0)] = 8.000000000000000444089210e-01;
            return;
        case 3:
            if (d[(3)] == 0.0 && t >= 500.0 && t <= 1000.0)
            {
                d[(3)] = 1.0;
            }
            return;
        case 5:
            if (d[(3)] == 0.0 && t >= 500.0 && t <= 1000.0)
            {
                d[(3)] = 1.0;
            }
            return;
        case 7:
            if (d[(3)] == 1.0 && t > 1000.0)
            {
                d[(3)] = 0.0;
            }
            return;
        case 9:
            if (d[(3)] == 1.0 && t > 1000.0)
            {
                d[(3)] = 0.0;
            }
            return;
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 2:
            d[(0)] = 0.000000000000000000000000e+00;
            return;
        case 4:
            if (d[(3)] == 0.0 && t >= 500.0 && t <= 1000.0)
            {
                d[(3)] = 1.0;
            }
            return;
        case 6:
            if (d[(3)] == 0.0 && t >= 500.0 && t <= 1000.0)
            {
                d[(3)] = 1.0;
            }
            return;
        case 8:
            if (d[(3)] == 1.0 && t > 1000.0)
            {
                d[(3)] = 0.0;
            }
            return;
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = d[(2)];
            return;
        case 1:
            out[0] = d[(6)];
            return;
        case 2:
            out[0] = d[(3)];
            return;
        case 3:
            out[0] = x[0];
            return;
        case 4:
            out[0] = x[3];
            return;
        case 5:
            out[0] = x[6];
            return;
        case 6:
            out[0] = x[9];
            return;
        case 7:
            out[0] = x[12];
            return;
        case 8:
            out[0] = x[15];
            return;
        case 9:
            out[0] = x[18];
            return;
        case 10:
            out[0] = x[21];
            return;
        case 11:
            out[0] = x[24];
            return;
        case 12:
            out[0] = x[27];
            return;
        case 13:
            out[0] = x[30];
            return;
        case 14:
            out[0] = x[33];
            return;
        case 15:
            out[0] = x[36];
            return;
        case 16:
            out[0] = x[39];
            return;
        case 17:
            out[0] = x[42];
            return;
        case 18:
            out[0] = x[45];
            return;
        case 19:
            out[0] = x[48];
            return;
        case 20:
            out[0] = x[51];
            return;
        case 21:
            out[0] = x[54];
            return;
        case 22:
            out[0] = x[57];
            return;
        case 23:
            out[0] = x[60];
            return;
        case 24:
            out[0] = x[63];
            return;
        case 25:
            out[0] = x[66];
            return;
        case 26:
            out[0] = x[69];
            return;
        case 27:
            out[0] = x[72];
            return;
        case 28:
            out[0] = x[75];
            return;
        case 29:
            out[0] = x[78];
            return;
        case 30:
            out[0] = x[81];
            return;
        case 31:
            out[0] = x[84];
            return;
        case 32:
            out[0] = x[87];
            return;
        case 33:
            out[0] = x[90];
            return;
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *discretes = (int*) malloc (7 * sizeof(int));
    int *events = (int*) malloc (10 * sizeof(int));
    int *outputs = (int*) malloc (34 * sizeof(int));
    int *states = (int*) malloc (31 * sizeof(int));
    int i;
    simulator->data = QSS_Data (31, 7, 10, 0, 41, "osteoadipo");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_cytoplasm = 1.000000000000000000000000e+00;
    __PAR_nucleus = 1.000000000000000000000000e+00;
    __PAR_k1 = 1.819999999999999951150187e-01;
    __PAR_k2 = 1.820000000000000089928065e-02;
    __PAR_k3 = 5.000000000000000277555756e-02;
    __PAR_k4 = 2.670000000000000150990331e-01;
    __PAR_k5 = 1.330000000000000071054274e-01;
    __PAR_k_plus6 = 9.089999999999999469313394e-02;
    __PAR_k_minus6 = 9.090000000000000301980663e-01;
    __PAR_k_plus7 = 1.000000000000000000000000e+00;
    __PAR_k_minus7 = 5.000000000000000000000000e+01;
    __PAR_k_plus8 = 1.000000000000000000000000e+00;
    __PAR_k_minus8 = 1.200000000000000000000000e+02;
    __PAR_k9 = 2.060000000000000000000000e+02;
    __PAR_k10 = 2.060000000000000000000000e+02;
    __PAR_k11 = 4.169999999999999817923424e-01;
    __PAR_V12 = 4.229999999999999871214129e-01;
    __PAR_k13 = 2.570000000000000127224620e-04;
    __PAR_k14 = 8.220000000000000580941545e-05;
    __PAR_k21 = 9.999999999999999547481118e-07;
    __PAR_k15 = 1.670000000000000095479180e-01;
    __PAR_k_plus16 = 1.000000000000000000000000e+00;
    __PAR_k_minus16 = 3.000000000000000000000000e+01;
    __PAR_k_plus17 = 1.000000000000000000000000e+00;
    __PAR_k_minus17 = 1.200000000000000000000000e+03;
    __PAR_Vmax1 = 1.500000000000000000000000e+02;
    __PAR_Km1 = 1.000000000000000000000000e+01;
    __PAR_Ki = 9.000000000000000000000000e+00;
    __PAR_Vmax2 = 1.500000000000000000000000e+01;
    __PAR_Km2 = 8.000000000000000000000000e+00;
    __PAR_kcat1 = 1.500000000000000000000000e+00;
    __PAR_Km3 = 1.500000000000000000000000e+01;
    __PAR_Vmax3 = 4.500000000000000000000000e+01;
    __PAR_Km4 = 1.500000000000000000000000e+01;
    __PAR_kcat2 = 1.500000000000000000000000e+00;
    __PAR_Km5 = 1.500000000000000000000000e+01;
    __PAR_Vmax4 = 4.500000000000000000000000e+01;
    __PAR_Km6 = 1.500000000000000000000000e+01;
    __PAR_kcat3 = 1.500000000000000000000000e+00;
    __PAR_Km7 = 1.500000000000000000000000e+01;
    __PAR_Vmax5 = 4.500000000000000000000000e+01;
    __PAR_Km8 = 1.500000000000000000000000e+01;
    __PAR_kcat4 = 1.500000000000000000000000e+00;
    __PAR_Km9 = 9.000000000000000000000000e+00;
    __PAR_k18 = 1.499999999999999944488849e-01;
    __PAR_k19 = 3.900000000000000000000000e+01;
    __PAR_Vmax6 = 4.500000000000000000000000e+01;
    __PAR_Km10 = 1.200000000000000000000000e+01;
    __PAR_kcat5 = 5.999999999999999777955395e-01;
    __PAR_n1 = 2.000000000000000000000000e+00;
    __PAR_Km11 = 1.500000000000000000000000e+01;
    __PAR_k20 = 1.499999999999999944488849e-02;
    __PAR_kcat6 = 1.500000000000000000000000e+00;
    __PAR_Km12 = 1.500000000000000000000000e+01;
    __PAR_kcat7 = 1.500000000000000000000000e+00;
    __PAR_Km13 = 1.500000000000000000000000e+01;
    __PAR_Vmax7 = 4.500000000000000000000000e+01;
    __PAR_Km14 = 1.500000000000000000000000e+01;
    __PAR_normal_bCatenin_TCF = 8.810000000000000497379915e+00;
    __PAR_n = 2.0;
    __PAR_ap = 2.000000000000000111022302e-01;
    __PAR_bp = 5.000000000000000000000000e-01;
    __PAR_mp = 10.0;
    __PAR_cpp = 1.000000000000000055511151e-01;
    __PAR_kp = 1.000000000000000055511151e-01;
    __PAR_ao = 1.000000000000000055511151e-01;
    __PAR_aa = 1.000000000000000055511151e-01;
    __PAR_bo = 1.0;
    __PAR_ba = 1.0;
    __PAR_mo = 1.0;
    __PAR_ma = 1.0;
    __PAR_coo = 1.000000000000000055511151e-01;
    __PAR_caa = 1.000000000000000055511151e-01;
    __PAR_coa = 1.000000000000000055511151e-01;
    __PAR_cao = 1.000000000000000055511151e-01;
    __PAR_cop = 5.000000000000000000000000e-01;
    __PAR_cap = 5.000000000000000000000000e-01;
    __PAR_ko = 1.000000000000000055511151e-01;
    __PAR_ka = 1.000000000000000055511151e-01;
    modelData->x[90] = 0.0;
    modelData->x[87] = 0.0;
    modelData->x[84] = 12.0;
    modelData->d[(1)] = 1.000000000000000000000000e+20;
    modelData->d[(6)] = 0.0;
    modelData->d[(2)] = 0.0;
    modelData->d[(3)] = 0.0;
    modelData->x[0] = 1.000000000000000000000000e+02;
    modelData->x[27] = 9.880999999999999783284466e-01;
    modelData->x[33] = 8.000000000000000383373888e-04;
    modelData->x[36] = 6.187899999999999955946350e+00;
    modelData->x[42] = 3.439200000000000034816594e+00;
    modelData->x[45] = 2.000000000000000000000000e+02;
    modelData->x[48] = 0.000000000000000000000000e+00;
    modelData->x[51] = 1.125584999999999951114660e+02;
    modelData->x[54] = 6.485999999999999765520897e+00;
    modelData->x[3] = 0.000000000000000000000000e+00;
    modelData->x[57] = 2.961136999999999943611328e+02;
    modelData->x[60] = 3.886299999999999865707423e+00;
    modelData->x[63] = 2.978897000000000048203219e+02;
    modelData->x[66] = 2.110300000000000064659389e+00;
    modelData->x[69] = 1.809594999999999913598003e+02;
    modelData->x[72] = 4.181788000000000238287612e+02;
    modelData->x[75] = 8.618999999999999994670929e-01;
    modelData->x[78] = 1.026299999999999990052402e+01;
    modelData->x[81] = 8.554399999999999781863380e-01;
    modelData->x[6] = 1.529999999999999936439732e-02;
    modelData->x[9] = 7.599999999999999984734433e-03;
    modelData->x[12] = 4.913719999999999998863132e+01;
    modelData->x[15] = 1.500000000000000031225023e-03;
    modelData->x[18] = 9.660190000000000054569682e+01;
    modelData->x[21] = 2.000000000000000041633363e-03;
    modelData->x[24] = 2.000000000000000041633363e-03;
    modelData->x[30] = 4.272240000000000037516656e+01;
    modelData->x[39] = 8.812099999999999155875230e+00;
    // Initialize model code.
    modelData->d[(5)] = 1.000000000000000020816682e-02;
    modelData->d[(4)] = 1.000000000000000020816682e-03;
    modelData->nDS[28] = 1;
    modelData->nDS[29] = 3;
    modelData->nDS[30] = 3;
    modelData->nDS[0]++;
    modelData->nDS[0]++;
    modelData->nDS[1]++;
    modelData->nDS[1]++;
    modelData->nDS[2]++;
    modelData->nDS[2]++;
    modelData->nDS[2]++;
    modelData->nDS[2]++;
    modelData->nDS[2]++;
    modelData->nDS[3]++;
    modelData->nDS[3]++;
    modelData->nDS[3]++;
    modelData->nDS[3]++;
    modelData->nDS[3]++;
    modelData->nDS[4]++;
    modelData->nDS[4]++;
    modelData->nDS[4]++;
    modelData->nDS[4]++;
    modelData->nDS[4]++;
    modelData->nDS[4]++;
    modelData->nDS[5]++;
    modelData->nDS[5]++;
    modelData->nDS[5]++;
    modelData->nDS[5]++;
    modelData->nDS[5]++;
    modelData->nDS[5]++;
    modelData->nDS[6]++;
    modelData->nDS[6]++;
    modelData->nDS[6]++;
    modelData->nDS[6]++;
    modelData->nDS[6]++;
    modelData->nDS[7]++;
    modelData->nDS[7]++;
    modelData->nDS[7]++;
    modelData->nDS[8]++;
    modelData->nDS[8]++;
    modelData->nDS[9]++;
    modelData->nDS[9]++;
    modelData->nDS[10]++;
    modelData->nDS[10]++;
    modelData->nDS[10]++;
    modelData->nDS[10]++;
    modelData->nDS[10]++;
    modelData->nDS[10]++;
    modelData->nDS[10]++;
    modelData->nDS[11]++;
    modelData->nDS[11]++;
    modelData->nDS[11]++;
    modelData->nDS[11]++;
    modelData->nDS[11]++;
    modelData->nDS[12]++;
    modelData->nDS[12]++;
    modelData->nDS[12]++;
    modelData->nDS[13]++;
    modelData->nDS[13]++;
    modelData->nDS[13]++;
    modelData->nDS[14]++;
    modelData->nDS[14]++;
    modelData->nDS[14]++;
    modelData->nDS[15]++;
    modelData->nDS[15]++;
    modelData->nDS[15]++;
    modelData->nDS[16]++;
    modelData->nDS[16]++;
    modelData->nDS[16]++;
    modelData->nDS[17]++;
    modelData->nDS[17]++;
    modelData->nDS[17]++;
    modelData->nDS[17]++;
    modelData->nDS[17]++;
    modelData->nDS[17]++;
    modelData->nDS[17]++;
    modelData->nDS[18]++;
    modelData->nDS[18]++;
    modelData->nDS[18]++;
    modelData->nDS[18]++;
    modelData->nDS[19]++;
    modelData->nDS[19]++;
    modelData->nDS[19]++;
    modelData->nDS[20]++;
    modelData->nDS[20]++;
    modelData->nDS[20]++;
    modelData->nDS[21]++;
    modelData->nDS[21]++;
    modelData->nDS[21]++;
    modelData->nDS[22]++;
    modelData->nDS[22]++;
    modelData->nDS[22]++;
    modelData->nDS[23]++;
    modelData->nDS[23]++;
    modelData->nDS[23]++;
    modelData->nDS[23]++;
    modelData->nDS[24]++;
    modelData->nDS[24]++;
    modelData->nDS[24]++;
    modelData->nDS[24]++;
    modelData->nDS[25]++;
    modelData->nDS[25]++;
    modelData->nDS[25]++;
    modelData->nDS[26]++;
    modelData->nDS[26]++;
    modelData->nDS[27]++;
    modelData->nDS[27]++;
    modelData->nDS[27]++;
    modelData->nSD[28]++;
    modelData->nSD[28]++;
    modelData->nSD[29]++;
    modelData->nSD[30]++;
    modelData->nSD[28]++;
    modelData->nSD[29]++;
    modelData->nSD[30]++;
    modelData->nSD[0]++;
    modelData->nSD[1]++;
    modelData->nSD[0]++;
    modelData->nSD[1]++;
    modelData->nSD[2]++;
    modelData->nSD[3]++;
    modelData->nSD[7]++;
    modelData->nSD[8]++;
    modelData->nSD[10]++;
    modelData->nSD[1]++;
    modelData->nSD[2]++;
    modelData->nSD[3]++;
    modelData->nSD[4]++;
    modelData->nSD[5]++;
    modelData->nSD[1]++;
    modelData->nSD[3]++;
    modelData->nSD[4]++;
    modelData->nSD[5]++;
    modelData->nSD[22]++;
    modelData->nSD[27]++;
    modelData->nSD[1]++;
    modelData->nSD[3]++;
    modelData->nSD[4]++;
    modelData->nSD[5]++;
    modelData->nSD[6]++;
    modelData->nSD[11]++;
    modelData->nSD[5]++;
    modelData->nSD[6]++;
    modelData->nSD[10]++;
    modelData->nSD[11]++;
    modelData->nSD[14]++;
    modelData->nSD[2]++;
    modelData->nSD[7]++;
    modelData->nSD[10]++;
    modelData->nSD[7]++;
    modelData->nSD[8]++;
    modelData->nSD[8]++;
    modelData->nSD[9]++;
    modelData->nSD[2]++;
    modelData->nSD[6]++;
    modelData->nSD[7]++;
    modelData->nSD[10]++;
    modelData->nSD[12]++;
    modelData->nSD[13]++;
    modelData->nSD[14]++;
    modelData->nSD[5]++;
    modelData->nSD[6]++;
    modelData->nSD[10]++;
    modelData->nSD[11]++;
    modelData->nSD[13]++;
    modelData->nSD[10]++;
    modelData->nSD[12]++;
    modelData->nSD[13]++;
    modelData->nSD[10]++;
    modelData->nSD[12]++;
    modelData->nSD[13]++;
    modelData->nSD[6]++;
    modelData->nSD[10]++;
    modelData->nSD[14]++;
    modelData->nSD[15]++;
    modelData->nSD[16]++;
    modelData->nSD[22]++;
    modelData->nSD[15]++;
    modelData->nSD[16]++;
    modelData->nSD[22]++;
    modelData->nSD[16]++;
    modelData->nSD[17]++;
    modelData->nSD[18]++;
    modelData->nSD[22]++;
    modelData->nSD[23]++;
    modelData->nSD[24]++;
    modelData->nSD[26]++;
    modelData->nSD[16]++;
    modelData->nSD[17]++;
    modelData->nSD[18]++;
    modelData->nSD[26]++;
    modelData->nSD[18]++;
    modelData->nSD[19]++;
    modelData->nSD[20]++;
    modelData->nSD[18]++;
    modelData->nSD[19]++;
    modelData->nSD[20]++;
    modelData->nSD[20]++;
    modelData->nSD[21]++;
    modelData->nSD[22]++;
    modelData->nSD[20]++;
    modelData->nSD[21]++;
    modelData->nSD[22]++;
    modelData->nSD[17]++;
    modelData->nSD[22]++;
    modelData->nSD[23]++;
    modelData->nSD[24]++;
    modelData->nSD[17]++;
    modelData->nSD[23]++;
    modelData->nSD[24]++;
    modelData->nSD[25]++;
    modelData->nSD[22]++;
    modelData->nSD[23]++;
    modelData->nSD[25]++;
    modelData->nSD[13]++;
    modelData->nSD[26]++;
    modelData->nSD[4]++;
    modelData->nSD[22]++;
    modelData->nSD[27]++;
    modelData->nZS[2] = 1;
    modelData->nSZ[13]++;
    modelData->nHZ[0] += 1;
    modelData->nHZ[1] += 1;
    modelData->nHZ[3] += 4;
    modelData->nHZ[4] += 4;
    modelData->nHZ[5] += 4;
    modelData->nHZ[6] += 4;
    modelData->nHZ[7] += 4;
    modelData->nHZ[8] += 4;
    modelData->nHZ[9] += 4;
    modelData->nHD[0] = 1;
    modelData->nHD[1] = 1;
    modelData->nHD[2] = 1;
    modelData->nHD[3] = 4;
    modelData->nHD[4] = 4;
    modelData->nHD[5] = 4;
    modelData->nHD[6] = 4;
    modelData->nHD[7] = 4;
    modelData->nHD[8] = 4;
    modelData->nHD[9] = 4;
    modelData->event[0].nLHSDsc = 2;
    modelData->event[1].nLHSDsc = 2;
    modelData->event[2].nLHSDsc = 1;
    modelData->event[3].nLHSDsc = 1;
    modelData->event[4].nLHSDsc = 1;
    modelData->event[5].nLHSDsc = 1;
    modelData->event[6].nLHSDsc = 1;
    modelData->event[7].nLHSDsc = 1;
    modelData->event[8].nLHSDsc = 1;
    modelData->event[9].nLHSDsc = 1;
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    cleanVector (states, 0, 31);

    modelData->DS[28][states[28]++] = 28;
    modelData->DS[29][states[29]++] = 28;
    modelData->DS[29][states[29]++] = 29;
    modelData->DS[29][states[29]++] = 30;
    modelData->DS[30][states[30]++] = 28;
    modelData->DS[30][states[30]++] = 29;
    modelData->DS[30][states[30]++] = 30;
    modelData->DS[0][states[0]++] = 0;
    modelData->DS[0][states[0]++] = 1;
    modelData->DS[1][states[1]++] = 0;
    modelData->DS[1][states[1]++] = 1;
    modelData->DS[2][states[2]++] = 2;
    modelData->DS[2][states[2]++] = 3;
    modelData->DS[2][states[2]++] = 7;
    modelData->DS[2][states[2]++] = 8;
    modelData->DS[2][states[2]++] = 10;
    modelData->DS[3][states[3]++] = 1;
    modelData->DS[3][states[3]++] = 2;
    modelData->DS[3][states[3]++] = 3;
    modelData->DS[3][states[3]++] = 4;
    modelData->DS[3][states[3]++] = 5;
    modelData->DS[4][states[4]++] = 1;
    modelData->DS[4][states[4]++] = 3;
    modelData->DS[4][states[4]++] = 4;
    modelData->DS[4][states[4]++] = 5;
    modelData->DS[4][states[4]++] = 22;
    modelData->DS[4][states[4]++] = 27;
    modelData->DS[5][states[5]++] = 1;
    modelData->DS[5][states[5]++] = 3;
    modelData->DS[5][states[5]++] = 4;
    modelData->DS[5][states[5]++] = 5;
    modelData->DS[5][states[5]++] = 6;
    modelData->DS[5][states[5]++] = 11;
    modelData->DS[6][states[6]++] = 5;
    modelData->DS[6][states[6]++] = 6;
    modelData->DS[6][states[6]++] = 10;
    modelData->DS[6][states[6]++] = 11;
    modelData->DS[6][states[6]++] = 14;
    modelData->DS[7][states[7]++] = 2;
    modelData->DS[7][states[7]++] = 7;
    modelData->DS[7][states[7]++] = 10;
    modelData->DS[8][states[8]++] = 7;
    modelData->DS[8][states[8]++] = 8;
    modelData->DS[9][states[9]++] = 8;
    modelData->DS[9][states[9]++] = 9;
    modelData->DS[10][states[10]++] = 2;
    modelData->DS[10][states[10]++] = 6;
    modelData->DS[10][states[10]++] = 7;
    modelData->DS[10][states[10]++] = 10;
    modelData->DS[10][states[10]++] = 12;
    modelData->DS[10][states[10]++] = 13;
    modelData->DS[10][states[10]++] = 14;
    modelData->DS[11][states[11]++] = 5;
    modelData->DS[11][states[11]++] = 6;
    modelData->DS[11][states[11]++] = 10;
    modelData->DS[11][states[11]++] = 11;
    modelData->DS[11][states[11]++] = 13;
    modelData->DS[12][states[12]++] = 10;
    modelData->DS[12][states[12]++] = 12;
    modelData->DS[12][states[12]++] = 13;
    modelData->DS[13][states[13]++] = 10;
    modelData->DS[13][states[13]++] = 12;
    modelData->DS[13][states[13]++] = 13;
    modelData->DS[14][states[14]++] = 6;
    modelData->DS[14][states[14]++] = 10;
    modelData->DS[14][states[14]++] = 14;
    modelData->DS[15][states[15]++] = 15;
    modelData->DS[15][states[15]++] = 16;
    modelData->DS[15][states[15]++] = 22;
    modelData->DS[16][states[16]++] = 15;
    modelData->DS[16][states[16]++] = 16;
    modelData->DS[16][states[16]++] = 22;
    modelData->DS[17][states[17]++] = 16;
    modelData->DS[17][states[17]++] = 17;
    modelData->DS[17][states[17]++] = 18;
    modelData->DS[17][states[17]++] = 22;
    modelData->DS[17][states[17]++] = 23;
    modelData->DS[17][states[17]++] = 24;
    modelData->DS[17][states[17]++] = 26;
    modelData->DS[18][states[18]++] = 16;
    modelData->DS[18][states[18]++] = 17;
    modelData->DS[18][states[18]++] = 18;
    modelData->DS[18][states[18]++] = 26;
    modelData->DS[19][states[19]++] = 18;
    modelData->DS[19][states[19]++] = 19;
    modelData->DS[19][states[19]++] = 20;
    modelData->DS[20][states[20]++] = 18;
    modelData->DS[20][states[20]++] = 19;
    modelData->DS[20][states[20]++] = 20;
    modelData->DS[21][states[21]++] = 20;
    modelData->DS[21][states[21]++] = 21;
    modelData->DS[21][states[21]++] = 22;
    modelData->DS[22][states[22]++] = 20;
    modelData->DS[22][states[22]++] = 21;
    modelData->DS[22][states[22]++] = 22;
    modelData->DS[23][states[23]++] = 17;
    modelData->DS[23][states[23]++] = 22;
    modelData->DS[23][states[23]++] = 23;
    modelData->DS[23][states[23]++] = 24;
    modelData->DS[24][states[24]++] = 17;
    modelData->DS[24][states[24]++] = 23;
    modelData->DS[24][states[24]++] = 24;
    modelData->DS[24][states[24]++] = 25;
    modelData->DS[25][states[25]++] = 22;
    modelData->DS[25][states[25]++] = 23;
    modelData->DS[25][states[25]++] = 25;
    modelData->DS[26][states[26]++] = 13;
    modelData->DS[26][states[26]++] = 26;
    modelData->DS[27][states[27]++] = 4;
    modelData->DS[27][states[27]++] = 22;
    modelData->DS[27][states[27]++] = 27;
    cleanVector (states, 0, 31);

    modelData->SD[28][states[28]++] = 28;
    modelData->SD[28][states[28]++] = 29;
    modelData->SD[29][states[29]++] = 29;
    modelData->SD[30][states[30]++] = 29;
    modelData->SD[28][states[28]++] = 30;
    modelData->SD[29][states[29]++] = 30;
    modelData->SD[30][states[30]++] = 30;
    modelData->SD[0][states[0]++] = 0;
    modelData->SD[1][states[1]++] = 0;
    modelData->SD[0][states[0]++] = 1;
    modelData->SD[1][states[1]++] = 1;
    modelData->SD[2][states[2]++] = 2;
    modelData->SD[3][states[3]++] = 2;
    modelData->SD[7][states[7]++] = 2;
    modelData->SD[8][states[8]++] = 2;
    modelData->SD[10][states[10]++] = 2;
    modelData->SD[1][states[1]++] = 3;
    modelData->SD[2][states[2]++] = 3;
    modelData->SD[3][states[3]++] = 3;
    modelData->SD[4][states[4]++] = 3;
    modelData->SD[5][states[5]++] = 3;
    modelData->SD[1][states[1]++] = 4;
    modelData->SD[3][states[3]++] = 4;
    modelData->SD[4][states[4]++] = 4;
    modelData->SD[5][states[5]++] = 4;
    modelData->SD[22][states[22]++] = 4;
    modelData->SD[27][states[27]++] = 4;
    modelData->SD[1][states[1]++] = 5;
    modelData->SD[3][states[3]++] = 5;
    modelData->SD[4][states[4]++] = 5;
    modelData->SD[5][states[5]++] = 5;
    modelData->SD[6][states[6]++] = 5;
    modelData->SD[11][states[11]++] = 5;
    modelData->SD[5][states[5]++] = 6;
    modelData->SD[6][states[6]++] = 6;
    modelData->SD[10][states[10]++] = 6;
    modelData->SD[11][states[11]++] = 6;
    modelData->SD[14][states[14]++] = 6;
    modelData->SD[2][states[2]++] = 7;
    modelData->SD[7][states[7]++] = 7;
    modelData->SD[10][states[10]++] = 7;
    modelData->SD[7][states[7]++] = 8;
    modelData->SD[8][states[8]++] = 8;
    modelData->SD[8][states[8]++] = 9;
    modelData->SD[9][states[9]++] = 9;
    modelData->SD[2][states[2]++] = 10;
    modelData->SD[6][states[6]++] = 10;
    modelData->SD[7][states[7]++] = 10;
    modelData->SD[10][states[10]++] = 10;
    modelData->SD[12][states[12]++] = 10;
    modelData->SD[13][states[13]++] = 10;
    modelData->SD[14][states[14]++] = 10;
    modelData->SD[5][states[5]++] = 11;
    modelData->SD[6][states[6]++] = 11;
    modelData->SD[10][states[10]++] = 11;
    modelData->SD[11][states[11]++] = 11;
    modelData->SD[13][states[13]++] = 11;
    modelData->SD[10][states[10]++] = 12;
    modelData->SD[12][states[12]++] = 12;
    modelData->SD[13][states[13]++] = 12;
    modelData->SD[10][states[10]++] = 13;
    modelData->SD[12][states[12]++] = 13;
    modelData->SD[13][states[13]++] = 13;
    modelData->SD[6][states[6]++] = 14;
    modelData->SD[10][states[10]++] = 14;
    modelData->SD[14][states[14]++] = 14;
    modelData->SD[15][states[15]++] = 15;
    modelData->SD[16][states[16]++] = 15;
    modelData->SD[22][states[22]++] = 15;
    modelData->SD[15][states[15]++] = 16;
    modelData->SD[16][states[16]++] = 16;
    modelData->SD[22][states[22]++] = 16;
    modelData->SD[16][states[16]++] = 17;
    modelData->SD[17][states[17]++] = 17;
    modelData->SD[18][states[18]++] = 17;
    modelData->SD[22][states[22]++] = 17;
    modelData->SD[23][states[23]++] = 17;
    modelData->SD[24][states[24]++] = 17;
    modelData->SD[26][states[26]++] = 17;
    modelData->SD[16][states[16]++] = 18;
    modelData->SD[17][states[17]++] = 18;
    modelData->SD[18][states[18]++] = 18;
    modelData->SD[26][states[26]++] = 18;
    modelData->SD[18][states[18]++] = 19;
    modelData->SD[19][states[19]++] = 19;
    modelData->SD[20][states[20]++] = 19;
    modelData->SD[18][states[18]++] = 20;
    modelData->SD[19][states[19]++] = 20;
    modelData->SD[20][states[20]++] = 20;
    modelData->SD[20][states[20]++] = 21;
    modelData->SD[21][states[21]++] = 21;
    modelData->SD[22][states[22]++] = 21;
    modelData->SD[20][states[20]++] = 22;
    modelData->SD[21][states[21]++] = 22;
    modelData->SD[22][states[22]++] = 22;
    modelData->SD[17][states[17]++] = 23;
    modelData->SD[22][states[22]++] = 23;
    modelData->SD[23][states[23]++] = 23;
    modelData->SD[24][states[24]++] = 23;
    modelData->SD[17][states[17]++] = 24;
    modelData->SD[23][states[23]++] = 24;
    modelData->SD[24][states[24]++] = 24;
    modelData->SD[25][states[25]++] = 24;
    modelData->SD[22][states[22]++] = 25;
    modelData->SD[23][states[23]++] = 25;
    modelData->SD[25][states[25]++] = 25;
    modelData->SD[13][states[13]++] = 26;
    modelData->SD[26][states[26]++] = 26;
    modelData->SD[4][states[4]++] = 27;
    modelData->SD[22][states[22]++] = 27;
    modelData->SD[27][states[27]++] = 27;
    cleanVector (events, 0, 10);

    modelData->ZS[2][events[2]++] = 13;
    cleanVector (states, 0, 31);

    modelData->SZ[13][states[13]++] = 2;
    cleanVector (events, 0, 10);

    modelData->HZ[0][events[0]++] = 0;
    modelData->HZ[1][events[1]++] = 1;
    modelData->HZ[3][events[3]++] = 3;
    modelData->HZ[3][events[3]++] = 4;
    modelData->HZ[3][events[3]++] = 7;
    modelData->HZ[3][events[3]++] = 8;
    modelData->HZ[4][events[4]++] = 3;
    modelData->HZ[4][events[4]++] = 4;
    modelData->HZ[4][events[4]++] = 7;
    modelData->HZ[4][events[4]++] = 8;
    modelData->HZ[5][events[5]++] = 3;
    modelData->HZ[5][events[5]++] = 4;
    modelData->HZ[5][events[5]++] = 7;
    modelData->HZ[5][events[5]++] = 8;
    modelData->HZ[6][events[6]++] = 3;
    modelData->HZ[6][events[6]++] = 4;
    modelData->HZ[6][events[6]++] = 7;
    modelData->HZ[6][events[6]++] = 8;
    modelData->HZ[7][events[7]++] = 3;
    modelData->HZ[7][events[7]++] = 4;
    modelData->HZ[7][events[7]++] = 7;
    modelData->HZ[7][events[7]++] = 8;
    modelData->HZ[8][events[8]++] = 3;
    modelData->HZ[8][events[8]++] = 4;
    modelData->HZ[8][events[8]++] = 7;
    modelData->HZ[8][events[8]++] = 8;
    modelData->HZ[9][events[9]++] = 3;
    modelData->HZ[9][events[9]++] = 4;
    modelData->HZ[9][events[9]++] = 7;
    modelData->HZ[9][events[9]++] = 8;
    cleanVector (events, 0, 10);

    modelData->HD[3][events[3]++] = 0;
    modelData->HD[4][events[4]++] = 0;
    modelData->HD[5][events[5]++] = 0;
    modelData->HD[6][events[6]++] = 0;
    modelData->HD[7][events[7]++] = 0;
    modelData->HD[8][events[8]++] = 0;
    modelData->HD[9][events[9]++] = 0;
    modelData->HD[3][events[3]++] = 1;
    modelData->HD[4][events[4]++] = 1;
    modelData->HD[5][events[5]++] = 1;
    modelData->HD[6][events[6]++] = 1;
    modelData->HD[7][events[7]++] = 1;
    modelData->HD[8][events[8]++] = 1;
    modelData->HD[9][events[9]++] = 1;
    modelData->HD[3][events[3]++] = 15;
    modelData->HD[4][events[4]++] = 15;
    modelData->HD[5][events[5]++] = 15;
    modelData->HD[6][events[6]++] = 15;
    modelData->HD[7][events[7]++] = 15;
    modelData->HD[8][events[8]++] = 15;
    modelData->HD[9][events[9]++] = 15;
    modelData->HD[3][events[3]++] = 16;
    modelData->HD[4][events[4]++] = 16;
    modelData->HD[5][events[5]++] = 16;
    modelData->HD[6][events[6]++] = 16;
    modelData->HD[7][events[7]++] = 16;
    modelData->HD[8][events[8]++] = 16;
    modelData->HD[9][events[9]++] = 16;
    modelData->HD[0][events[0]++] = 28;
    modelData->HD[2][events[2]++] = 29;
    modelData->HD[1][events[1]++] = 30;
    cleanVector (events, 0, 10);

    modelData->event[0].LHSDsc[events[0]++] = 2;
    modelData->event[0].LHSDsc[events[0]++] = 5;
    modelData->event[1].LHSDsc[events[1]++] = 4;
    modelData->event[1].LHSDsc[events[1]++] = 6;
    modelData->event[2].LHSDsc[events[2]++] = 0;
    modelData->event[3].LHSDsc[events[3]++] = 3;
    modelData->event[4].LHSDsc[events[4]++] = 3;
    modelData->event[5].LHSDsc[events[5]++] = 3;
    modelData->event[6].LHSDsc[events[6]++] = 3;
    modelData->event[7].LHSDsc[events[7]++] = 3;
    modelData->event[8].LHSDsc[events[8]++] = 3;
    modelData->event[9].LHSDsc[events[9]++] = 3;
    cleanVector (events, 0, 10);

    modelData->event[0].direction = 1;
    modelData->event[0].relation = 2;
    modelData->event[1].direction = 1;
    modelData->event[1].relation = 2;
    modelData->event[2].direction = 0;
    modelData->event[2].relation = 3;
    modelData->event[3].direction = 1;
    modelData->event[3].relation = 3;
    modelData->event[4].direction = -1;
    modelData->event[4].relation = 0;
    modelData->event[5].direction = 1;
    modelData->event[5].relation = 3;
    modelData->event[6].direction = -1;
    modelData->event[6].relation = 1;
    modelData->event[7].direction = 1;
    modelData->event[7].relation = 3;
    modelData->event[8].direction = -1;
    modelData->event[8].relation = 0;
    modelData->event[9].direction = 1;
    modelData->event[9].relation = 2;
    simulator->time = QSS_Time (31, 10, 0, 0, ST_Binary, NULL);

    simulator->output = SD_Output ("osteoadipo", 34, 7, 31, NULL, 0, 0, CI_Step, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[3] = 1;
    modelOutput->nSO[0]++;
    modelOutput->nOS[4] = 1;
    modelOutput->nSO[1]++;
    modelOutput->nOS[5] = 1;
    modelOutput->nSO[2]++;
    modelOutput->nOS[6] = 1;
    modelOutput->nSO[3]++;
    modelOutput->nOS[7] = 1;
    modelOutput->nSO[4]++;
    modelOutput->nOS[8] = 1;
    modelOutput->nSO[5]++;
    modelOutput->nOS[9] = 1;
    modelOutput->nSO[6]++;
    modelOutput->nOS[10] = 1;
    modelOutput->nSO[7]++;
    modelOutput->nOS[11] = 1;
    modelOutput->nSO[8]++;
    modelOutput->nOS[12] = 1;
    modelOutput->nSO[9]++;
    modelOutput->nOS[13] = 1;
    modelOutput->nSO[10]++;
    modelOutput->nOS[14] = 1;
    modelOutput->nSO[11]++;
    modelOutput->nOS[15] = 1;
    modelOutput->nSO[12]++;
    modelOutput->nOS[16] = 1;
    modelOutput->nSO[13]++;
    modelOutput->nOS[17] = 1;
    modelOutput->nSO[14]++;
    modelOutput->nOS[18] = 1;
    modelOutput->nSO[15]++;
    modelOutput->nOS[19] = 1;
    modelOutput->nSO[16]++;
    modelOutput->nOS[20] = 1;
    modelOutput->nSO[17]++;
    modelOutput->nOS[21] = 1;
    modelOutput->nSO[18]++;
    modelOutput->nOS[22] = 1;
    modelOutput->nSO[19]++;
    modelOutput->nOS[23] = 1;
    modelOutput->nSO[20]++;
    modelOutput->nOS[24] = 1;
    modelOutput->nSO[21]++;
    modelOutput->nOS[25] = 1;
    modelOutput->nSO[22]++;
    modelOutput->nOS[26] = 1;
    modelOutput->nSO[23]++;
    modelOutput->nOS[27] = 1;
    modelOutput->nSO[24]++;
    modelOutput->nOS[28] = 1;
    modelOutput->nSO[25]++;
    modelOutput->nOS[29] = 1;
    modelOutput->nSO[26]++;
    modelOutput->nOS[30] = 1;
    modelOutput->nSO[27]++;
    modelOutput->nOS[31] = 1;
    modelOutput->nSO[28]++;
    modelOutput->nOS[32] = 1;
    modelOutput->nSO[29]++;
    modelOutput->nOS[33] = 1;
    modelOutput->nSO[30]++;
    modelOutput->nOD[0] = 1;
    modelOutput->nDO[2]++;
    modelOutput->nOD[1] = 1;
    modelOutput->nDO[6]++;
    modelOutput->nOD[2] = 1;
    modelOutput->nDO[3]++;
    SD_allocOutputMatrix (modelOutput, 31, 7);
    cleanVector (states, 0, 31);

    cleanVector (discretes, 0, 7);

    cleanVector (outputs, 0, 34);

    sprintf (modelOutput->variable[0].name, "S__zd");
    sprintf (modelOutput->variable[1].name, "S__za");
    sprintf (modelOutput->variable[2].name, "W_W");
    sprintf (modelOutput->variable[3].name, "W_X1");
    sprintf (modelOutput->variable[4].name, "W_X2");
    sprintf (modelOutput->variable[5].name, "W_X3");
    sprintf (modelOutput->variable[6].name, "W_X4");
    sprintf (modelOutput->variable[7].name, "W_X5");
    sprintf (modelOutput->variable[8].name, "W_X6");
    sprintf (modelOutput->variable[9].name, "W_X7");
    sprintf (modelOutput->variable[10].name, "W_X8");
    sprintf (modelOutput->variable[11].name, "W_X9");
    sprintf (modelOutput->variable[12].name, "W_X10");
    sprintf (modelOutput->variable[13].name, "W_bCatenin");
    sprintf (modelOutput->variable[14].name, "W_X12");
    sprintf (modelOutput->variable[15].name, "W_X13");
    sprintf (modelOutput->variable[16].name, "W_bCatenin_TCF");
    sprintf (modelOutput->variable[17].name, "W_X15");
    sprintf (modelOutput->variable[18].name, "W_X16");
    sprintf (modelOutput->variable[19].name, "W_X17");
    sprintf (modelOutput->variable[20].name, "W_X18");
    sprintf (modelOutput->variable[21].name, "W_X19");
    sprintf (modelOutput->variable[22].name, "W_X20");
    sprintf (modelOutput->variable[23].name, "W_X21");
    sprintf (modelOutput->variable[24].name, "W_X22");
    sprintf (modelOutput->variable[25].name, "W_X23");
    sprintf (modelOutput->variable[26].name, "W_X24");
    sprintf (modelOutput->variable[27].name, "W_X25");
    sprintf (modelOutput->variable[28].name, "W_X26");
    sprintf (modelOutput->variable[29].name, "W_X27");
    sprintf (modelOutput->variable[30].name, "W_X28");
    sprintf (modelOutput->variable[31].name, "D_xp");
    sprintf (modelOutput->variable[32].name, "D_xo");
    sprintf (modelOutput->variable[33].name, "D_xa");
    cleanVector (outputs, 0, 34);

    modelOutput->SO[0][states[0]++] = 3;
    modelOutput->OS[3][outputs[3]++] = 0;
    modelOutput->SO[1][states[1]++] = 4;
    modelOutput->OS[4][outputs[4]++] = 1;
    modelOutput->SO[2][states[2]++] = 5;
    modelOutput->OS[5][outputs[5]++] = 2;
    modelOutput->SO[3][states[3]++] = 6;
    modelOutput->OS[6][outputs[6]++] = 3;
    modelOutput->SO[4][states[4]++] = 7;
    modelOutput->OS[7][outputs[7]++] = 4;
    modelOutput->SO[5][states[5]++] = 8;
    modelOutput->OS[8][outputs[8]++] = 5;
    modelOutput->SO[6][states[6]++] = 9;
    modelOutput->OS[9][outputs[9]++] = 6;
    modelOutput->SO[7][states[7]++] = 10;
    modelOutput->OS[10][outputs[10]++] = 7;
    modelOutput->SO[8][states[8]++] = 11;
    modelOutput->OS[11][outputs[11]++] = 8;
    modelOutput->SO[9][states[9]++] = 12;
    modelOutput->OS[12][outputs[12]++] = 9;
    modelOutput->SO[10][states[10]++] = 13;
    modelOutput->OS[13][outputs[13]++] = 10;
    modelOutput->SO[11][states[11]++] = 14;
    modelOutput->OS[14][outputs[14]++] = 11;
    modelOutput->SO[12][states[12]++] = 15;
    modelOutput->OS[15][outputs[15]++] = 12;
    modelOutput->SO[13][states[13]++] = 16;
    modelOutput->OS[16][outputs[16]++] = 13;
    modelOutput->SO[14][states[14]++] = 17;
    modelOutput->OS[17][outputs[17]++] = 14;
    modelOutput->SO[15][states[15]++] = 18;
    modelOutput->OS[18][outputs[18]++] = 15;
    modelOutput->SO[16][states[16]++] = 19;
    modelOutput->OS[19][outputs[19]++] = 16;
    modelOutput->SO[17][states[17]++] = 20;
    modelOutput->OS[20][outputs[20]++] = 17;
    modelOutput->SO[18][states[18]++] = 21;
    modelOutput->OS[21][outputs[21]++] = 18;
    modelOutput->SO[19][states[19]++] = 22;
    modelOutput->OS[22][outputs[22]++] = 19;
    modelOutput->SO[20][states[20]++] = 23;
    modelOutput->OS[23][outputs[23]++] = 20;
    modelOutput->SO[21][states[21]++] = 24;
    modelOutput->OS[24][outputs[24]++] = 21;
    modelOutput->SO[22][states[22]++] = 25;
    modelOutput->OS[25][outputs[25]++] = 22;
    modelOutput->SO[23][states[23]++] = 26;
    modelOutput->OS[26][outputs[26]++] = 23;
    modelOutput->SO[24][states[24]++] = 27;
    modelOutput->OS[27][outputs[27]++] = 24;
    modelOutput->SO[25][states[25]++] = 28;
    modelOutput->OS[28][outputs[28]++] = 25;
    modelOutput->SO[26][states[26]++] = 29;
    modelOutput->OS[29][outputs[29]++] = 26;
    modelOutput->SO[27][states[27]++] = 30;
    modelOutput->OS[30][outputs[30]++] = 27;
    modelOutput->SO[28][states[28]++] = 31;
    modelOutput->OS[31][outputs[31]++] = 28;
    modelOutput->SO[29][states[29]++] = 32;
    modelOutput->OS[32][outputs[32]++] = 29;
    modelOutput->SO[30][states[30]++] = 33;
    modelOutput->OS[33][outputs[33]++] = 30;
    cleanVector (outputs, 0, 34);

    modelOutput->DO[2][discretes[2]++] = 0;
    modelOutput->OD[0][outputs[0]++] = 2;
    modelOutput->DO[6][discretes[6]++] = 1;
    modelOutput->OD[1][outputs[1]++] = 6;
    modelOutput->DO[3][discretes[3]++] = 2;
    modelOutput->OD[2][outputs[2]++] = 3;
    simulator->model = QSS_Model (MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg);
    free (discretes);
    free (events);
    free (outputs);
    free (states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
