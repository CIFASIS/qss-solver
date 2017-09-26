#include <math.h>
#include <stdlib.h>
#include "ModeloCompletoFinalEscalado_functions.h"

#include "../../src/test-suite/ModeloCompletoFinalEscalado/ModeloCompletoFinal.c"
double __fsolve1(double boost_panel1_capacitor1_v)
{
double boost_panel1_panel_n_i;
boost_panel1_panel_n_i = fsolve1(boost_panel1_capacitor1_v);
return boost_panel1_panel_n_i;
}
double __fsolve2(double boost_panel2_capacitor1_v)
{
double boost_panel2_panel_n_i;
boost_panel2_panel_n_i = fsolve2(boost_panel2_capacitor1_v);
return boost_panel2_panel_n_i;
}
double __fsolve3(double algSuperv_Pref_FC)
{
double fCellAndElectro_fuellCell_IFC;
fCellAndElectro_fuellCell_IFC = fsolve3(algSuperv_Pref_FC);
return fCellAndElectro_fuellCell_IFC;
}
double __fsolve4(double fCellAndElectro_electComp_electrolizadorAnda1_u)
{
double fCellAndElectro_electComp_electrolizadorAnda1_Ucell;
fCellAndElectro_electComp_electrolizadorAnda1_Ucell = fsolve4(fCellAndElectro_electComp_electrolizadorAnda1_u);
return fCellAndElectro_electComp_electrolizadorAnda1_Ucell;
}
