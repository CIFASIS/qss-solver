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
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 4;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	double tmp10[3];
	int i0 = i;
	int j0 = 0;
	int j100 = 0;
	int j110;
	int j120 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 99)
	{
		alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
		alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
		alg[6] = 1.0;
		alg[9] = 5.0;
		j100 = j0-1;
	if (j0 >= 1 && j0 <= 99)
	{
		alg[(j0-1+4) * 3] = x[(j100) * 3];
		}
		alg[(j0+104) * 3] = x[(j0) * 3];
		alg[(j0+304) * 3] = x[(j0) * 3];
		alg[1212] = alg[6];
		alg[1215] = alg[9];
			alg[1518] = alg[0];
			alg[1521] = alg[3];
		alg[(j0+508) * 3] = alg[1212];
		alg[(j0+608) * 3] = alg[1215];
		alg[(j0+708) * 3] = alg[(j0+508) * 3];
		alg[(j0+808) * 3] = alg[(j0+608) * 3];
	for(j110 = 0;j110 < 3; j110++)
	{
		tmp10[j110] = 0;
	}
	for(j110 = 0; j110 <= 1; j110++)
	{
		tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
		tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
		tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
	}
		alg[2724] = tmp10[0];
		alg[2727] = alg[2724];
		alg[2730] = alg[2727];
		j120 = j0;
	if (j120 >= 1 && j120 <= 99)
	{
		alg[(j0+910) * 3] = 0.0;
		}
		j120 = j0;
	if (j120 >= 0 && j120 <= 0)
	{
		alg[(j0+1010) * 3] = 0.0;
		}
		j120 = j0-1;
	if (j120 >= 0 && j120 <= 98)
	{
		alg[(j0+1010) * 3] = alg[(j120+4) * 3];
		}
		alg[(j0+1110) * 3] = alg[(j0+1010) * 3];
		alg[(j0+1210) * 3] = alg[(j0+910) * 3];
		alg[(j0+1310) * 3] = alg[(j0+1210) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(j0+708) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(j0+1110) * 3]*__PAR_VectorSum_10_w[(2)];
		alg[(j0+1410) * 3] = alg[(j0+1310) * 3];
		alg[(j0+1510) * 3] = alg[(j0+1310) * 3];
		alg[(j0+1610) * 3] = alg[(j0+1410) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(j0+304) * 3]*__PAR_VectorSum_11_w[(1)];
		alg[(j0+1710) * 3] = alg[(j0+1610) * 3];
		alg[(j0+1810) * 3] = d[(j0)]*__PAR_vector_sat_4_xl+(1.0-d[(j0)])*(d[(j0+100)]*1000000+(1.0-d[(j0+100)])*alg[(j0+1510) * 3]);
		alg[(j0+1910) * 3] = d[(j0+200)]*__PAR_vector_sat_9_xl+(1.0-d[(j0+200)])*(d[(j0+300)]*1000000+(1.0-d[(j0+300)])*alg[(j0+1710) * 3]);
		alg[(j0+2010) * 3] = alg[(j0+1910) * 3];
		alg[(j0+2110) * 3] = alg[(j0+1810) * 3];
		alg[(j0+2210) * 3] = alg[(j0+2010) * 3]*alg[(j0+2010) * 3];
		alg[(j0+2310) * 3] = alg[(j0+2110) * 3]*alg[(j0+2110) * 3];
		alg[(j0+2410) * 3] = alg[(j0+2210) * 3];
		alg[(j0+2510) * 3] = alg[(j0+2310) * 3];
		alg[(j0+2610) * 3] = alg[(j0+808) * 3]*__PAR_VectorSum_3_w[(0)]+alg[(j0+2510) * 3]*__PAR_VectorSum_3_w[(1)]+alg[(j0+2410) * 3]*__PAR_VectorSum_3_w[(2)]+alg[(j0+104) * 3]*__PAR_VectorSum_3_w[(3)];
		alg[(j0+2710) * 3] = alg[(j0+2610) * 3];
		dx[1] = alg[(j0+2710) * 3];
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp10[3];
	int i0 = i;
	int j0 = 0;
	int j0140 = 0;
	int j0160 = 0;
	int j100 = 0;
	int j110;
	int j120 = 0;
	int j130 = 0;
	int j150 = 0;
	j0 = i0+1;
	if(j0 >=1 && j0 <= 99)
	{
		alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
		alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
		alg[6] = 1.0;
		alg[9] = 5.0;
		j130 = j0-1;
	if (j0 >= 1 && j0 <= 99)
	{
		alg[(j0-1+4) * 3] = x[(j130) * 3];
		}
		alg[(j0+104) * 3] = x[(j0) * 3];
		alg[(j0+304) * 3] = x[(j0) * 3];
		alg[1212] = alg[6];
		alg[1215] = alg[9];
		alg[1518] = alg[0];
		alg[1521] = alg[3];
		alg[(j0+508) * 3] = alg[1212];
		alg[(j0+608) * 3] = alg[1215];
		alg[(j0+708) * 3] = alg[(j0+508) * 3];
		alg[(j0+808) * 3] = alg[(j0+608) * 3];
	for(j110 = 0;j110 < 3; j110++)
	{
		tmp10[j110] = 0;
	}
	for(j110 = 0; j110 <= 1; j110++)
	{
		tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
		tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
		tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
	}
		alg[2724] = tmp10[0];
		alg[2727] = alg[2724];
		alg[2730] = alg[2727];
		j0140 = j0;
	if (j0140 >= 1 && j0140 <= 99)
	{
		alg[(j0+910) * 3] = 0.0;
		}
		j0140 = j0;
	if (j0140 >= 0 && j0140 <= 0)
	{
		alg[(j0+1010) * 3] = 0.0;
		}
		j0140 = j0-1;
	if (j0140 >= 0 && j0140 <= 98)
	{
		alg[(j0+1010) * 3] = alg[(j0140+4) * 3];
		}
		alg[(j0+1110) * 3] = alg[(j0+1010) * 3];
		alg[(j0+1210) * 3] = alg[(j0+910) * 3];
		alg[(j0+1310) * 3] = alg[(j0+1210) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(j0+708) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(j0+1110) * 3]*__PAR_VectorSum_10_w[(2)];
		alg[(j0+1410) * 3] = alg[(j0+1310) * 3];
		alg[(j0+1510) * 3] = alg[(j0+1310) * 3];
		alg[(j0+1610) * 3] = alg[(j0+1410) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(j0+304) * 3]*__PAR_VectorSum_11_w[(1)];
		alg[(j0+1710) * 3] = alg[(j0+1610) * 3];
		alg[(j0+1810) * 3] = d[(j0)]*__PAR_vector_sat_4_xl+(1.0-d[(j0)])*(d[(j0+100)]*1000000+(1.0-d[(j0+100)])*alg[(j0+1510) * 3]);
		alg[(j0+1910) * 3] = d[(j0+200)]*__PAR_vector_sat_9_xl+(1.0-d[(j0+200)])*(d[(j0+300)]*1000000+(1.0-d[(j0+300)])*alg[(j0+1710) * 3]);
		alg[(j0+2010) * 3] = alg[(j0+1910) * 3];
		alg[(j0+2110) * 3] = alg[(j0+1810) * 3];
		alg[(j0+2210) * 3] = alg[(j0+2010) * 3]*alg[(j0+2010) * 3];
		alg[(j0+2310) * 3] = alg[(j0+2110) * 3]*alg[(j0+2110) * 3];
		alg[(j0+2410) * 3] = alg[(j0+2210) * 3];
		alg[(j0+2510) * 3] = alg[(j0+2310) * 3];
		alg[(j0+2610) * 3] = alg[(j0+808) * 3]*__PAR_VectorSum_3_w[(0)]+alg[(j0+2510) * 3]*__PAR_VectorSum_3_w[(1)]+alg[(j0+2410) * 3]*__PAR_VectorSum_3_w[(2)]+alg[(j0+104) * 3]*__PAR_VectorSum_3_w[(3)];
		alg[(j0+2710) * 3] = alg[(j0+2610) * 3];
		der[(j0) * 3 + 1] = alg[(j0+2710) * 3];
	}
	j0 = i0;
	if(j0 >=0 && j0 <= 99)
	{
		alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
		alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
		alg[6] = 1.0;
		alg[9] = 5.0;
		j150 = j0-1;
	if (j0 >= 1 && j0 <= 99)
	{
		alg[(j0-1+4) * 3] = x[(j150) * 3];
		}
		alg[(j0+104) * 3] = x[(j0) * 3];
		alg[(j0+304) * 3] = x[(j0) * 3];
		alg[1212] = alg[6];
		alg[1215] = alg[9];
		alg[1518] = alg[0];
		alg[1521] = alg[3];
		alg[(j0+508) * 3] = alg[1212];
		alg[(j0+608) * 3] = alg[1215];
		alg[(j0+708) * 3] = alg[(j0+508) * 3];
		alg[(j0+808) * 3] = alg[(j0+608) * 3];
	for(j110 = 0;j110 < 3; j110++)
	{
		tmp10[j110] = 0;
	}
	for(j110 = 0; j110 <= 1; j110++)
	{
		tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
		tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
		tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
	}
		alg[2724] = tmp10[0];
		alg[2727] = alg[2724];
		alg[2730] = alg[2727];
		j0160 = j0;
	if (j0160 >= 1 && j0160 <= 99)
	{
		alg[(j0+910) * 3] = 0.0;
		}
		j0160 = j0;
	if (j0160 >= 0 && j0160 <= 0)
	{
		alg[(j0+1010) * 3] = 0.0;
		}
		j0160 = j0-1;
	if (j0160 >= 0 && j0160 <= 98)
	{
		alg[(j0+1010) * 3] = alg[(j0160+4) * 3];
		}
		alg[(j0+1110) * 3] = alg[(j0+1010) * 3];
		alg[(j0+1210) * 3] = alg[(j0+910) * 3];
		alg[(j0+1310) * 3] = alg[(j0+1210) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(j0+708) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(j0+1110) * 3]*__PAR_VectorSum_10_w[(2)];
		alg[(j0+1410) * 3] = alg[(j0+1310) * 3];
		alg[(j0+1510) * 3] = alg[(j0+1310) * 3];
		alg[(j0+1610) * 3] = alg[(j0+1410) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(j0+304) * 3]*__PAR_VectorSum_11_w[(1)];
		alg[(j0+1710) * 3] = alg[(j0+1610) * 3];
		alg[(j0+1810) * 3] = d[(j0)]*__PAR_vector_sat_4_xl+(1.0-d[(j0)])*(d[(j0+100)]*1000000+(1.0-d[(j0+100)])*alg[(j0+1510) * 3]);
		alg[(j0+1910) * 3] = d[(j0+200)]*__PAR_vector_sat_9_xl+(1.0-d[(j0+200)])*(d[(j0+300)]*1000000+(1.0-d[(j0+300)])*alg[(j0+1710) * 3]);
		alg[(j0+2010) * 3] = alg[(j0+1910) * 3];
		alg[(j0+2110) * 3] = alg[(j0+1810) * 3];
		alg[(j0+2210) * 3] = alg[(j0+2010) * 3]*alg[(j0+2010) * 3];
		alg[(j0+2310) * 3] = alg[(j0+2110) * 3]*alg[(j0+2110) * 3];
		alg[(j0+2410) * 3] = alg[(j0+2210) * 3];
		alg[(j0+2510) * 3] = alg[(j0+2310) * 3];
		alg[(j0+2610) * 3] = alg[(j0+808) * 3]*__PAR_VectorSum_3_w[(0)]+alg[(j0+2510) * 3]*__PAR_VectorSum_3_w[(1)]+alg[(j0+2410) * 3]*__PAR_VectorSum_3_w[(2)]+alg[(j0+104) * 3]*__PAR_VectorSum_3_w[(3)];
		alg[(j0+2710) * 3] = alg[(j0+2610) * 3];
		der[(j0) * 3 + 1] = alg[(j0+2710) * 3];
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	double tmp10[3];
	int i0 = i;
	int j110;
	int j170 = 0;
	int j180 = 0;
	int j190 = 0;
	int j200 = 0;
	int j210 = 0;
	int j220 = 0;
	int j230 = 0;
	int j240 = 0;
	int j250 = 0;
	int j260 = 0;
	int j270 = 0;
	int j280 = 0;
	int j290 = 0;
	int j300 = 0;
	int j310 = 0;
	int j320 = 0;
	switch(i)
	{
		case 800:
			zc[0] = t-(5.0);
			return;
		case 801:
			zc[0] = t-(10.0);
			return;
		case 802:
			zc[0] = t-(15.0);
			return;
		case 803:
			zc[0] = t-(17.0);
			return;
		default:
			if(i0 >= 0 && i0 <= 99)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j170 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1+4) * 3] = x[(j170) * 3];
				}
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0+508) * 3] = alg[1212];
				alg[(i0+708) * 3] = alg[(i0+508) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j180 = i0;
	if (j180 >= 1 && j180 <= 99)
	{
				alg[(i0+910) * 3] = 0.0;
				}
				j180 = i0;
	if (j180 >= 0 && j180 <= 0)
	{
				alg[(i0+1010) * 3] = 0.0;
				}
				j180 = i0-1;
	if (j180 >= 0 && j180 <= 98)
	{
				alg[(i0+1010) * 3] = alg[(j180+4) * 3];
				}
				alg[(i0+1110) * 3] = alg[(i0+1010) * 3];
				alg[(i0+1210) * 3] = alg[(i0+910) * 3];
				alg[(i0+1310) * 3] = alg[(i0+1210) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+708) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+1110) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+1510) * 3] = alg[(i0+1310) * 3];
				zc[0] = alg[(i0+1510) * 3]-(__PAR_vector_sat_4_xl);
			}
			if(i0 >= 100 && i0 <= 199)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j190 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-96) * 3] = x[(j190-100) * 3];
				}
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0+408) * 3] = alg[1212];
				alg[(i0+608) * 3] = alg[(i0+408) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j200 = i0;
	if (j200 >= 1 && j200 <= 99)
	{
				alg[(i0+810) * 3] = 0.0;
				}
				j200 = i0;
	if (j200 >= 0 && j200 <= 0)
	{
				alg[(i0+910) * 3] = 0.0;
				}
				j200 = i0-1;
	if (j200 >= 0 && j200 <= 98)
	{
				alg[(i0+910) * 3] = alg[(j200-96) * 3];
				}
				alg[(i0+1010) * 3] = alg[(i0+910) * 3];
				alg[(i0+1110) * 3] = alg[(i0+810) * 3];
				alg[(i0+1210) * 3] = alg[(i0+1110) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+608) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+1010) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+1410) * 3] = alg[(i0+1210) * 3];
				zc[0] = alg[(i0+1410) * 3]-(__PAR_vector_sat_4_xl);
			}
			if(i0 >= 200 && i0 <= 299)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j210 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-196) * 3] = x[(j210-200) * 3];
				}
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0+308) * 3] = alg[1212];
				alg[(i0+508) * 3] = alg[(i0+308) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j220 = i0;
	if (j220 >= 1 && j220 <= 99)
	{
				alg[(i0+710) * 3] = 0.0;
				}
				j220 = i0;
	if (j220 >= 0 && j220 <= 0)
	{
				alg[(i0+810) * 3] = 0.0;
				}
				j220 = i0-1;
	if (j220 >= 0 && j220 <= 98)
	{
				alg[(i0+810) * 3] = alg[(j220-196) * 3];
				}
				alg[(i0+910) * 3] = alg[(i0+810) * 3];
				alg[(i0+1010) * 3] = alg[(i0+710) * 3];
				alg[(i0+1110) * 3] = alg[(i0+1010) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+508) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+910) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+1310) * 3] = alg[(i0+1110) * 3];
				zc[0] = alg[(i0+1310) * 3]-(1000000);
			}
			if(i0 >= 300 && i0 <= 399)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j230 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-296) * 3] = x[(j230-300) * 3];
				}
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0+208) * 3] = alg[1212];
				alg[(i0+408) * 3] = alg[(i0+208) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j240 = i0;
	if (j240 >= 1 && j240 <= 99)
	{
				alg[(i0+610) * 3] = 0.0;
				}
				j240 = i0;
	if (j240 >= 0 && j240 <= 0)
	{
				alg[(i0+710) * 3] = 0.0;
				}
				j240 = i0-1;
	if (j240 >= 0 && j240 <= 98)
	{
				alg[(i0+710) * 3] = alg[(j240-296) * 3];
				}
				alg[(i0+810) * 3] = alg[(i0+710) * 3];
				alg[(i0+910) * 3] = alg[(i0+610) * 3];
				alg[(i0+1010) * 3] = alg[(i0+910) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+408) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+810) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+1210) * 3] = alg[(i0+1010) * 3];
				zc[0] = alg[(i0+1210) * 3]-(1000000);
			}
			if(i0 >= 400 && i0 <= 499)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j250 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-396) * 3] = x[(j250-400) * 3];
				}
				alg[(i0-96) * 3] = x[(i0-400) * 3];
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0+108) * 3] = alg[1212];
				alg[(i0+308) * 3] = alg[(i0+108) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j260 = i0;
	if (j260 >= 1 && j260 <= 99)
	{
				alg[(i0+510) * 3] = 0.0;
				}
				j260 = i0;
	if (j260 >= 0 && j260 <= 0)
	{
				alg[(i0+610) * 3] = 0.0;
				}
				j260 = i0-1;
	if (j260 >= 0 && j260 <= 98)
	{
				alg[(i0+610) * 3] = alg[(j260-396) * 3];
				}
				alg[(i0+710) * 3] = alg[(i0+610) * 3];
				alg[(i0+810) * 3] = alg[(i0+510) * 3];
				alg[(i0+910) * 3] = alg[(i0+810) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+308) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+710) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+1010) * 3] = alg[(i0+910) * 3];
				alg[(i0+1210) * 3] = alg[(i0+1010) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(i0-96) * 3]*__PAR_VectorSum_11_w[(1)];
				alg[(i0+1310) * 3] = alg[(i0+1210) * 3];
				zc[0] = alg[(i0+1310) * 3]-(__PAR_vector_sat_9_xl);
			}
			if(i0 >= 500 && i0 <= 599)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j270 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-496) * 3] = x[(j270-500) * 3];
				}
				alg[(i0-196) * 3] = x[(i0-500) * 3];
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0+8) * 3] = alg[1212];
				alg[(i0+208) * 3] = alg[(i0+8) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j280 = i0;
	if (j280 >= 1 && j280 <= 99)
	{
				alg[(i0+410) * 3] = 0.0;
				}
				j280 = i0;
	if (j280 >= 0 && j280 <= 0)
	{
				alg[(i0+510) * 3] = 0.0;
				}
				j280 = i0-1;
	if (j280 >= 0 && j280 <= 98)
	{
				alg[(i0+510) * 3] = alg[(j280-496) * 3];
				}
				alg[(i0+610) * 3] = alg[(i0+510) * 3];
				alg[(i0+710) * 3] = alg[(i0+410) * 3];
				alg[(i0+810) * 3] = alg[(i0+710) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+208) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+610) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+910) * 3] = alg[(i0+810) * 3];
				alg[(i0+1110) * 3] = alg[(i0+910) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(i0-196) * 3]*__PAR_VectorSum_11_w[(1)];
				alg[(i0+1210) * 3] = alg[(i0+1110) * 3];
				zc[0] = alg[(i0+1210) * 3]-(__PAR_vector_sat_9_xl);
			}
			if(i0 >= 600 && i0 <= 699)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j290 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-596) * 3] = x[(j290-600) * 3];
				}
				alg[(i0-296) * 3] = x[(i0-600) * 3];
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0-92) * 3] = alg[1212];
				alg[(i0+108) * 3] = alg[(i0-92) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j300 = i0;
	if (j300 >= 1 && j300 <= 99)
	{
				alg[(i0+310) * 3] = 0.0;
				}
				j300 = i0;
	if (j300 >= 0 && j300 <= 0)
	{
				alg[(i0+410) * 3] = 0.0;
				}
				j300 = i0-1;
	if (j300 >= 0 && j300 <= 98)
	{
				alg[(i0+410) * 3] = alg[(j300-596) * 3];
				}
				alg[(i0+510) * 3] = alg[(i0+410) * 3];
				alg[(i0+610) * 3] = alg[(i0+310) * 3];
				alg[(i0+710) * 3] = alg[(i0+610) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+108) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+510) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+810) * 3] = alg[(i0+710) * 3];
				alg[(i0+1010) * 3] = alg[(i0+810) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(i0-296) * 3]*__PAR_VectorSum_11_w[(1)];
				alg[(i0+1110) * 3] = alg[(i0+1010) * 3];
				zc[0] = alg[(i0+1110) * 3]-(1000000);
			}
			if(i0 >= 700 && i0 <= 799)
			{
				alg[0] = ((-d[(401)]*t*d[(400)]))+5.0*d[(401)]+5.0*d[(401)]*d[(400)]+t*d[(400)]-5.0*d[(400)];
				alg[3] = ((-75.0/2.0*d[(402)]))+5.0*d[(403)]+75.0/2.0*d[(402)]*d[(403)]-5.0/2.0*d[(402)]*d[(403)]*t+5.0/2.0*d[(402)]*t;
				alg[6] = 1.0;
				j310 = i0-1;
	if (i0 >= 1 && i0 <= 99)
	{
				alg[(i0-1-696) * 3] = x[(j310-700) * 3];
				}
				alg[(i0-396) * 3] = x[(i0-700) * 3];
				alg[1212] = alg[6];
					alg[1518] = alg[0];
					alg[1521] = alg[3];
				alg[(i0-192) * 3] = alg[1212];
				alg[(i0+8) * 3] = alg[(i0-192) * 3];
			for(j110 = 0;j110 < 3; j110++)
			{
				tmp10[j110] = 0;
			}
			for(j110 = 0; j110 <= 1; j110++)
			{
				tmp10[0] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 0];
				tmp10[1] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 1];
				tmp10[2] += __PAR_WSum_20_w[j110]*alg[(j110+506) * 3 + 2];
			}
				alg[2724] = tmp10[0];
				alg[2727] = alg[2724];
				alg[2730] = alg[2727];
				j320 = i0;
	if (j320 >= 1 && j320 <= 99)
	{
				alg[(i0+210) * 3] = 0.0;
				}
				j320 = i0;
	if (j320 >= 0 && j320 <= 0)
	{
				alg[(i0+310) * 3] = 0.0;
				}
				j320 = i0-1;
	if (j320 >= 0 && j320 <= 98)
	{
				alg[(i0+310) * 3] = alg[(j320-696) * 3];
				}
				alg[(i0+410) * 3] = alg[(i0+310) * 3];
				alg[(i0+510) * 3] = alg[(i0+210) * 3];
				alg[(i0+610) * 3] = alg[(i0+510) * 3]*__PAR_VectorSum_10_w[(0)]+alg[(i0+8) * 3]*__PAR_VectorSum_10_w[(1)]+alg[(i0+410) * 3]*__PAR_VectorSum_10_w[(2)];
				alg[(i0+710) * 3] = alg[(i0+610) * 3];
				alg[(i0+910) * 3] = alg[(i0+710) * 3]*__PAR_VectorSum_11_w[(0)]+alg[(i0-396) * 3]*__PAR_VectorSum_11_w[(1)];
				alg[(i0+1010) * 3] = alg[(i0+910) * 3];
				zc[0] = alg[(i0+1010) * 3]-(1000000);
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
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
			if(i0 >= 100 && i0 <= 199)
			{
				d[(i0-100)] = 0.0;
			}
			if(i0 >= 200 && i0 <= 299)
			{
				d[(i0-100)] = 1.0;
			}
			if(i0 >= 500 && i0 <= 599)
			{
				d[(i0-300)] = 0.0;
			}
			if(i0 >= 600 && i0 <= 699)
			{
				d[(i0-300)] = 1.0;
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 99)
	{
		d[(i0)] = 1.0;
	}
	if(i0 >= 300 && i0 <= 399)
	{
		d[(i0-200)] = 0.0;
	}
	if(i0 >= 400 && i0 <= 499)
	{
		d[(i0-200)] = 1.0;
	}
	if(i0 >= 700 && i0 <= 799)
	{
		d[(i0-400)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
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
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(404*sizeof(int));
	int *events = (int*)malloc(804*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(100*sizeof(int));
	int i0;
	int j0 = 0;
	int td = 0;
	simulator->data = QSS_Data(100,404,804,100,2812,"inverters_qss");
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
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->x[(i0) * 3] = 5.0;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->d[(i0)] = 0.0;
		modelData->d[(i0+100)] = 0.0;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->d[(i0+200)] = 1.0;
		modelData->d[(i0+300)] = 0.0;
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
	for(i0 = 0; i0 <= 3; i0++)
	{
		__PAR_VectorSum_3_w[(i0)] = __PAR_VectorSum_3_p[(i0)];
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
	for(i0 = 0; i0 <= 2; i0++)
	{
		__PAR_VectorSum_10_w[(i0)] = __PAR_VectorSum_10_p[(i0)];
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
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_VectorSum_11_w[(i0)] = __PAR_VectorSum_11_p[(i0)];
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
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_WSum_20_w[(i0)] = __PAR_WSum_20_p[(i0)];
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nDS[i0]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nDS[i0]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSD[i0-1]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nSD[i0]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+100]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+200]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+300]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+400]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+400]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+500]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+500]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+600]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+600]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+700]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nZS[i0+700]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0-1]++;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	modelData->nHZ[800] += 800;
	modelData->nHZ[801] += 800;
	modelData->nHZ[802] += 800;
	modelData->nHZ[803] += 800;
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 100; i0 <= 199; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 200; i0 <= 299; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 300; i0 <= 399; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 400; i0 <= 499; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 500; i0 <= 599; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 600; i0 <= 699; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 700; i0 <= 799; i0++)
	{
		modelData->nHD[i0]++;
	}
	modelData->nHD[800] = 1;
	modelData->nHD[801] = 1;
	modelData->nHD[802] = 1;
	modelData->nHD[803] = 1;
	for(i0 = 0; i0 <= 99; i0++)
	{
	modelData->event[i0].nLHSDsc = 1;
	modelData->event[i0+100].nLHSDsc = 1;
	modelData->event[i0+200].nLHSDsc = 1;
	modelData->event[i0+300].nLHSDsc = 1;
	modelData->event[i0+400].nLHSDsc = 1;
	modelData->event[i0+500].nLHSDsc = 1;
	modelData->event[i0+600].nLHSDsc = 1;
	modelData->event[i0+700].nLHSDsc = 1;
	}
	modelData->event[800].nLHSDsc = 1;
	modelData->event[801].nLHSDsc = 1;
	modelData->event[802].nLHSDsc = 1;
	modelData->event[803].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
	for (i0 = 0; i0 <= 99;i0++)
	{
	modelData->TD[td++] = i0;
	}
		cleanVector(states,0,100);

	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0-1;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0;
	}
		cleanVector(states,0,100);

	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SD[i0-1][states[i0-1]++] = i0;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,804);

	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0][events[i0]++] = i0-1;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+100][events[i0+100]++] = i0-1;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+200][events[i0+200]++] = i0-1;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+300][events[i0+300]++] = i0-1;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+400][events[i0+400]++] = i0-1;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+400][events[i0+400]++] = i0;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+500][events[i0+500]++] = i0-1;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+500][events[i0+500]++] = i0;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+600][events[i0+600]++] = i0-1;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+600][events[i0+600]++] = i0;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+700][events[i0+700]++] = i0-1;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->ZS[i0+700][events[i0+700]++] = i0;
	}
		cleanVector(states,0,100);

	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+100;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+200;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+300;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+400;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+400;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+500;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+500;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+600;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+600;
	}
	for ( i0 = 1; i0 <= 99; i0++) 
	{
	modelData->SZ[i0-1][states[i0-1]++] = i0+700;
	}
	for ( i0 = 0; i0 <= 99; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+700;
	}
		cleanVector(events,0,804);

	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+100;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+200;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+300;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+400;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+500;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+600;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[800][events[800]++] = i0+700;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+100;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+200;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+300;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+400;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+500;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+600;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[801][events[801]++] = i0+700;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+100;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+200;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+300;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+400;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+500;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+600;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[802][events[802]++] = i0+700;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+100;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+200;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+300;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+400;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+500;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+600;
	}
	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HZ[803][events[803]++] = i0+700;
	}
		cleanVector(events,0,804);

	for(i0 = 0; i0 <= 99; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 100; i0 <= 199; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-100;
	}
	for(i0 = 200; i0 <= 299; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-200;
	}
	for(i0 = 300; i0 <= 399; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-300;
	}
	for(i0 = 400; i0 <= 499; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-400;
	}
	for(i0 = 500; i0 <= 599; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-500;
	}
	for(i0 = 600; i0 <= 699; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-600;
	}
	for(i0 = 700; i0 <= 799; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-700;
	}
	modelData->HD[800][events[800]++] = 0;
	modelData->HD[801][events[801]++] = 0;
	modelData->HD[802][events[802]++] = 0;
	modelData->HD[803][events[803]++] = 0;
		cleanVector(events,0,804);

	for(i0 = 0; i0 <= 99; i0++)
	{
	modelData->event[i0].LHSDsc[events[i0]++] = i0;
	modelData->event[i0+100].LHSDsc[events[i0+100]++] = i0;
	modelData->event[i0+200].LHSDsc[events[i0+200]++] = i0+100;
	modelData->event[i0+300].LHSDsc[events[i0+300]++] = i0+100;
	modelData->event[i0+400].LHSDsc[events[i0+400]++] = i0+200;
	modelData->event[i0+500].LHSDsc[events[i0+500]++] = i0+200;
	modelData->event[i0+600].LHSDsc[events[i0+600]++] = i0+300;
	modelData->event[i0+700].LHSDsc[events[i0+700]++] = i0+300;
	}
	modelData->event[800].LHSDsc[events[800]++] = 400;
	modelData->event[801].LHSDsc[events[801]++] = 401;
	modelData->event[802].LHSDsc[events[802]++] = 402;
	modelData->event[803].LHSDsc[events[803]++] = 403;
		cleanVector(events,0,804);

	for(i0 = 0; i0 <= 99; i0++)
	{
	modelData->event[i0].direction = -1;
	modelData->event[i0].relation = 0;
	modelData->event[i0+100].direction = 1;
	modelData->event[i0+100].relation = 3;
	modelData->event[i0+200].direction = 1;
	modelData->event[i0+200].relation = 2;
	modelData->event[i0+300].direction = -1;
	modelData->event[i0+300].relation = 1;
	modelData->event[i0+400].direction = -1;
	modelData->event[i0+400].relation = 0;
	modelData->event[i0+500].direction = 1;
	modelData->event[i0+500].relation = 3;
	modelData->event[i0+600].direction = 1;
	modelData->event[i0+600].relation = 2;
	modelData->event[i0+700].direction = -1;
	modelData->event[i0+700].relation = 1;
	}
	modelData->event[800].direction = 1;
	modelData->event[800].relation = 2;
	modelData->event[801].direction = 1;
	modelData->event[801].relation = 2;
	modelData->event[802].direction = 1;
	modelData->event[802].relation = 2;
	modelData->event[803].direction = 1;
	modelData->event[803].relation = 2;
	simulator->time = QSS_Time(100,804,100,0,ST_Binary,NULL);

	simulator->output = SD_Output("inverters_qss",2,404,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[99]++;
	SD_allocOutputMatrix(modelOutput,100,404);
		cleanVector(states,0,100);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"VecInt_2_y_1[1]");
		sprintf(modelOutput->variable[1].name,"VecInt_2_y_1[100]");
		cleanVector(outputs,0,2);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
		modelOutput->SO[99][states[99]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 99;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
