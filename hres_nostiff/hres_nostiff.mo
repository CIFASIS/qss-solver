model hres_nostiff
  function fsolve1
    input Real boost_panel1_capacitor1_v;
    output Real boost_panel1_panel_n_i;
    external "C"boost_panel1_panel_n_i = fsolve1(boost_panel1_capacitor1_v)    annotation(Library={"m","gsl","blas"},Include="#include \"hres.c\"
");
  end fsolve1;
  function fsolve2
    input Real boost_panel2_capacitor1_v;
    output Real boost_panel2_panel_n_i;
    external "C"boost_panel2_panel_n_i = fsolve2(boost_panel2_capacitor1_v)    annotation(Library={"m","gsl","blas"},Include="#include \"hres.c\"
");
  end fsolve2;
  function fsolve3
    input Real algSuperv_Pref_FC;    output Real fCellAndElectro_fuellCell_IFC;
    external "C"fCellAndElectro_fuellCell_IFC = fsolve3(algSuperv_Pref_FC)    annotation(Library={"m","gsl","blas"},Include="#include \"hres.c\"
");
  end fsolve3;
  function fsolve4
    input Real fCellAndElectro_electComp_electrolizadorAnda1_u;
    output Real fCellAndElectro_electComp_electrolizadorAnda1_Ucell;
    external "C"fCellAndElectro_electComp_electrolizadorAnda1_Ucell = fsolve4(fCellAndElectro_electComp_electrolizadorAnda1_u)    annotation(Library={"m","gsl","blas"},Include="#include \"hres.c\"
");
  end fsolve4;
  Real ground_p_i;
  parameter Real lambda2_k(start=1)=100;
  parameter Real capacitor2_C(start=1)=0.005;
  Real capacitor2_v(start=50);
  Real capacitor2_n_i;
  parameter Real boost_panel1_panel_q=1.6e-19;
  parameter Real boost_panel1_panel_Ac=1.6;
  parameter Real boost_panel1_panel_K=1.3805e-23;
  parameter Real boost_panel1_panel_K1=0.005532;
  parameter Real boost_panel1_panel_Ior=1.0647e-06;
  parameter Real boost_panel1_panel_Tref=303;
  parameter Real boost_panel1_panel_Eg=1.1;
  parameter Real boost_panel1_panel_Isc=8.51;
  parameter Real boost_panel1_panel_Rspv=0.01;
  parameter Real boost_panel1_panel_Tpv=273+25;
  parameter Real boost_panel1_panel_Irs=boost_panel1_panel_Ior*(((boost_panel1_panel_Tpv/boost_panel1_panel_Tref)^3))*exp(boost_panel1_panel_q*boost_panel1_panel_Eg*(1/boost_panel1_panel_Tref-1/boost_panel1_panel_Tpv)/boost_panel1_panel_K/boost_panel1_panel_Ac);
  parameter Integer boost_panel1_panel_Np=1;
  parameter Integer boost_panel1_panel_Ns=60;
  Real boost_panel1_panel_n_i;
  Real boost_panel1_mmt1_y;
  discrete Real boost_panel1_mmt1_potact;
  Real boost_panel1_mmt1_pot;
  Real boost_panel1_mmt1_potActFiltrada;
  discrete Real boost_panel1_mmt1_potprev;
  discrete Real boost_panel1_mmt1_prevu;
  discrete Real boost_panel1_mmt1_actu;
  discrete Real boost_panel1_mmt1_deltau;
  discrete Real boost_panel1_mmt1_deltap;
  discrete Real boost_panel1_mmt1_vref(start=36);
  parameter Real boost_panel1_mmt1_Ts=0.1;
  parameter Real boost_panel1_mmt1_deltaVpvRefPanel=0.5;
  parameter Real boost_panel1_capacitor1_C(start=1)=0.00047;
  Real boost_panel1_capacitor1_v(start=0);
  Real boost_panel1_capacitor1_n_i;
  Real boost_panel1_cont_pwm1_vgate;
  discrete Real boost_panel1_cont_pwm1_vout;
  discrete Real boost_panel1_cont_pwm1_dc;
  discrete Real boost_panel1_cont_pwm1_tnext(start=1e+20);
  parameter Real boost_panel1_cont_pwm1_T=0.0001;
  parameter Real boost_panel1_cont_pwm1_iTime=1.1e-06;
  parameter Real boost_panel1_boost1_inductor_L(start=1)=0.015;
  Real boost_panel1_boost1_inductor_v;
  Real boost_panel1_boost1_inductor_i(start=0);
  Real boost_panel1_boost1_inductor_n_v;
  parameter Real boost_panel1_boost1_idealDiode_Ron(min=0)=1e-05;
  parameter Real boost_panel1_boost1_idealDiode_Goff(min=0)=1e-05;
  parameter Real boost_panel1_boost1_idealDiode_Vknee(min=0)=0;
  Real boost_panel1_boost1_idealDiode_off(start=1);
  Real boost_panel1_boost1_idealDiode_s(start=0,unit="1");
  constant Real boost_panel1_boost1_idealDiode_unitVoltage=1;
  constant Real boost_panel1_boost1_idealDiode_unitCurrent=1;
  Real boost_panel1_boost1_idealDiode_v;
  Real boost_panel1_boost1_idealDiode_n_i;
  parameter Real boost_panel1_boost1_idealDiode_useHeatPort=0;
  parameter Real boost_panel1_boost1_idealDiode_T=293.15;
  Real boost_panel1_boost1_idealDiode_LossPower;
  parameter Real boost_panel1_boost1_controlledIdealClosingSwitch_level=0.5;
  parameter Real boost_panel1_boost1_controlledIdealClosingSwitch_Ron(min=0)=1e-05;
  parameter Real boost_panel1_boost1_controlledIdealClosingSwitch_Goff(min=0)=1e-05;
  Real boost_panel1_boost1_controlledIdealClosingSwitch_s(unit="1");
  constant Real boost_panel1_boost1_controlledIdealClosingSwitch_unitVoltage=1;
  constant Real boost_panel1_boost1_controlledIdealClosingSwitch_unitCurrent=1;
  parameter Real boost_panel1_boost1_controlledIdealClosingSwitch_useHeatPort=0;
  parameter Real boost_panel1_boost1_controlledIdealClosingSwitch_T=293.15;
  Real boost_panel1_boost1_controlledIdealClosingSwitch_LossPower;
  Real boost_panel1_boost1_pin_ground_i;
  Real boost_panel1_pin_n_i;
  Real batteryAndBockBoost_bookBoostDB_pin_p_out_i;
  Real batteryAndBockBoost_bookBoostDB_pin_n_i;
  parameter Real batteryAndBockBoost_bookBoostDB_inductor_L(start=1)=0.015;
  Real batteryAndBockBoost_bookBoostDB_inductor_v;
  Real batteryAndBockBoost_bookBoostDB_inductor_i(start=0);
  Real batteryAndBockBoost_bookBoostDB_inductor_p_v;
  Real batteryAndBockBoost_bookBoostDB_inductor_n_i;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode_Goff(min=0)=1e-05;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode_Vknee(min=0)=0;
  Real batteryAndBockBoost_bookBoostDB_idealDiode_off(start=1);
  Real batteryAndBockBoost_bookBoostDB_idealDiode_s(start=0,unit="1");
  constant Real batteryAndBockBoost_bookBoostDB_idealDiode_unitVoltage=1;
  constant Real batteryAndBockBoost_bookBoostDB_idealDiode_unitCurrent=1;
  Real batteryAndBockBoost_bookBoostDB_idealDiode_n_v;
  Real batteryAndBockBoost_bookBoostDB_idealDiode_n_i;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode_useHeatPort=0;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode_T=293.15;
  Real batteryAndBockBoost_bookBoostDB_idealDiode_LossPower;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode1_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode1_Goff(min=0)=1e-05;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode1_Vknee(min=0)=0;
  Real batteryAndBockBoost_bookBoostDB_idealDiode1_off(start=1);
  Real batteryAndBockBoost_bookBoostDB_idealDiode1_s(start=0,unit="1");
  constant Real batteryAndBockBoost_bookBoostDB_idealDiode1_unitVoltage=1;
  constant Real batteryAndBockBoost_bookBoostDB_idealDiode1_unitCurrent=1;
  Real batteryAndBockBoost_bookBoostDB_idealDiode1_v;
  Real batteryAndBockBoost_bookBoostDB_idealDiode1_n_i;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode1_useHeatPort=0;
  parameter Real batteryAndBockBoost_bookBoostDB_idealDiode1_T=293.15;
  Real batteryAndBockBoost_bookBoostDB_idealDiode1_LossPower;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_Goff(min=0)=1e-05;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_control;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_s(unit="1");
  constant Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_unitVoltage=1;
  constant Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_unitCurrent=1;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_v;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_n_i;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_useHeatPort=0;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_T=293.15;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_LossPower;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_Goff(min=0)=1e-05;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_s(unit="1");
  constant Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_unitVoltage=1;
  constant Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_unitCurrent=1;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_n_i;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_useHeatPort=0;
  parameter Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_T=293.15;
  Real batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_LossPower;
  Real batteryAndBockBoost_bookBoostDB_Sw2;
  parameter Real batteryAndBockBoost_const_k(start=1)=56;
  Real batteryAndBockBoost_bookBoostPWMcontrol1_yref;
  parameter Real batteryAndBockBoost_bookBoostPWMcontrol1_Kp=3;
  parameter Real batteryAndBockBoost_bookBoostPWMcontrol1_Ki=0;
  Real batteryAndBockBoost_bookBoostPWMcontrol1_Err;
  Real batteryAndBockBoost_bookBoostPWMcontrol1_intErr;
  parameter Real batteryAndBockBoost_bookBoostPWMcontrol1_T=5e-05;
  parameter Real batteryAndBockBoost_bookBoostPWMcontrol1_iTime=0;
  discrete Real batteryAndBockBoost_bookBoostPWMcontrol1_tnext(start=1e-20);
  discrete Real batteryAndBockBoost_bookBoostPWMcontrol1_DC;
  discrete Real batteryAndBockBoost_bookBoostPWMcontrol1_Output;
  Real batteryAndBockBoost_voltageControl_Err;
  Real batteryAndBockBoost_voltageControl_intErr;
  parameter Real batteryAndBockBoost_voltageControl_Kp=0.95;
  parameter Real batteryAndBockBoost_voltageControl_Ki=90;
  parameter Real batteryAndBockBoost_voltageControl_Imax=17;
  discrete Real batteryAndBockBoost_voltageControl_Sat(start=1);
  discrete Real batteryAndBockBoost_voltageControl_SatLevel(start=0);
  Real batteryAndBockBoost_voltageControl_IrefCont;
  Real batteryAndBockBoost_pin_n_i;
  parameter Real batteryAndBockBoost_battery_E0=12.6463;
  parameter Real batteryAndBockBoost_battery_Q=6;
  parameter Real batteryAndBockBoost_battery_A=0.66;
  parameter Real batteryAndBockBoost_battery_B=2884.61;
  parameter Real batteryAndBockBoost_battery_K=0.33;
  parameter Real batteryAndBockBoost_battery_R=0.25;
  parameter Real batteryAndBockBoost_battery_SoC0=41.5;
  Real batteryAndBockBoost_battery_Qt;
  Real batteryAndBockBoost_battery_SoC;
  Real batteryAndBockBoost_battery_E;
  Real batteryAndBockBoost_battery1_pin_n_v;
  parameter Real batteryAndBockBoost_battery1_E0=12.6463;
  parameter Real batteryAndBockBoost_battery1_Q=6;
  parameter Real batteryAndBockBoost_battery1_A=0.66;
  parameter Real batteryAndBockBoost_battery1_B=2884.61;
  parameter Real batteryAndBockBoost_battery1_K=0.33;
  parameter Real batteryAndBockBoost_battery1_R=0.25;
  parameter Real batteryAndBockBoost_battery1_SoC0=41.5;
  Real batteryAndBockBoost_battery1_Qt;
  Real batteryAndBockBoost_battery1_SoC;
  Real batteryAndBockBoost_battery1_v;
  Real batteryAndBockBoost_battery1_E;
  Real batteryAndBockBoost_battery2_pin_p_i;
  Real batteryAndBockBoost_battery2_pin_n_v;
  parameter Real batteryAndBockBoost_battery2_E0=12.6463;
  parameter Real batteryAndBockBoost_battery2_Q=6;
  parameter Real batteryAndBockBoost_battery2_A=0.66;
  parameter Real batteryAndBockBoost_battery2_B=2884.61;
  parameter Real batteryAndBockBoost_battery2_K=0.33;
  parameter Real batteryAndBockBoost_battery2_R=0.25;
  parameter Real batteryAndBockBoost_battery2_SoC0=41.5;
  Real batteryAndBockBoost_battery2_Qt;
  Real batteryAndBockBoost_battery2_v;
  Real batteryAndBockBoost_battery2_E;
  Real batteryAndBockBoost_u;
  parameter Real boost_panel2_panel_q=1.6e-19;
  parameter Real boost_panel2_panel_Ac=1.6;
  parameter Real boost_panel2_panel_K=1.3805e-23;
  parameter Real boost_panel2_panel_K1=0.005532;
  parameter Real boost_panel2_panel_Ior=1.0647e-06;
  parameter Real boost_panel2_panel_Tref=303;
  parameter Real boost_panel2_panel_Eg=1.1;
  parameter Real boost_panel2_panel_Isc=8.51;
  parameter Real boost_panel2_panel_Rspv=0.01;
  parameter Real boost_panel2_panel_Tpv=273+25;
  parameter Real boost_panel2_panel_Irs=boost_panel2_panel_Ior*(((boost_panel2_panel_Tpv/boost_panel2_panel_Tref)^3))*exp(boost_panel2_panel_q*boost_panel2_panel_Eg*(1/boost_panel2_panel_Tref-1/boost_panel2_panel_Tpv)/boost_panel2_panel_K/boost_panel2_panel_Ac);
  parameter Integer boost_panel2_panel_Np=1;
  parameter Integer boost_panel2_panel_Ns=60;
  Real boost_panel2_panel_n_i;
  Real boost_panel2_mmt1_y;
  discrete Real boost_panel2_mmt1_potact;
  Real boost_panel2_mmt1_pot;
  Real boost_panel2_mmt1_potActFiltrada;
  discrete Real boost_panel2_mmt1_potprev;
  discrete Real boost_panel2_mmt1_prevu;
  discrete Real boost_panel2_mmt1_actu;
  discrete Real boost_panel2_mmt1_deltau;
  discrete Real boost_panel2_mmt1_deltap;
  discrete Real boost_panel2_mmt1_vref(start=36);
  parameter Real boost_panel2_mmt1_Ts=0.1;
  parameter Real boost_panel2_mmt1_deltaVpvRefPanel=0.5;
  parameter Real boost_panel2_capacitor1_C(start=1)=0.00047;
  Real boost_panel2_capacitor1_v(start=0);
  Real boost_panel2_capacitor1_n_i;
  Real boost_panel2_cont_pwm1_vgate;
  discrete Real boost_panel2_cont_pwm1_vout;
  discrete Real boost_panel2_cont_pwm1_dc;
  discrete Real boost_panel2_cont_pwm1_tnext(start=1e+20);
  parameter Real boost_panel2_cont_pwm1_T=0.0001;
  parameter Real boost_panel2_cont_pwm1_iTime=1.1e-06;
  parameter Real boost_panel2_boost1_inductor_L(start=1)=0.015;
  Real boost_panel2_boost1_inductor_v;
  Real boost_panel2_boost1_inductor_i(start=0);
  Real boost_panel2_boost1_inductor_n_v;
  parameter Real boost_panel2_boost1_idealDiode_Ron(min=0)=1e-05;
  parameter Real boost_panel2_boost1_idealDiode_Goff(min=0)=1e-05;
  parameter Real boost_panel2_boost1_idealDiode_Vknee(min=0)=0;
  Real boost_panel2_boost1_idealDiode_off(start=1);
  Real boost_panel2_boost1_idealDiode_s(start=0,unit="1");
  constant Real boost_panel2_boost1_idealDiode_unitVoltage=1;
  constant Real boost_panel2_boost1_idealDiode_unitCurrent=1;
  Real boost_panel2_boost1_idealDiode_v;
  Real boost_panel2_boost1_idealDiode_n_i;
  parameter Real boost_panel2_boost1_idealDiode_useHeatPort=0;
  parameter Real boost_panel2_boost1_idealDiode_T=293.15;
  Real boost_panel2_boost1_idealDiode_LossPower;
  parameter Real boost_panel2_boost1_controlledIdealClosingSwitch_level=0.5;
  parameter Real boost_panel2_boost1_controlledIdealClosingSwitch_Ron(min=0)=1e-05;
  parameter Real boost_panel2_boost1_controlledIdealClosingSwitch_Goff(min=0)=1e-05;
  Real boost_panel2_boost1_controlledIdealClosingSwitch_s(unit="1");
  constant Real boost_panel2_boost1_controlledIdealClosingSwitch_unitVoltage=1;
  constant Real boost_panel2_boost1_controlledIdealClosingSwitch_unitCurrent=1;
  parameter Real boost_panel2_boost1_controlledIdealClosingSwitch_useHeatPort=0;
  parameter Real boost_panel2_boost1_controlledIdealClosingSwitch_T=293.15;
  Real boost_panel2_boost1_controlledIdealClosingSwitch_LossPower;
  Real boost_panel2_boost1_pin_ground_i;
  Real boost_panel2_pin_n_i;
  Real PCons_Sensor_pc_i;
  Real PCons_Sensor_product_y;
  Real PGen_Sensor_pc_i;
  Real PGen_Sensor_product_y;
  Real algSuperv_Pref_FC;
  parameter Real algSuperv_SOCmax=80;
  parameter Real algSuperv_SOCmin=40;
  parameter Real algSuperv_PminElect=200;
  parameter Real algSuperv_PminFC=200;
  parameter Real algSuperv_histelect=0.1;
  parameter Real algSuperv_histFC=0.1;
  discrete Real algSuperv_satElect;
  discrete Real algSuperv_EB(start=0);
  discrete Real algSuperv_satFC;
  discrete Real algSuperv_FC(start=0);
  Real algSuperv_Pgenfilt;
  Real algSuperv_deltaP;
  parameter Real lambda1_k(start=1)=70;
  Real fCellAndElectro_fuellCell_i;
  parameter Real fCellAndElectro_fuellCell_React0=((-1.0526));
  Real fCellAndElectro_fuellCell_IFC(start=0);
  parameter Real fCellAndElectro_fuellCell_p_H2=0.3;
  parameter Real fCellAndElectro_fuellCell_p_O2=1;
  parameter Real fCellAndElectro_fuellCell_Nc=47;
  parameter Real fCellAndElectro_fuellCell_R=8.3143;
  parameter Real fCellAndElectro_fuellCell_F=96487;
  parameter Real fCellAndElectro_fuellCell_ke=0.00085;
  parameter Real fCellAndElectro_fuellCell_deltaG=((-237154));
  parameter Real fCellAndElectro_fuellCell_ne=2;
  parameter Real fCellAndElectro_fuellCell_nu_0=26.523;
  parameter Real fCellAndElectro_fuellCell_a=0.089224;
  parameter Real fCellAndElectro_fuellCell_I_limit=75;
  parameter Real fCellAndElectro_fuellCell_T=298;
  parameter Real fCellAndElectro_fuellCell_MH2=0.002016;
  Real fCellAndElectro_fuellCell_fH2_FC;
  parameter Real fCellAndElectro_tanque_R=8.314;
  parameter Real fCellAndElectro_tanque_T=273+40;
  parameter Real fCellAndElectro_tanque_V=0.1;
  Real fCellAndElectro_tanque_p_tanque(start=800000);
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_F=96485;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_Urev=1.229;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_r1=7.331e-05;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_r2=((-1.107e-07));
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_r3=0;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_s1=0.1586;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_s2=0.001378;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_s3=((-1.606e-05));
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_t1=0.01599;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_t2=((-1.302));
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_t3=421.3;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_A=0.25;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_nc=1;
  parameter Real fCellAndElectro_electComp_electrolizadorAnda1_T=40;
  Real fCellAndElectro_electComp_electrolizadorAnda1_Ucell(start=1);
  Real fCellAndElectro_electComp_electrolizadorAnda1_u;
  Real fCellAndElectro_electComp_compresor_FH2_electrolizador;
  Real fCellAndElectro_electComp_compresor_p_tanq;
  Real fCellAndElectro_electComp_compresor_PotConsumida;
  parameter Real fCellAndElectro_electComp_compresor_R=8.314;
  parameter Real fCellAndElectro_electComp_compresor_k=1.4;
  parameter Real fCellAndElectro_electComp_compresor_nu_iso=0.9;
  parameter Real fCellAndElectro_electComp_compresor_nu_m=0.8;
  parameter Real fCellAndElectro_electComp_compresor_T=273+40;
  parameter Real fCellAndElectro_electComp_compresor_p_elect=1;
  Real fCellAndElectro_electComp_pin_n_i;
  Real fCellAndElectro_pin_p_i;
  Real fCellAndElectro_pin_n_i;
  discrete Real variableR1_R(start=1005);
  Real variableR1_n_i;
  parameter Real resistor_R(start=1)=100;
  parameter Real resistor_T_ref=300.15;
  parameter Real resistor_alpha=0;
  parameter Real resistor_useHeatPort=0;
  parameter Real resistor_T=resistor_T_ref;
  parameter Real inductor_L(start=1)=1e-05;
  discrete Real d0;
  discrete Real d1;
  discrete Real d2;
  discrete Real d3;
  discrete Real d4;
  discrete Real d5;
  discrete Real d6;
  discrete Real d7; 
  discrete Real d8;
  discrete Real d9;
  discrete Real d10;
  discrete Real d11;
  discrete Real d12;
  discrete Real d13;
  discrete Real d14;
  discrete Real d15;
  discrete Real d16;
  discrete Real d17;
  discrete Real d18;
  discrete Real d19;
  discrete Real d20(start=0);
  discrete Real d21(start=boost_panel1_cont_pwm1_iTime);
  discrete Real d22(start=batteryAndBockBoost_bookBoostPWMcontrol1_iTime);
  discrete Real d23(start=0);
  discrete Real d24(start=boost_panel2_cont_pwm1_iTime);

//Muestreo de salida
  discrete Real lastTsOut(start=0);
  parameter Real TsOut=0.0125137;
  discrete Real Tsubsamp(start=1e-4);
  discrete Real Vc2;
  discrete Real Ibatery;
  discrete Real Pgen;
  discrete Real Pcons;
  discrete Real PFcel;
  discrete Real Pelectro;
  discrete Real SOC;
  discrete Real ptanque;
  discrete Real lastsubsamp(start=0);

Real Qt1;
Real Qt2;
Real Qt3;

initial algorithm
//  batteryAndBockBoost_battery_Qt := (1-batteryAndBockBoost_battery_SoC0/100)*batteryAndBockBoost_battery_Q;
//  batteryAndBockBoost_battery1_Qt := (1-batteryAndBockBoost_battery1_SoC0/100)*batteryAndBockBoost_battery1_Q;
//  batteryAndBockBoost_battery2_Qt := (1-batteryAndBockBoost_battery2_SoC0/100)*batteryAndBockBoost_battery2_Q;

Qt1:=1000*((1-batteryAndBockBoost_battery_SoC0/100)*batteryAndBockBoost_battery_Q);
Qt2:=1000*( (1-batteryAndBockBoost_battery1_SoC0/100)*batteryAndBockBoost_battery1_Q);
Qt3:=1000*( (1-batteryAndBockBoost_battery2_SoC0/100)*batteryAndBockBoost_battery2_Q);
algSuperv_EB := 0;
algSuperv_FC := 0;
algSuperv_satElect := 1;
algSuperv_satFC := 1;
batteryAndBockBoost_bookBoostPWMcontrol1_DC := 0.98;
batteryAndBockBoost_bookBoostPWMcontrol1_Output := 1;
batteryAndBockBoost_bookBoostPWMcontrol1_tnext := 0.000049;
batteryAndBockBoost_voltageControl_Sat := 1;
batteryAndBockBoost_voltageControl_SatLevel := 0;
boost_panel1_cont_pwm1_dc := 0;
boost_panel1_cont_pwm1_tnext := 1e20;
boost_panel1_cont_pwm1_vout := 0;
boost_panel1_mmt1_actu := 0;
boost_panel1_mmt1_deltap := 0;
boost_panel1_mmt1_deltau := 0;
boost_panel1_mmt1_potact := 0;
boost_panel1_mmt1_potprev := 0;
boost_panel1_mmt1_prevu := 0;
boost_panel1_mmt1_vref := 36;
boost_panel2_cont_pwm1_dc := 0;
boost_panel2_cont_pwm1_tnext := 1e20;
boost_panel2_cont_pwm1_vout := 0;
boost_panel2_mmt1_actu := 0;
boost_panel2_mmt1_deltap := 0;
boost_panel2_mmt1_deltau := 0;
boost_panel2_mmt1_potact := 0;
boost_panel2_mmt1_potprev := 0;
boost_panel2_mmt1_prevu := 0;
boost_panel2_mmt1_vref := 36;
d0 := 1;
d10 := 1;
d11 := 0;
d12 := 0;
d13 := 1;
d14 := 1;
d15 := 1;
d16 := 1;
d17 := 1;
d18 := 0;
d19 := 0;
d1 := 1;
d20 := 0.1;
d21 := 0.0000011;
d22 := 0.00005;
d23 := 0.1;
d24 := 0.0000011;
d2 := 1;
d3 := 0;
d4 := 0;
d5 := 1;
d6 := 1;
d7 := 1;
d8 := 1;
d9 := 1;
Ibatery := 5.69999;
lastsubsamp := 0.0001;
lastTsOut := 0.125137;
Pcons := 487562;
Pelectro := 0;
PFcel := 0;
Pgen := 0;
ptanque := 800000;
SOC := 41.5;
Tsubsamp := 0.0001;
variableR1_R := 1005;
Vc2 := 50;

equation

batteryAndBockBoost_battery_Qt=Qt1/1000;
batteryAndBockBoost_battery2_Qt=Qt2/1000;
batteryAndBockBoost_battery1_Qt=Qt3/1000;

  boost_panel1_panel_n_i = fsolve1(boost_panel1_capacitor1_v);
  boost_panel1_mmt1_pot = boost_panel1_capacitor1_v*boost_panel1_boost1_inductor_i;
  der(boost_panel1_mmt1_potActFiltrada) = (boost_panel1_mmt1_pot-boost_panel1_mmt1_potActFiltrada)*100;
  boost_panel1_mmt1_y = pre(boost_panel1_mmt1_vref);
  boost_panel1_cont_pwm1_vgate = pre(boost_panel1_cont_pwm1_vout);
  boost_panel1_boost1_idealDiode_off = pre(d0);
  batteryAndBockBoost_bookBoostDB_inductor_n_i = (-batteryAndBockBoost_bookBoostDB_inductor_i);
  batteryAndBockBoost_bookBoostDB_idealDiode_off = pre(d5);
  batteryAndBockBoost_bookBoostDB_idealDiode1_off = pre(d8);
  batteryAndBockBoost_bookBoostDB_Sw2 = pre(batteryAndBockBoost_bookBoostPWMcontrol1_Output);
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_control = 1-batteryAndBockBoost_bookBoostDB_Sw2;
  batteryAndBockBoost_voltageControl_Err = 56-capacitor2_v;
  der(batteryAndBockBoost_voltageControl_intErr) = batteryAndBockBoost_voltageControl_Err;
  batteryAndBockBoost_voltageControl_IrefCont = 0.95*batteryAndBockBoost_voltageControl_Err+90*batteryAndBockBoost_voltageControl_intErr;
  batteryAndBockBoost_bookBoostPWMcontrol1_yref = pre(batteryAndBockBoost_voltageControl_Sat)*batteryAndBockBoost_voltageControl_IrefCont+pre(batteryAndBockBoost_voltageControl_SatLevel);
  batteryAndBockBoost_battery_E = 12.6463-1.98/(6-batteryAndBockBoost_battery_Qt)+0.66*exp((-2884.61*batteryAndBockBoost_battery_Qt));
  batteryAndBockBoost_battery_SoC = (1-batteryAndBockBoost_battery_Qt/6)*100;
  batteryAndBockBoost_battery1_E = 12.6463-1.98/(6-batteryAndBockBoost_battery1_Qt)+0.66*exp((-2884.61*batteryAndBockBoost_battery1_Qt));
  batteryAndBockBoost_battery1_SoC = (1-batteryAndBockBoost_battery1_Qt/6)*100;
  batteryAndBockBoost_battery2_E = 12.6463-1.98/(6-batteryAndBockBoost_battery2_Qt)+0.66*exp((-2884.61*batteryAndBockBoost_battery2_Qt));
  batteryAndBockBoost_u = (1-batteryAndBockBoost_battery2_Qt/6)*100;
  boost_panel2_panel_n_i = fsolve2(boost_panel2_capacitor1_v);
  boost_panel2_mmt1_pot = boost_panel2_capacitor1_v*boost_panel2_boost1_inductor_i;
  der(boost_panel2_mmt1_potActFiltrada) = (boost_panel2_mmt1_pot-boost_panel2_mmt1_potActFiltrada)*100;
  boost_panel2_mmt1_y = pre(boost_panel2_mmt1_vref);
  boost_panel2_cont_pwm1_vgate = pre(boost_panel2_cont_pwm1_vout);
  boost_panel2_boost1_idealDiode_off = pre(d15);
  fCellAndElectro_electComp_compresor_p_tanq = 1e-05*fCellAndElectro_tanque_p_tanque;
  variableR1_n_i = (-capacitor2_v*(1/(variableR1_R)));
  PCons_Sensor_pc_i = -variableR1_n_i;
  boost_panel1_capacitor1_n_i = (-boost_panel1_panel_n_i)+boost_panel1_boost1_inductor_i;
  batteryAndBockBoost_battery2_pin_p_i = batteryAndBockBoost_bookBoostDB_inductor_n_i;
  boost_panel2_capacitor1_n_i = boost_panel2_boost1_inductor_i-boost_panel2_panel_n_i;
  batteryAndBockBoost_bookBoostPWMcontrol1_Err = batteryAndBockBoost_bookBoostPWMcontrol1_yref-(-batteryAndBockBoost_battery2_pin_p_i);
  batteryAndBockBoost_battery1_pin_n_v = batteryAndBockBoost_battery_E-0.25*(-batteryAndBockBoost_battery2_pin_p_i);
  batteryAndBockBoost_battery1_v = batteryAndBockBoost_battery1_E-0.25*(-batteryAndBockBoost_battery2_pin_p_i);
  batteryAndBockBoost_battery2_v = batteryAndBockBoost_battery2_E-0.25*(-batteryAndBockBoost_battery2_pin_p_i);
  PCons_Sensor_product_y = capacitor2_v*PCons_Sensor_pc_i;
  algSuperv_deltaP = algSuperv_Pgenfilt-PCons_Sensor_product_y;
  fCellAndElectro_electComp_electrolizadorAnda1_u = algSuperv_EB*(algSuperv_deltaP*(1-algSuperv_satElect)+algSuperv_satElect*200);
  algSuperv_Pref_FC = algSuperv_FC*((-algSuperv_deltaP*(1-algSuperv_satFC))+200*algSuperv_satFC);
  fCellAndElectro_fuellCell_i = algSuperv_Pref_FC*(1/(capacitor2_v));
  fCellAndElectro_fuellCell_IFC = fsolve3(algSuperv_Pref_FC);
  fCellAndElectro_fuellCell_fH2_FC = 47*fCellAndElectro_fuellCell_IFC/2/96487;
  fCellAndElectro_electComp_pin_n_i = (-(1/(capacitor2_v))*fCellAndElectro_electComp_electrolizadorAnda1_u);
  fCellAndElectro_electComp_electrolizadorAnda1_Ucell = fsolve4(fCellAndElectro_electComp_electrolizadorAnda1_u);
  fCellAndElectro_electComp_compresor_FH2_electrolizador = fCellAndElectro_electComp_electrolizadorAnda1_u/(fCellAndElectro_electComp_electrolizadorAnda1_Ucell*2*96485);
  fCellAndElectro_pin_p_i = (-fCellAndElectro_fuellCell_i)-fCellAndElectro_electComp_pin_n_i;
  fCellAndElectro_pin_n_i = fCellAndElectro_electComp_pin_n_i+fCellAndElectro_fuellCell_i;
  batteryAndBockBoost_battery2_pin_n_v = batteryAndBockBoost_battery1_v+batteryAndBockBoost_battery1_pin_n_v;
  boost_panel1_boost1_idealDiode_s = (1/((-1)-(0.99998)*d1*d4-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(9.9999e-06)*d4+(0.99999)*d2))*(1/((-1e-05)-(0.99999)*d1))*((-(0.99998)*d3*d1*boost_panel1_boost1_inductor_i)-(0.99998)*d1*d4*capacitor2_v+(0.99999)*d1*boost_panel1_boost1_inductor_i-(9.9999e-06)*d4*capacitor2_v-(9.9999e-06)*d3*boost_panel1_boost1_inductor_i-(9.9999e-06)*d1*capacitor2_v+(1e-05)*boost_panel1_boost1_inductor_i-(1e-10)*capacitor2_v);
  boost_panel1_boost1_idealDiode_n_i = (1/((-1)-(0.99998)*d1*d4-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(9.9999e-06)*d4+(0.99999)*d2))*((0.99998)*d3*d1*boost_panel1_boost1_inductor_i+(0.99998)*d1*d4*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i+(9.9999e-06)*d4*capacitor2_v+(9.9999e-06)*d3*boost_panel1_boost1_inductor_i+(9.9999e-06)*d1*capacitor2_v-(1e-05)*boost_panel1_boost1_inductor_i+(1e-10)*capacitor2_v)*(1/((-1e-05)-(0.99999)*d1))*(1-(0.99999)*d2);
  boost_panel1_boost1_idealDiode_v = (1/((-1)-(0.99998)*d1*d4-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(9.9999e-06)*d4+(0.99999)*d2))*((0.99998)*d3*d1*boost_panel1_boost1_inductor_i+(0.99998)*d1*d4*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i+(9.9999e-06)*d4*capacitor2_v+(9.9999e-06)*d3*boost_panel1_boost1_inductor_i+(9.9999e-06)*d1*capacitor2_v-(1e-05)*boost_panel1_boost1_inductor_i+(1e-10)*capacitor2_v);
  boost_panel1_boost1_inductor_n_v = (-(1/((-1)-(0.99998)*d1*d4-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(9.9999e-06)*d4+(0.99999)*d2))*((-1)+(0.99999)*d3)*((0.99999)*d2*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i-capacitor2_v));
  boost_panel1_boost1_controlledIdealClosingSwitch_s = (1/((-1)-(0.99998)*d1*d4-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(9.9999e-06)*d4+(0.99999)*d2))*((0.99999)*d2*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i-capacitor2_v);
  boost_panel1_boost1_pin_ground_i = (1/((-1)-(0.99998)*d1*d4-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(9.9999e-06)*d4+(0.99999)*d2))*((0.99999)*d2*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i-capacitor2_v)*((-1e-05)-(0.99999)*d4);
  batteryAndBockBoost_bookBoostDB_idealDiode_s = (-((-1)+(0.99999)*d13)*(1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((9.9999e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99998)*d9*d12*capacitor2_v-(0.99999)*d11*capacitor2_v-(1e-05)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v-(0.99999)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(1)*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d11*d10*capacitor2_v));
  batteryAndBockBoost_bookBoostDB_idealDiode_n_i = (-((-1)+(0.99999)*d13)*(1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((-1)+(0.99999)*d7)*((9.9999e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99998)*d9*d12*capacitor2_v-(0.99999)*d11*capacitor2_v-(1e-05)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v-(0.99999)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(1)*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d11*d10*capacitor2_v));
  batteryAndBockBoost_bookBoostDB_idealDiode1_s = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((-(9.9998e-06)*d11*capacitor2_v*d7)-(9.9998e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d13*capacitor2_v*d7+(9.9998e-11)*d11*d6*capacitor2_v-(9.9999e-11)*d6*capacitor2_v+(9.9999e-11)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(0.99998)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(0.99998)*d9*d13*capacitor2_v+(9.9999e-06)*capacitor2_v*d7+(0.99997)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(0.99997)*d11*d9*d13*capacitor2_v+(9.9997e-06)*d11*d13*capacitor2_v*d7-(9.9998e-06)*d14*d9*capacitor2_v-(9.9999e-11)*d14*capacitor2_v+(9.9999e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9998e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(9.9997e-06)*d14*d11*d6*capacitor2_v-(9.9997e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-06)*d11*capacitor2_v-(6.31089e-30)*d12*d13*capacitor2_v-(1e-10)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99996)*d14*d11*d9*d6*capacitor2_v+(0.99998)*d11*d9*capacitor2_v-(0.99999)*d9*capacitor2_v+(0.99996)*d11*d9*d13*capacitor2_v*d7+(9.9998e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i-(9.9999e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i-(0.99997)*d14*d9*d6*capacitor2_v-(9.9998e-06)*d14*d6*capacitor2_v+(0.99997)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(0.99996)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(9.9998e-11)*d14*d11*capacitor2_v-(0.99997)*d11*d9*capacitor2_v*d7+(9.9997e-06)*d14*d11*d9*capacitor2_v+(9.9997e-06)*d11*d9*d6*capacitor2_v-(9.9997e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d9*d6*capacitor2_v-(1e-05)*capacitor2_v-(0.99997)*d9*d13*capacitor2_v*d7+(0.99998)*d9*capacitor2_v*d7+(9.9999e-06)*d13*capacitor2_v-(9.9999e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(9.9998e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(9.9998e-06)*d11*d13*capacitor2_v)*(1/((-1e-05)-(0.99999)*d9));
  batteryAndBockBoost_bookBoostDB_idealDiode1_n_i = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((9.9998e-06)*d11*capacitor2_v*d7+(9.9998e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d13*capacitor2_v*d7-(9.9998e-11)*d11*d6*capacitor2_v+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(0.99998)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(0.99998)*d9*d13*capacitor2_v-(9.9999e-06)*capacitor2_v*d7-(0.99997)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(0.99997)*d11*d9*d13*capacitor2_v-(9.9997e-06)*d11*d13*capacitor2_v*d7+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-11)*d14*capacitor2_v-(9.9999e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i-(9.9998e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-06)*d11*capacitor2_v+(6.31089e-30)*d12*d13*capacitor2_v+(1e-10)*batteryAndBockBoost_bookBoostDB_inductor_n_i-(0.99996)*d14*d11*d9*d6*capacitor2_v-(0.99998)*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(0.99996)*d11*d9*d13*capacitor2_v*d7-(9.9998e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99997)*d14*d9*d6*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v-(0.99997)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(0.99996)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9998e-11)*d14*d11*capacitor2_v+(0.99997)*d11*d9*capacitor2_v*d7-(9.9997e-06)*d14*d11*d9*capacitor2_v-(9.9997e-06)*d11*d9*d6*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d9*d6*capacitor2_v+(1e-05)*capacitor2_v+(0.99997)*d9*d13*capacitor2_v*d7-(0.99998)*d9*capacitor2_v*d7-(9.9999e-06)*d13*capacitor2_v+(9.9999e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(9.9998e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(9.9998e-06)*d11*d13*capacitor2_v)*(1/((-1e-05)-(0.99999)*d9))*(1-(0.99999)*d10);
  batteryAndBockBoost_bookBoostDB_idealDiode1_v = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((9.9998e-06)*d11*capacitor2_v*d7+(9.9998e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d13*capacitor2_v*d7-(9.9998e-11)*d11*d6*capacitor2_v+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(0.99998)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(0.99998)*d9*d13*capacitor2_v-(9.9999e-06)*capacitor2_v*d7-(0.99997)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(0.99997)*d11*d9*d13*capacitor2_v-(9.9997e-06)*d11*d13*capacitor2_v*d7+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-11)*d14*capacitor2_v-(9.9999e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i-(9.9998e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-06)*d11*capacitor2_v+(6.31089e-30)*d12*d13*capacitor2_v+(1e-10)*batteryAndBockBoost_bookBoostDB_inductor_n_i-(0.99996)*d14*d11*d9*d6*capacitor2_v-(0.99998)*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(0.99996)*d11*d9*d13*capacitor2_v*d7-(9.9998e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99997)*d14*d9*d6*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v-(0.99997)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(0.99996)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9998e-11)*d14*d11*capacitor2_v+(0.99997)*d11*d9*capacitor2_v*d7-(9.9997e-06)*d14*d11*d9*capacitor2_v-(9.9997e-06)*d11*d9*d6*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d9*d6*capacitor2_v+(1e-05)*capacitor2_v+(0.99997)*d9*d13*capacitor2_v*d7-(0.99998)*d9*capacitor2_v*d7-(9.9999e-06)*d13*capacitor2_v+(9.9999e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(9.9998e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(9.9998e-06)*d11*d13*capacitor2_v);
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_s = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((-(9.9998e-06)*d11*capacitor2_v*d7)-(9.9998e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d13*capacitor2_v*d7+(9.9998e-11)*d11*d6*capacitor2_v-(9.9999e-11)*d6*capacitor2_v+(9.9999e-11)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(0.99998)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(0.99998)*d9*d13*capacitor2_v+(9.9999e-06)*capacitor2_v*d7+(0.99997)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(0.99997)*d11*d9*d13*capacitor2_v+(9.9997e-06)*d11*d13*capacitor2_v*d7-(9.9998e-06)*d14*d9*capacitor2_v-(9.9999e-11)*d14*capacitor2_v+(9.9999e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9998e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(9.9997e-06)*d14*d11*d6*capacitor2_v-(9.9997e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-06)*d11*capacitor2_v-(6.31089e-30)*d12*d13*capacitor2_v-(1e-10)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99996)*d14*d11*d9*d6*capacitor2_v+(0.99998)*d11*d9*capacitor2_v-(0.99999)*d9*capacitor2_v+(0.99996)*d11*d9*d13*capacitor2_v*d7+(9.9998e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i-(9.9999e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i-(0.99997)*d14*d9*d6*capacitor2_v-(9.9998e-06)*d14*d6*capacitor2_v+(0.99997)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(0.99996)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(9.9998e-11)*d14*d11*capacitor2_v-(0.99997)*d11*d9*capacitor2_v*d7+(9.9997e-06)*d14*d11*d9*capacitor2_v+(9.9997e-06)*d11*d9*d6*capacitor2_v-(9.9997e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d9*d6*capacitor2_v-(1e-05)*capacitor2_v-(0.99997)*d9*d13*capacitor2_v*d7+(0.99998)*d9*capacitor2_v*d7+(9.9999e-06)*d13*capacitor2_v-(9.9999e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(9.9998e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(9.9998e-06)*d11*d13*capacitor2_v)*(1/((-1)+(0.99999)*d11));
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_n_i = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((9.9998e-06)*d11*capacitor2_v*d7+(9.9998e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d13*capacitor2_v*d7-(9.9998e-11)*d11*d6*capacitor2_v+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(0.99998)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(0.99998)*d9*d13*capacitor2_v-(9.9999e-06)*capacitor2_v*d7-(0.99997)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(0.99997)*d11*d9*d13*capacitor2_v-(9.9997e-06)*d11*d13*capacitor2_v*d7+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-11)*d14*capacitor2_v-(9.9999e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i-(9.9998e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-06)*d11*capacitor2_v+(6.31089e-30)*d12*d13*capacitor2_v+(1e-10)*batteryAndBockBoost_bookBoostDB_inductor_n_i-(0.99996)*d14*d11*d9*d6*capacitor2_v-(0.99998)*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(0.99996)*d11*d9*d13*capacitor2_v*d7-(9.9998e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99997)*d14*d9*d6*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v-(0.99997)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(0.99996)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9998e-11)*d14*d11*capacitor2_v+(0.99997)*d11*d9*capacitor2_v*d7-(9.9997e-06)*d14*d11*d9*capacitor2_v-(9.9997e-06)*d11*d9*d6*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d9*d6*capacitor2_v+(1e-05)*capacitor2_v+(0.99997)*d9*d13*capacitor2_v*d7-(0.99998)*d9*capacitor2_v*d7-(9.9999e-06)*d13*capacitor2_v+(9.9999e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(9.9998e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(9.9998e-06)*d11*d13*capacitor2_v)*(1e-05+(0.99999)*d12)*(1/((-1)+(0.99999)*d11));
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_v = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((9.9998e-06)*d11*capacitor2_v*d7+(9.9998e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d13*capacitor2_v*d7-(9.9998e-11)*d11*d6*capacitor2_v+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(0.99998)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(0.99998)*d9*d13*capacitor2_v-(9.9999e-06)*capacitor2_v*d7-(0.99997)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(0.99997)*d11*d9*d13*capacitor2_v-(9.9997e-06)*d11*d13*capacitor2_v*d7+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-11)*d14*capacitor2_v-(9.9999e-11)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i-(9.9998e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-06)*d11*capacitor2_v+(6.31089e-30)*d12*d13*capacitor2_v+(1e-10)*batteryAndBockBoost_bookBoostDB_inductor_n_i-(0.99996)*d14*d11*d9*d6*capacitor2_v-(0.99998)*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(0.99996)*d11*d9*d13*capacitor2_v*d7-(9.9998e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99997)*d14*d9*d6*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v-(0.99997)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13+(0.99996)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6*d13-(9.9998e-11)*d14*d11*capacitor2_v+(0.99997)*d11*d9*capacitor2_v*d7-(9.9997e-06)*d14*d11*d9*capacitor2_v-(9.9997e-06)*d11*d9*d6*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13-(9.9998e-06)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i*d13+(9.9998e-06)*d9*d6*capacitor2_v+(1e-05)*capacitor2_v+(0.99997)*d9*d13*capacitor2_v*d7-(0.99998)*d9*capacitor2_v*d7-(9.9999e-06)*d13*capacitor2_v+(9.9999e-06)*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6-(9.9998e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i*d6+(9.9998e-06)*d11*d13*capacitor2_v);
  batteryAndBockBoost_bookBoostDB_idealDiode_n_v = (-((-1)+(0.99999)*d13)*(1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((-1e-05)-(0.99999)*d6)*((9.9999e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99998)*d9*d12*capacitor2_v-(0.99999)*d11*capacitor2_v-(1e-05)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v-(0.99999)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(1)*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d11*d10*capacitor2_v));
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_s = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((-1e-05)-(0.99999)*d6)*((9.9999e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99998)*d9*d12*capacitor2_v-(0.99999)*d11*capacitor2_v-(1e-05)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v-(0.99999)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(1)*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d11*d10*capacitor2_v);
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_n_i = (1/((-2e-05)-(1.99996e-05)*d9*d6+(0.99996)*d9*d12*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d11*d9*d6+(0.99996)*d11*d9*d13*d7-(9.9999e-11)*d14-(0.99997)*d9*d13*d7+(0.99998)*d11*d9-(9.9998e-06)*d12*d6+(1.99998e-05)*d11+(9.9997e-06)*d9*d12*d13+(0.99996)*d11*d10*d6*d13-(0.99999)*d9+(0.99996)*d14*d11*d9*d6-(9.9998e-06)*d11*d7-(9.9998e-06)*d9*d12+(9.9997e-06)*d14*d11*d6-(0.99997)*d11*d10*d6-(9.9998e-06)*d14*d9-(0.99997)*d11*d6*d13+(0.99998)*d6*d13-(0.99997)*d10*d6*d13+(0.99998)*d10*d6+(9.9997e-06)*d9*d6*d13-(9.9998e-06)*d10*d13-(9.9999e-11)*d12-(9.9998e-06)*d11*d10+(9.9998e-11)*d14*d11+(9.9999e-06)*d10+(9.9997e-06)*d14*d11*d9+(9.9997e-06)*d11*d10*d13+(0.99998)*d9*d7-(0.99999)*d6-(0.99997)*d14*d9*d6-(9.9998e-06)*d14*d6-(0.99997)*d11*d9*d7-(0.99997)*d9*d12*d6+(9.9997e-06)*d11*d13*d7+(9.9998e-11)*d12*d13+(1.99998e-05)*d13-(9.9998e-06)*d13*d7-(0.99997)*d11*d9*d13+(9.9997e-06)*d12*d6*d13+(0.99998)*d11*d6+(9.9999e-06)*d7+(0.99998)*d9*d13))*((-1e-05)-(0.99999)*d14)*((-1e-05)-(0.99999)*d6)*((9.9999e-06)*d11*batteryAndBockBoost_bookBoostDB_inductor_n_i+(0.99998)*d9*d12*capacitor2_v-(0.99999)*d11*capacitor2_v-(1e-05)*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v-(0.99999)*d9*batteryAndBockBoost_bookBoostDB_inductor_n_i+(1)*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d11*d10*capacitor2_v);
  boost_panel2_boost1_idealDiode_v = (-(1/((-1)+(0.99999)*d18-(9.9999e-06)*d16-(0.99998)*d16*d19-(9.9999e-06)*d19+(0.99999)*d17-(0.99998)*d18*d17))*((-1e-05)-(0.99999)*d16)*((0.99999)*d18*boost_panel2_boost1_inductor_i+(0.99999)*d19*capacitor2_v-boost_panel2_boost1_inductor_i+(1e-05)*capacitor2_v));
  boost_panel2_boost1_idealDiode_s = (1/((-1)+(0.99999)*d18-(9.9999e-06)*d16-(0.99998)*d16*d19-(9.9999e-06)*d19+(0.99999)*d17-(0.99998)*d18*d17))*((0.99999)*d18*boost_panel2_boost1_inductor_i+(0.99999)*d19*capacitor2_v-boost_panel2_boost1_inductor_i+(1e-05)*capacitor2_v);
  boost_panel2_boost1_inductor_n_v = (1/((-1)+(0.99999)*d18-(9.9999e-06)*d16-(0.99998)*d16*d19-(9.9999e-06)*d19+(0.99999)*d17-(0.99998)*d18*d17))*((9.9999e-06)*d18*boost_panel2_boost1_inductor_i+(0.99999)*d18*capacitor2_v+(0.99998)*d18*d16*boost_panel2_boost1_inductor_i-(0.99998)*d18*d17*capacitor2_v+(0.99999)*d17*capacitor2_v-(1e-05)*boost_panel2_boost1_inductor_i-capacitor2_v-(0.99999)*d16*boost_panel2_boost1_inductor_i);
  boost_panel2_boost1_controlledIdealClosingSwitch_s = ((-(9.9999e-06)*d18*boost_panel2_boost1_inductor_i)-(0.99999)*d18*capacitor2_v-(0.99998)*d18*d16*boost_panel2_boost1_inductor_i+(0.99998)*d18*d17*capacitor2_v-(0.99999)*d17*capacitor2_v+(1e-05)*boost_panel2_boost1_inductor_i+capacitor2_v+(0.99999)*d16*boost_panel2_boost1_inductor_i)*(1/((-1)+(0.99999)*d18-(9.9999e-06)*d16-(0.99998)*d16*d19-(9.9999e-06)*d19+(0.99999)*d17-(0.99998)*d18*d17))*(1/((-1)+(0.99999)*d18));
  boost_panel2_boost1_pin_ground_i = (1/((-1)+(0.99999)*d18-(9.9999e-06)*d16-(0.99998)*d16*d19-(9.9999e-06)*d19+(0.99999)*d17-(0.99998)*d18*d17))*((-(0.99998)*d16*d19*boost_panel2_boost1_inductor_i)+(9.9999e-11)*d18*boost_panel2_boost1_inductor_i+(0.99998)*d19*d17*capacitor2_v+(9.9999e-06)*d18*capacitor2_v+(9.9998e-06)*d18*d16*boost_panel2_boost1_inductor_i-(0.99997)*d18*d19*d17*capacitor2_v+(9.9998e-06)*d18*d19*boost_panel2_boost1_inductor_i-(0.99999)*d19*capacitor2_v-(9.9998e-06)*d18*d17*capacitor2_v-(9.9999e-06)*d19*boost_panel2_boost1_inductor_i+(0.99997)*d18*d16*d19*boost_panel2_boost1_inductor_i+(0.99998)*d18*d19*capacitor2_v+(9.9999e-06)*d17*capacitor2_v-(1e-10)*boost_panel2_boost1_inductor_i-(1e-05)*capacitor2_v-(9.9999e-06)*d16*boost_panel2_boost1_inductor_i)*(1/((-1)+(0.99999)*d18));
  boost_panel2_boost1_idealDiode_n_i = (1/((-1)+(0.99999)*d18-(9.9999e-06)*d16-(0.99998)*d16*d19-(9.9999e-06)*d19+(0.99999)*d17-(0.99998)*d18*d17))*((-1)+(0.99999)*d17)*((0.99999)*d18*boost_panel2_boost1_inductor_i+(0.99999)*d19*capacitor2_v-boost_panel2_boost1_inductor_i+(1e-05)*capacitor2_v);
  batteryAndBockBoost_bookBoostDB_pin_p_out_i = batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_n_i+batteryAndBockBoost_bookBoostDB_idealDiode1_n_i;
  batteryAndBockBoost_bookBoostDB_pin_n_i = batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_n_i-batteryAndBockBoost_bookBoostDB_idealDiode_n_i;
  PGen_Sensor_pc_i = (-boost_panel1_boost1_idealDiode_n_i)-boost_panel2_boost1_idealDiode_n_i;
  capacitor2_n_i = (-PGen_Sensor_pc_i)+fCellAndElectro_pin_p_i+batteryAndBockBoost_bookBoostDB_pin_p_out_i+PCons_Sensor_pc_i;
  boost_panel2_pin_n_i = boost_panel2_capacitor1_n_i+boost_panel2_panel_n_i+boost_panel2_boost1_pin_ground_i;
  boost_panel1_pin_n_i = boost_panel1_capacitor1_n_i+boost_panel1_panel_n_i+boost_panel1_boost1_pin_ground_i;
  batteryAndBockBoost_pin_n_i = batteryAndBockBoost_bookBoostDB_pin_n_i-batteryAndBockBoost_battery2_pin_p_i;
  batteryAndBockBoost_bookBoostDB_inductor_p_v = batteryAndBockBoost_battery2_v+batteryAndBockBoost_battery2_pin_n_v;
  PGen_Sensor_product_y = capacitor2_v*PGen_Sensor_pc_i;
  ground_p_i = (-boost_panel2_pin_n_i)-fCellAndElectro_pin_n_i-variableR1_n_i-capacitor2_n_i-boost_panel1_pin_n_i-batteryAndBockBoost_pin_n_i;
  der(fCellAndElectro_tanque_p_tanque) = 26022.8*(fCellAndElectro_electComp_compresor_FH2_electrolizador-fCellAndElectro_fuellCell_fH2_FC);
  fCellAndElectro_electComp_compresor_PotConsumida = fCellAndElectro_electComp_compresor_FH2_electrolizador*((((fCellAndElectro_electComp_compresor_p_tanq/1)^0.285714))-1)*313*8.314*1.4/0.4/0.72;
  der(capacitor2_v) = (-(200)*capacitor2_n_i);
  der(boost_panel2_capacitor1_v) = (-(2127.66)*boost_panel2_capacitor1_n_i);
  boost_panel2_boost1_inductor_v = boost_panel2_capacitor1_v-boost_panel2_boost1_inductor_n_v;
  der(boost_panel2_boost1_inductor_i) = (66.6667)*boost_panel2_boost1_inductor_v;
  boost_panel2_boost1_idealDiode_LossPower = boost_panel2_boost1_idealDiode_v*(-boost_panel2_boost1_idealDiode_n_i);
  boost_panel2_boost1_controlledIdealClosingSwitch_LossPower = (boost_panel2_boost1_inductor_n_v)*(-boost_panel2_boost1_pin_ground_i);
  der(boost_panel1_capacitor1_v) = (-(2127.66)*boost_panel1_capacitor1_n_i);
  boost_panel1_boost1_inductor_v = boost_panel1_capacitor1_v-boost_panel1_boost1_inductor_n_v;
  der(boost_panel1_boost1_inductor_i) = (66.6667)*boost_panel1_boost1_inductor_v;
  boost_panel1_boost1_idealDiode_LossPower = boost_panel1_boost1_idealDiode_v*(-boost_panel1_boost1_idealDiode_n_i);
  boost_panel1_boost1_controlledIdealClosingSwitch_LossPower = (boost_panel1_boost1_inductor_n_v)*(-boost_panel1_boost1_pin_ground_i);
  der(batteryAndBockBoost_bookBoostPWMcontrol1_intErr) = batteryAndBockBoost_bookBoostPWMcontrol1_Err;
  batteryAndBockBoost_bookBoostDB_inductor_v = batteryAndBockBoost_bookBoostDB_inductor_p_v-batteryAndBockBoost_bookBoostDB_idealDiode_n_v;
  der(batteryAndBockBoost_bookBoostDB_inductor_i) = (66.6667)*batteryAndBockBoost_bookBoostDB_inductor_v;
  batteryAndBockBoost_bookBoostDB_idealDiode_LossPower = (-batteryAndBockBoost_bookBoostDB_idealDiode_n_v)*(-batteryAndBockBoost_bookBoostDB_idealDiode_n_i);
  batteryAndBockBoost_bookBoostDB_idealDiode1_LossPower = batteryAndBockBoost_bookBoostDB_idealDiode1_v*(-batteryAndBockBoost_bookBoostDB_idealDiode1_n_i);
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_LossPower = batteryAndBockBoost_bookBoostDB_idealDiode_n_v*(-batteryAndBockBoost_bookBoostDB_idealClosingSwitch2_n_i);
  batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_LossPower = batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_v*(-batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_n_i);

der(Qt1)=1000* (-batteryAndBockBoost_battery2_pin_p_i)/3600;
der(Qt2)=1000* (-batteryAndBockBoost_battery2_pin_p_i)/3600;
der(Qt3)=1000* (-batteryAndBockBoost_battery2_pin_p_i)/3600;

//  der(batteryAndBockBoost_battery_Qt) = (-batteryAndBockBoost_battery2_pin_p_i)/3600;
//  der(batteryAndBockBoost_battery2_Qt) = (-batteryAndBockBoost_battery2_pin_p_i)/3600;
//  der(batteryAndBockBoost_battery1_Qt) = (-batteryAndBockBoost_battery2_pin_p_i)/3600;


  der(algSuperv_Pgenfilt) = (PGen_Sensor_product_y-algSuperv_Pgenfilt)*100;
algorithm
    when time>pre(d20) then 
      d20:=time+boost_panel1_mmt1_Ts;
      boost_panel1_mmt1_potprev:=boost_panel1_mmt1_potact;
      boost_panel1_mmt1_potact:=boost_panel1_mmt1_potActFiltrada;
      boost_panel1_mmt1_prevu:=boost_panel1_mmt1_actu;
      boost_panel1_mmt1_actu:=boost_panel1_capacitor1_v;
      boost_panel1_mmt1_deltau:=boost_panel1_mmt1_actu-boost_panel1_mmt1_prevu;
      boost_panel1_mmt1_deltap:=boost_panel1_mmt1_potact-boost_panel1_mmt1_potprev;
      if abs(boost_panel1_mmt1_deltau)>0.1*boost_panel1_mmt1_deltaVpvRefPanel then 
        if abs(boost_panel1_mmt1_deltap)>0.2 then 
          if boost_panel1_mmt1_deltap/boost_panel1_mmt1_deltau>0 then 
            boost_panel1_mmt1_vref:=boost_panel1_mmt1_vref+boost_panel1_mmt1_deltaVpvRefPanel;
          end if;
          if boost_panel1_mmt1_deltap/boost_panel1_mmt1_deltau<0 then 
            boost_panel1_mmt1_vref:=boost_panel1_mmt1_vref-boost_panel1_mmt1_deltaVpvRefPanel;
          end if;
        end if;
      end if;
    end when;
    when time>pre(d21) then 
      d21:=time+boost_panel1_cont_pwm1_T;
      boost_panel1_cont_pwm1_dc:=0.85-(boost_panel1_mmt1_y-boost_panel1_capacitor1_v)*0.1;
      if boost_panel1_cont_pwm1_dc>0.98 then 
        boost_panel1_cont_pwm1_dc:=0.98;
      elseif boost_panel1_cont_pwm1_dc<0.3 then 
        boost_panel1_cont_pwm1_dc:=0.3;
      end if;
      boost_panel1_cont_pwm1_tnext:=time+boost_panel1_cont_pwm1_T*boost_panel1_cont_pwm1_dc;
      boost_panel1_cont_pwm1_vout:=1;
    end when;
    when time>boost_panel1_cont_pwm1_tnext then 
      boost_panel1_cont_pwm1_vout:=0;
    end when;
    when time>pre(d22) then 
      d22:=time+batteryAndBockBoost_bookBoostPWMcontrol1_T;
      batteryAndBockBoost_bookBoostPWMcontrol1_DC:=0.5+batteryAndBockBoost_bookBoostPWMcontrol1_Kp*batteryAndBockBoost_bookBoostPWMcontrol1_Err+batteryAndBockBoost_bookBoostPWMcontrol1_Ki*batteryAndBockBoost_bookBoostPWMcontrol1_intErr;
      if batteryAndBockBoost_bookBoostPWMcontrol1_DC>0.98 then 
        batteryAndBockBoost_bookBoostPWMcontrol1_DC:=0.98;
      elseif batteryAndBockBoost_bookBoostPWMcontrol1_DC<0.1 then 
        batteryAndBockBoost_bookBoostPWMcontrol1_DC:=0.1;
      end if;
      batteryAndBockBoost_bookBoostPWMcontrol1_tnext:=time+batteryAndBockBoost_bookBoostPWMcontrol1_T*batteryAndBockBoost_bookBoostPWMcontrol1_DC;
      batteryAndBockBoost_bookBoostPWMcontrol1_Output:=1;
    end when;
    when time>batteryAndBockBoost_bookBoostPWMcontrol1_tnext then 
      batteryAndBockBoost_bookBoostPWMcontrol1_Output:=0;
    end when;
    when batteryAndBockBoost_voltageControl_IrefCont>batteryAndBockBoost_voltageControl_Imax then 
      batteryAndBockBoost_voltageControl_Sat:=0;
      batteryAndBockBoost_voltageControl_SatLevel:=batteryAndBockBoost_voltageControl_Imax;
    end when;
    when batteryAndBockBoost_voltageControl_IrefCont<batteryAndBockBoost_voltageControl_Imax then 
      batteryAndBockBoost_voltageControl_Sat:=1;
      batteryAndBockBoost_voltageControl_SatLevel:=0;
    end when;
    when batteryAndBockBoost_voltageControl_IrefCont<((-batteryAndBockBoost_voltageControl_Imax)) then 
      batteryAndBockBoost_voltageControl_Sat:=0;
      batteryAndBockBoost_voltageControl_SatLevel:=((-batteryAndBockBoost_voltageControl_Imax));
    end when;
    when batteryAndBockBoost_voltageControl_IrefCont>((-batteryAndBockBoost_voltageControl_Imax)) then 
      batteryAndBockBoost_voltageControl_Sat:=1;
      batteryAndBockBoost_voltageControl_SatLevel:=0;
    end when;
    when time>pre(d23) then 
      d23:=time+boost_panel2_mmt1_Ts;
      boost_panel2_mmt1_potprev:=boost_panel2_mmt1_potact;
      boost_panel2_mmt1_potact:=boost_panel2_mmt1_potActFiltrada;
      boost_panel2_mmt1_prevu:=boost_panel2_mmt1_actu;
      boost_panel2_mmt1_actu:=boost_panel2_capacitor1_v;
      boost_panel2_mmt1_deltau:=boost_panel2_mmt1_actu-boost_panel2_mmt1_prevu;
      boost_panel2_mmt1_deltap:=boost_panel2_mmt1_potact-boost_panel2_mmt1_potprev;
      if abs(boost_panel2_mmt1_deltau)>0.1*boost_panel2_mmt1_deltaVpvRefPanel then 
        if abs(boost_panel2_mmt1_deltap)>0.2 then 
          if boost_panel2_mmt1_deltap/boost_panel2_mmt1_deltau>0 then 
            boost_panel2_mmt1_vref:=boost_panel2_mmt1_vref+boost_panel2_mmt1_deltaVpvRefPanel;
          end if;
          if boost_panel2_mmt1_deltap/boost_panel2_mmt1_deltau<0 then 
            boost_panel2_mmt1_vref:=boost_panel2_mmt1_vref-boost_panel2_mmt1_deltaVpvRefPanel;
          end if;
        end if;
      end if;
    end when;
    when time>pre(d24) then 
      d24:=time+boost_panel2_cont_pwm1_T;
      boost_panel2_cont_pwm1_dc:=0.85-(boost_panel2_mmt1_y-boost_panel2_capacitor1_v)*0.1;
      if boost_panel2_cont_pwm1_dc>0.98 then 
        boost_panel2_cont_pwm1_dc:=0.98;
      elseif boost_panel2_cont_pwm1_dc<0.3 then 
        boost_panel2_cont_pwm1_dc:=0.3;
      end if;
      boost_panel2_cont_pwm1_tnext:=time+boost_panel2_cont_pwm1_T*boost_panel2_cont_pwm1_dc;
      boost_panel2_cont_pwm1_vout:=1;
    end when;
    when time>boost_panel2_cont_pwm1_tnext then 
      boost_panel2_cont_pwm1_vout:=0;
    end when;
    when batteryAndBockBoost_u>algSuperv_SOCmax then 
      algSuperv_EB:=1;
    end when;
    when batteryAndBockBoost_u<algSuperv_SOCmax-(algSuperv_SOCmax-algSuperv_SOCmin)*algSuperv_histelect then 
      algSuperv_EB:=0;
    end when;
    when algSuperv_deltaP<algSuperv_PminElect then 
      algSuperv_satElect:=1;
    end when;
    when algSuperv_deltaP>algSuperv_PminElect then 
      algSuperv_satElect:=0;
    end when;
    when batteryAndBockBoost_u<algSuperv_SOCmin then 
      algSuperv_FC:=1;
    end when;
    when batteryAndBockBoost_u>algSuperv_SOCmin+(algSuperv_SOCmax-algSuperv_SOCmin)*algSuperv_histFC then 
      algSuperv_FC:=0;
    end when;
    when ((-algSuperv_deltaP))<algSuperv_PminFC then 
      algSuperv_satFC:=1;
    end when;
    when ((-algSuperv_deltaP))>algSuperv_PminFC then 
      algSuperv_satFC:=0;
    end when;
    when time>1150+1.2*0 then 
      variableR1_R:=10;
    end when;
    when time>1450+1.5*0 then 
      variableR1_R:=5;
    end when;
    when time>2500+2.5*0 then 
      variableR1_R:=1005;
    end when;
    when boost_panel1_boost1_idealDiode_s<0 then 
      d0:=1;
    elsewhen boost_panel1_boost1_idealDiode_s>=0 then 
      d0:=0;
    end when;
    when boost_panel1_boost1_idealDiode_off>0.5 then 
      d1:=1;
    elsewhen boost_panel1_boost1_idealDiode_off<0.5 then 
      d1:=0;
    end when;
    when boost_panel1_boost1_idealDiode_off>0.5 then 
      d2:=1;
    elsewhen boost_panel1_boost1_idealDiode_off<0.5 then 
      d2:=0;
    end when;
    when boost_panel1_cont_pwm1_vgate>boost_panel1_boost1_controlledIdealClosingSwitch_level then 
      d3:=1;
    elsewhen boost_panel1_cont_pwm1_vgate<=boost_panel1_boost1_controlledIdealClosingSwitch_level then 
      d3:=0;
    end when;
    when boost_panel1_cont_pwm1_vgate>boost_panel1_boost1_controlledIdealClosingSwitch_level then 
      d4:=1;
    elsewhen boost_panel1_cont_pwm1_vgate<=boost_panel1_boost1_controlledIdealClosingSwitch_level then 
      d4:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealDiode_s<0 then 
      d5:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealDiode_s>=0 then 
      d5:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealDiode_off>0.5 then 
      d6:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealDiode_off<0.5 then 
      d6:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealDiode_off>0.5 then 
      d7:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealDiode_off<0.5 then 
      d7:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealDiode1_s<0 then 
      d8:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealDiode1_s>=0 then 
      d8:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealDiode1_off>0.5 then 
      d9:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealDiode1_off<0.5 then 
      d9:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealDiode1_off>0.5 then 
      d10:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealDiode1_off<0.5 then 
      d10:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_control>0.5 then 
      d11:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_control<0.5 then 
      d11:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_control>0.5 then 
      d12:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_idealClosingSwitch1_control<0.5 then 
      d12:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_Sw2>0.5 then 
      d13:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_Sw2<0.5 then 
      d13:=0;
    end when;
    when batteryAndBockBoost_bookBoostDB_Sw2>0.5 then 
      d14:=1;
    elsewhen batteryAndBockBoost_bookBoostDB_Sw2<0.5 then 
      d14:=0;
    end when;
    when boost_panel2_boost1_idealDiode_s<0 then 
      d15:=1;
    elsewhen boost_panel2_boost1_idealDiode_s>=0 then 
      d15:=0;
    end when;
    when boost_panel2_boost1_idealDiode_off>0.5 then 
      d16:=1;
    elsewhen boost_panel2_boost1_idealDiode_off<0.5 then 
      d16:=0;
    end when;
    when boost_panel2_boost1_idealDiode_off>0.5 then 
      d17:=1;
    elsewhen boost_panel2_boost1_idealDiode_off<0.5 then 
      d17:=0;
    end when;
    when boost_panel2_cont_pwm1_vgate>boost_panel2_boost1_controlledIdealClosingSwitch_level then 
      d18:=1;
    elsewhen boost_panel2_cont_pwm1_vgate<=boost_panel2_boost1_controlledIdealClosingSwitch_level then 
      d18:=0;
    end when;
    when boost_panel2_cont_pwm1_vgate>boost_panel2_boost1_controlledIdealClosingSwitch_level then 
      d19:=1;
    elsewhen boost_panel2_cont_pwm1_vgate<=boost_panel2_boost1_controlledIdealClosingSwitch_level then 
      d19:=0;
    end when;

//Muestreo de variables
    when time>pre(lastTsOut) then 
      lastTsOut:=time+TsOut;
	Vc2:=capacitor2_v;
	Ibatery:=batteryAndBockBoost_voltageControl_IrefCont;
	Pgen:=algSuperv_Pgenfilt;
	Pcons:=PCons_Sensor_product_y;
	PFcel:=algSuperv_Pref_FC;
	Pelectro:=fCellAndElectro_electComp_electrolizadorAnda1_u;
	SOC:=batteryAndBockBoost_u;
	ptanque:=fCellAndElectro_tanque_p_tanque;
    end when;


	when time>0.25 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;


	when time>983.4 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>984.2 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

	when time>1149.8 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>1150.25 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

	when time>1340.4 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>1341.3 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

	when time>1449.85 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>1450.4 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

	when time>2333.3 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>2334 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

	when time>2499.9 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>2500.25 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

	when time>2569.1 then
		Tsubsamp:=1e-3;
		lastsubsamp:=time+Tsubsamp;
	end when;
	when time>2569.6 then
		Tsubsamp:=1e10;
		lastsubsamp:=time+Tsubsamp;
	end when;

//Out: SOC,algSuperv_EB,algSuperv_FC,Vc2,Ibatery,Pgen,Pcons,PFcel,Pelectro,ptanque

    when time>pre(lastsubsamp) then 
      lastsubsamp:=time+Tsubsamp;
	Vc2:=capacitor2_v;
	Ibatery:=batteryAndBockBoost_voltageControl_IrefCont;
	Pgen:=algSuperv_Pgenfilt;
	Pcons:=PCons_Sensor_product_y;
	PFcel:=algSuperv_Pref_FC;
	Pelectro:=fCellAndElectro_electComp_electrolizadorAnda1_u;
	SOC:=batteryAndBockBoost_u;
	ptanque:=fCellAndElectro_tanque_p_tanque;
    end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_Period={2},
		MMO_Output={SOC},
		MMO_OutputType=CI_Sampled,
		StartTime=0.0,
		StopTime=3000,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end hres_nostiff;
