#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>

#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_IndexShift_1_p[2];
double __PAR_VecInt_2_p[5];
double __PAR_VecInt_2_x0 = 0;
double __PAR_VectorSum_3_p[10];
double __PAR_VectorSum_3_w[4];
double __PAR_vector_sat_4_p[3];
double __PAR_vector_sat_4_xl = 0;
double __PAR_Scalar2Vector_5_p[2];
double __PAR_Vec2Scalar_6_p[2];
int __reverse__PAR_Vec2Scalar_6_Index = 0;
int __PAR_Vec2Scalar_6_Index = 0;
double __PAR_vector_pow2_7_p[1];
double __PAR_vector_pow2_8_p[1];
double __PAR_vector_sat_9_p[3];
double __PAR_vector_sat_9_xl = 0;
double __PAR_VectorSum_10_p[10];
double __PAR_VectorSum_10_w[3];
double __PAR_VectorSum_11_p[10];
double __PAR_VectorSum_11_w[2];
double __PAR_Scalar2Vector_12_p[2];
double __PAR_ramp_sci_13_p[5];
double __PAR_ramp_sci_13_t0 = 0;
double __PAR_ramp_sci_13_tr = 0;
double __PAR_ramp_sci_13_v = 0;
double __PAR_ramp_sci_14_p[5];
double __PAR_ramp_sci_14_t0 = 0;
double __PAR_ramp_sci_14_tr = 0;
double __PAR_ramp_sci_14_v = 0;
double __PAR_Constant_15_p[1];
double __PAR_Constant_15_k = 0;
double __PAR_Constant_16_p[1];
double __PAR_Constant_16_k = 0;
double __PAR_Scalar2Vector_17_p[2];
double __PAR_Vec2Scalar_18_p[2];
int __reverse__PAR_Vec2Scalar_18_Index = 0;
int __PAR_Vec2Scalar_18_Index = 0;
double __PAR_WSum_20_p[9];
double __PAR_WSum_20_w[2];

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
    double tmp0[3];
    int j = 0;
    int j10;
    int j11 = 0;
    int j9 = 0;
    j = i;
    if (j >= 0 && j <= 99)
    {
        alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
        alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                + 5.0 / 2.0 * d[(402)] * t;
        alg[6] = 1.0;
        alg[9] = 5.0;
        j9 = j - 1;
        if (j >= 1 && j <= 99)
        {
            alg[(j - 1 + 4) * 3] = x[(j9) * 3];
        }
        alg[(j + 104) * 3] = x[(j) * 3];
        alg[(j + 304) * 3] = x[(j) * 3];
        alg[1212] = alg[6];
        alg[1215] = alg[9];
        alg[1518] = alg[0];
        alg[1521] = alg[3];
        alg[(j + 508) * 3] = alg[1212];
        alg[(j + 608) * 3] = alg[1215];
        alg[(j + 708) * 3] = alg[(j + 508) * 3];
        alg[(j + 808) * 3] = alg[(j + 608) * 3];
        for (j10 = 0; j10 < 3; j10++)
        {
            tmp0[j10] = 0;
        }
        for (j10 = 0; j10 <= 1; j10++)
        {
            tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
            tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
            tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
        }
        alg[2724] = tmp0[0];
        alg[2727] = alg[2724];
        alg[2730] = alg[2727];
        j11 = j;
        if (j11 >= 1 && j11 <= 99)
        {
            alg[(j + 910) * 3] = 0.0;
        }
        j11 = j;
        if (j11 >= 0 && j11 <= 0)
        {
            alg[(j + 1010) * 3] = 0.0;
        }
        j11 = j - 1;
        if (j11 >= 0 && j11 <= 98)
        {
            alg[(j + 1010) * 3] = alg[(j11 + 4) * 3];
        }
        alg[(j + 1110) * 3] = alg[(j + 1010) * 3];
        alg[(j + 1210) * 3] = alg[(j + 910) * 3];
        alg[(j + 1310) * 3] = alg[(j + 1210) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(j + 708) * 3] * __PAR_VectorSum_10_w[(1)]
                + alg[(j + 1110) * 3] * __PAR_VectorSum_10_w[(2)];
        alg[(j + 1410) * 3] = alg[(j + 1310) * 3];
        alg[(j + 1510) * 3] = alg[(j + 1310) * 3];
        alg[(j + 1610) * 3] = alg[(j + 1410) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(j + 304) * 3] * __PAR_VectorSum_11_w[(1)];
        alg[(j + 1710) * 3] = alg[(j + 1610) * 3];
        alg[(j + 1810) * 3] = d[(j)] * __PAR_vector_sat_4_xl + (1.0 - d[(j)]) * (d[(j + 100)] * 1000000 + (1.0 - d[(j + 100)]) * alg[(j + 1510) * 3]);
        alg[(j + 1910) * 3] = d[(j + 200)] * __PAR_vector_sat_9_xl
                + (1.0 - d[(j + 200)]) * (d[(j + 300)] * 1000000 + (1.0 - d[(j + 300)]) * alg[(j + 1710) * 3]);
        alg[(j + 2010) * 3] = alg[(j + 1910) * 3];
        alg[(j + 2110) * 3] = alg[(j + 1810) * 3];
        alg[(j + 2210) * 3] = alg[(j + 2010) * 3] * alg[(j + 2010) * 3];
        alg[(j + 2310) * 3] = alg[(j + 2110) * 3] * alg[(j + 2110) * 3];
        alg[(j + 2410) * 3] = alg[(j + 2210) * 3];
        alg[(j + 2510) * 3] = alg[(j + 2310) * 3];
        alg[(j + 2610) * 3] = alg[(j + 808) * 3] * __PAR_VectorSum_3_w[(0)] + alg[(j + 2510) * 3] * __PAR_VectorSum_3_w[(1)]
                + alg[(j + 2410) * 3] * __PAR_VectorSum_3_w[(2)] + alg[(j + 104) * 3] * __PAR_VectorSum_3_w[(3)];
        alg[(j + 2710) * 3] = alg[(j + 2610) * 3];
        dx[1] = alg[(j + 2710) * 3];
    }
}

void
MOD_dependencies (int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
    double tmp0[3];
    int j = 0;
    int j10;
    int j11 = 0;
    int j12 = 0;
    int j13 = 0;
    int j14 = 0;
    int j15 = 0;
    int j9 = 0;
    j = i + 1;
    if (j >= 0 && j <= 99)
    {
        alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
        alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                + 5.0 / 2.0 * d[(402)] * t;
        alg[6] = 1.0;
        alg[9] = 5.0;
        j12 = j - 1;
        if (j >= 1 && j <= 99)
        {
            alg[(j - 1 + 4) * 3] = x[(j12) * 3];
        }
        alg[(j + 104) * 3] = x[(j) * 3];
        alg[(j + 304) * 3] = x[(j) * 3];
        alg[1212] = alg[6];
        alg[1215] = alg[9];
        alg[1518] = alg[0];
        alg[1521] = alg[3];
        alg[(j + 508) * 3] = alg[1212];
        alg[(j + 608) * 3] = alg[1215];
        alg[(j + 708) * 3] = alg[(j + 508) * 3];
        alg[(j + 808) * 3] = alg[(j + 608) * 3];
        for (j10 = 0; j10 < 3; j10++)
        {
            tmp0[j10] = 0;
        }
        for (j10 = 0; j10 <= 1; j10++)
        {
            tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
            tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
            tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
        }
        alg[2724] = tmp0[0];
        alg[2727] = alg[2724];
        alg[2730] = alg[2727];
        j13 = j;
        if (j13 >= 1 && j13 <= 99)
        {
            alg[(j + 910) * 3] = 0.0;
        }
        j13 = j;
        if (j13 >= 0 && j13 <= 0)
        {
            alg[(j + 1010) * 3] = 0.0;
        }
        j13 = j - 1;
        if (j13 >= 0 && j13 <= 98)
        {
            alg[(j + 1010) * 3] = alg[(j13 + 4) * 3];
        }
        alg[(j + 1110) * 3] = alg[(j + 1010) * 3];
        alg[(j + 1210) * 3] = alg[(j + 910) * 3];
        alg[(j + 1310) * 3] = alg[(j + 1210) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(j + 708) * 3] * __PAR_VectorSum_10_w[(1)]
                + alg[(j + 1110) * 3] * __PAR_VectorSum_10_w[(2)];
        alg[(j + 1410) * 3] = alg[(j + 1310) * 3];
        alg[(j + 1510) * 3] = alg[(j + 1310) * 3];
        alg[(j + 1610) * 3] = alg[(j + 1410) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(j + 304) * 3] * __PAR_VectorSum_11_w[(1)];
        alg[(j + 1710) * 3] = alg[(j + 1610) * 3];
        alg[(j + 1810) * 3] = d[(j)] * __PAR_vector_sat_4_xl + (1.0 - d[(j)]) * (d[(j + 100)] * 1000000 + (1.0 - d[(j + 100)]) * alg[(j + 1510) * 3]);
        alg[(j + 1910) * 3] = d[(j + 200)] * __PAR_vector_sat_9_xl
                + (1.0 - d[(j + 200)]) * (d[(j + 300)] * 1000000 + (1.0 - d[(j + 300)]) * alg[(j + 1710) * 3]);
        alg[(j + 2010) * 3] = alg[(j + 1910) * 3];
        alg[(j + 2110) * 3] = alg[(j + 1810) * 3];
        alg[(j + 2210) * 3] = alg[(j + 2010) * 3] * alg[(j + 2010) * 3];
        alg[(j + 2310) * 3] = alg[(j + 2110) * 3] * alg[(j + 2110) * 3];
        alg[(j + 2410) * 3] = alg[(j + 2210) * 3];
        alg[(j + 2510) * 3] = alg[(j + 2310) * 3];
        alg[(j + 2610) * 3] = alg[(j + 808) * 3] * __PAR_VectorSum_3_w[(0)] + alg[(j + 2510) * 3] * __PAR_VectorSum_3_w[(1)]
                + alg[(j + 2410) * 3] * __PAR_VectorSum_3_w[(2)] + alg[(j + 104) * 3] * __PAR_VectorSum_3_w[(3)];
        alg[(j + 2710) * 3] = alg[(j + 2610) * 3];
        der[(j) * 3 + 1] = alg[(j + 2710) * 3];
    }
    j = i;
    if (j >= 0 && j <= 99)
    {
        alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
        alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                + 5.0 / 2.0 * d[(402)] * t;
        alg[6] = 1.0;
        alg[9] = 5.0;
        j14 = j - 1;
        if (j >= 1 && j <= 99)
        {
            alg[(j - 1 + 4) * 3] = x[(j14) * 3];
        }
        alg[(j + 104) * 3] = x[(j) * 3];
        alg[(j + 304) * 3] = x[(j) * 3];
        alg[1212] = alg[6];
        alg[1215] = alg[9];
        alg[1518] = alg[0];
        alg[1521] = alg[3];
        alg[(j + 508) * 3] = alg[1212];
        alg[(j + 608) * 3] = alg[1215];
        alg[(j + 708) * 3] = alg[(j + 508) * 3];
        alg[(j + 808) * 3] = alg[(j + 608) * 3];
        for (j10 = 0; j10 < 3; j10++)
        {
            tmp0[j10] = 0;
        }
        for (j10 = 0; j10 <= 1; j10++)
        {
            tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
            tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
            tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
        }
        alg[2724] = tmp0[0];
        alg[2727] = alg[2724];
        alg[2730] = alg[2727];
        j15 = j;
        if (j15 >= 1 && j15 <= 99)
        {
            alg[(j + 910) * 3] = 0.0;
        }
        j15 = j;
        if (j15 >= 0 && j15 <= 0)
        {
            alg[(j + 1010) * 3] = 0.0;
        }
        j15 = j - 1;
        if (j15 >= 0 && j15 <= 98)
        {
            alg[(j + 1010) * 3] = alg[(j15 + 4) * 3];
        }
        alg[(j + 1110) * 3] = alg[(j + 1010) * 3];
        alg[(j + 1210) * 3] = alg[(j + 910) * 3];
        alg[(j + 1310) * 3] = alg[(j + 1210) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(j + 708) * 3] * __PAR_VectorSum_10_w[(1)]
                + alg[(j + 1110) * 3] * __PAR_VectorSum_10_w[(2)];
        alg[(j + 1410) * 3] = alg[(j + 1310) * 3];
        alg[(j + 1510) * 3] = alg[(j + 1310) * 3];
        alg[(j + 1610) * 3] = alg[(j + 1410) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(j + 304) * 3] * __PAR_VectorSum_11_w[(1)];
        alg[(j + 1710) * 3] = alg[(j + 1610) * 3];
        alg[(j + 1810) * 3] = d[(j)] * __PAR_vector_sat_4_xl + (1.0 - d[(j)]) * (d[(j + 100)] * 1000000 + (1.0 - d[(j + 100)]) * alg[(j + 1510) * 3]);
        alg[(j + 1910) * 3] = d[(j + 200)] * __PAR_vector_sat_9_xl
                + (1.0 - d[(j + 200)]) * (d[(j + 300)] * 1000000 + (1.0 - d[(j + 300)]) * alg[(j + 1710) * 3]);
        alg[(j + 2010) * 3] = alg[(j + 1910) * 3];
        alg[(j + 2110) * 3] = alg[(j + 1810) * 3];
        alg[(j + 2210) * 3] = alg[(j + 2010) * 3] * alg[(j + 2010) * 3];
        alg[(j + 2310) * 3] = alg[(j + 2110) * 3] * alg[(j + 2110) * 3];
        alg[(j + 2410) * 3] = alg[(j + 2210) * 3];
        alg[(j + 2510) * 3] = alg[(j + 2310) * 3];
        alg[(j + 2610) * 3] = alg[(j + 808) * 3] * __PAR_VectorSum_3_w[(0)] + alg[(j + 2510) * 3] * __PAR_VectorSum_3_w[(1)]
                + alg[(j + 2410) * 3] * __PAR_VectorSum_3_w[(2)] + alg[(j + 104) * 3] * __PAR_VectorSum_3_w[(3)];
        alg[(j + 2710) * 3] = alg[(j + 2610) * 3];
        der[(j) * 3 + 1] = alg[(j + 2710) * 3];
    }
}

void
MOD_zeroCrossing (int i, double *x, double *d, double *alg, double t, double *zc)
{
    double tmp0[3];
    int j10;
    int j16 = 0;
    int j17 = 0;
    int j18 = 0;
    int j19 = 0;
    int j20 = 0;
    int j21 = 0;
    int j22 = 0;
    int j23 = 0;
    int j24 = 0;
    int j25 = 0;
    int j26 = 0;
    int j27 = 0;
    int j28 = 0;
    int j29 = 0;
    int j30 = 0;
    int j31 = 0;
    switch (i)
    {
        case 800:
            zc[0] = t - (5.0);
            return;
        case 801:
            zc[0] = t - (10.0);
            return;
        case 802:
            zc[0] = t - (15.0);
            return;
        case 803:
            zc[0] = t - (17.0);
            return;
        default:
            if (i >= 0 && i <= 99)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j16 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 + 4) * 3] = x[(j16) * 3];
                }
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i + 508) * 3] = alg[1212];
                alg[(i + 708) * 3] = alg[(i + 508) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j17 = i;
                if (j17 >= 1 && j17 <= 99)
                {
                    alg[(i + 910) * 3] = 0.0;
                }
                j17 = i;
                if (j17 >= 0 && j17 <= 0)
                {
                    alg[(i + 1010) * 3] = 0.0;
                }
                j17 = i - 1;
                if (j17 >= 0 && j17 <= 98)
                {
                    alg[(i + 1010) * 3] = alg[(j17 + 4) * 3];
                }
                alg[(i + 1110) * 3] = alg[(i + 1010) * 3];
                alg[(i + 1210) * 3] = alg[(i + 910) * 3];
                alg[(i + 1310) * 3] = alg[(i + 1210) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 708) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 1110) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 1510) * 3] = alg[(i + 1310) * 3];
                zc[0] = alg[(i + 1510) * 3] - (__PAR_vector_sat_4_xl);
            }
            if (i >= 100 && i <= 199)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j18 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 96) * 3] = x[(j18 - 100) * 3];
                }
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i + 408) * 3] = alg[1212];
                alg[(i + 608) * 3] = alg[(i + 408) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j19 = i;
                if (j19 >= 1 && j19 <= 99)
                {
                    alg[(i + 810) * 3] = 0.0;
                }
                j19 = i;
                if (j19 >= 0 && j19 <= 0)
                {
                    alg[(i + 910) * 3] = 0.0;
                }
                j19 = i - 1;
                if (j19 >= 0 && j19 <= 98)
                {
                    alg[(i + 910) * 3] = alg[(j19 - 96) * 3];
                }
                alg[(i + 1010) * 3] = alg[(i + 910) * 3];
                alg[(i + 1110) * 3] = alg[(i + 810) * 3];
                alg[(i + 1210) * 3] = alg[(i + 1110) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 608) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 1010) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 1410) * 3] = alg[(i + 1210) * 3];
                zc[0] = alg[(i + 1410) * 3] - (__PAR_vector_sat_4_xl);
            }
            if (i >= 200 && i <= 299)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j20 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 196) * 3] = x[(j20 - 200) * 3];
                }
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i + 308) * 3] = alg[1212];
                alg[(i + 508) * 3] = alg[(i + 308) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j21 = i;
                if (j21 >= 1 && j21 <= 99)
                {
                    alg[(i + 710) * 3] = 0.0;
                }
                j21 = i;
                if (j21 >= 0 && j21 <= 0)
                {
                    alg[(i + 810) * 3] = 0.0;
                }
                j21 = i - 1;
                if (j21 >= 0 && j21 <= 98)
                {
                    alg[(i + 810) * 3] = alg[(j21 - 196) * 3];
                }
                alg[(i + 910) * 3] = alg[(i + 810) * 3];
                alg[(i + 1010) * 3] = alg[(i + 710) * 3];
                alg[(i + 1110) * 3] = alg[(i + 1010) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 508) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 910) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 1310) * 3] = alg[(i + 1110) * 3];
                zc[0] = alg[(i + 1310) * 3] - (1000000);
            }
            if (i >= 300 && i <= 399)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j22 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 296) * 3] = x[(j22 - 300) * 3];
                }
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i + 208) * 3] = alg[1212];
                alg[(i + 408) * 3] = alg[(i + 208) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j23 = i;
                if (j23 >= 1 && j23 <= 99)
                {
                    alg[(i + 610) * 3] = 0.0;
                }
                j23 = i;
                if (j23 >= 0 && j23 <= 0)
                {
                    alg[(i + 710) * 3] = 0.0;
                }
                j23 = i - 1;
                if (j23 >= 0 && j23 <= 98)
                {
                    alg[(i + 710) * 3] = alg[(j23 - 296) * 3];
                }
                alg[(i + 810) * 3] = alg[(i + 710) * 3];
                alg[(i + 910) * 3] = alg[(i + 610) * 3];
                alg[(i + 1010) * 3] = alg[(i + 910) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 408) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 810) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 1210) * 3] = alg[(i + 1010) * 3];
                zc[0] = alg[(i + 1210) * 3] - (1000000);
            }
            if (i >= 400 && i <= 499)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j24 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 396) * 3] = x[(j24 - 400) * 3];
                }
                alg[(i - 96) * 3] = x[(i - 400) * 3];
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i + 108) * 3] = alg[1212];
                alg[(i + 308) * 3] = alg[(i + 108) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j25 = i;
                if (j25 >= 1 && j25 <= 99)
                {
                    alg[(i + 510) * 3] = 0.0;
                }
                j25 = i;
                if (j25 >= 0 && j25 <= 0)
                {
                    alg[(i + 610) * 3] = 0.0;
                }
                j25 = i - 1;
                if (j25 >= 0 && j25 <= 98)
                {
                    alg[(i + 610) * 3] = alg[(j25 - 396) * 3];
                }
                alg[(i + 710) * 3] = alg[(i + 610) * 3];
                alg[(i + 810) * 3] = alg[(i + 510) * 3];
                alg[(i + 910) * 3] = alg[(i + 810) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 308) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 710) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 1010) * 3] = alg[(i + 910) * 3];
                alg[(i + 1210) * 3] = alg[(i + 1010) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(i - 96) * 3] * __PAR_VectorSum_11_w[(1)];
                alg[(i + 1310) * 3] = alg[(i + 1210) * 3];
                zc[0] = alg[(i + 1310) * 3] - (__PAR_vector_sat_9_xl);
            }
            if (i >= 500 && i <= 599)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j26 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 496) * 3] = x[(j26 - 500) * 3];
                }
                alg[(i - 196) * 3] = x[(i - 500) * 3];
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i + 8) * 3] = alg[1212];
                alg[(i + 208) * 3] = alg[(i + 8) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j27 = i;
                if (j27 >= 1 && j27 <= 99)
                {
                    alg[(i + 410) * 3] = 0.0;
                }
                j27 = i;
                if (j27 >= 0 && j27 <= 0)
                {
                    alg[(i + 510) * 3] = 0.0;
                }
                j27 = i - 1;
                if (j27 >= 0 && j27 <= 98)
                {
                    alg[(i + 510) * 3] = alg[(j27 - 496) * 3];
                }
                alg[(i + 610) * 3] = alg[(i + 510) * 3];
                alg[(i + 710) * 3] = alg[(i + 410) * 3];
                alg[(i + 810) * 3] = alg[(i + 710) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 208) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 610) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 910) * 3] = alg[(i + 810) * 3];
                alg[(i + 1110) * 3] = alg[(i + 910) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(i - 196) * 3] * __PAR_VectorSum_11_w[(1)];
                alg[(i + 1210) * 3] = alg[(i + 1110) * 3];
                zc[0] = alg[(i + 1210) * 3] - (__PAR_vector_sat_9_xl);
            }
            if (i >= 600 && i <= 699)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j28 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 596) * 3] = x[(j28 - 600) * 3];
                }
                alg[(i - 296) * 3] = x[(i - 600) * 3];
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i - 92) * 3] = alg[1212];
                alg[(i + 108) * 3] = alg[(i - 92) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j29 = i;
                if (j29 >= 1 && j29 <= 99)
                {
                    alg[(i + 310) * 3] = 0.0;
                }
                j29 = i;
                if (j29 >= 0 && j29 <= 0)
                {
                    alg[(i + 410) * 3] = 0.0;
                }
                j29 = i - 1;
                if (j29 >= 0 && j29 <= 98)
                {
                    alg[(i + 410) * 3] = alg[(j29 - 596) * 3];
                }
                alg[(i + 510) * 3] = alg[(i + 410) * 3];
                alg[(i + 610) * 3] = alg[(i + 310) * 3];
                alg[(i + 710) * 3] = alg[(i + 610) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 108) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 510) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 810) * 3] = alg[(i + 710) * 3];
                alg[(i + 1010) * 3] = alg[(i + 810) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(i - 296) * 3] * __PAR_VectorSum_11_w[(1)];
                alg[(i + 1110) * 3] = alg[(i + 1010) * 3];
                zc[0] = alg[(i + 1110) * 3] - (1000000);
            }
            if (i >= 700 && i <= 799)
            {
                alg[0] = ((-d[(401)] * t * d[(400)])) + 5.0 * d[(401)] + 5.0 * d[(401)] * d[(400)] + t * d[(400)] - 5.0 * d[(400)];
                alg[3] = ((-75.0 / 2.0 * d[(402)])) + 5.0 * d[(403)] + 75.0 / 2.0 * d[(402)] * d[(403)] - 5.0 / 2.0 * d[(402)] * d[(403)] * t
                        + 5.0 / 2.0 * d[(402)] * t;
                alg[6] = 1.0;
                j30 = i - 1;
                if (i >= 1 && i <= 99)
                {
                    alg[(i - 1 - 696) * 3] = x[(j30 - 700) * 3];
                }
                alg[(i - 396) * 3] = x[(i - 700) * 3];
                alg[1212] = alg[6];
                alg[1518] = alg[0];
                alg[1521] = alg[3];
                alg[(i - 192) * 3] = alg[1212];
                alg[(i + 8) * 3] = alg[(i - 192) * 3];
                for (j10 = 0; j10 < 3; j10++)
                {
                    tmp0[j10] = 0;
                }
                for (j10 = 0; j10 <= 1; j10++)
                {
                    tmp0[0] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 0];
                    tmp0[1] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 1];
                    tmp0[2] += __PAR_WSum_20_w[j10] * alg[(j10 + 506) * 3 + 2];
                }
                alg[2724] = tmp0[0];
                alg[2727] = alg[2724];
                alg[2730] = alg[2727];
                j31 = i;
                if (j31 >= 1 && j31 <= 99)
                {
                    alg[(i + 210) * 3] = 0.0;
                }
                j31 = i;
                if (j31 >= 0 && j31 <= 0)
                {
                    alg[(i + 310) * 3] = 0.0;
                }
                j31 = i - 1;
                if (j31 >= 0 && j31 <= 98)
                {
                    alg[(i + 310) * 3] = alg[(j31 - 696) * 3];
                }
                alg[(i + 410) * 3] = alg[(i + 310) * 3];
                alg[(i + 510) * 3] = alg[(i + 210) * 3];
                alg[(i + 610) * 3] = alg[(i + 510) * 3] * __PAR_VectorSum_10_w[(0)] + alg[(i + 8) * 3] * __PAR_VectorSum_10_w[(1)]
                        + alg[(i + 410) * 3] * __PAR_VectorSum_10_w[(2)];
                alg[(i + 710) * 3] = alg[(i + 610) * 3];
                alg[(i + 910) * 3] = alg[(i + 710) * 3] * __PAR_VectorSum_11_w[(0)] + alg[(i - 396) * 3] * __PAR_VectorSum_11_w[(1)];
                alg[(i + 1010) * 3] = alg[(i + 910) * 3];
                zc[0] = alg[(i + 1010) * 3] - (1000000);
            }
    }
}

void
MOD_handlerPos (int i, double *x, double *d, double *alg, double t)
{
    switch (i)
    {
        case 800:
            d[(400)] = 1.0;
            return;
        case 801:
            d[(401)] = 1.0;
            return;
        case 802:
            d[(402)] = 1.0;
            return;
        case 803:
            d[(403)] = 1.0;
            return;
        default:
            if (i >= 100 && i <= 199)
            {
                d[(i - 100)] = 0.0;
            }
            if (i >= 200 && i <= 299)
            {
                d[(i - 100)] = 1.0;
            }
            if (i >= 500 && i <= 599)
            {
                d[(i - 300)] = 0.0;
            }
            if (i >= 600 && i <= 699)
            {
                d[(i - 300)] = 1.0;
            }
    }
}

void
MOD_handlerNeg (int i, double *x, double *d, double *alg, double t)
{
    if (i >= 0 && i <= 99)
    {
        d[(i)] = 1.0;
    }
    if (i >= 300 && i <= 399)
    {
        d[(i - 200)] = 0.0;
    }
    if (i >= 400 && i <= 499)
    {
        d[(i - 200)] = 1.0;
    }
    if (i >= 700 && i <= 799)
    {
        d[(i - 400)] = 0.0;
    }
}

void
MOD_output (int i, double *x, double *d, double *alg, double t, double *out)
{
    switch (i)
    {
        case 0:
            out[0] = x[0];
            return;
        case 1:
            out[0] = x[297];
            return;
    }
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
    int *discretes = (int*) malloc (404 * sizeof(int));
    int *events = (int*) malloc (804 * sizeof(int));
    int *outputs = (int*) malloc (2 * sizeof(int));
    int *states = (int*) malloc (100 * sizeof(int));
    int i32;
    int i33;
    int i34;
    int i35;
    int i36;
    int i37;
    int i38;
    int i;
    int j = 0;
    int td = 0;
    simulator->data = QSS_Data (100, 404, 804, 100, 2812, "inverters_qss");
    QSS_data modelData = simulator->data;
    const double t = 0;

    // Allocate main data structures.
    __PAR_VecInt_2_x0 = 5.0;
    __PAR_vector_sat_4_xl = 0.0;
    __PAR_Vec2Scalar_6_Index = 1.0;
    __PAR_vector_sat_9_xl = 0.0;
    __PAR_ramp_sci_13_t0 = 5.0;
    __PAR_ramp_sci_13_tr = 5.0;
    __PAR_ramp_sci_13_v = 5.0;
    __PAR_ramp_sci_14_t0 = 15.0;
    __PAR_ramp_sci_14_tr = 2.0;
    __PAR_ramp_sci_14_v = 5.0;
    __PAR_Constant_15_k = 1.0;
    __PAR_Constant_16_k = 5.0;
    __PAR_Vec2Scalar_18_Index = 100.0;
    // Initialize model code.
    for (i32 = 0; i32 <= 99; i32++)
    {
        modelData->x[(i32) * 3] = 5.0;
    }
    for (i33 = 0; i33 <= 99; i33++)
    {
        modelData->d[(i33)] = 0.0;
        modelData->d[(i33 + 100)] = 0.0;
    }
    for (i34 = 0; i34 <= 99; i34++)
    {
        modelData->d[(i34 + 200)] = 1.0;
        modelData->d[(i34 + 300)] = 0.0;
    }
    modelData->d[(401)] = 0.0;
    modelData->d[(400)] = 0.0;
    modelData->d[(403)] = 0.0;
    modelData->d[(402)] = 0.0;
    __PAR_IndexShift_1_p[(0)] = 1.0;
    __PAR_IndexShift_1_p[(1)] = 100.0;
    __PAR_VecInt_2_p[(0)] = 0.0;
    __PAR_VecInt_2_p[(1)] = 1.000000000000000047921736e-04;
    __PAR_VecInt_2_p[(2)] = 1.000000000000000020816682e-02;
    __PAR_VecInt_2_p[(3)] = 5.0;
    __PAR_VecInt_2_p[(4)] = 100.0;
    __PAR_VectorSum_3_p[(0)] = 1.0;
    __PAR_VectorSum_3_p[(1)] = (((-100.0)));
    __PAR_VectorSum_3_p[(2)] = 100.0;
    __PAR_VectorSum_3_p[(3)] = (((-1.0)));
    __PAR_VectorSum_3_p[(4)] = 0.0;
    __PAR_VectorSum_3_p[(5)] = 0.0;
    __PAR_VectorSum_3_p[(6)] = 0.0;
    __PAR_VectorSum_3_p[(7)] = 0.0;
    __PAR_VectorSum_3_p[(8)] = 4.0;
    __PAR_VectorSum_3_p[(9)] = 100.0;
    for (i35 = 0; i35 <= 3; i35++)
    {
        __PAR_VectorSum_3_w[(i35)] = __PAR_VectorSum_3_p[(i35)];
    }
    __PAR_vector_sat_4_p[(0)] = 0.0;
    __PAR_vector_sat_4_p[(1)] = 1.000000000000000000000000e+06;
    __PAR_vector_sat_4_p[(2)] = 100.0;
    __PAR_Scalar2Vector_5_p[(0)] = 0.0;
    __PAR_Scalar2Vector_5_p[(1)] = 100.0;
    __PAR_Vec2Scalar_6_p[(0)] = 0.0;
    __PAR_Vec2Scalar_6_p[(1)] = 100.0;
    __PAR_vector_pow2_7_p[(0)] = 100.0;
    __PAR_vector_pow2_8_p[(0)] = 100.0;
    __PAR_vector_sat_9_p[(0)] = 0.0;
    __PAR_vector_sat_9_p[(1)] = 1.000000000000000000000000e+06;
    __PAR_vector_sat_9_p[(2)] = 100.0;
    __PAR_VectorSum_10_p[(0)] = 1.0;
    __PAR_VectorSum_10_p[(1)] = (((-1.0)));
    __PAR_VectorSum_10_p[(2)] = 1.0;
    __PAR_VectorSum_10_p[(3)] = 0.0;
    __PAR_VectorSum_10_p[(4)] = 0.0;
    __PAR_VectorSum_10_p[(5)] = 0.0;
    __PAR_VectorSum_10_p[(6)] = 0.0;
    __PAR_VectorSum_10_p[(7)] = 0.0;
    __PAR_VectorSum_10_p[(8)] = 3.0;
    __PAR_VectorSum_10_p[(9)] = 100.0;
    for (i36 = 0; i36 <= 2; i36++)
    {
        __PAR_VectorSum_10_w[(i36)] = __PAR_VectorSum_10_p[(i36)];
    }
    __PAR_VectorSum_11_p[(0)] = 1.0;
    __PAR_VectorSum_11_p[(1)] = (((-1.0)));
    __PAR_VectorSum_11_p[(2)] = 0.0;
    __PAR_VectorSum_11_p[(3)] = 0.0;
    __PAR_VectorSum_11_p[(4)] = 0.0;
    __PAR_VectorSum_11_p[(5)] = 0.0;
    __PAR_VectorSum_11_p[(6)] = 0.0;
    __PAR_VectorSum_11_p[(7)] = 0.0;
    __PAR_VectorSum_11_p[(8)] = 2.0;
    __PAR_VectorSum_11_p[(9)] = 100.0;
    for (i37 = 0; i37 <= 1; i37++)
    {
        __PAR_VectorSum_11_w[(i37)] = __PAR_VectorSum_11_p[(i37)];
    }
    __PAR_Scalar2Vector_12_p[(0)] = (((-1.0)));
    __PAR_Scalar2Vector_12_p[(1)] = 100.0;
    __PAR_ramp_sci_13_p[(0)] = 5.0;
    __PAR_ramp_sci_13_p[(1)] = 5.0;
    __PAR_ramp_sci_13_p[(2)] = 5.0;
    __PAR_ramp_sci_13_p[(3)] = 0.0;
    __PAR_ramp_sci_13_p[(4)] = 1.000000000000000020816682e-02;
    __PAR_ramp_sci_14_p[(0)] = 15.0;
    __PAR_ramp_sci_14_p[(1)] = 2.0;
    __PAR_ramp_sci_14_p[(2)] = 5.0;
    __PAR_ramp_sci_14_p[(3)] = 0.0;
    __PAR_ramp_sci_14_p[(4)] = 1.000000000000000020816682e-02;
    __PAR_Constant_15_p[(0)] = 1.0;
    __PAR_Constant_16_p[(0)] = 5.0;
    __PAR_Scalar2Vector_17_p[(0)] = (((-1.0)));
    __PAR_Scalar2Vector_17_p[(1)] = 100.0;
    __PAR_Vec2Scalar_18_p[(0)] = 99.0;
    __PAR_Vec2Scalar_18_p[(1)] = 100.0;
    __PAR_WSum_20_p[(0)] = 1.0;
    __PAR_WSum_20_p[(1)] = (((-1.0)));
    __PAR_WSum_20_p[(2)] = 0.0;
    __PAR_WSum_20_p[(3)] = 0.0;
    __PAR_WSum_20_p[(4)] = 0.0;
    __PAR_WSum_20_p[(5)] = 0.0;
    __PAR_WSum_20_p[(6)] = 0.0;
    __PAR_WSum_20_p[(7)] = 0.0;
    __PAR_WSum_20_p[(8)] = 2.0;
    for (i38 = 0; i38 <= 1; i38++)
    {
        __PAR_WSum_20_w[(i38)] = __PAR_WSum_20_p[(i38)];
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nDS[i]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nDS[i]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSD[i - 1]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nSD[i]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 100]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 200]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 300]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 400]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nZS[i + 400]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 500]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nZS[i + 500]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 600]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nZS[i + 600]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nZS[i + 700]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nZS[i + 700]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nSZ[i]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nSZ[i]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nSZ[i]++;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->nSZ[i - 1]++;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->nSZ[i]++;
    }
    modelData->nHZ[800] += 800;
    modelData->nHZ[801] += 800;
    modelData->nHZ[802] += 800;
    modelData->nHZ[803] += 800;
    for (i = 0; i <= 99; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 100; i <= 199; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 200; i <= 299; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 300; i <= 399; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 400; i <= 499; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 500; i <= 599; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 600; i <= 699; i++)
    {
        modelData->nHD[i]++;
    }
    for (i = 700; i <= 799; i++)
    {
        modelData->nHD[i]++;
    }
    modelData->nHD[800] = 1;
    modelData->nHD[801] = 1;
    modelData->nHD[802] = 1;
    modelData->nHD[803] = 1;
    for (i = 0; i <= 99; i++)
    {
        modelData->event[i].nLHSDsc = 1;
        modelData->event[i + 100].nLHSDsc = 1;
        modelData->event[i + 200].nLHSDsc = 1;
        modelData->event[i + 300].nLHSDsc = 1;
        modelData->event[i + 400].nLHSDsc = 1;
        modelData->event[i + 500].nLHSDsc = 1;
        modelData->event[i + 600].nLHSDsc = 1;
        modelData->event[i + 700].nLHSDsc = 1;
    }
    modelData->event[800].nLHSDsc = 1;
    modelData->event[801].nLHSDsc = 1;
    modelData->event[802].nLHSDsc = 1;
    modelData->event[803].nLHSDsc = 1;
    QSS_allocDataMatrix (modelData);
    // Initialize model data.
    // Initialize model time.
    for (i = 0; i <= 99; i++)
    {
        modelData->TD[td++] = i;
    }
    cleanVector (states, 0, 100);

    for (i = 1; i <= 99; i++)
    {
        modelData->DS[i][states[i]++] = i - 1;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->DS[i][states[i]++] = i;
    }
    cleanVector (states, 0, 100);

    for (i = 1; i <= 99; i++)
    {
        modelData->SD[i - 1][states[i - 1]++] = i;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->SD[i][states[i]++] = i;
    }
    cleanVector (events, 0, 804);

    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i][events[i]++] = i - 1;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 100][events[i + 100]++] = i - 1;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 200][events[i + 200]++] = i - 1;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 300][events[i + 300]++] = i - 1;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 400][events[i + 400]++] = i - 1;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->ZS[i + 400][events[i + 400]++] = i;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 500][events[i + 500]++] = i - 1;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->ZS[i + 500][events[i + 500]++] = i;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 600][events[i + 600]++] = i - 1;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->ZS[i + 600][events[i + 600]++] = i;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->ZS[i + 700][events[i + 700]++] = i - 1;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->ZS[i + 700][events[i + 700]++] = i;
    }
    cleanVector (states, 0, 100);

    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 100;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 200;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 300;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 400;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->SZ[i][states[i]++] = i + 400;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 500;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->SZ[i][states[i]++] = i + 500;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 600;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->SZ[i][states[i]++] = i + 600;
    }
    for (i = 1; i <= 99; i++)
    {
        modelData->SZ[i - 1][states[i - 1]++] = i + 700;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->SZ[i][states[i]++] = i + 700;
    }
    cleanVector (events, 0, 804);

    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 100;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 200;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 300;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 400;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 500;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 600;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[800][events[800]++] = i + 700;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 100;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 200;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 300;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 400;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 500;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 600;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[801][events[801]++] = i + 700;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 100;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 200;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 300;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 400;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 500;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 600;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[802][events[802]++] = i + 700;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 100;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 200;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 300;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 400;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 500;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 600;
    }
    for (i = 0; i <= 99; i++)
    {
        modelData->HZ[803][events[803]++] = i + 700;
    }
    cleanVector (events, 0, 804);

    for (i = 0; i <= 99; i++)
    {
        modelData->HD[i][events[i]++] = i;
    }
    for (i = 100; i <= 199; i++)
    {
        modelData->HD[i][events[i]++] = i - 100;
    }
    for (i = 200; i <= 299; i++)
    {
        modelData->HD[i][events[i]++] = i - 200;
    }
    for (i = 300; i <= 399; i++)
    {
        modelData->HD[i][events[i]++] = i - 300;
    }
    for (i = 400; i <= 499; i++)
    {
        modelData->HD[i][events[i]++] = i - 400;
    }
    for (i = 500; i <= 599; i++)
    {
        modelData->HD[i][events[i]++] = i - 500;
    }
    for (i = 600; i <= 699; i++)
    {
        modelData->HD[i][events[i]++] = i - 600;
    }
    for (i = 700; i <= 799; i++)
    {
        modelData->HD[i][events[i]++] = i - 700;
    }
    modelData->HD[800][events[800]++] = 0;
    modelData->HD[801][events[801]++] = 0;
    modelData->HD[802][events[802]++] = 0;
    modelData->HD[803][events[803]++] = 0;
    cleanVector (events, 0, 804);

    for (i = 0; i <= 99; i++)
    {
        modelData->event[i].LHSDsc[events[i]++] = i;
        modelData->event[i + 100].LHSDsc[events[i + 100]++] = i;
        modelData->event[i + 200].LHSDsc[events[i + 200]++] = i + 100;
        modelData->event[i + 300].LHSDsc[events[i + 300]++] = i + 100;
        modelData->event[i + 400].LHSDsc[events[i + 400]++] = i + 200;
        modelData->event[i + 500].LHSDsc[events[i + 500]++] = i + 200;
        modelData->event[i + 600].LHSDsc[events[i + 600]++] = i + 300;
        modelData->event[i + 700].LHSDsc[events[i + 700]++] = i + 300;
    }
    modelData->event[800].LHSDsc[events[800]++] = 400;
    modelData->event[801].LHSDsc[events[801]++] = 401;
    modelData->event[802].LHSDsc[events[802]++] = 402;
    modelData->event[803].LHSDsc[events[803]++] = 403;
    cleanVector (events, 0, 804);

    for (i = 0; i <= 99; i++)
    {
        modelData->event[i].direction = -1;
        modelData->event[i].relation = 0;
        modelData->event[i + 100].direction = 1;
        modelData->event[i + 100].relation = 3;
        modelData->event[i + 200].direction = 1;
        modelData->event[i + 200].relation = 2;
        modelData->event[i + 300].direction = -1;
        modelData->event[i + 300].relation = 1;
        modelData->event[i + 400].direction = -1;
        modelData->event[i + 400].relation = 0;
        modelData->event[i + 500].direction = 1;
        modelData->event[i + 500].relation = 3;
        modelData->event[i + 600].direction = 1;
        modelData->event[i + 600].relation = 2;
        modelData->event[i + 700].direction = -1;
        modelData->event[i + 700].relation = 1;
    }
    modelData->event[800].direction = 1;
    modelData->event[800].relation = 2;
    modelData->event[801].direction = 1;
    modelData->event[801].relation = 2;
    modelData->event[802].direction = 1;
    modelData->event[802].relation = 2;
    modelData->event[803].direction = 1;
    modelData->event[803].relation = 2;
    simulator->time = QSS_Time (100, 804, 100, 0, ST_Binary, NULL);

    simulator->output = SD_Output ("inverters_qss", 2, 404, 100, NULL, 0, 0, CI_Step, SD_Memory, MOD_output);
    SD_output modelOutput = simulator->output;

    modelOutput->nOS[0] = 1;
    modelOutput->nSO[0]++;
    modelOutput->nOS[1] = 1;
    modelOutput->nSO[99]++;
    SD_allocOutputMatrix (modelOutput, 100, 404);
    cleanVector (states, 0, 100);

    cleanVector (outputs, 0, 2);

    sprintf (modelOutput->variable[0].name, "VecInt_2_y_1[1]");
    sprintf (modelOutput->variable[1].name, "VecInt_2_y_1[100]");
    cleanVector (outputs, 0, 2);

    modelOutput->SO[0][states[0]++] = 0;
    modelOutput->OS[0][outputs[0]++] = 0;
    modelOutput->SO[99][states[99]++] = 1;
    modelOutput->OS[1][outputs[1]++] = 99;
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
