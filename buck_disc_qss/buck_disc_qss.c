#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_QSSIntegrator_1_p[4];
double __PAR_QSSIntegrator_1_x0 = 0;
double __PAR_QSSIntegrator_2_p[4];
double __PAR_QSSIntegrator_2_x0 = 0;
double __PAR_WSum_3_p[9];
double __PAR_WSum_3_w[2];
double __PAR_WSum_11_p[9];
double __PAR_WSum_11_w[2];
double __PAR_qss_switch_12_p[1];
double __PAR_qss_switch_12_level = 0;
double __PAR_qss_switch_13_p[1];
double __PAR_qss_switch_13_level = 0;
double __PAR_Constant_14_p[1];
double __PAR_Constant_14_k = 0;
double __PAR_Constant_15_p[1];
double __PAR_Constant_15_k = 0;
double __PAR_square_sci_16_p[3];
double __PAR_square_sci_16_amplitude = 0;
double __PAR_square_sci_16_freq = 0;
double __PAR_square_sci_16_DC = 0;
double __PAR_hysteretic_18_p[4];
double __PAR_hysteretic_18_xl = 0;
double __PAR_hysteretic_18_xu = 0;
double __PAR_hysteretic_18_yl = 0;
double __PAR_hysteretic_18_yu = 0;
double __PAR_qss_switch_19_p[1];
double __PAR_qss_switch_19_level = 0;
double __PAR_WSum_20_p[9];
double __PAR_WSum_20_w[2];
double __PAR_WSum_21_p[9];
double __PAR_WSum_21_w[2];

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 6;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	switch(i)
	{
		case 0:
			alg[16] = x[0];
			alg[16 + 1] = x[1];
			alg[16 + 2] = x[2]*2;
			alg[20] = x[4];
			alg[20 + 1] = x[5];
			alg[20 + 2] = x[6]*2;
			alg[76] = alg[16]*__PAR_WSum_20_w[0]+alg[20]*__PAR_WSum_20_w[1];
			alg[76 + 1] = alg[17]*__PAR_WSum_20_w[0]+__PAR_WSum_20_w[1]*alg[21];
			alg[76 + 2] = __PAR_WSum_20_w[0]*alg[18]+__PAR_WSum_20_w[1]*alg[22];
			alg[80] = alg[76];
			alg[80 + 1] = alg[77];
			alg[80 + 2] = alg[78];
			dx[1] = alg[80];
			dx[2] = (alg[81])/2;
			dx[3] = (alg[82])/6;
			return;
		case 1:
			alg[0] = __PAR_Constant_14_k;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 100000.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[24] = alg[4];
			alg[24 + 1] = alg[5];
			alg[24 + 2] = alg[6];
			alg[32] = alg[0];
			alg[32 + 1] = alg[1];
			alg[32 + 2] = alg[2];
			alg[36] = alg[0];
			alg[36 + 1] = alg[1];
			alg[36 + 2] = alg[2];
			alg[44] = alg[4];
			alg[44 + 1] = alg[5];
			alg[44 + 2] = alg[6];
			alg[48] = alg[36]*d[(0)]+alg[44]*(1.0-d[(0)]);
			alg[48 + 1] = -alg[45]*(-1.0+d[(0)])+alg[37]*d[(0)];
			alg[48 + 2] = d[(0)]*alg[38]-(-1.0+d[(0)])*alg[46];
			alg[52] = alg[24]*d[(1)]+alg[32]*(1.0-d[(1)]);
			alg[52 + 1] = -(-1.0+d[(1)])*alg[33]+alg[25]*d[(1)];
			alg[52 + 2] = alg[26]*d[(1)]-alg[34]*(-1.0+d[(1)]);
			alg[64] = alg[52];
			alg[64 + 1] = alg[53];
			alg[64 + 2] = alg[54];
			alg[68] = alg[48];
			alg[68 + 1] = alg[49];
			alg[68 + 2] = alg[50];
			alg[72] = alg[64]*__PAR_WSum_3_w[0]+alg[68]*__PAR_WSum_3_w[1];
			alg[72 + 1] = __PAR_WSum_3_w[1]*alg[69]+alg[65]*__PAR_WSum_3_w[0];
			alg[72 + 2] = __PAR_WSum_3_w[0]*alg[66]+alg[70]*__PAR_WSum_3_w[1];
			alg[84] = alg[72];
			alg[84 + 1] = alg[73];
			alg[84 + 2] = alg[74];
			alg[88] = 1.0/alg[84];
			alg[88 + 1] = -alg[85]*(pow(alg[84],-2.0));
			alg[88 + 2] = -(pow(alg[84],-2.0))*alg[86]+2.0*(pow(alg[84],-3.0))*(pow(alg[85],2.0));
			alg[92] = alg[88];
			alg[92 + 1] = alg[89];
			alg[92 + 2] = alg[90];
			alg[96] = alg[48];
			alg[96 + 1] = alg[49];
			alg[96 + 2] = alg[50];
			alg[100] = alg[52];
			alg[100 + 1] = alg[53];
			alg[100 + 2] = alg[54];
			alg[104] = alg[88];
			alg[104 + 1] = alg[89];
			alg[104 + 2] = alg[90];
			alg[108] = alg[92]*alg[96];
			alg[108 + 1] = alg[93]*alg[96]+alg[97]*alg[92];
			alg[108 + 2] = 2.0*alg[97]*alg[93]+alg[94]*alg[96]+alg[92]*alg[98];
			alg[112] = alg[108];
			alg[112 + 1] = alg[109];
			alg[112 + 2] = alg[110];
			alg[116] = alg[52];
			alg[116 + 1] = alg[53];
			alg[116 + 2] = alg[54];
			alg[120] = alg[112]*alg[116];
			alg[120 + 1] = alg[116]*alg[113]+alg[112]*alg[117];
			alg[120 + 2] = alg[116]*alg[114]+alg[112]*alg[118]+2.0*alg[113]*alg[117];
			alg[124] = alg[120];
			alg[124 + 1] = alg[121];
			alg[124 + 2] = alg[122];
			alg[128] = x[4];
			alg[128 + 1] = x[5];
			alg[128 + 2] = x[6]*2;
			alg[132] = alg[124]*alg[128];
			alg[132 + 1] = alg[125]*alg[128]+alg[124]*alg[129];
			alg[132 + 2] = 2.0*alg[129]*alg[125]+alg[126]*alg[128]+alg[130]*alg[124];
			alg[136] = alg[100]*alg[104];
			alg[136 + 1] = alg[105]*alg[100]+alg[104]*alg[101];
			alg[136 + 2] = 2.0*alg[105]*alg[101]+alg[104]*alg[102]+alg[106]*alg[100];
			alg[140] = alg[136];
			alg[140 + 1] = alg[137];
			alg[140 + 2] = alg[138];
			alg[144] = alg[132];
			alg[144 + 1] = alg[133];
			alg[144 + 2] = alg[134];
			alg[148] = alg[140]*__PAR_WSum_11_w[0]+alg[144]*__PAR_WSum_11_w[1];
			alg[148 + 1] = alg[145]*__PAR_WSum_11_w[1]+alg[141]*__PAR_WSum_11_w[0];
			alg[148 + 2] = alg[142]*__PAR_WSum_11_w[0]+__PAR_WSum_11_w[1]*alg[146];
			alg[152] = alg[148];
			alg[152 + 1] = alg[149];
			alg[152 + 2] = alg[150];
			alg[156] = x[0];
			alg[156 + 1] = x[1];
			alg[156 + 2] = x[2]*2;
			alg[184] = alg[152]*__PAR_WSum_21_w[0]+alg[156]*__PAR_WSum_21_w[1];
			alg[184 + 1] = __PAR_WSum_21_w[0]*alg[153]+__PAR_WSum_21_w[1]*alg[157];
			alg[184 + 2] = alg[158]*__PAR_WSum_21_w[1]+__PAR_WSum_21_w[0]*alg[154];
			alg[188] = alg[184];
			alg[188 + 1] = alg[185];
			alg[188 + 2] = alg[186];
			dx[1] = alg[188];
			dx[2] = (alg[189])/2;
			dx[3] = (alg[190])/6;
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			alg[0] = __PAR_Constant_14_k;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 100000.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[16] = x[0];
			alg[16 + 1] = x[1];
			alg[16 + 2] = x[2]*2;
			alg[20] = x[4];
			alg[20 + 1] = x[5];
			alg[20 + 2] = x[6]*2;
			alg[24] = alg[4];
			alg[24 + 1] = alg[5];
			alg[24 + 2] = alg[6];
			alg[32] = alg[0];
			alg[32 + 1] = alg[1];
			alg[32 + 2] = alg[2];
			alg[36] = alg[0];
			alg[36 + 1] = alg[1];
			alg[36 + 2] = alg[2];
			alg[44] = alg[4];
			alg[44 + 1] = alg[5];
			alg[44 + 2] = alg[6];
			alg[48] = alg[36]*d[(0)]+alg[44]*(1.0-d[(0)]);
			alg[48 + 1] = -alg[45]*(-1.0+d[(0)])+alg[37]*d[(0)];
			alg[48 + 2] = d[(0)]*alg[38]-(-1.0+d[(0)])*alg[46];
			alg[52] = alg[24]*d[(1)]+alg[32]*(1.0-d[(1)]);
			alg[52 + 1] = -(-1.0+d[(1)])*alg[33]+alg[25]*d[(1)];
			alg[52 + 2] = alg[26]*d[(1)]-alg[34]*(-1.0+d[(1)]);
			alg[64] = alg[52];
			alg[64 + 1] = alg[53];
			alg[64 + 2] = alg[54];
			alg[68] = alg[48];
			alg[68 + 1] = alg[49];
			alg[68 + 2] = alg[50];
			alg[72] = alg[64]*__PAR_WSum_3_w[0]+alg[68]*__PAR_WSum_3_w[1];
			alg[72 + 1] = __PAR_WSum_3_w[1]*alg[69]+alg[65]*__PAR_WSum_3_w[0];
			alg[72 + 2] = __PAR_WSum_3_w[0]*alg[66]+alg[70]*__PAR_WSum_3_w[1];
			alg[76] = alg[16]*__PAR_WSum_20_w[0]+alg[20]*__PAR_WSum_20_w[1];
			alg[76 + 1] = alg[17]*__PAR_WSum_20_w[0]+__PAR_WSum_20_w[1]*alg[21];
			alg[76 + 2] = __PAR_WSum_20_w[0]*alg[18]+__PAR_WSum_20_w[1]*alg[22];
			alg[80] = alg[76];
			alg[80 + 1] = alg[77];
			alg[80 + 2] = alg[78];
			alg[84] = alg[72];
			alg[84 + 1] = alg[73];
			alg[84 + 2] = alg[74];
			alg[88] = 1.0/alg[84];
			alg[88 + 1] = -alg[85]*(pow(alg[84],-2.0));
			alg[88 + 2] = -(pow(alg[84],-2.0))*alg[86]+2.0*(pow(alg[84],-3.0))*(pow(alg[85],2.0));
			alg[92] = alg[88];
			alg[92 + 1] = alg[89];
			alg[92 + 2] = alg[90];
			alg[96] = alg[48];
			alg[96 + 1] = alg[49];
			alg[96 + 2] = alg[50];
			alg[100] = alg[52];
			alg[100 + 1] = alg[53];
			alg[100 + 2] = alg[54];
			alg[104] = alg[88];
			alg[104 + 1] = alg[89];
			alg[104 + 2] = alg[90];
			alg[108] = alg[92]*alg[96];
			alg[108 + 1] = alg[93]*alg[96]+alg[97]*alg[92];
			alg[108 + 2] = 2.0*alg[97]*alg[93]+alg[94]*alg[96]+alg[92]*alg[98];
			alg[112] = alg[108];
			alg[112 + 1] = alg[109];
			alg[112 + 2] = alg[110];
			alg[116] = alg[52];
			alg[116 + 1] = alg[53];
			alg[116 + 2] = alg[54];
			alg[120] = alg[112]*alg[116];
			alg[120 + 1] = alg[116]*alg[113]+alg[112]*alg[117];
			alg[120 + 2] = alg[116]*alg[114]+alg[112]*alg[118]+2.0*alg[113]*alg[117];
			alg[124] = alg[120];
			alg[124 + 1] = alg[121];
			alg[124 + 2] = alg[122];
			alg[128] = x[4];
			alg[128 + 1] = x[5];
			alg[128 + 2] = x[6]*2;
			alg[132] = alg[124]*alg[128];
			alg[132 + 1] = alg[125]*alg[128]+alg[124]*alg[129];
			alg[132 + 2] = 2.0*alg[129]*alg[125]+alg[126]*alg[128]+alg[130]*alg[124];
			alg[136] = alg[100]*alg[104];
			alg[136 + 1] = alg[105]*alg[100]+alg[104]*alg[101];
			alg[136 + 2] = 2.0*alg[105]*alg[101]+alg[104]*alg[102]+alg[106]*alg[100];
			alg[140] = alg[136];
			alg[140 + 1] = alg[137];
			alg[140 + 2] = alg[138];
			alg[144] = alg[132];
			alg[144 + 1] = alg[133];
			alg[144 + 2] = alg[134];
			alg[148] = alg[140]*__PAR_WSum_11_w[0]+alg[144]*__PAR_WSum_11_w[1];
			alg[148 + 1] = alg[145]*__PAR_WSum_11_w[1]+alg[141]*__PAR_WSum_11_w[0];
			alg[148 + 2] = alg[142]*__PAR_WSum_11_w[0]+__PAR_WSum_11_w[1]*alg[146];
			alg[152] = alg[148];
			alg[152 + 1] = alg[149];
			alg[152 + 2] = alg[150];
			alg[156] = x[0];
			alg[156 + 1] = x[1];
			alg[156 + 2] = x[2]*2;
			alg[184] = alg[152]*__PAR_WSum_21_w[0]+alg[156]*__PAR_WSum_21_w[1];
			alg[184 + 1] = __PAR_WSum_21_w[0]*alg[153]+__PAR_WSum_21_w[1]*alg[157];
			alg[184 + 2] = alg[158]*__PAR_WSum_21_w[1]+__PAR_WSum_21_w[0]*alg[154];
			alg[188] = alg[184];
			alg[188 + 1] = alg[185];
			alg[188 + 2] = alg[186];
			der[0 + 1] = alg[80];
			der[0 + 2] = (alg[81])/2;
			der[0 + 3] = (alg[82])/6;
			der[4 + 1] = alg[188];
			der[4 + 2] = (alg[189])/2;
			der[4 + 3] = (alg[190])/6;
			return;
		case 1:
			alg[0] = __PAR_Constant_14_k;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 100000.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[16] = x[0];
			alg[16 + 1] = x[1];
			alg[16 + 2] = x[2]*2;
			alg[20] = x[4];
			alg[20 + 1] = x[5];
			alg[20 + 2] = x[6]*2;
			alg[24] = alg[4];
			alg[24 + 1] = alg[5];
			alg[24 + 2] = alg[6];
			alg[32] = alg[0];
			alg[32 + 1] = alg[1];
			alg[32 + 2] = alg[2];
			alg[36] = alg[0];
			alg[36 + 1] = alg[1];
			alg[36 + 2] = alg[2];
			alg[44] = alg[4];
			alg[44 + 1] = alg[5];
			alg[44 + 2] = alg[6];
			alg[48] = alg[36]*d[(0)]+alg[44]*(1.0-d[(0)]);
			alg[48 + 1] = -alg[45]*(-1.0+d[(0)])+alg[37]*d[(0)];
			alg[48 + 2] = d[(0)]*alg[38]-(-1.0+d[(0)])*alg[46];
			alg[52] = alg[24]*d[(1)]+alg[32]*(1.0-d[(1)]);
			alg[52 + 1] = -(-1.0+d[(1)])*alg[33]+alg[25]*d[(1)];
			alg[52 + 2] = alg[26]*d[(1)]-alg[34]*(-1.0+d[(1)]);
			alg[64] = alg[52];
			alg[64 + 1] = alg[53];
			alg[64 + 2] = alg[54];
			alg[68] = alg[48];
			alg[68 + 1] = alg[49];
			alg[68 + 2] = alg[50];
			alg[72] = alg[64]*__PAR_WSum_3_w[0]+alg[68]*__PAR_WSum_3_w[1];
			alg[72 + 1] = __PAR_WSum_3_w[1]*alg[69]+alg[65]*__PAR_WSum_3_w[0];
			alg[72 + 2] = __PAR_WSum_3_w[0]*alg[66]+alg[70]*__PAR_WSum_3_w[1];
			alg[76] = alg[16]*__PAR_WSum_20_w[0]+alg[20]*__PAR_WSum_20_w[1];
			alg[76 + 1] = alg[17]*__PAR_WSum_20_w[0]+__PAR_WSum_20_w[1]*alg[21];
			alg[76 + 2] = __PAR_WSum_20_w[0]*alg[18]+__PAR_WSum_20_w[1]*alg[22];
			alg[80] = alg[76];
			alg[80 + 1] = alg[77];
			alg[80 + 2] = alg[78];
			alg[84] = alg[72];
			alg[84 + 1] = alg[73];
			alg[84 + 2] = alg[74];
			alg[88] = 1.0/alg[84];
			alg[88 + 1] = -alg[85]*(pow(alg[84],-2.0));
			alg[88 + 2] = -(pow(alg[84],-2.0))*alg[86]+2.0*(pow(alg[84],-3.0))*(pow(alg[85],2.0));
			alg[92] = alg[88];
			alg[92 + 1] = alg[89];
			alg[92 + 2] = alg[90];
			alg[96] = alg[48];
			alg[96 + 1] = alg[49];
			alg[96 + 2] = alg[50];
			alg[100] = alg[52];
			alg[100 + 1] = alg[53];
			alg[100 + 2] = alg[54];
			alg[104] = alg[88];
			alg[104 + 1] = alg[89];
			alg[104 + 2] = alg[90];
			alg[108] = alg[92]*alg[96];
			alg[108 + 1] = alg[93]*alg[96]+alg[97]*alg[92];
			alg[108 + 2] = 2.0*alg[97]*alg[93]+alg[94]*alg[96]+alg[92]*alg[98];
			alg[112] = alg[108];
			alg[112 + 1] = alg[109];
			alg[112 + 2] = alg[110];
			alg[116] = alg[52];
			alg[116 + 1] = alg[53];
			alg[116 + 2] = alg[54];
			alg[120] = alg[112]*alg[116];
			alg[120 + 1] = alg[116]*alg[113]+alg[112]*alg[117];
			alg[120 + 2] = alg[116]*alg[114]+alg[112]*alg[118]+2.0*alg[113]*alg[117];
			alg[124] = alg[120];
			alg[124 + 1] = alg[121];
			alg[124 + 2] = alg[122];
			alg[128] = x[4];
			alg[128 + 1] = x[5];
			alg[128 + 2] = x[6]*2;
			alg[132] = alg[124]*alg[128];
			alg[132 + 1] = alg[125]*alg[128]+alg[124]*alg[129];
			alg[132 + 2] = 2.0*alg[129]*alg[125]+alg[126]*alg[128]+alg[130]*alg[124];
			alg[136] = alg[100]*alg[104];
			alg[136 + 1] = alg[105]*alg[100]+alg[104]*alg[101];
			alg[136 + 2] = 2.0*alg[105]*alg[101]+alg[104]*alg[102]+alg[106]*alg[100];
			alg[140] = alg[136];
			alg[140 + 1] = alg[137];
			alg[140 + 2] = alg[138];
			alg[144] = alg[132];
			alg[144 + 1] = alg[133];
			alg[144 + 2] = alg[134];
			alg[148] = alg[140]*__PAR_WSum_11_w[0]+alg[144]*__PAR_WSum_11_w[1];
			alg[148 + 1] = alg[145]*__PAR_WSum_11_w[1]+alg[141]*__PAR_WSum_11_w[0];
			alg[148 + 2] = alg[142]*__PAR_WSum_11_w[0]+__PAR_WSum_11_w[1]*alg[146];
			alg[152] = alg[148];
			alg[152 + 1] = alg[149];
			alg[152 + 2] = alg[150];
			alg[156] = x[0];
			alg[156 + 1] = x[1];
			alg[156 + 2] = x[2]*2;
			alg[184] = alg[152]*__PAR_WSum_21_w[0]+alg[156]*__PAR_WSum_21_w[1];
			alg[184 + 1] = __PAR_WSum_21_w[0]*alg[153]+__PAR_WSum_21_w[1]*alg[157];
			alg[184 + 2] = alg[158]*__PAR_WSum_21_w[1]+__PAR_WSum_21_w[0]*alg[154];
			alg[188] = alg[184];
			alg[188 + 1] = alg[185];
			alg[188 + 2] = alg[186];
			der[0 + 1] = alg[80];
			der[0 + 2] = (alg[81])/2;
			der[0 + 3] = (alg[82])/6;
			der[4 + 1] = alg[188];
			der[4 + 2] = (alg[189])/2;
			der[4 + 3] = (alg[190])/6;
			return;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			alg[8] = d[(2)];
			alg[8 + 1] = 0.0;
			alg[8 + 2] = 0.0;
			alg[40] = alg[8];
			alg[40 + 1] = alg[9];
			alg[40 + 2] = alg[10];
			zc[0] = alg[40]-(__PAR_qss_switch_12_level);
			zc[1] = alg[41];
			zc[2] = (alg[42])/2;
			return;
		case 1:
			alg[12] = d[(4)];
			alg[12 + 1] = 0.0;
			alg[12 + 2] = 0.0;
			alg[28] = alg[12];
			alg[28 + 1] = alg[13];
			alg[28 + 2] = alg[14];
			zc[0] = alg[28]-(0.0);
			zc[1] = alg[29];
			zc[2] = (alg[30])/2;
			return;
		case 2:
			zc[0] = t-(d[(3)]);
			zc[1] = 1.0;
			zc[2] = (0.0)/2;
			return;
		case 3:
			alg[0] = __PAR_Constant_14_k;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 100000.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[24] = alg[4];
			alg[24 + 1] = alg[5];
			alg[24 + 2] = alg[6];
			alg[32] = alg[0];
			alg[32 + 1] = alg[1];
			alg[32 + 2] = alg[2];
			alg[36] = alg[0];
			alg[36 + 1] = alg[1];
			alg[36 + 2] = alg[2];
			alg[44] = alg[4];
			alg[44 + 1] = alg[5];
			alg[44 + 2] = alg[6];
			alg[48] = alg[36]*d[(0)]+alg[44]*(1.0-d[(0)]);
			alg[48 + 1] = -alg[45]*(-1.0+d[(0)])+alg[37]*d[(0)];
			alg[48 + 2] = d[(0)]*alg[38]-(-1.0+d[(0)])*alg[46];
			alg[52] = alg[24]*d[(1)]+alg[32]*(1.0-d[(1)]);
			alg[52 + 1] = -(-1.0+d[(1)])*alg[33]+alg[25]*d[(1)];
			alg[52 + 2] = alg[26]*d[(1)]-alg[34]*(-1.0+d[(1)]);
			alg[56] = alg[52];
			alg[56 + 1] = alg[53];
			alg[56 + 2] = alg[54];
			alg[60] = 1.0/alg[56];
			alg[60 + 1] = -(pow(alg[56],-2.0))*alg[57];
			alg[60 + 2] = 2.0*(pow(alg[56],-3.0))*(pow(alg[57],2.0))-(pow(alg[56],-2.0))*alg[58];
			alg[64] = alg[52];
			alg[64 + 1] = alg[53];
			alg[64 + 2] = alg[54];
			alg[68] = alg[48];
			alg[68 + 1] = alg[49];
			alg[68 + 2] = alg[50];
			alg[72] = alg[64]*__PAR_WSum_3_w[0]+alg[68]*__PAR_WSum_3_w[1];
			alg[72 + 1] = __PAR_WSum_3_w[1]*alg[69]+alg[65]*__PAR_WSum_3_w[0];
			alg[72 + 2] = __PAR_WSum_3_w[0]*alg[66]+alg[70]*__PAR_WSum_3_w[1];
			alg[84] = alg[72];
			alg[84 + 1] = alg[73];
			alg[84 + 2] = alg[74];
			alg[88] = 1.0/alg[84];
			alg[88 + 1] = -alg[85]*(pow(alg[84],-2.0));
			alg[88 + 2] = -(pow(alg[84],-2.0))*alg[86]+2.0*(pow(alg[84],-3.0))*(pow(alg[85],2.0));
			alg[92] = alg[88];
			alg[92 + 1] = alg[89];
			alg[92 + 2] = alg[90];
			alg[96] = alg[48];
			alg[96 + 1] = alg[49];
			alg[96 + 2] = alg[50];
			alg[100] = alg[52];
			alg[100 + 1] = alg[53];
			alg[100 + 2] = alg[54];
			alg[104] = alg[88];
			alg[104 + 1] = alg[89];
			alg[104 + 2] = alg[90];
			alg[108] = alg[92]*alg[96];
			alg[108 + 1] = alg[93]*alg[96]+alg[97]*alg[92];
			alg[108 + 2] = 2.0*alg[97]*alg[93]+alg[94]*alg[96]+alg[92]*alg[98];
			alg[112] = alg[108];
			alg[112 + 1] = alg[109];
			alg[112 + 2] = alg[110];
			alg[116] = alg[52];
			alg[116 + 1] = alg[53];
			alg[116 + 2] = alg[54];
			alg[120] = alg[112]*alg[116];
			alg[120 + 1] = alg[116]*alg[113]+alg[112]*alg[117];
			alg[120 + 2] = alg[116]*alg[114]+alg[112]*alg[118]+2.0*alg[113]*alg[117];
			alg[124] = alg[120];
			alg[124 + 1] = alg[121];
			alg[124 + 2] = alg[122];
			alg[128] = x[4];
			alg[128 + 1] = x[5];
			alg[128 + 2] = x[6]*2;
			alg[132] = alg[124]*alg[128];
			alg[132 + 1] = alg[125]*alg[128]+alg[124]*alg[129];
			alg[132 + 2] = 2.0*alg[129]*alg[125]+alg[126]*alg[128]+alg[130]*alg[124];
			alg[136] = alg[100]*alg[104];
			alg[136 + 1] = alg[105]*alg[100]+alg[104]*alg[101];
			alg[136 + 2] = 2.0*alg[105]*alg[101]+alg[104]*alg[102]+alg[106]*alg[100];
			alg[140] = alg[136];
			alg[140 + 1] = alg[137];
			alg[140 + 2] = alg[138];
			alg[144] = alg[132];
			alg[144 + 1] = alg[133];
			alg[144 + 2] = alg[134];
			alg[148] = alg[140]*__PAR_WSum_11_w[0]+alg[144]*__PAR_WSum_11_w[1];
			alg[148 + 1] = alg[145]*__PAR_WSum_11_w[1]+alg[141]*__PAR_WSum_11_w[0];
			alg[148 + 2] = alg[142]*__PAR_WSum_11_w[0]+__PAR_WSum_11_w[1]*alg[146];
			alg[160] = alg[60];
			alg[160 + 1] = alg[61];
			alg[160 + 2] = alg[62];
			alg[164] = alg[148];
			alg[164 + 1] = alg[149];
			alg[164 + 2] = alg[150];
			alg[168] = alg[160]*alg[164];
			alg[168 + 1] = alg[160]*alg[165]+alg[161]*alg[164];
			alg[168 + 2] = alg[166]*alg[160]+2.0*alg[165]*alg[161]+alg[164]*alg[162];
			alg[172] = alg[148];
			alg[172 + 1] = alg[149];
			alg[172 + 2] = alg[150];
			alg[180] = alg[168];
			alg[180 + 1] = alg[169];
			alg[180 + 2] = alg[170];
			alg[192] = alg[172]*d[(5)]+alg[180]*(1.0-d[(5)]);
			alg[192 + 1] = -alg[181]*(-1.0+d[(5)])+d[(5)]*alg[173];
			alg[192 + 2] = d[(5)]*alg[174]-alg[182]*(-1.0+d[(5)]);
			alg[196] = alg[192];
			alg[196 + 1] = alg[193];
			alg[196 + 2] = alg[194];
			zc[0] = alg[196]-(__PAR_hysteretic_18_xu);
			zc[1] = alg[197];
			zc[2] = (alg[198])/2;
			return;
		case 4:
			alg[0] = __PAR_Constant_14_k;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 100000.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[24] = alg[4];
			alg[24 + 1] = alg[5];
			alg[24 + 2] = alg[6];
			alg[32] = alg[0];
			alg[32 + 1] = alg[1];
			alg[32 + 2] = alg[2];
			alg[36] = alg[0];
			alg[36 + 1] = alg[1];
			alg[36 + 2] = alg[2];
			alg[44] = alg[4];
			alg[44 + 1] = alg[5];
			alg[44 + 2] = alg[6];
			alg[48] = alg[36]*d[(0)]+alg[44]*(1.0-d[(0)]);
			alg[48 + 1] = -alg[45]*(-1.0+d[(0)])+alg[37]*d[(0)];
			alg[48 + 2] = d[(0)]*alg[38]-(-1.0+d[(0)])*alg[46];
			alg[52] = alg[24]*d[(1)]+alg[32]*(1.0-d[(1)]);
			alg[52 + 1] = -(-1.0+d[(1)])*alg[33]+alg[25]*d[(1)];
			alg[52 + 2] = alg[26]*d[(1)]-alg[34]*(-1.0+d[(1)]);
			alg[56] = alg[52];
			alg[56 + 1] = alg[53];
			alg[56 + 2] = alg[54];
			alg[60] = 1.0/alg[56];
			alg[60 + 1] = -(pow(alg[56],-2.0))*alg[57];
			alg[60 + 2] = 2.0*(pow(alg[56],-3.0))*(pow(alg[57],2.0))-(pow(alg[56],-2.0))*alg[58];
			alg[64] = alg[52];
			alg[64 + 1] = alg[53];
			alg[64 + 2] = alg[54];
			alg[68] = alg[48];
			alg[68 + 1] = alg[49];
			alg[68 + 2] = alg[50];
			alg[72] = alg[64]*__PAR_WSum_3_w[0]+alg[68]*__PAR_WSum_3_w[1];
			alg[72 + 1] = __PAR_WSum_3_w[1]*alg[69]+alg[65]*__PAR_WSum_3_w[0];
			alg[72 + 2] = __PAR_WSum_3_w[0]*alg[66]+alg[70]*__PAR_WSum_3_w[1];
			alg[84] = alg[72];
			alg[84 + 1] = alg[73];
			alg[84 + 2] = alg[74];
			alg[88] = 1.0/alg[84];
			alg[88 + 1] = -alg[85]*(pow(alg[84],-2.0));
			alg[88 + 2] = -(pow(alg[84],-2.0))*alg[86]+2.0*(pow(alg[84],-3.0))*(pow(alg[85],2.0));
			alg[92] = alg[88];
			alg[92 + 1] = alg[89];
			alg[92 + 2] = alg[90];
			alg[96] = alg[48];
			alg[96 + 1] = alg[49];
			alg[96 + 2] = alg[50];
			alg[100] = alg[52];
			alg[100 + 1] = alg[53];
			alg[100 + 2] = alg[54];
			alg[104] = alg[88];
			alg[104 + 1] = alg[89];
			alg[104 + 2] = alg[90];
			alg[108] = alg[92]*alg[96];
			alg[108 + 1] = alg[93]*alg[96]+alg[97]*alg[92];
			alg[108 + 2] = 2.0*alg[97]*alg[93]+alg[94]*alg[96]+alg[92]*alg[98];
			alg[112] = alg[108];
			alg[112 + 1] = alg[109];
			alg[112 + 2] = alg[110];
			alg[116] = alg[52];
			alg[116 + 1] = alg[53];
			alg[116 + 2] = alg[54];
			alg[120] = alg[112]*alg[116];
			alg[120 + 1] = alg[116]*alg[113]+alg[112]*alg[117];
			alg[120 + 2] = alg[116]*alg[114]+alg[112]*alg[118]+2.0*alg[113]*alg[117];
			alg[124] = alg[120];
			alg[124 + 1] = alg[121];
			alg[124 + 2] = alg[122];
			alg[128] = x[4];
			alg[128 + 1] = x[5];
			alg[128 + 2] = x[6]*2;
			alg[132] = alg[124]*alg[128];
			alg[132 + 1] = alg[125]*alg[128]+alg[124]*alg[129];
			alg[132 + 2] = 2.0*alg[129]*alg[125]+alg[126]*alg[128]+alg[130]*alg[124];
			alg[136] = alg[100]*alg[104];
			alg[136 + 1] = alg[105]*alg[100]+alg[104]*alg[101];
			alg[136 + 2] = 2.0*alg[105]*alg[101]+alg[104]*alg[102]+alg[106]*alg[100];
			alg[140] = alg[136];
			alg[140 + 1] = alg[137];
			alg[140 + 2] = alg[138];
			alg[144] = alg[132];
			alg[144 + 1] = alg[133];
			alg[144 + 2] = alg[134];
			alg[148] = alg[140]*__PAR_WSum_11_w[0]+alg[144]*__PAR_WSum_11_w[1];
			alg[148 + 1] = alg[145]*__PAR_WSum_11_w[1]+alg[141]*__PAR_WSum_11_w[0];
			alg[148 + 2] = alg[142]*__PAR_WSum_11_w[0]+__PAR_WSum_11_w[1]*alg[146];
			alg[160] = alg[60];
			alg[160 + 1] = alg[61];
			alg[160 + 2] = alg[62];
			alg[164] = alg[148];
			alg[164 + 1] = alg[149];
			alg[164 + 2] = alg[150];
			alg[168] = alg[160]*alg[164];
			alg[168 + 1] = alg[160]*alg[165]+alg[161]*alg[164];
			alg[168 + 2] = alg[166]*alg[160]+2.0*alg[165]*alg[161]+alg[164]*alg[162];
			alg[172] = alg[148];
			alg[172 + 1] = alg[149];
			alg[172 + 2] = alg[150];
			alg[180] = alg[168];
			alg[180 + 1] = alg[169];
			alg[180 + 2] = alg[170];
			alg[192] = alg[172]*d[(5)]+alg[180]*(1.0-d[(5)]);
			alg[192 + 1] = -alg[181]*(-1.0+d[(5)])+d[(5)]*alg[173];
			alg[192 + 2] = d[(5)]*alg[174]-alg[182]*(-1.0+d[(5)]);
			alg[196] = alg[192];
			alg[196 + 1] = alg[193];
			alg[196 + 2] = alg[194];
			zc[0] = alg[196]-(__PAR_hysteretic_18_xl);
			zc[1] = alg[197];
			zc[2] = (alg[198])/2;
			return;
		case 5:
			alg[12] = d[(4)];
			alg[12 + 1] = 0.0;
			alg[12 + 2] = 0.0;
			alg[176] = alg[12];
			alg[176 + 1] = alg[13];
			alg[176 + 2] = alg[14];
			zc[0] = alg[176]-(0.0);
			zc[1] = alg[177];
			zc[2] = (alg[178])/2;
			return;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			d[(0)] = 1.0;
			return;
		case 1:
			d[(1)] = 1.0;
			return;
		case 2:
			d[(2)] = 1.0-d[(2)];
			d[(3)] = t+d[(2)]*__PAR_square_sci_16_DC/10000.0+(1.0-d[(2)])*(1.0-__PAR_square_sci_16_DC)/10000.0;
			return;
		case 3:
			d[(4)] = 1.0;
			return;
		case 5:
			d[(5)] = 1.0;
			return;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			d[(0)] = 0.0;
			return;
		case 1:
			d[(1)] = 0.0;
			return;
		case 4:
			d[(4)] = __PAR_hysteretic_18_yl;
			return;
		case 5:
			d[(5)] = 0.0;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[4];
			return;
		case 1:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(6*sizeof(int));
	int *events = (int*)malloc(6*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0;
	simulator->data = QSS_Data(2,6,6,0,50,"buck_disc_qss");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_QSSIntegrator_1_x0 = 0.0;
	__PAR_QSSIntegrator_2_x0 = 0.0;
	__PAR_qss_switch_12_level = 5.000000000000000000000000e-01;
	__PAR_qss_switch_13_level = 0.0;
	__PAR_Constant_14_k = 1.000000000000000081803054e-05;
	__PAR_Constant_15_k = 100000.0;
	__PAR_square_sci_16_amplitude = 1.0;
	__PAR_square_sci_16_freq = 10000.0;
	__PAR_square_sci_16_DC = 50.0/100.0;
	__PAR_hysteretic_18_xl = ((((-9.999999999999999547481118e-07))));
	__PAR_hysteretic_18_xu = 9.999999999999999547481118e-07;
	__PAR_hysteretic_18_yl = ((((-1.0))));
	__PAR_hysteretic_18_yu = 1.0;
	__PAR_qss_switch_19_level = 0.0;
	modelData->x[4] = 0.0;
	modelData->x[0] = 0.0;
	modelData->d[(2)] = 1.0;
	modelData->d[(3)] = 0.0;
	// Initialize model code.
	if(modelData->alg[40]>__PAR_qss_switch_12_level)
	{
		modelData->d[(0)] = 1.0;
	}
	else if(modelData->alg[40]<__PAR_qss_switch_12_level)
	{
		modelData->d[(0)] = 0.0;
	}
	if(modelData->alg[28]>0.0)
	{
		modelData->d[(1)] = 1.0;
	}
	else if(modelData->alg[28]<0.0)
	{
		modelData->d[(1)] = 0.0;
	}
		modelData->d[(3)] = __PAR_square_sci_16_DC/10000.0;
	if(modelData->alg[196]>__PAR_hysteretic_18_xu)
	{
		modelData->d[(4)] = 1.0;
	}
	if(modelData->alg[196]<__PAR_hysteretic_18_xl)
	{
		modelData->d[(4)] = __PAR_hysteretic_18_yl;
	}
	if(modelData->alg[176]>0.0)
	{
		modelData->d[(5)] = 1.0;
	}
	else if(modelData->alg[176]<0.0)
	{
		modelData->d[(5)] = 0.0;
	}
		__PAR_QSSIntegrator_1_p[(0)] = 0.0;
		__PAR_QSSIntegrator_1_p[(1)] = 1.000000000000000055511151e-01;
		__PAR_QSSIntegrator_1_p[(2)] = 1.000000000000000020816682e-03;
		__PAR_QSSIntegrator_1_p[(3)] = 0.0;
		__PAR_QSSIntegrator_2_p[(0)] = 0.0;
		__PAR_QSSIntegrator_2_p[(1)] = 1.000000000000000020816682e-03;
		__PAR_QSSIntegrator_2_p[(2)] = 1.000000000000000020816682e-03;
		__PAR_QSSIntegrator_2_p[(3)] = 0.0;
		__PAR_WSum_3_p[(0)] = 1.0;
		__PAR_WSum_3_p[(1)] = 1.0;
		__PAR_WSum_3_p[(2)] = 0.0;
		__PAR_WSum_3_p[(3)] = 0.0;
		__PAR_WSum_3_p[(4)] = 0.0;
		__PAR_WSum_3_p[(5)] = 0.0;
		__PAR_WSum_3_p[(6)] = 0.0;
		__PAR_WSum_3_p[(7)] = 0.0;
		__PAR_WSum_3_p[(8)] = 2.0;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_WSum_3_w[(i0)] = __PAR_WSum_3_p[(i0)];
	}
		__PAR_WSum_11_p[(0)] = 24.0;
		__PAR_WSum_11_p[(1)] = ((((-1.0))));
		__PAR_WSum_11_p[(2)] = 0.0;
		__PAR_WSum_11_p[(3)] = 0.0;
		__PAR_WSum_11_p[(4)] = 0.0;
		__PAR_WSum_11_p[(5)] = 0.0;
		__PAR_WSum_11_p[(6)] = 0.0;
		__PAR_WSum_11_p[(7)] = 0.0;
		__PAR_WSum_11_p[(8)] = 2.0;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_WSum_11_w[(i0)] = __PAR_WSum_11_p[(i0)];
	}
		__PAR_qss_switch_12_p[(0)] = 5.000000000000000000000000e-01;
		__PAR_qss_switch_13_p[(0)] = 0.0;
		__PAR_Constant_14_p[(0)] = 1.000000000000000081803054e-05;
		__PAR_Constant_15_p[(0)] = 100000.0;
		__PAR_square_sci_16_p[(0)] = 1.0;
		__PAR_square_sci_16_p[(1)] = 10000.0;
		__PAR_square_sci_16_p[(2)] = 50.0;
		__PAR_hysteretic_18_p[(0)] = ((((-9.999999999999999547481118e-07))));
		__PAR_hysteretic_18_p[(1)] = 9.999999999999999547481118e-07;
		__PAR_hysteretic_18_p[(2)] = ((((-1.0))));
		__PAR_hysteretic_18_p[(3)] = 1.0;
		__PAR_qss_switch_19_p[(0)] = 0.0;
		__PAR_WSum_20_p[(0)] = ((((-1000.0))));
		__PAR_WSum_20_p[(1)] = 10000.0;
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
		__PAR_WSum_21_p[(0)] = 10000.0;
		__PAR_WSum_21_p[(1)] = ((((-10000.0))));
		__PAR_WSum_21_p[(2)] = 0.0;
		__PAR_WSum_21_p[(3)] = 0.0;
		__PAR_WSum_21_p[(4)] = 0.0;
		__PAR_WSum_21_p[(5)] = 0.0;
		__PAR_WSum_21_p[(6)] = 0.0;
		__PAR_WSum_21_p[(7)] = 0.0;
		__PAR_WSum_21_p[(8)] = 2.0;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_WSum_21_w[(i0)] = __PAR_WSum_21_p[(i0)];
	}
		modelData->d[(5)] = 1.0;
		modelData->d[(4)] = 1.0;
		modelData->d[(2)] = 1.0;
		modelData->d[(1)] = 1.0;
		modelData->d[(0)] = 1.0;
	modelData->nDS[0]++;
	modelData->nDS[0]++;
	modelData->nDS[1]++;
	modelData->nDS[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nZS[3]++;
	modelData->nZS[4]++;
	modelData->nSZ[1]++;
	modelData->nSZ[1]++;
	modelData->nHZ[0] += 2;
	modelData->nHZ[1] += 2;
	modelData->nHZ[2] += 1;
	modelData->nHZ[2] += 1;
	modelData->nHZ[3] += 2;
	modelData->nHZ[4] += 2;
	modelData->nHZ[5] += 2;
	modelData->nHD[0] = 1;
	modelData->nHD[1] = 1;
	modelData->event[0].nLHSDsc = 1;
	modelData->event[1].nLHSDsc = 1;
	modelData->event[2].nLHSDsc = 2;
	modelData->event[3].nLHSDsc = 1;
	modelData->event[4].nLHSDsc = 1;
	modelData->event[5].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
		cleanVector(states,0,2);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
		cleanVector(events,0,6);

	modelData->ZS[3][events[3]++] = 1;
	modelData->ZS[4][events[4]++] = 1;
		cleanVector(states,0,2);

	modelData->SZ[1][states[1]++] = 3;
	modelData->SZ[1][states[1]++] = 4;
		cleanVector(events,0,6);

	modelData->HZ[0][events[0]++] = 3;
	modelData->HZ[0][events[0]++] = 4;
	modelData->HZ[1][events[1]++] = 3;
	modelData->HZ[1][events[1]++] = 4;
	modelData->HZ[2][events[2]++] = 2;
	modelData->HZ[2][events[2]++] = 0;
	modelData->HZ[3][events[3]++] = 1;
	modelData->HZ[3][events[3]++] = 5;
	modelData->HZ[4][events[4]++] = 1;
	modelData->HZ[4][events[4]++] = 5;
	modelData->HZ[5][events[5]++] = 3;
	modelData->HZ[5][events[5]++] = 4;
		cleanVector(events,0,6);

	modelData->HD[0][events[0]++] = 1;
	modelData->HD[1][events[1]++] = 1;
		cleanVector(events,0,6);

	modelData->event[0].LHSDsc[events[0]++] = 0;
	modelData->event[1].LHSDsc[events[1]++] = 1;
	modelData->event[2].LHSDsc[events[2]++] = 2;
	modelData->event[2].LHSDsc[events[2]++] = 3;
	modelData->event[3].LHSDsc[events[3]++] = 4;
	modelData->event[4].LHSDsc[events[4]++] = 4;
	modelData->event[5].LHSDsc[events[5]++] = 5;
		cleanVector(events,0,6);

	modelData->event[0].direction = 0;
	modelData->event[0].relation = 2;
	modelData->event[1].direction = 0;
	modelData->event[1].relation = 2;
	modelData->event[2].direction = 1;
	modelData->event[2].relation = 2;
	modelData->event[3].direction = 1;
	modelData->event[3].relation = 2;
	modelData->event[4].direction = -1;
	modelData->event[4].relation = 0;
	modelData->event[5].direction = 0;
	modelData->event[5].relation = 2;
	simulator->time = QSS_Time(2,6,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("buck_disc_qss",2,6,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[1]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,2,6);
		cleanVector(states,0,2);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"QSSIntegrator_1_y[1]");
		sprintf(modelOutput->variable[1].name,"QSSIntegrator_2_y[1]");
		cleanVector(outputs,0,2);

		modelOutput->SO[1][states[1]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 1;
		modelOutput->SO[0][states[0]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 0;
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
