#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_g = 0;
double __PAR_m = 0;
double __PAR_L = 0;
double __PAR_kc = 0;
double __PAR_bc = 0;
double __PAR_R = 0;
double __PAR_J = 0;
double __PAR_bslip = 0;
double __PAR_bnoslip = 0;
double __PAR_baxle = 0;
double __PAR_vslipmax = 0;
double __PAR_fric_coef_dyn = 0;
double __PAR_r = 0;
double __PAR_phi = 0;

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
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			dx[1] = (d[(10)]-__PAR_R*alg[300]-__PAR_baxle*__PAR_m*__PAR_g*x[0])/__PAR_J;
			return;
		case 10:
			dx[1] = x[60];
			return;
		case 20:
			alg[90] = 0.0;
			alg[120] = 0.0;
			alg[150] = -__PAR_m*__PAR_g;
			alg[180] = cos(x[30]/__PAR_r)*cos(__PAR_phi);
			alg[210] = -sin(x[30]/__PAR_r);
			alg[240] = cos(x[30]/__PAR_r)*sin(__PAR_phi);
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			alg[330] = alg[180]*alg[90]+alg[210]*alg[120]+alg[240]*alg[150]+alg[300]+__PAR_kc*(x[33]-x[30]-__PAR_L)+__PAR_bc*(x[63]-x[60]);
			dx[1] = alg[330]/__PAR_m;
			return;
		case 9:
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			dx[1] = (-__PAR_R*alg[327]-__PAR_baxle*__PAR_m*__PAR_g*x[27])/__PAR_J;
			return;
		case 19:
			dx[1] = x[87];
			return;
		case 29:
			alg[117] = 0.0;
			alg[147] = 0.0;
			alg[177] = -__PAR_m*__PAR_g;
			alg[207] = cos(x[57]/__PAR_r)*cos(__PAR_phi);
			alg[237] = -sin(x[57]/__PAR_r);
			alg[267] = cos(x[57]/__PAR_r)*sin(__PAR_phi);
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			alg[357] = alg[207]*alg[117]+alg[237]*alg[147]+alg[267]*alg[177]+alg[327]+__PAR_kc*(x[54]-x[57]+__PAR_L)+__PAR_bc*(x[84]-x[87]);
			dx[1] = alg[357]/__PAR_m;
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 8)
			{
				alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
				alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
				dx[1] = (-__PAR_R*alg[(j0+100) * 3]-__PAR_baxle*__PAR_m*__PAR_g*x[(j0) * 3])/__PAR_J;
			}
			j0 = i0-10;
			if(j0 >=1 && j0 <= 8)
			{
				dx[1] = x[(j0+20) * 3];
			}
			j0 = i0-20;
			if(j0 >=1 && j0 <= 8)
			{
				alg[(j0+30) * 3] = 0.0;
				alg[(j0+40) * 3] = 0.0;
				alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
				alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
				alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
				alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
				alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
				alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
				alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
				dx[1] = alg[(j0+110) * 3]/__PAR_m;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			alg[90] = 0.0;
			alg[120] = 0.0;
			alg[150] = -__PAR_m*__PAR_g;
			alg[180] = cos(x[30]/__PAR_r)*cos(__PAR_phi);
			alg[210] = -sin(x[30]/__PAR_r);
			alg[240] = cos(x[30]/__PAR_r)*sin(__PAR_phi);
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			alg[330] = alg[180]*alg[90]+alg[210]*alg[120]+alg[240]*alg[150]+alg[300]+__PAR_kc*(x[33]-x[30]-__PAR_L)+__PAR_bc*(x[63]-x[60]);
			der[0 + 1] = (d[(10)]-__PAR_R*alg[300]-__PAR_baxle*__PAR_m*__PAR_g*x[0])/__PAR_J;
			der[60 + 1] = alg[330]/__PAR_m;
			break;
		case 9:
			alg[117] = 0.0;
			alg[147] = 0.0;
			alg[177] = -__PAR_m*__PAR_g;
			alg[207] = cos(x[57]/__PAR_r)*cos(__PAR_phi);
			alg[237] = -sin(x[57]/__PAR_r);
			alg[267] = cos(x[57]/__PAR_r)*sin(__PAR_phi);
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			alg[357] = alg[207]*alg[117]+alg[237]*alg[147]+alg[267]*alg[177]+alg[327]+__PAR_kc*(x[54]-x[57]+__PAR_L)+__PAR_bc*(x[84]-x[87]);
			der[27 + 1] = (-__PAR_R*alg[327]-__PAR_baxle*__PAR_m*__PAR_g*x[27])/__PAR_J;
			der[87 + 1] = alg[357]/__PAR_m;
			break;
		case 10:
			alg[90] = 0.0;
			alg[120] = 0.0;
			alg[150] = -__PAR_m*__PAR_g;
			alg[180] = cos(x[30]/__PAR_r)*cos(__PAR_phi);
			alg[210] = -sin(x[30]/__PAR_r);
			alg[240] = cos(x[30]/__PAR_r)*sin(__PAR_phi);
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			alg[330] = alg[180]*alg[90]+alg[210]*alg[120]+alg[240]*alg[150]+alg[300]+__PAR_kc*(x[33]-x[30]-__PAR_L)+__PAR_bc*(x[63]-x[60]);
			der[60 + 1] = alg[330]/__PAR_m;
			break;
		case 11:
			alg[90] = 0.0;
			alg[120] = 0.0;
			alg[150] = -__PAR_m*__PAR_g;
			alg[180] = cos(x[30]/__PAR_r)*cos(__PAR_phi);
			alg[210] = -sin(x[30]/__PAR_r);
			alg[240] = cos(x[30]/__PAR_r)*sin(__PAR_phi);
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			alg[330] = alg[180]*alg[90]+alg[210]*alg[120]+alg[240]*alg[150]+alg[300]+__PAR_kc*(x[33]-x[30]-__PAR_L)+__PAR_bc*(x[63]-x[60]);
			der[60 + 1] = alg[330]/__PAR_m;
			break;
		case 18:
			alg[117] = 0.0;
			alg[147] = 0.0;
			alg[177] = -__PAR_m*__PAR_g;
			alg[207] = cos(x[57]/__PAR_r)*cos(__PAR_phi);
			alg[237] = -sin(x[57]/__PAR_r);
			alg[267] = cos(x[57]/__PAR_r)*sin(__PAR_phi);
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			alg[357] = alg[207]*alg[117]+alg[237]*alg[147]+alg[267]*alg[177]+alg[327]+__PAR_kc*(x[54]-x[57]+__PAR_L)+__PAR_bc*(x[84]-x[87]);
			der[87 + 1] = alg[357]/__PAR_m;
			break;
		case 19:
			alg[117] = 0.0;
			alg[147] = 0.0;
			alg[177] = -__PAR_m*__PAR_g;
			alg[207] = cos(x[57]/__PAR_r)*cos(__PAR_phi);
			alg[237] = -sin(x[57]/__PAR_r);
			alg[267] = cos(x[57]/__PAR_r)*sin(__PAR_phi);
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			alg[357] = alg[207]*alg[117]+alg[237]*alg[147]+alg[267]*alg[177]+alg[327]+__PAR_kc*(x[54]-x[57]+__PAR_L)+__PAR_bc*(x[84]-x[87]);
			der[87 + 1] = alg[357]/__PAR_m;
			break;
		case 20:
			alg[90] = 0.0;
			alg[120] = 0.0;
			alg[150] = -__PAR_m*__PAR_g;
			alg[180] = cos(x[30]/__PAR_r)*cos(__PAR_phi);
			alg[210] = -sin(x[30]/__PAR_r);
			alg[240] = cos(x[30]/__PAR_r)*sin(__PAR_phi);
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			alg[330] = alg[180]*alg[90]+alg[210]*alg[120]+alg[240]*alg[150]+alg[300]+__PAR_kc*(x[33]-x[30]-__PAR_L)+__PAR_bc*(x[63]-x[60]);
			der[0 + 1] = (d[(10)]-__PAR_R*alg[300]-__PAR_baxle*__PAR_m*__PAR_g*x[0])/__PAR_J;
			der[30 + 1] = x[60];
			der[60 + 1] = alg[330]/__PAR_m;
			break;
		case 21:
			alg[90] = 0.0;
			alg[120] = 0.0;
			alg[150] = -__PAR_m*__PAR_g;
			alg[180] = cos(x[30]/__PAR_r)*cos(__PAR_phi);
			alg[210] = -sin(x[30]/__PAR_r);
			alg[240] = cos(x[30]/__PAR_r)*sin(__PAR_phi);
			alg[270] = __PAR_R*x[0]-x[60];
			alg[300] = d[0]*__PAR_bnoslip*alg[270]*__PAR_m*__PAR_g+(1.0-d[0])*(__PAR_fric_coef_dyn*alg[270]*__PAR_m*__PAR_g+__PAR_bslip*alg[270]*__PAR_m*__PAR_g);
			alg[330] = alg[180]*alg[90]+alg[210]*alg[120]+alg[240]*alg[150]+alg[300]+__PAR_kc*(x[33]-x[30]-__PAR_L)+__PAR_bc*(x[63]-x[60]);
			der[60 + 1] = alg[330]/__PAR_m;
			break;
		case 28:
			alg[117] = 0.0;
			alg[147] = 0.0;
			alg[177] = -__PAR_m*__PAR_g;
			alg[207] = cos(x[57]/__PAR_r)*cos(__PAR_phi);
			alg[237] = -sin(x[57]/__PAR_r);
			alg[267] = cos(x[57]/__PAR_r)*sin(__PAR_phi);
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			alg[357] = alg[207]*alg[117]+alg[237]*alg[147]+alg[267]*alg[177]+alg[327]+__PAR_kc*(x[54]-x[57]+__PAR_L)+__PAR_bc*(x[84]-x[87]);
			der[87 + 1] = alg[357]/__PAR_m;
			break;
		case 29:
			alg[117] = 0.0;
			alg[147] = 0.0;
			alg[177] = -__PAR_m*__PAR_g;
			alg[207] = cos(x[57]/__PAR_r)*cos(__PAR_phi);
			alg[237] = -sin(x[57]/__PAR_r);
			alg[267] = cos(x[57]/__PAR_r)*sin(__PAR_phi);
			alg[297] = __PAR_R*x[27]-x[87];
			alg[327] = d[9]*__PAR_bnoslip*alg[297]*__PAR_m*__PAR_g+(1.0-d[9])*(__PAR_fric_coef_dyn*alg[297]*__PAR_m*__PAR_g+__PAR_bslip*alg[297]*__PAR_m*__PAR_g);
			alg[357] = alg[207]*alg[117]+alg[237]*alg[147]+alg[267]*alg[177]+alg[327]+__PAR_kc*(x[54]-x[57]+__PAR_L)+__PAR_bc*(x[84]-x[87]);
			der[27 + 1] = (-__PAR_R*alg[327]-__PAR_baxle*__PAR_m*__PAR_g*x[27])/__PAR_J;
			der[57 + 1] = x[87];
			der[87 + 1] = alg[357]/__PAR_m;
			break;
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0) * 3 + 1] = (-__PAR_R*alg[(j0+100) * 3]-__PAR_baxle*__PAR_m*__PAR_g*x[(j0) * 3])/__PAR_J;
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
	j0 = i0-9;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
	j0 = i0-10;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
	j0 = i0-11;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
	j0 = i0-19;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
	j0 = i0-20;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0) * 3 + 1] = (-__PAR_R*alg[(j0+100) * 3]-__PAR_baxle*__PAR_m*__PAR_g*x[(j0) * 3])/__PAR_J;
		der[(j0+10) * 3 + 1] = x[(j0+20) * 3];
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
	j0 = i0-21;
	if(j0 >=1 && j0 <= 8)
	{
		alg[(j0+30) * 3] = 0.0;
		alg[(j0+40) * 3] = 0.0;
		alg[(j0+50) * 3] = -__PAR_m*__PAR_g;
		alg[(j0+60) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*cos(__PAR_phi);
		alg[(j0+70) * 3] = -sin(x[(j0+10) * 3]/__PAR_r);
		alg[(j0+80) * 3] = cos(x[(j0+10) * 3]/__PAR_r)*sin(__PAR_phi);
		alg[(j0+90) * 3] = __PAR_R*x[(j0) * 3]-x[(j0+20) * 3];
		alg[(j0+100) * 3] = d[(j0)]*__PAR_bnoslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g+(1.0-d[(j0)])*(__PAR_fric_coef_dyn*alg[(j0+90) * 3]*__PAR_m*__PAR_g+__PAR_bslip*alg[(j0+90) * 3]*__PAR_m*__PAR_g);
		alg[(j0+110) * 3] = alg[(j0+60) * 3]*alg[(j0+30) * 3]+alg[(j0+70) * 3]*alg[(j0+40) * 3]+alg[(j0+80) * 3]*alg[(j0+50) * 3]+alg[(j0+100) * 3]+__PAR_kc*(x[(j0+11) * 3]+x[(j0+9) * 3]-2.0*x[(j0+10) * 3])+__PAR_bc*(x[(j0+21) * 3]+x[(j0+19) * 3]-2.0*x[(j0+20) * 3]);
		der[(j0+20) * 3 + 1] = alg[(j0+110) * 3]/__PAR_m;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	int j310 = 0;
	int j320 = 0;
	switch(i)
	{
		case 20:
			zc[0] = t-(30.0);
			return;
		default:
			if(i0 >= 0 && i0 <= 9)
			{
				alg[270] = __PAR_R*x[0]-x[60];
				j310 = i0;
	if (j310 >= 1 && j310 <= 8)
	{
				alg[(i0+90) * 3] = __PAR_R*x[(j310) * 3]-x[(j310+20) * 3];
				}
				alg[297] = __PAR_R*x[27]-x[87];
				zc[0] = alg[(i0+90) * 3]-(__PAR_vslipmax);
			}
			if(i0 >= 10 && i0 <= 19)
			{
				alg[270] = __PAR_R*x[0]-x[60];
				j320 = i0;
	if (j320 >= 1 && j320 <= 8)
	{
				alg[(i0+80) * 3] = __PAR_R*x[(j320-10) * 3]-x[(j320+10) * 3];
				}
				alg[297] = __PAR_R*x[27]-x[87];
				zc[0] = alg[(i0+80) * 3]-(__PAR_vslipmax);
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 20:
			d[(10)] = 0.0;
			return;
		default:
			if(i0 >= 0 && i0 <= 9)
			{
				d[(i0)] = 0.0;
			}
			if(i0 >= 10 && i0 <= 19)
			{
				d[(i0-10)] = 1.0;
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 9)
	{
		d[(i0)] = 1.0;
	}
	if(i0 >= 10 && i0 <= 19)
	{
		d[(i0-10)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[60];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(11*sizeof(int));
	int *events = (int*)malloc(21*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(30*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(30,11,21,0,120,"train4");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_g = 9.810000000000000497379915e+00;
	__PAR_m = 126000.0;
	__PAR_L = 20.0;
	__PAR_kc = 1.000000000000000000000000e+07;
	__PAR_bc = 1.000000000000000000000000e+07;
	__PAR_R = 1.0;
	__PAR_J = 100.0;
	__PAR_bslip = 1.000000000000000020816682e-03;
	__PAR_bnoslip = 1.0;
	__PAR_baxle = 1.000000000000000047921736e-04;
	__PAR_vslipmax = 1.000000000000000055511151e-01;
	__PAR_fric_coef_dyn = 5.999999999999999777955395e-01;
	__PAR_r = 1.000000000000000000000000e+04;
	__PAR_phi = 1.000000000000000047921736e-04;
	for(i0 = 0; i0 <= 9;i0++)
	{
		modelData->d[i0] = 1.0;
	}
	modelData->d[(10)] = 190000.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 9; i0++)
	{
		modelData->x[(i0+10) * 3] = ((i0+1)-1.0)*__PAR_L;
	}
	modelData->nDS[0] = 1;
	modelData->nDS[10] = 1;
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->nDS[i0] = 1;
		modelData->nDS[i0+10] = 1;
	}
	modelData->nDS[9] = 1;
	modelData->nDS[19] = 1;
	modelData->nDS[0]++;
	modelData->nDS[20]++;
	modelData->nDS[20]++;
	modelData->nDS[20]++;
	modelData->nDS[20]++;
	modelData->nDS[20]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nDS[i0+20]++;
	}
	modelData->nDS[9]++;
	modelData->nDS[29]++;
	modelData->nDS[29]++;
	modelData->nDS[29]++;
	modelData->nDS[29]++;
	modelData->nDS[29]++;
	modelData->nSD[0]++;
	modelData->nSD[20]++;
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->nSD[i0]++;
		modelData->nSD[i0+20]++;
	}
	modelData->nSD[9]++;
	modelData->nSD[29]++;
	modelData->nSD[20]++;
	modelData->nSD[0]++;
	modelData->nSD[10]++;
	modelData->nSD[11]++;
	modelData->nSD[20]++;
	modelData->nSD[21]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+9]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+10]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+11]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+19]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+20]++;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSD[i0+21]++;
	}
	modelData->nSD[29]++;
	modelData->nSD[9]++;
	modelData->nSD[18]++;
	modelData->nSD[19]++;
	modelData->nSD[28]++;
	modelData->nSD[29]++;
	modelData->nZS[0]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nZS[i0]++;
	}
	modelData->nZS[9]++;
	modelData->nZS[0]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nZS[i0]++;
	}
	modelData->nZS[9]++;
	modelData->nZS[10]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nZS[i0+10]++;
	}
	modelData->nZS[19]++;
	modelData->nZS[10]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nZS[i0+10]++;
	}
	modelData->nZS[19]++;
	modelData->nSZ[0]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	modelData->nSZ[9]++;
	modelData->nSZ[20]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSZ[i0+20]++;
	}
	modelData->nSZ[29]++;
	modelData->nSZ[0]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSZ[i0]++;
	}
	modelData->nSZ[9]++;
	modelData->nSZ[20]++;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->nSZ[i0+20]++;
	}
	modelData->nSZ[29]++;
	modelData->nHD[0]++;
	modelData->nHD[10]++;
	modelData->nHD[0]++;
	modelData->nHD[10]++;
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 11; i0 <= 18; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 11; i0 <= 18; i0++)
	{
		modelData->nHD[i0]++;
	}
	modelData->nHD[9]++;
	modelData->nHD[19]++;
	modelData->nHD[9]++;
	modelData->nHD[19]++;
	modelData->nHD[20] = 1;
	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].nLHSDsc = 1;
	modelData->event[i0+10].nLHSDsc = 1;
	}
	modelData->event[20].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,30);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[10][states[10]++] = 20;
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0+10][states[i0+10]++] = i0+20;
	}
	modelData->DS[9][states[9]++] = 9;
	modelData->DS[19][states[19]++] = 29;
	modelData->DS[0][states[0]++] = 20;
	modelData->DS[20][states[20]++] = 0;
	modelData->DS[20][states[20]++] = 10;
	modelData->DS[20][states[20]++] = 11;
	modelData->DS[20][states[20]++] = 20;
	modelData->DS[20][states[20]++] = 21;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0][states[i0]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0+9;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0+10;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0+11;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0+19;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->DS[i0+20][states[i0+20]++] = i0+21;
	}
	modelData->DS[9][states[9]++] = 29;
	modelData->DS[29][states[29]++] = 9;
	modelData->DS[29][states[29]++] = 18;
	modelData->DS[29][states[29]++] = 19;
	modelData->DS[29][states[29]++] = 28;
	modelData->DS[29][states[29]++] = 29;
		cleanVector(states,0,30);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[20][states[20]++] = 10;
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+20][states[i0+20]++] = i0+10;
	}
	modelData->SD[9][states[9]++] = 9;
	modelData->SD[29][states[29]++] = 19;
	modelData->SD[20][states[20]++] = 0;
	modelData->SD[0][states[0]++] = 20;
	modelData->SD[10][states[10]++] = 20;
	modelData->SD[11][states[11]++] = 20;
	modelData->SD[20][states[20]++] = 20;
	modelData->SD[21][states[21]++] = 20;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+20][states[i0+20]++] = i0;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0][states[i0]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+9][states[i0+9]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+10][states[i0+10]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+11][states[i0+11]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+19][states[i0+19]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+20][states[i0+20]++] = i0+20;
	}
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SD[i0+21][states[i0+21]++] = i0+20;
	}
	modelData->SD[29][states[29]++] = 9;
	modelData->SD[9][states[9]++] = 29;
	modelData->SD[18][states[18]++] = 29;
	modelData->SD[19][states[19]++] = 29;
	modelData->SD[28][states[28]++] = 29;
	modelData->SD[29][states[29]++] = 29;
		cleanVector(events,0,21);

	modelData->ZS[0][events[0]++] = 0;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->ZS[i0][events[i0]++] = i0;
	}
	modelData->ZS[9][events[9]++] = 9;
	modelData->ZS[0][events[0]++] = 20;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->ZS[i0][events[i0]++] = i0+20;
	}
	modelData->ZS[9][events[9]++] = 29;
	modelData->ZS[10][events[10]++] = 0;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->ZS[i0+10][events[i0+10]++] = i0;
	}
	modelData->ZS[19][events[19]++] = 9;
	modelData->ZS[10][events[10]++] = 20;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->ZS[i0+10][events[i0+10]++] = i0+20;
	}
	modelData->ZS[19][events[19]++] = 29;
		cleanVector(states,0,30);

	modelData->SZ[0][states[0]++] = 0;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0;
	}
	modelData->SZ[9][states[9]++] = 9;
	modelData->SZ[20][states[20]++] = 0;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SZ[i0+20][states[i0+20]++] = i0;
	}
	modelData->SZ[29][states[29]++] = 9;
	modelData->SZ[0][states[0]++] = 10;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SZ[i0][states[i0]++] = i0+10;
	}
	modelData->SZ[9][states[9]++] = 19;
	modelData->SZ[20][states[20]++] = 10;
	for ( i0 = 1; i0 <= 8; i0++) 
	{
	modelData->SZ[i0+20][states[i0+20]++] = i0+10;
	}
	modelData->SZ[29][states[29]++] = 19;
		cleanVector(events,0,21);

	modelData->HD[20][events[20]++] = 0;
	modelData->HD[0][events[0]++] = 0;
	modelData->HD[10][events[10]++] = 0;
	modelData->HD[0][events[0]++] = 20;
	modelData->HD[10][events[10]++] = 20;
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 11; i0 <= 18; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-10;
	}
	for(i0 = 1; i0 <= 8; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+20;
	}
	for(i0 = 11; i0 <= 18; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+10;
	}
	modelData->HD[9][events[9]++] = 9;
	modelData->HD[19][events[19]++] = 9;
	modelData->HD[9][events[9]++] = 29;
	modelData->HD[19][events[19]++] = 29;
		cleanVector(events,0,21);

	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].LHSDsc[events[i0]++] = i0;
	modelData->event[i0+10].LHSDsc[events[i0+10]++] = i0;
	}
	modelData->event[20].LHSDsc[events[20]++] = 10;
		cleanVector(events,0,21);

	for(i0 = 0; i0 <= 9; i0++)
	{
	modelData->event[i0].direction = 0;
	modelData->event[i0].relation = 2;
	modelData->event[i0+10].direction = 0;
	modelData->event[i0+10].relation = 0;
	}
	modelData->event[20].direction = 1;
	modelData->event[20].relation = 2;
	simulator->time = QSS_Time(30,21,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.2;
	simulator->output = SD_Output("train4",1,11,30,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[20]++;
	SD_allocOutputMatrix(modelOutput,30,11);
		cleanVector(states,0,30);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"vl[1]");
		cleanVector(outputs,0,1);

		modelOutput->SO[20][states[20]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 20;
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
