#include <math.h>
#include <stdlib.h>
#include "SistHibGen1batery_qss_functions.h"

#include "../../src/test-suite/SistHibGen1batery_qss/SistHibGen1batery.c"
void __fsolve1(double boost_panel1_capacitor1_v,double *boost_panel1_panel_exponente,double *boost_panel1_panel_n_i)
{
fsolve1(boost_panel1_capacitor1_v,boost_panel1_panel_exponente,boost_panel1_panel_n_i);
}
void __fsolve2(double boost_panel2_capacitor1_v,double *boost_panel2_panel_exponente,double *boost_panel2_panel_n_i)
{
fsolve2(boost_panel2_capacitor1_v,boost_panel2_panel_exponente,boost_panel2_panel_n_i);
}
void __fsolve3(double boost_panel3_capacitor1_v,double *boost_panel3_panel_exponente,double *boost_panel3_panel_n_i)
{
fsolve3(boost_panel3_capacitor1_v,boost_panel3_panel_exponente,boost_panel3_panel_n_i);
}
void __fsolve4(double boost_panel4_capacitor1_v,double *boost_panel4_panel_exponente,double *boost_panel4_panel_n_i)
{
fsolve4(boost_panel4_capacitor1_v,boost_panel4_panel_exponente,boost_panel4_panel_n_i);
}
