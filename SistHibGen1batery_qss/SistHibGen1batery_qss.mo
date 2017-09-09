model SistHibGen1batery_qss
  function fsolve1
    input Real boost_panel1_capacitor1_v;
    output Real boost_panel1_panel_exponente;
    output Real boost_panel1_panel_n_i;
    external "C" fsolve1(boost_panel1_capacitor1_v,boost_panel1_panel_exponente,boost_panel1_panel_n_i)    annotation(Library={"m","gsl","blas"},Include="#include \"../../src/test-suite/SistHibGen1batery_qss/SistHibGen1batery.c\"
");
  end fsolve1;
  function fsolve2
    input Real boost_panel2_capacitor1_v;
    output Real boost_panel2_panel_exponente;
    output Real boost_panel2_panel_n_i;
    external "C" fsolve2(boost_panel2_capacitor1_v,boost_panel2_panel_exponente,boost_panel2_panel_n_i)    annotation(Library={"m","gsl","blas"},Include="#include \"../../src/test-suite/SistHibGen1batery_qss/SistHibGen1batery.c\"
");
  end fsolve2;
  function fsolve3
    input Real boost_panel3_capacitor1_v;
    output Real boost_panel3_panel_exponente;
    output Real boost_panel3_panel_n_i;
    external "C" fsolve3(boost_panel3_capacitor1_v,boost_panel3_panel_exponente,boost_panel3_panel_n_i)    annotation(Library={"m","gsl","blas"},Include="#include \"../../src/test-suite/SistHibGen1batery_qss/SistHibGen1batery.c\"
");
  end fsolve3;
  function fsolve4
    input Real boost_panel4_capacitor1_v;
    output Real boost_panel4_panel_exponente;
    output Real boost_panel4_panel_n_i;
    external "C" fsolve4(boost_panel4_capacitor1_v,boost_panel4_panel_exponente,boost_panel4_panel_n_i)    annotation(Library={"m","gsl","blas"},Include="#include \"../../src/test-suite/SistHibGen1batery_qss/SistHibGen1batery.c\"
");
  end fsolve4;
  Real ground_p_i;
  parameter Real const_k(start=1)=80;
  parameter Real capacitor2_C(start=1)=0.01;
  Real capacitor2_v(start=60);
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
  parameter Real boost_panel1_panel_Irs=boost_panel1_panel_Ior*(boost_panel1_panel_Tpv/boost_panel1_panel_Tref)^3*exp(boost_panel1_panel_q*boost_panel1_panel_Eg*(1/boost_panel1_panel_Tref-1/boost_panel1_panel_Tpv)/boost_panel1_panel_K/boost_panel1_panel_Ac);
  parameter Integer boost_panel1_panel_Np=1;
  parameter Integer boost_panel1_panel_Ns=60;
  Real boost_panel1_panel_exponente;
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
  parameter Real boost_panel1_cont_pwm1_iTime=0;
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
  Real batteryAndBockBoost1_bookBoostDB_pin_p_out_i;
  Real batteryAndBockBoost1_bookBoostDB_pin_n_i;
  parameter Real batteryAndBockBoost1_bookBoostDB_inductor_L(start=1)=0.015;
  Real batteryAndBockBoost1_bookBoostDB_inductor_v;
  Real batteryAndBockBoost1_bookBoostDB_inductor_i(start=0);
  Real batteryAndBockBoost1_bookBoostDB_inductor_p_v;
  Real batteryAndBockBoost1_bookBoostDB_inductor_n_i;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode_Goff(min=0)=1e-05;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode_Vknee(min=0)=0;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode_off(start=1);
  Real batteryAndBockBoost1_bookBoostDB_idealDiode_s(start=0,unit="1");
  constant Real batteryAndBockBoost1_bookBoostDB_idealDiode_unitVoltage=1;
  constant Real batteryAndBockBoost1_bookBoostDB_idealDiode_unitCurrent=1;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode_n_v;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode_n_i;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode_useHeatPort=0;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode_T=293.15;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode_LossPower;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode1_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode1_Goff(min=0)=1e-05;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode1_Vknee(min=0)=0;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode1_off(start=1);
  Real batteryAndBockBoost1_bookBoostDB_idealDiode1_s(start=0,unit="1");
  constant Real batteryAndBockBoost1_bookBoostDB_idealDiode1_unitVoltage=1;
  constant Real batteryAndBockBoost1_bookBoostDB_idealDiode1_unitCurrent=1;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode1_v;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode1_n_i;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode1_useHeatPort=0;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealDiode1_T=293.15;
  Real batteryAndBockBoost1_bookBoostDB_idealDiode1_LossPower;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_Goff(min=0)=1e-05;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_control;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_s(unit="1");
  constant Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_unitVoltage=1;
  constant Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_unitCurrent=1;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_v;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_n_i;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_useHeatPort=0;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_T=293.15;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_LossPower;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_Ron(min=0)=1e-05;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_Goff(min=0)=1e-05;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_s(unit="1");
  constant Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_unitVoltage=1;
  constant Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_unitCurrent=1;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_n_i;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_useHeatPort=0;
  parameter Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_T=293.15;
  Real batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_LossPower;
  parameter Real batteryAndBockBoost1_const_k(start=1)=56;
  Real batteryAndBockBoost1_bookBoostPWMcontrol1_yref;
  parameter Real batteryAndBockBoost1_bookBoostPWMcontrol1_Kp=1.5;
  parameter Real batteryAndBockBoost1_bookBoostPWMcontrol1_Ki=0;
  Real batteryAndBockBoost1_bookBoostPWMcontrol1_Err;
  Real batteryAndBockBoost1_bookBoostPWMcontrol1_intErr;
  parameter Real batteryAndBockBoost1_bookBoostPWMcontrol1_T=5e-05;
  parameter Real batteryAndBockBoost1_bookBoostPWMcontrol1_iTime=0;
  discrete Real batteryAndBockBoost1_bookBoostPWMcontrol1_tnext(start=1e-20);
  discrete Real batteryAndBockBoost1_bookBoostPWMcontrol1_DC;
  discrete Real batteryAndBockBoost1_bookBoostPWMcontrol1_Output;
  Real batteryAndBockBoost1_not2_u;
  Real batteryAndBockBoost1_voltageControl_Err;
  Real batteryAndBockBoost1_voltageControl_intErr;
  parameter Real batteryAndBockBoost1_voltageControl_Kp=0.95;
  parameter Real batteryAndBockBoost1_voltageControl_Ki=10;
  parameter Real batteryAndBockBoost1_voltageControl_Imax=17;
  discrete Real batteryAndBockBoost1_voltageControl_Sat(start=1);
  discrete Real batteryAndBockBoost1_voltageControl_SatLevel(start=0);
  Real batteryAndBockBoost1_voltageControl_IrefCont;
  Real batteryAndBockBoost1_pin_n_i;
  parameter Real batteryAndBockBoost1_battery_E0=12.6463;
  parameter Real batteryAndBockBoost1_battery_Q=65;
  parameter Real batteryAndBockBoost1_battery_A=0.66;
  parameter Real batteryAndBockBoost1_battery_B=2884.61;
  parameter Real batteryAndBockBoost1_battery_K=0.33;
  parameter Real batteryAndBockBoost1_battery_R=0.25;
  parameter Real batteryAndBockBoost1_battery_SoC0=60;
  Real batteryAndBockBoost1_battery_Qt;
  Real batteryAndBockBoost1_battery_SoC;
  Real batteryAndBockBoost1_battery_E;
  Real batteryAndBockBoost1_battery1_pin_n_v;
  parameter Real batteryAndBockBoost1_battery1_E0=12.6463;
  parameter Real batteryAndBockBoost1_battery1_Q=65;
  parameter Real batteryAndBockBoost1_battery1_A=0.66;
  parameter Real batteryAndBockBoost1_battery1_B=2884.61;
  parameter Real batteryAndBockBoost1_battery1_K=0.33;
  parameter Real batteryAndBockBoost1_battery1_R=0.25;
  parameter Real batteryAndBockBoost1_battery1_SoC0=60;
  Real batteryAndBockBoost1_battery1_Qt;
  Real batteryAndBockBoost1_battery1_SoC;
  Real batteryAndBockBoost1_battery1_v;
  Real batteryAndBockBoost1_battery1_E;
  Real batteryAndBockBoost1_battery2_pin_n_v;
  parameter Real batteryAndBockBoost1_battery2_E0=12.6463;
  parameter Real batteryAndBockBoost1_battery2_Q=65;
  parameter Real batteryAndBockBoost1_battery2_A=0.66;
  parameter Real batteryAndBockBoost1_battery2_B=2884.61;
  parameter Real batteryAndBockBoost1_battery2_K=0.33;
  parameter Real batteryAndBockBoost1_battery2_R=0.25;
  parameter Real batteryAndBockBoost1_battery2_SoC0=60;
  Real batteryAndBockBoost1_battery2_Qt;
  Real batteryAndBockBoost1_battery2_SoC;
  Real batteryAndBockBoost1_battery2_v;
  Real batteryAndBockBoost1_battery2_E;
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
  parameter Real boost_panel2_panel_Irs=boost_panel2_panel_Ior*(boost_panel2_panel_Tpv/boost_panel2_panel_Tref)^3*exp(boost_panel2_panel_q*boost_panel2_panel_Eg*(1/boost_panel2_panel_Tref-1/boost_panel2_panel_Tpv)/boost_panel2_panel_K/boost_panel2_panel_Ac);
  parameter Integer boost_panel2_panel_Np=1;
  parameter Integer boost_panel2_panel_Ns=60;
  Real boost_panel2_panel_exponente;
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
  parameter Real boost_panel3_panel_q=1.6e-19;
  parameter Real boost_panel3_panel_Ac=1.6;
  parameter Real boost_panel3_panel_K=1.3805e-23;
  parameter Real boost_panel3_panel_K1=0.005532;
  parameter Real boost_panel3_panel_Ior=1.0647e-06;
  parameter Real boost_panel3_panel_Tref=303;
  parameter Real boost_panel3_panel_Eg=1.1;
  parameter Real boost_panel3_panel_Isc=8.51;
  parameter Real boost_panel3_panel_Rspv=0.01;
  parameter Real boost_panel3_panel_Tpv=273+25;
  parameter Real boost_panel3_panel_Irs=boost_panel3_panel_Ior*(boost_panel3_panel_Tpv/boost_panel3_panel_Tref)^3*exp(boost_panel3_panel_q*boost_panel3_panel_Eg*(1/boost_panel3_panel_Tref-1/boost_panel3_panel_Tpv)/boost_panel3_panel_K/boost_panel3_panel_Ac);
  parameter Integer boost_panel3_panel_Np=1;
  parameter Integer boost_panel3_panel_Ns=60;
  Real boost_panel3_panel_exponente;
  Real boost_panel3_panel_n_i;
  Real boost_panel3_mmt1_y;
  discrete Real boost_panel3_mmt1_potact;
  Real boost_panel3_mmt1_pot;
  Real boost_panel3_mmt1_potActFiltrada;
  discrete Real boost_panel3_mmt1_potprev;
  discrete Real boost_panel3_mmt1_prevu;
  discrete Real boost_panel3_mmt1_actu;
  discrete Real boost_panel3_mmt1_deltau;
  discrete Real boost_panel3_mmt1_deltap;
  discrete Real boost_panel3_mmt1_vref(start=36);
  parameter Real boost_panel3_mmt1_Ts=0.1;
  parameter Real boost_panel3_mmt1_deltaVpvRefPanel=0.5;
  parameter Real boost_panel3_capacitor1_C(start=1)=0.00047;
  Real boost_panel3_capacitor1_v(start=0);
  Real boost_panel3_capacitor1_n_i;
  Real boost_panel3_cont_pwm1_vgate;
  discrete Real boost_panel3_cont_pwm1_vout;
  discrete Real boost_panel3_cont_pwm1_dc;
  discrete Real boost_panel3_cont_pwm1_tnext(start=1e+20);
  parameter Real boost_panel3_cont_pwm1_T=0.0001;
  parameter Real boost_panel3_cont_pwm1_iTime=1.2e-06;
  parameter Real boost_panel3_boost1_inductor_L(start=1)=0.015;
  Real boost_panel3_boost1_inductor_v;
  Real boost_panel3_boost1_inductor_i(start=0);
  Real boost_panel3_boost1_inductor_n_v;
  parameter Real boost_panel3_boost1_idealDiode_Ron(min=0)=1e-05;
  parameter Real boost_panel3_boost1_idealDiode_Goff(min=0)=1e-05;
  parameter Real boost_panel3_boost1_idealDiode_Vknee(min=0)=0;
  Real boost_panel3_boost1_idealDiode_off(start=1);
  Real boost_panel3_boost1_idealDiode_s(start=0,unit="1");
  constant Real boost_panel3_boost1_idealDiode_unitVoltage=1;
  constant Real boost_panel3_boost1_idealDiode_unitCurrent=1;
  Real boost_panel3_boost1_idealDiode_v;
  Real boost_panel3_boost1_idealDiode_n_i;
  parameter Real boost_panel3_boost1_idealDiode_useHeatPort=0;
  parameter Real boost_panel3_boost1_idealDiode_T=293.15;
  Real boost_panel3_boost1_idealDiode_LossPower;
  parameter Real boost_panel3_boost1_controlledIdealClosingSwitch_level=0.5;
  parameter Real boost_panel3_boost1_controlledIdealClosingSwitch_Ron(min=0)=1e-05;
  parameter Real boost_panel3_boost1_controlledIdealClosingSwitch_Goff(min=0)=1e-05;
  Real boost_panel3_boost1_controlledIdealClosingSwitch_s(unit="1");
  constant Real boost_panel3_boost1_controlledIdealClosingSwitch_unitVoltage=1;
  constant Real boost_panel3_boost1_controlledIdealClosingSwitch_unitCurrent=1;
  parameter Real boost_panel3_boost1_controlledIdealClosingSwitch_useHeatPort=0;
  parameter Real boost_panel3_boost1_controlledIdealClosingSwitch_T=293.15;
  Real boost_panel3_boost1_controlledIdealClosingSwitch_LossPower;
  Real boost_panel3_boost1_pin_ground_i;
  Real boost_panel3_pin_n_i;
  parameter Real const1_k(start=1)=75;
  parameter Real const2_k(start=1)=82;
  parameter Real boost_panel4_panel_q=1.6e-19;
  parameter Real boost_panel4_panel_Ac=1.6;
  parameter Real boost_panel4_panel_K=1.3805e-23;
  parameter Real boost_panel4_panel_K1=0.005532;
  parameter Real boost_panel4_panel_Ior=1.0647e-06;
  parameter Real boost_panel4_panel_Tref=303;
  parameter Real boost_panel4_panel_Eg=1.1;
  parameter Real boost_panel4_panel_Isc=8.51;
  parameter Real boost_panel4_panel_Rspv=0.01;
  parameter Real boost_panel4_panel_Tpv=273+25;
  parameter Real boost_panel4_panel_Irs=boost_panel4_panel_Ior*(boost_panel4_panel_Tpv/boost_panel4_panel_Tref)^3*exp(boost_panel4_panel_q*boost_panel4_panel_Eg*(1/boost_panel4_panel_Tref-1/boost_panel4_panel_Tpv)/boost_panel4_panel_K/boost_panel4_panel_Ac);
  parameter Integer boost_panel4_panel_Np=1;
  parameter Integer boost_panel4_panel_Ns=60;
  Real boost_panel4_panel_exponente;
  Real boost_panel4_panel_n_i;
  Real boost_panel4_mmt1_y;
  discrete Real boost_panel4_mmt1_potact;
  Real boost_panel4_mmt1_pot;
  Real boost_panel4_mmt1_potActFiltrada;
  discrete Real boost_panel4_mmt1_potprev;
  discrete Real boost_panel4_mmt1_prevu;
  discrete Real boost_panel4_mmt1_actu;
  discrete Real boost_panel4_mmt1_deltau;
  discrete Real boost_panel4_mmt1_deltap;
  discrete Real boost_panel4_mmt1_vref(start=36);
  parameter Real boost_panel4_mmt1_Ts=0.1;
  parameter Real boost_panel4_mmt1_deltaVpvRefPanel=0.5;
  parameter Real boost_panel4_capacitor1_C(start=1)=0.00047;
  Real boost_panel4_capacitor1_v(start=0);
  Real boost_panel4_capacitor1_n_i;
  Real boost_panel4_cont_pwm1_vgate;
  discrete Real boost_panel4_cont_pwm1_vout;
  discrete Real boost_panel4_cont_pwm1_dc;
  discrete Real boost_panel4_cont_pwm1_tnext(start=1e+20);
  parameter Real boost_panel4_cont_pwm1_T=0.0001;
  parameter Real boost_panel4_cont_pwm1_iTime=9e-07;
  parameter Real boost_panel4_boost1_inductor_L(start=1)=0.015;
  Real boost_panel4_boost1_inductor_v;
  Real boost_panel4_boost1_inductor_i(start=0);
  Real boost_panel4_boost1_inductor_n_v;
  parameter Real boost_panel4_boost1_idealDiode_Ron(min=0)=1e-05;
  parameter Real boost_panel4_boost1_idealDiode_Goff(min=0)=1e-05;
  parameter Real boost_panel4_boost1_idealDiode_Vknee(min=0)=0;
  Real boost_panel4_boost1_idealDiode_off(start=1);
  Real boost_panel4_boost1_idealDiode_s(start=0,unit="1");
  constant Real boost_panel4_boost1_idealDiode_unitVoltage=1;
  constant Real boost_panel4_boost1_idealDiode_unitCurrent=1;
  Real boost_panel4_boost1_idealDiode_v;
  Real boost_panel4_boost1_idealDiode_n_i;
  parameter Real boost_panel4_boost1_idealDiode_useHeatPort=0;
  parameter Real boost_panel4_boost1_idealDiode_T=293.15;
  Real boost_panel4_boost1_idealDiode_LossPower;
  parameter Real boost_panel4_boost1_controlledIdealClosingSwitch_level=0.5;
  parameter Real boost_panel4_boost1_controlledIdealClosingSwitch_Ron(min=0)=1e-05;
  parameter Real boost_panel4_boost1_controlledIdealClosingSwitch_Goff(min=0)=1e-05;
  Real boost_panel4_boost1_controlledIdealClosingSwitch_s(unit="1");
  constant Real boost_panel4_boost1_controlledIdealClosingSwitch_unitVoltage=1;
  constant Real boost_panel4_boost1_controlledIdealClosingSwitch_unitCurrent=1;
  parameter Real boost_panel4_boost1_controlledIdealClosingSwitch_useHeatPort=0;
  parameter Real boost_panel4_boost1_controlledIdealClosingSwitch_T=293.15;
  Real boost_panel4_boost1_controlledIdealClosingSwitch_LossPower;
  Real boost_panel4_boost1_pin_ground_i;
  Real boost_panel4_pin_n_i;
  parameter Real const3_k(start=1)=60;
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
  discrete Real d20;
  discrete Real d21;
  discrete Real d22;
  discrete Real d23;
  discrete Real d24;
  discrete Real d25;
  discrete Real d26;
  discrete Real d27;
  discrete Real d28;
  discrete Real d29;
  discrete Real d30(start=0);
  discrete Real d31(start=boost_panel1_cont_pwm1_iTime);
  discrete Real d32(start=batteryAndBockBoost1_bookBoostPWMcontrol1_iTime);
  discrete Real d33(start=0);
  discrete Real d34(start=boost_panel2_cont_pwm1_iTime);
  discrete Real d35(start=0);
  discrete Real d36(start=boost_panel3_cont_pwm1_iTime);
  discrete Real d37(start=0);
  discrete Real d38(start=boost_panel4_cont_pwm1_iTime);
initial algorithm
  batteryAndBockBoost1_battery_Qt := (1-batteryAndBockBoost1_battery_SoC0/100)*batteryAndBockBoost1_battery_Q;
  batteryAndBockBoost1_battery1_Qt := (1-batteryAndBockBoost1_battery1_SoC0/100)*batteryAndBockBoost1_battery1_Q;
  batteryAndBockBoost1_battery2_Qt := (1-batteryAndBockBoost1_battery2_SoC0/100)*batteryAndBockBoost1_battery2_Q;
equation
  boost_panel1_mmt1_pot = boost_panel1_capacitor1_v*boost_panel1_boost1_inductor_i;
  der(boost_panel1_mmt1_potActFiltrada) = (boost_panel1_mmt1_pot-boost_panel1_mmt1_potActFiltrada)*100;
  boost_panel1_mmt1_y = pre(boost_panel1_mmt1_vref);
  boost_panel1_cont_pwm1_vgate = pre(boost_panel1_cont_pwm1_vout);
  boost_panel1_boost1_idealDiode_off = pre(d0);
  batteryAndBockBoost1_bookBoostDB_inductor_n_i = (-batteryAndBockBoost1_bookBoostDB_inductor_i);
  batteryAndBockBoost1_bookBoostDB_idealDiode_off = pre(d5);
  batteryAndBockBoost1_bookBoostDB_idealDiode1_off = pre(d8);
  batteryAndBockBoost1_not2_u = pre(batteryAndBockBoost1_bookBoostPWMcontrol1_Output);
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_control = 1-batteryAndBockBoost1_not2_u;
  batteryAndBockBoost1_voltageControl_Err = 56-capacitor2_v;
  der(batteryAndBockBoost1_voltageControl_intErr) = batteryAndBockBoost1_voltageControl_Err;
  batteryAndBockBoost1_voltageControl_IrefCont = 0.95*batteryAndBockBoost1_voltageControl_Err+10*batteryAndBockBoost1_voltageControl_intErr;
  batteryAndBockBoost1_bookBoostPWMcontrol1_yref = pre(batteryAndBockBoost1_voltageControl_Sat)*batteryAndBockBoost1_voltageControl_IrefCont+pre(batteryAndBockBoost1_voltageControl_SatLevel);
  der(batteryAndBockBoost1_battery_Qt) = ((-batteryAndBockBoost1_bookBoostDB_inductor_n_i))/3600;
  batteryAndBockBoost1_battery_E = 12.6463-21.45/(65-batteryAndBockBoost1_battery_Qt)+0.66*exp(((-2884.61*batteryAndBockBoost1_battery_Qt)));
  batteryAndBockBoost1_battery1_pin_n_v = batteryAndBockBoost1_battery_E-0.25*((-batteryAndBockBoost1_bookBoostDB_inductor_n_i));
  batteryAndBockBoost1_battery_SoC = (1-batteryAndBockBoost1_battery_Qt/65)*100;
  der(batteryAndBockBoost1_battery1_Qt) = ((-batteryAndBockBoost1_bookBoostDB_inductor_n_i))/3600;
  batteryAndBockBoost1_battery1_E = 12.6463-21.45/(65-batteryAndBockBoost1_battery1_Qt)+0.66*exp(((-2884.61*batteryAndBockBoost1_battery1_Qt)));
  batteryAndBockBoost1_battery1_v = batteryAndBockBoost1_battery1_E-0.25*((-batteryAndBockBoost1_bookBoostDB_inductor_n_i));
  batteryAndBockBoost1_battery1_SoC = (1-batteryAndBockBoost1_battery1_Qt/65)*100;
  der(batteryAndBockBoost1_battery2_Qt) = ((-batteryAndBockBoost1_bookBoostDB_inductor_n_i))/3600;
  batteryAndBockBoost1_battery2_E = 12.6463-21.45/(65-batteryAndBockBoost1_battery2_Qt)+0.66*exp(((-2884.61*batteryAndBockBoost1_battery2_Qt)));
  batteryAndBockBoost1_battery2_v = batteryAndBockBoost1_battery2_E-0.25*((-batteryAndBockBoost1_bookBoostDB_inductor_n_i));
  batteryAndBockBoost1_battery2_SoC = (1-batteryAndBockBoost1_battery2_Qt/65)*100;
  boost_panel2_mmt1_pot = boost_panel2_capacitor1_v*boost_panel2_boost1_inductor_i;
  der(boost_panel2_mmt1_potActFiltrada) = (boost_panel2_mmt1_pot-boost_panel2_mmt1_potActFiltrada)*100;
  boost_panel2_mmt1_y = pre(boost_panel2_mmt1_vref);
  boost_panel2_cont_pwm1_vgate = pre(boost_panel2_cont_pwm1_vout);
  boost_panel2_boost1_idealDiode_off = pre(d15);
  boost_panel3_mmt1_pot = boost_panel3_capacitor1_v*boost_panel3_boost1_inductor_i;
  der(boost_panel3_mmt1_potActFiltrada) = (boost_panel3_mmt1_pot-boost_panel3_mmt1_potActFiltrada)*100;
  boost_panel3_mmt1_y = pre(boost_panel3_mmt1_vref);
  boost_panel3_cont_pwm1_vgate = pre(boost_panel3_cont_pwm1_vout);
  boost_panel3_boost1_idealDiode_off = pre(d20);
  boost_panel4_mmt1_pot = boost_panel4_capacitor1_v*boost_panel4_boost1_inductor_i;
  der(boost_panel4_mmt1_potActFiltrada) = (boost_panel4_mmt1_pot-boost_panel4_mmt1_potActFiltrada)*100;
  boost_panel4_mmt1_y = pre(boost_panel4_mmt1_vref);
  boost_panel4_cont_pwm1_vgate = pre(boost_panel4_cont_pwm1_vout);
  boost_panel4_boost1_idealDiode_off = pre(d25);
  batteryAndBockBoost1_bookBoostPWMcontrol1_Err = batteryAndBockBoost1_bookBoostPWMcontrol1_yref-((-batteryAndBockBoost1_bookBoostDB_inductor_n_i));
  batteryAndBockBoost1_battery2_pin_n_v = batteryAndBockBoost1_battery1_pin_n_v+batteryAndBockBoost1_battery1_v;
  batteryAndBockBoost1_bookBoostDB_inductor_p_v = batteryAndBockBoost1_battery2_v+batteryAndBockBoost1_battery2_pin_n_v;
  (boost_panel1_panel_exponente,boost_panel1_panel_n_i) = fsolve1(boost_panel1_capacitor1_v);
  boost_panel1_boost1_idealDiode_s = (1/((-1)-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(0.99998)*d1*d4-(9.9999e-06)*d4+(0.99999)*d2))*(1/((-1e-05)-(0.99999)*d1))*((-(0.99998)*d1*d4*capacitor2_v)+(0.99999)*d1*boost_panel1_boost1_inductor_i-(9.9999e-06)*d4*capacitor2_v-(9.9999e-06)*d1*capacitor2_v-(0.99998)*d3*d1*boost_panel1_boost1_inductor_i-(9.9999e-06)*d3*boost_panel1_boost1_inductor_i+(1e-05)*boost_panel1_boost1_inductor_i-(1e-10)*capacitor2_v);
  boost_panel1_boost1_idealDiode_n_i = (1/((-1)-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(0.99998)*d1*d4-(9.9999e-06)*d4+(0.99999)*d2))*(1/((-1e-05)-(0.99999)*d1))*((0.99998)*d1*d4*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i+(9.9999e-06)*d4*capacitor2_v+(9.9999e-06)*d1*capacitor2_v+(0.99998)*d3*d1*boost_panel1_boost1_inductor_i+(9.9999e-06)*d3*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i+(1e-10)*capacitor2_v)*(1-(0.99999)*d2);
  boost_panel1_boost1_idealDiode_v = (1/((-1)-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(0.99998)*d1*d4-(9.9999e-06)*d4+(0.99999)*d2))*((0.99998)*d1*d4*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i+(9.9999e-06)*d4*capacitor2_v+(9.9999e-06)*d1*capacitor2_v+(0.99998)*d3*d1*boost_panel1_boost1_inductor_i+(9.9999e-06)*d3*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i+(1e-10)*capacitor2_v);
  boost_panel1_boost1_inductor_n_v = (-((0.99999)*d2*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i-capacitor2_v)*((-1)+(0.99999)*d3)*(1/((-1)-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(0.99998)*d1*d4-(9.9999e-06)*d4+(0.99999)*d2)));
  boost_panel1_boost1_controlledIdealClosingSwitch_s = ((0.99999)*d2*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i-capacitor2_v)*(1/((-1)-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(0.99998)*d1*d4-(9.9999e-06)*d4+(0.99999)*d2));
  boost_panel1_boost1_pin_ground_i = ((0.99999)*d2*capacitor2_v-(0.99999)*d1*boost_panel1_boost1_inductor_i-(1e-05)*boost_panel1_boost1_inductor_i-capacitor2_v)*(1/((-1)-(0.99998)*d3*d2+(0.99999)*d3-(9.9999e-06)*d1-(0.99998)*d1*d4-(9.9999e-06)*d4+(0.99999)*d2))*((-1e-05)-(0.99999)*d4);
  batteryAndBockBoost1_bookBoostDB_idealDiode_s = (-((0.99998)*d11*d10*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d9*d12*capacitor2_v-(1e-05)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99999)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v+(9.9999e-06)*d9*capacitor2_v-(0.99999)*d11*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(1)*capacitor2_v)*((-1)+(0.99999)*d13)*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6)));
  batteryAndBockBoost1_bookBoostDB_idealDiode_n_i = (-((0.99998)*d11*d10*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d9*d12*capacitor2_v-(1e-05)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99999)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v+(9.9999e-06)*d9*capacitor2_v-(0.99999)*d11*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(1)*capacitor2_v)*((-1)+(0.99999)*d13)*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((-1)+(0.99999)*d7));
  batteryAndBockBoost1_bookBoostDB_idealDiode1_s = (1/((-1e-05)-(0.99999)*d9))*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((-(9.9998e-06)*d11*d13*capacitor2_v)-(9.9997e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-11)*d6*capacitor2_v+(9.9999e-11)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(9.9997e-06)*d14*d11*d6*capacitor2_v-(0.99997)*d11*d9*d13*capacitor2_v-(9.9998e-06)*d14*d6*capacitor2_v-(0.99997)*d7*d11*d9*capacitor2_v-(9.9998e-06)*d7*d13*capacitor2_v-(0.99996)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(0.99998)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(9.9999e-06)*d13*capacitor2_v+(9.9998e-11)*d11*d6*capacitor2_v-(0.99997)*d7*d9*d13*capacitor2_v-(9.9998e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(0.99997)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-11)*d14*capacitor2_v-(9.9997e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(9.9997e-06)*d11*d9*d6*capacitor2_v-(1e-10)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9999e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9997e-06)*d14*d11*d9*capacitor2_v-(0.99999)*d9*capacitor2_v+(9.9998e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(0.99997)*d14*d9*d6*capacitor2_v+(0.99998)*d9*d13*capacitor2_v-(9.9998e-06)*d7*d11*capacitor2_v-(9.9998e-06)*d14*d9*capacitor2_v-(9.9999e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(9.9999e-06)*d11*capacitor2_v+(9.9998e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(0.99998)*d11*d9*capacitor2_v+(0.99997)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(9.9999e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d7*capacitor2_v+(9.9998e-11)*d14*d11*capacitor2_v+(9.9998e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(9.9997e-06)*d7*d11*d13*capacitor2_v+(0.99998)*d7*d9*capacitor2_v-(9.9998e-06)*d9*d6*capacitor2_v+(0.99996)*d7*d11*d9*d13*capacitor2_v+(9.9998e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(0.99996)*d14*d11*d9*d6*capacitor2_v-(1e-05)*capacitor2_v);
  batteryAndBockBoost1_bookBoostDB_idealDiode1_n_i = (1/((-1e-05)-(0.99999)*d9))*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((9.9998e-06)*d11*d13*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(0.99997)*d11*d9*d13*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v+(0.99997)*d7*d11*d9*capacitor2_v+(9.9998e-06)*d7*d13*capacitor2_v+(0.99996)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99998)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d13*capacitor2_v-(9.9998e-11)*d11*d6*capacitor2_v+(0.99997)*d7*d9*d13*capacitor2_v+(9.9998e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(0.99997)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d14*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d11*d9*d6*capacitor2_v+(1e-10)*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9997e-06)*d14*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(9.9998e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99997)*d14*d9*d6*capacitor2_v-(0.99998)*d9*d13*capacitor2_v+(9.9998e-06)*d7*d11*capacitor2_v+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d11*capacitor2_v-(9.9998e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99998)*d11*d9*capacitor2_v-(0.99997)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9999e-06)*d7*capacitor2_v-(9.9998e-11)*d14*d11*capacitor2_v-(9.9998e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d7*d11*d13*capacitor2_v-(0.99998)*d7*d9*capacitor2_v+(9.9998e-06)*d9*d6*capacitor2_v-(0.99996)*d7*d11*d9*d13*capacitor2_v-(9.9998e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(0.99996)*d14*d11*d9*d6*capacitor2_v+(1e-05)*capacitor2_v)*(1-(0.99999)*d10);
  batteryAndBockBoost1_bookBoostDB_idealDiode1_v = (1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((9.9998e-06)*d11*d13*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(0.99997)*d11*d9*d13*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v+(0.99997)*d7*d11*d9*capacitor2_v+(9.9998e-06)*d7*d13*capacitor2_v+(0.99996)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99998)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d13*capacitor2_v-(9.9998e-11)*d11*d6*capacitor2_v+(0.99997)*d7*d9*d13*capacitor2_v+(9.9998e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(0.99997)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d14*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d11*d9*d6*capacitor2_v+(1e-10)*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9997e-06)*d14*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(9.9998e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99997)*d14*d9*d6*capacitor2_v-(0.99998)*d9*d13*capacitor2_v+(9.9998e-06)*d7*d11*capacitor2_v+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d11*capacitor2_v-(9.9998e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99998)*d11*d9*capacitor2_v-(0.99997)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9999e-06)*d7*capacitor2_v-(9.9998e-11)*d14*d11*capacitor2_v-(9.9998e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d7*d11*d13*capacitor2_v-(0.99998)*d7*d9*capacitor2_v+(9.9998e-06)*d9*d6*capacitor2_v-(0.99996)*d7*d11*d9*d13*capacitor2_v-(9.9998e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(0.99996)*d14*d11*d9*d6*capacitor2_v+(1e-05)*capacitor2_v);
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_s = (1/((-1)+(0.99999)*d11))*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((-(9.9998e-06)*d11*d13*capacitor2_v)-(9.9997e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-11)*d6*capacitor2_v+(9.9999e-11)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(9.9997e-06)*d14*d11*d6*capacitor2_v-(0.99997)*d11*d9*d13*capacitor2_v-(9.9998e-06)*d14*d6*capacitor2_v-(0.99997)*d7*d11*d9*capacitor2_v-(9.9998e-06)*d7*d13*capacitor2_v-(0.99996)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(0.99998)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(9.9999e-06)*d13*capacitor2_v+(9.9998e-11)*d11*d6*capacitor2_v-(0.99997)*d7*d9*d13*capacitor2_v-(9.9998e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(0.99997)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(9.9999e-11)*d14*capacitor2_v-(9.9997e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(9.9997e-06)*d11*d9*d6*capacitor2_v-(1e-10)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9999e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9997e-06)*d14*d11*d9*capacitor2_v-(0.99999)*d9*capacitor2_v+(9.9998e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13-(0.99997)*d14*d9*d6*capacitor2_v+(0.99998)*d9*d13*capacitor2_v-(9.9998e-06)*d7*d11*capacitor2_v-(9.9998e-06)*d14*d9*capacitor2_v-(9.9999e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(9.9999e-06)*d11*capacitor2_v+(9.9998e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(0.99998)*d11*d9*capacitor2_v+(0.99997)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(9.9999e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d7*capacitor2_v+(9.9998e-11)*d14*d11*capacitor2_v+(9.9998e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13+(9.9997e-06)*d7*d11*d13*capacitor2_v+(0.99998)*d7*d9*capacitor2_v-(9.9998e-06)*d9*d6*capacitor2_v+(0.99996)*d7*d11*d9*d13*capacitor2_v+(9.9998e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6+(0.99996)*d14*d11*d9*d6*capacitor2_v-(1e-05)*capacitor2_v);
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_n_i = (1/((-1)+(0.99999)*d11))*(1e-05+(0.99999)*d12)*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((9.9998e-06)*d11*d13*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(0.99997)*d11*d9*d13*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v+(0.99997)*d7*d11*d9*capacitor2_v+(9.9998e-06)*d7*d13*capacitor2_v+(0.99996)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99998)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d13*capacitor2_v-(9.9998e-11)*d11*d6*capacitor2_v+(0.99997)*d7*d9*d13*capacitor2_v+(9.9998e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(0.99997)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d14*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d11*d9*d6*capacitor2_v+(1e-10)*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9997e-06)*d14*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(9.9998e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99997)*d14*d9*d6*capacitor2_v-(0.99998)*d9*d13*capacitor2_v+(9.9998e-06)*d7*d11*capacitor2_v+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d11*capacitor2_v-(9.9998e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99998)*d11*d9*capacitor2_v-(0.99997)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9999e-06)*d7*capacitor2_v-(9.9998e-11)*d14*d11*capacitor2_v-(9.9998e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d7*d11*d13*capacitor2_v-(0.99998)*d7*d9*capacitor2_v+(9.9998e-06)*d9*d6*capacitor2_v-(0.99996)*d7*d11*d9*d13*capacitor2_v-(9.9998e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(0.99996)*d14*d11*d9*d6*capacitor2_v+(1e-05)*capacitor2_v);
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_v = (1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((9.9998e-06)*d11*d13*capacitor2_v+(9.9997e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d6*capacitor2_v-(9.9999e-11)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d14*d11*d6*capacitor2_v+(0.99997)*d11*d9*d13*capacitor2_v+(9.9998e-06)*d14*d6*capacitor2_v+(0.99997)*d7*d11*d9*capacitor2_v+(9.9998e-06)*d7*d13*capacitor2_v+(0.99996)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99998)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d13*capacitor2_v-(9.9998e-11)*d11*d6*capacitor2_v+(0.99997)*d7*d9*d13*capacitor2_v+(9.9998e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(0.99997)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(9.9999e-11)*d14*capacitor2_v+(9.9997e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d11*d9*d6*capacitor2_v+(1e-10)*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9997e-06)*d14*d11*d9*capacitor2_v+(0.99999)*d9*capacitor2_v-(9.9998e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6*d13+(0.99997)*d14*d9*d6*capacitor2_v-(0.99998)*d9*d13*capacitor2_v+(9.9998e-06)*d7*d11*capacitor2_v+(9.9998e-06)*d14*d9*capacitor2_v+(9.9999e-06)*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-06)*d11*capacitor2_v-(9.9998e-06)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99998)*d11*d9*capacitor2_v-(0.99997)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(9.9999e-11)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(9.9999e-06)*d7*capacitor2_v-(9.9998e-11)*d14*d11*capacitor2_v-(9.9998e-06)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d13-(9.9997e-06)*d7*d11*d13*capacitor2_v-(0.99998)*d7*d9*capacitor2_v+(9.9998e-06)*d9*d6*capacitor2_v-(0.99996)*d7*d11*d9*d13*capacitor2_v-(9.9998e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i*d6-(0.99996)*d14*d11*d9*d6*capacitor2_v+(1e-05)*capacitor2_v);
  batteryAndBockBoost1_bookBoostDB_idealDiode_n_v = (-((0.99998)*d11*d10*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d9*d12*capacitor2_v-(1e-05)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99999)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v+(9.9999e-06)*d9*capacitor2_v-(0.99999)*d11*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(1)*capacitor2_v)*((-1)+(0.99999)*d13)*((-1e-05)-(0.99999)*d6)*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6)));
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_s = ((0.99998)*d11*d10*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d9*d12*capacitor2_v-(1e-05)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99999)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v+(9.9999e-06)*d9*capacitor2_v-(0.99999)*d11*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(1)*capacitor2_v)*((-1e-05)-(0.99999)*d6)*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6));
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_n_i = ((0.99998)*d11*d10*capacitor2_v-(0.99999)*d10*capacitor2_v+(0.99998)*d9*d12*capacitor2_v-(1e-05)*batteryAndBockBoost1_bookBoostDB_inductor_n_i-(0.99999)*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d12*capacitor2_v+(9.9999e-06)*d9*capacitor2_v-(0.99999)*d11*capacitor2_v+(0.99998)*d11*d9*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(9.9999e-06)*d11*batteryAndBockBoost1_bookBoostDB_inductor_n_i+(1)*capacitor2_v)*((-1e-05)-(0.99999)*d6)*(1/((-2e-05)+(9.9999e-06)*d7-(9.9998e-06)*d12*d6-(1.99996e-05)*d9*d6+(9.9998e-11)*d14*d11+(0.99996)*d9*d12*d6*d13+(0.99996)*d14*d11*d9*d6+(0.99996)*d7*d11*d9*d13+(9.9997e-06)*d11*d10*d13-(9.9999e-11)*d14-(0.99997)*d14*d9*d6+(0.99998)*d6*d13-(9.9998e-06)*d14*d9+(1.99998e-05)*d11+(0.99998)*d11*d6-(0.99999)*d9+(9.9997e-06)*d9*d12*d13+(9.9997e-06)*d11*d9*d6-(9.9998e-06)*d9*d12-(9.9998e-06)*d10*d13-(9.9998e-06)*d11*d10+(9.9997e-06)*d14*d11*d6-(0.99997)*d10*d6*d13-(0.99997)*d11*d6*d13-(0.99997)*d11*d10*d6+(0.99998)*d11*d9-(9.9999e-11)*d12+(9.9997e-06)*d7*d11*d13+(0.99998)*d7*d9+(9.9999e-06)*d10+(9.9998e-11)*d12*d13+(9.9997e-06)*d14*d11*d9-(0.99999)*d6-(9.9998e-06)*d7*d11+(0.99998)*d9*d13+(9.9997e-06)*d9*d6*d13-(0.99997)*d7*d9*d13+(0.99998)*d10*d6+(1.99998e-05)*d13+(0.99996)*d11*d10*d6*d13-(1.99996e-05)*d11*d13+(9.9997e-06)*d12*d6*d13-(0.99997)*d11*d9*d13-(0.99997)*d7*d11*d9-(9.9998e-06)*d7*d13-(0.99997)*d9*d12*d6-(9.9998e-06)*d14*d6))*((-1e-05)-(0.99999)*d14);
  (boost_panel2_panel_exponente,boost_panel2_panel_n_i) = fsolve2(boost_panel2_capacitor1_v);
  boost_panel2_boost1_idealDiode_s = ((-(9.9999e-06)*capacitor2_v*d19)+(1e-05)*boost_panel2_boost1_inductor_i-(1e-10)*capacitor2_v-(0.99998)*boost_panel2_boost1_inductor_i*d18*d16+(0.99999)*boost_panel2_boost1_inductor_i*d16-(9.9999e-06)*capacitor2_v*d16-(0.99998)*capacitor2_v*d16*d19-(9.9999e-06)*boost_panel2_boost1_inductor_i*d18)*(1/((-1)-(0.99998)*d18*d17+(0.99999)*d18-(0.99998)*d16*d19-(9.9999e-06)*d16-(9.9999e-06)*d19+(0.99999)*d17))*(1/((-1e-05)-(0.99999)*d16));
  boost_panel2_boost1_idealDiode_n_i = (1-(0.99999)*d17)*(1/((-1)-(0.99998)*d18*d17+(0.99999)*d18-(0.99998)*d16*d19-(9.9999e-06)*d16-(9.9999e-06)*d19+(0.99999)*d17))*((9.9999e-06)*capacitor2_v*d19-(1e-05)*boost_panel2_boost1_inductor_i+(1e-10)*capacitor2_v+(0.99998)*boost_panel2_boost1_inductor_i*d18*d16-(0.99999)*boost_panel2_boost1_inductor_i*d16+(9.9999e-06)*capacitor2_v*d16+(0.99998)*capacitor2_v*d16*d19+(9.9999e-06)*boost_panel2_boost1_inductor_i*d18)*(1/((-1e-05)-(0.99999)*d16));
  boost_panel2_boost1_idealDiode_v = (1/((-1)-(0.99998)*d18*d17+(0.99999)*d18-(0.99998)*d16*d19-(9.9999e-06)*d16-(9.9999e-06)*d19+(0.99999)*d17))*((9.9999e-06)*capacitor2_v*d19-(1e-05)*boost_panel2_boost1_inductor_i+(1e-10)*capacitor2_v+(0.99998)*boost_panel2_boost1_inductor_i*d18*d16-(0.99999)*boost_panel2_boost1_inductor_i*d16+(9.9999e-06)*capacitor2_v*d16+(0.99998)*capacitor2_v*d16*d19+(9.9999e-06)*boost_panel2_boost1_inductor_i*d18);
  boost_panel2_boost1_inductor_n_v = (-((-(1e-05)*boost_panel2_boost1_inductor_i)+(0.99999)*capacitor2_v*d17-capacitor2_v-(0.99999)*boost_panel2_boost1_inductor_i*d16)*((-1)+(0.99999)*d18)*(1/((-1)-(0.99998)*d18*d17+(0.99999)*d18-(0.99998)*d16*d19-(9.9999e-06)*d16-(9.9999e-06)*d19+(0.99999)*d17)));
  boost_panel2_boost1_controlledIdealClosingSwitch_s = ((-(1e-05)*boost_panel2_boost1_inductor_i)+(0.99999)*capacitor2_v*d17-capacitor2_v-(0.99999)*boost_panel2_boost1_inductor_i*d16)*(1/((-1)-(0.99998)*d18*d17+(0.99999)*d18-(0.99998)*d16*d19-(9.9999e-06)*d16-(9.9999e-06)*d19+(0.99999)*d17));
  boost_panel2_boost1_pin_ground_i = (-((1e-05)*boost_panel2_boost1_inductor_i-(0.99999)*capacitor2_v*d17+capacitor2_v+(0.99999)*boost_panel2_boost1_inductor_i*d16)*((-1e-05)-(0.99999)*d19)*(1/((-1)-(0.99998)*d18*d17+(0.99999)*d18-(0.99998)*d16*d19-(9.9999e-06)*d16-(9.9999e-06)*d19+(0.99999)*d17)));
  (boost_panel3_panel_exponente,boost_panel3_panel_n_i) = fsolve3(boost_panel3_capacitor1_v);
  boost_panel3_boost1_idealDiode_s = (1/((-1)-(0.99998)*d21*d24+(0.99999)*d23-(9.9999e-06)*d21-(9.9999e-06)*d24+(0.99999)*d22-(0.99998)*d23*d22))*(1/((-1e-05)-(0.99999)*d21))*((-(0.99998)*d23*d21*boost_panel3_boost1_inductor_i)-(9.9999e-06)*d23*boost_panel3_boost1_inductor_i+(0.99999)*d21*boost_panel3_boost1_inductor_i-(9.9999e-06)*d21*capacitor2_v+(1e-05)*boost_panel3_boost1_inductor_i-(1e-10)*capacitor2_v-(0.99998)*d21*d24*capacitor2_v-(9.9999e-06)*d24*capacitor2_v);
  boost_panel3_boost1_idealDiode_n_i = (1/((-1)-(0.99998)*d21*d24+(0.99999)*d23-(9.9999e-06)*d21-(9.9999e-06)*d24+(0.99999)*d22-(0.99998)*d23*d22))*((0.99998)*d23*d21*boost_panel3_boost1_inductor_i+(9.9999e-06)*d23*boost_panel3_boost1_inductor_i-(0.99999)*d21*boost_panel3_boost1_inductor_i+(9.9999e-06)*d21*capacitor2_v-(1e-05)*boost_panel3_boost1_inductor_i+(1e-10)*capacitor2_v+(0.99998)*d21*d24*capacitor2_v+(9.9999e-06)*d24*capacitor2_v)*(1/((-1e-05)-(0.99999)*d21))*(1-(0.99999)*d22);
  boost_panel3_boost1_idealDiode_v = (1/((-1)-(0.99998)*d21*d24+(0.99999)*d23-(9.9999e-06)*d21-(9.9999e-06)*d24+(0.99999)*d22-(0.99998)*d23*d22))*((0.99998)*d23*d21*boost_panel3_boost1_inductor_i+(9.9999e-06)*d23*boost_panel3_boost1_inductor_i-(0.99999)*d21*boost_panel3_boost1_inductor_i+(9.9999e-06)*d21*capacitor2_v-(1e-05)*boost_panel3_boost1_inductor_i+(1e-10)*capacitor2_v+(0.99998)*d21*d24*capacitor2_v+(9.9999e-06)*d24*capacitor2_v);
  boost_panel3_boost1_inductor_n_v = (-(1/((-1)-(0.99998)*d21*d24+(0.99999)*d23-(9.9999e-06)*d21-(9.9999e-06)*d24+(0.99999)*d22-(0.99998)*d23*d22))*((-(0.99999)*d21*boost_panel3_boost1_inductor_i)+(0.99999)*d22*capacitor2_v-(1e-05)*boost_panel3_boost1_inductor_i-capacitor2_v)*((-1)+(0.99999)*d23));
  boost_panel3_boost1_controlledIdealClosingSwitch_s = (1/((-1)-(0.99998)*d21*d24+(0.99999)*d23-(9.9999e-06)*d21-(9.9999e-06)*d24+(0.99999)*d22-(0.99998)*d23*d22))*((-(0.99999)*d21*boost_panel3_boost1_inductor_i)+(0.99999)*d22*capacitor2_v-(1e-05)*boost_panel3_boost1_inductor_i-capacitor2_v);
  boost_panel3_boost1_pin_ground_i = (1/((-1)-(0.99998)*d21*d24+(0.99999)*d23-(9.9999e-06)*d21-(9.9999e-06)*d24+(0.99999)*d22-(0.99998)*d23*d22))*((-(0.99999)*d21*boost_panel3_boost1_inductor_i)+(0.99999)*d22*capacitor2_v-(1e-05)*boost_panel3_boost1_inductor_i-capacitor2_v)*((-1e-05)-(0.99999)*d24);
  (boost_panel4_panel_exponente,boost_panel4_panel_n_i) = fsolve4(boost_panel4_capacitor1_v);
  boost_panel4_boost1_idealDiode_s = (1/((-1)-(9.9999e-06)*d26-(9.9999e-06)*d29+(0.99999)*d27-(0.99998)*d26*d29-(0.99998)*d27*d28+(0.99999)*d28))*((-(9.9999e-06)*d26*capacitor2_v)+(0.99999)*d26*boost_panel4_boost1_inductor_i+(1e-05)*boost_panel4_boost1_inductor_i-(0.99998)*d26*boost_panel4_boost1_inductor_i*d28-(1e-10)*capacitor2_v-(0.99998)*d26*d29*capacitor2_v-(9.9999e-06)*boost_panel4_boost1_inductor_i*d28-(9.9999e-06)*d29*capacitor2_v)*(1/((-1e-05)-(0.99999)*d26));
  boost_panel4_boost1_idealDiode_n_i = (1-(0.99999)*d27)*((9.9999e-06)*d26*capacitor2_v-(0.99999)*d26*boost_panel4_boost1_inductor_i-(1e-05)*boost_panel4_boost1_inductor_i+(0.99998)*d26*boost_panel4_boost1_inductor_i*d28+(1e-10)*capacitor2_v+(0.99998)*d26*d29*capacitor2_v+(9.9999e-06)*boost_panel4_boost1_inductor_i*d28+(9.9999e-06)*d29*capacitor2_v)*(1/((-1)-(9.9999e-06)*d26-(9.9999e-06)*d29+(0.99999)*d27-(0.99998)*d26*d29-(0.99998)*d27*d28+(0.99999)*d28))*(1/((-1e-05)-(0.99999)*d26));
  boost_panel4_boost1_idealDiode_v = ((9.9999e-06)*d26*capacitor2_v-(0.99999)*d26*boost_panel4_boost1_inductor_i-(1e-05)*boost_panel4_boost1_inductor_i+(0.99998)*d26*boost_panel4_boost1_inductor_i*d28+(1e-10)*capacitor2_v+(0.99998)*d26*d29*capacitor2_v+(9.9999e-06)*boost_panel4_boost1_inductor_i*d28+(9.9999e-06)*d29*capacitor2_v)*(1/((-1)-(9.9999e-06)*d26-(9.9999e-06)*d29+(0.99999)*d27-(0.99998)*d26*d29-(0.99998)*d27*d28+(0.99999)*d28));
  boost_panel4_boost1_inductor_n_v = (-((-1)+(0.99999)*d28)*(1/((-1)-(9.9999e-06)*d26-(9.9999e-06)*d29+(0.99999)*d27-(0.99998)*d26*d29-(0.99998)*d27*d28+(0.99999)*d28))*((0.99999)*d27*capacitor2_v-(0.99999)*d26*boost_panel4_boost1_inductor_i-(1e-05)*boost_panel4_boost1_inductor_i-capacitor2_v));
  boost_panel4_boost1_controlledIdealClosingSwitch_s = (1/((-1)-(9.9999e-06)*d26-(9.9999e-06)*d29+(0.99999)*d27-(0.99998)*d26*d29-(0.99998)*d27*d28+(0.99999)*d28))*((0.99999)*d27*capacitor2_v-(0.99999)*d26*boost_panel4_boost1_inductor_i-(1e-05)*boost_panel4_boost1_inductor_i-capacitor2_v);
  boost_panel4_boost1_pin_ground_i = (1/((-1)-(9.9999e-06)*d26-(9.9999e-06)*d29+(0.99999)*d27-(0.99998)*d26*d29-(0.99998)*d27*d28+(0.99999)*d28))*((-1e-05)-(0.99999)*d29)*((0.99999)*d27*capacitor2_v-(0.99999)*d26*boost_panel4_boost1_inductor_i-(1e-05)*boost_panel4_boost1_inductor_i-capacitor2_v);
  boost_panel4_capacitor1_n_i = (-boost_panel4_panel_n_i)+boost_panel4_boost1_inductor_i;
  boost_panel3_capacitor1_n_i = boost_panel3_boost1_inductor_i-boost_panel3_panel_n_i;
  boost_panel2_capacitor1_n_i = boost_panel2_boost1_inductor_i-boost_panel2_panel_n_i;
  boost_panel1_capacitor1_n_i = boost_panel1_boost1_inductor_i-boost_panel1_panel_n_i;
  batteryAndBockBoost1_bookBoostDB_pin_p_out_i = batteryAndBockBoost1_bookBoostDB_idealDiode1_n_i+batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_n_i;
  batteryAndBockBoost1_bookBoostDB_pin_n_i = (-batteryAndBockBoost1_bookBoostDB_idealDiode_n_i)+batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_n_i;
  capacitor2_n_i = boost_panel4_boost1_idealDiode_n_i+boost_panel3_boost1_idealDiode_n_i+boost_panel1_boost1_idealDiode_n_i+batteryAndBockBoost1_bookBoostDB_pin_p_out_i+boost_panel2_boost1_idealDiode_n_i;
  boost_panel4_pin_n_i = boost_panel4_capacitor1_n_i+boost_panel4_panel_n_i+boost_panel4_boost1_pin_ground_i;
  boost_panel3_pin_n_i = boost_panel3_capacitor1_n_i+boost_panel3_panel_n_i+boost_panel3_boost1_pin_ground_i;
  boost_panel2_pin_n_i = boost_panel2_panel_n_i+boost_panel2_boost1_pin_ground_i+boost_panel2_capacitor1_n_i;
  boost_panel1_pin_n_i = boost_panel1_boost1_pin_ground_i+boost_panel1_capacitor1_n_i+boost_panel1_panel_n_i;
  batteryAndBockBoost1_pin_n_i = batteryAndBockBoost1_bookBoostDB_pin_n_i-batteryAndBockBoost1_bookBoostDB_inductor_n_i;
  ground_p_i = (-boost_panel4_pin_n_i)-boost_panel1_pin_n_i-boost_panel2_pin_n_i-capacitor2_n_i-boost_panel3_pin_n_i-batteryAndBockBoost1_pin_n_i;
  der(capacitor2_v) = (-(100)*capacitor2_n_i);
  der(boost_panel4_capacitor1_v) = (-(2127.66)*boost_panel4_capacitor1_n_i);
  boost_panel4_boost1_inductor_v = boost_panel4_capacitor1_v-boost_panel4_boost1_inductor_n_v;
  der(boost_panel4_boost1_inductor_i) = (66.6667)*boost_panel4_boost1_inductor_v;
  boost_panel4_boost1_idealDiode_LossPower = boost_panel4_boost1_idealDiode_v*((-boost_panel4_boost1_idealDiode_n_i));
  boost_panel4_boost1_controlledIdealClosingSwitch_LossPower = (boost_panel4_boost1_inductor_n_v)*((-boost_panel4_boost1_pin_ground_i));
  der(boost_panel3_capacitor1_v) = (-(2127.66)*boost_panel3_capacitor1_n_i);
  boost_panel3_boost1_inductor_v = boost_panel3_capacitor1_v-boost_panel3_boost1_inductor_n_v;
  der(boost_panel3_boost1_inductor_i) = (66.6667)*boost_panel3_boost1_inductor_v;
  boost_panel3_boost1_idealDiode_LossPower = boost_panel3_boost1_idealDiode_v*((-boost_panel3_boost1_idealDiode_n_i));
  boost_panel3_boost1_controlledIdealClosingSwitch_LossPower = (boost_panel3_boost1_inductor_n_v)*((-boost_panel3_boost1_pin_ground_i));
  der(boost_panel2_capacitor1_v) = (-(2127.66)*boost_panel2_capacitor1_n_i);
  boost_panel2_boost1_inductor_v = boost_panel2_capacitor1_v-boost_panel2_boost1_inductor_n_v;
  der(boost_panel2_boost1_inductor_i) = (66.6667)*boost_panel2_boost1_inductor_v;
  boost_panel2_boost1_idealDiode_LossPower = boost_panel2_boost1_idealDiode_v*((-boost_panel2_boost1_idealDiode_n_i));
  boost_panel2_boost1_controlledIdealClosingSwitch_LossPower = (boost_panel2_boost1_inductor_n_v)*((-boost_panel2_boost1_pin_ground_i));
  der(boost_panel1_capacitor1_v) = (-(2127.66)*boost_panel1_capacitor1_n_i);
  boost_panel1_boost1_inductor_v = boost_panel1_capacitor1_v-boost_panel1_boost1_inductor_n_v;
  der(boost_panel1_boost1_inductor_i) = (66.6667)*boost_panel1_boost1_inductor_v;
  boost_panel1_boost1_idealDiode_LossPower = boost_panel1_boost1_idealDiode_v*((-boost_panel1_boost1_idealDiode_n_i));
  boost_panel1_boost1_controlledIdealClosingSwitch_LossPower = (boost_panel1_boost1_inductor_n_v)*((-boost_panel1_boost1_pin_ground_i));
  der(batteryAndBockBoost1_bookBoostPWMcontrol1_intErr) = batteryAndBockBoost1_bookBoostPWMcontrol1_Err;
  batteryAndBockBoost1_bookBoostDB_inductor_v = batteryAndBockBoost1_bookBoostDB_inductor_p_v-batteryAndBockBoost1_bookBoostDB_idealDiode_n_v;
  der(batteryAndBockBoost1_bookBoostDB_inductor_i) = (66.6667)*batteryAndBockBoost1_bookBoostDB_inductor_v;
  batteryAndBockBoost1_bookBoostDB_idealDiode_LossPower = ((-batteryAndBockBoost1_bookBoostDB_idealDiode_n_v))*((-batteryAndBockBoost1_bookBoostDB_idealDiode_n_i));
  batteryAndBockBoost1_bookBoostDB_idealDiode1_LossPower = batteryAndBockBoost1_bookBoostDB_idealDiode1_v*((-batteryAndBockBoost1_bookBoostDB_idealDiode1_n_i));
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_LossPower = batteryAndBockBoost1_bookBoostDB_idealDiode_n_v*((-batteryAndBockBoost1_bookBoostDB_idealClosingSwitch2_n_i));
  batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_LossPower = batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_v*((-batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_n_i));
algorithm
    when time>pre(d30) then 
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
      d30:=d30+boost_panel1_mmt1_Ts;
    end when;
    when time>pre(d31) then 
      boost_panel1_cont_pwm1_dc:=0.85-(boost_panel1_mmt1_y-boost_panel1_capacitor1_v)*0.1;
      if boost_panel1_cont_pwm1_dc>0.98 then 
        boost_panel1_cont_pwm1_dc:=0.98;
      elseif boost_panel1_cont_pwm1_dc<0.3 then 
        boost_panel1_cont_pwm1_dc:=0.3;
      end if;
      boost_panel1_cont_pwm1_tnext:=time+boost_panel1_cont_pwm1_T*boost_panel1_cont_pwm1_dc;
      boost_panel1_cont_pwm1_vout:=1;
      d31:=d31+boost_panel1_cont_pwm1_T;
    end when;
    when time>boost_panel1_cont_pwm1_tnext then 
      boost_panel1_cont_pwm1_vout:=0;
    end when;
    when time>pre(d32) then 
      batteryAndBockBoost1_bookBoostPWMcontrol1_DC:=0.5+batteryAndBockBoost1_bookBoostPWMcontrol1_Kp*batteryAndBockBoost1_bookBoostPWMcontrol1_Err+batteryAndBockBoost1_bookBoostPWMcontrol1_Ki*batteryAndBockBoost1_bookBoostPWMcontrol1_intErr;
      if batteryAndBockBoost1_bookBoostPWMcontrol1_DC>0.98 then 
        batteryAndBockBoost1_bookBoostPWMcontrol1_DC:=0.98;
      elseif batteryAndBockBoost1_bookBoostPWMcontrol1_DC<0.1 then 
        batteryAndBockBoost1_bookBoostPWMcontrol1_DC:=0.1;
      end if;
      batteryAndBockBoost1_bookBoostPWMcontrol1_tnext:=time+batteryAndBockBoost1_bookBoostPWMcontrol1_T*batteryAndBockBoost1_bookBoostPWMcontrol1_DC;
      batteryAndBockBoost1_bookBoostPWMcontrol1_Output:=1;
      d32:=d32+batteryAndBockBoost1_bookBoostPWMcontrol1_T;
    end when;
    when time>batteryAndBockBoost1_bookBoostPWMcontrol1_tnext then 
      batteryAndBockBoost1_bookBoostPWMcontrol1_Output:=0;
    end when;
    when batteryAndBockBoost1_voltageControl_IrefCont>batteryAndBockBoost1_voltageControl_Imax then 
      batteryAndBockBoost1_voltageControl_Sat:=0;
      batteryAndBockBoost1_voltageControl_SatLevel:=batteryAndBockBoost1_voltageControl_Imax;
    end when;
    when batteryAndBockBoost1_voltageControl_IrefCont<batteryAndBockBoost1_voltageControl_Imax then 
      batteryAndBockBoost1_voltageControl_Sat:=1;
      batteryAndBockBoost1_voltageControl_SatLevel:=0;
    end when;
    when batteryAndBockBoost1_voltageControl_IrefCont<(((((-batteryAndBockBoost1_voltageControl_Imax))))) then 
      batteryAndBockBoost1_voltageControl_Sat:=0;
      batteryAndBockBoost1_voltageControl_SatLevel:=((((-batteryAndBockBoost1_voltageControl_Imax))));
    end when;
    when batteryAndBockBoost1_voltageControl_IrefCont>(((((-batteryAndBockBoost1_voltageControl_Imax))))) then 
      batteryAndBockBoost1_voltageControl_Sat:=1;
      batteryAndBockBoost1_voltageControl_SatLevel:=0;
    end when;
    when time>pre(d33) then 
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
      d33:=d33+boost_panel2_mmt1_Ts;
    end when;
    when time>pre(d34) then 
      boost_panel2_cont_pwm1_dc:=0.85-(boost_panel2_mmt1_y-boost_panel2_capacitor1_v)*0.1;
      if boost_panel2_cont_pwm1_dc>0.98 then 
        boost_panel2_cont_pwm1_dc:=0.98;
      elseif boost_panel2_cont_pwm1_dc<0.3 then 
        boost_panel2_cont_pwm1_dc:=0.3;
      end if;
      boost_panel2_cont_pwm1_tnext:=time+boost_panel2_cont_pwm1_T*boost_panel2_cont_pwm1_dc;
      boost_panel2_cont_pwm1_vout:=1;
      d34:=d34+boost_panel2_cont_pwm1_T;
    end when;
    when time>boost_panel2_cont_pwm1_tnext then 
      boost_panel2_cont_pwm1_vout:=0;
    end when;
    when time>pre(d35) then 
      boost_panel3_mmt1_potprev:=boost_panel3_mmt1_potact;
      boost_panel3_mmt1_potact:=boost_panel3_mmt1_potActFiltrada;
      boost_panel3_mmt1_prevu:=boost_panel3_mmt1_actu;
      boost_panel3_mmt1_actu:=boost_panel3_capacitor1_v;
      boost_panel3_mmt1_deltau:=boost_panel3_mmt1_actu-boost_panel3_mmt1_prevu;
      boost_panel3_mmt1_deltap:=boost_panel3_mmt1_potact-boost_panel3_mmt1_potprev;
      if abs(boost_panel3_mmt1_deltau)>0.1*boost_panel3_mmt1_deltaVpvRefPanel then 
        if abs(boost_panel3_mmt1_deltap)>0.2 then 
          if boost_panel3_mmt1_deltap/boost_panel3_mmt1_deltau>0 then 
            boost_panel3_mmt1_vref:=boost_panel3_mmt1_vref+boost_panel3_mmt1_deltaVpvRefPanel;
          end if;
          if boost_panel3_mmt1_deltap/boost_panel3_mmt1_deltau<0 then 
            boost_panel3_mmt1_vref:=boost_panel3_mmt1_vref-boost_panel3_mmt1_deltaVpvRefPanel;
          end if;
        end if;
      end if;
      d35:=d35+boost_panel3_mmt1_Ts;
    end when;
    when time>pre(d36) then 
      boost_panel3_cont_pwm1_dc:=0.85-(boost_panel3_mmt1_y-boost_panel3_capacitor1_v)*0.1;
      if boost_panel3_cont_pwm1_dc>0.98 then 
        boost_panel3_cont_pwm1_dc:=0.98;
      elseif boost_panel3_cont_pwm1_dc<0.3 then 
        boost_panel3_cont_pwm1_dc:=0.3;
      end if;
      boost_panel3_cont_pwm1_tnext:=time+boost_panel3_cont_pwm1_T*boost_panel3_cont_pwm1_dc;
      boost_panel3_cont_pwm1_vout:=1;
      d36:=d36+boost_panel3_cont_pwm1_T;
    end when;
    when time>boost_panel3_cont_pwm1_tnext then 
      boost_panel3_cont_pwm1_vout:=0;
    end when;
    when time>pre(d37) then 
      boost_panel4_mmt1_potprev:=boost_panel4_mmt1_potact;
      boost_panel4_mmt1_potact:=boost_panel4_mmt1_potActFiltrada;
      boost_panel4_mmt1_prevu:=boost_panel4_mmt1_actu;
      boost_panel4_mmt1_actu:=boost_panel4_capacitor1_v;
      boost_panel4_mmt1_deltau:=boost_panel4_mmt1_actu-boost_panel4_mmt1_prevu;
      boost_panel4_mmt1_deltap:=boost_panel4_mmt1_potact-boost_panel4_mmt1_potprev;
      if abs(boost_panel4_mmt1_deltau)>0.1*boost_panel4_mmt1_deltaVpvRefPanel then 
        if abs(boost_panel4_mmt1_deltap)>0.2 then 
          if boost_panel4_mmt1_deltap/boost_panel4_mmt1_deltau>0 then 
            boost_panel4_mmt1_vref:=boost_panel4_mmt1_vref+boost_panel4_mmt1_deltaVpvRefPanel;
          end if;
          if boost_panel4_mmt1_deltap/boost_panel4_mmt1_deltau<0 then 
            boost_panel4_mmt1_vref:=boost_panel4_mmt1_vref-boost_panel4_mmt1_deltaVpvRefPanel;
          end if;
        end if;
      end if;
      d37:=d37+boost_panel4_mmt1_Ts;
    end when;
    when time>pre(d38) then 
      boost_panel4_cont_pwm1_dc:=0.85-(boost_panel4_mmt1_y-boost_panel4_capacitor1_v)*0.1;
      if boost_panel4_cont_pwm1_dc>0.98 then 
        boost_panel4_cont_pwm1_dc:=0.98;
      elseif boost_panel4_cont_pwm1_dc<0.3 then 
        boost_panel4_cont_pwm1_dc:=0.3;
      end if;
      boost_panel4_cont_pwm1_tnext:=time+boost_panel4_cont_pwm1_T*boost_panel4_cont_pwm1_dc;
      boost_panel4_cont_pwm1_vout:=1;
      d38:=d38+boost_panel4_cont_pwm1_T;
    end when;
    when time>boost_panel4_cont_pwm1_tnext then 
      boost_panel4_cont_pwm1_vout:=0;
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
    when batteryAndBockBoost1_bookBoostDB_idealDiode_s<0 then 
      d5:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealDiode_s>=0 then 
      d5:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealDiode_off>0.5 then 
      d6:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealDiode_off<0.5 then 
      d6:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealDiode_off>0.5 then 
      d7:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealDiode_off<0.5 then 
      d7:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealDiode1_s<0 then 
      d8:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealDiode1_s>=0 then 
      d8:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealDiode1_off>0.5 then 
      d9:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealDiode1_off<0.5 then 
      d9:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealDiode1_off>0.5 then 
      d10:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealDiode1_off<0.5 then 
      d10:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_control>0.5 then 
      d11:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_control<0.5 then 
      d11:=0;
    end when;
    when batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_control>0.5 then 
      d12:=1;
    elsewhen batteryAndBockBoost1_bookBoostDB_idealClosingSwitch1_control<0.5 then 
      d12:=0;
    end when;
    when batteryAndBockBoost1_not2_u>0.5 then 
      d13:=1;
    elsewhen batteryAndBockBoost1_not2_u<0.5 then 
      d13:=0;
    end when;
    when batteryAndBockBoost1_not2_u>0.5 then 
      d14:=1;
    elsewhen batteryAndBockBoost1_not2_u<0.5 then 
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
    when boost_panel3_boost1_idealDiode_s<0 then 
      d20:=1;
    elsewhen boost_panel3_boost1_idealDiode_s>=0 then 
      d20:=0;
    end when;
    when boost_panel3_boost1_idealDiode_off>0.5 then 
      d21:=1;
    elsewhen boost_panel3_boost1_idealDiode_off<0.5 then 
      d21:=0;
    end when;
    when boost_panel3_boost1_idealDiode_off>0.5 then 
      d22:=1;
    elsewhen boost_panel3_boost1_idealDiode_off<0.5 then 
      d22:=0;
    end when;
    when boost_panel3_cont_pwm1_vgate>boost_panel3_boost1_controlledIdealClosingSwitch_level then 
      d23:=1;
    elsewhen boost_panel3_cont_pwm1_vgate<=boost_panel3_boost1_controlledIdealClosingSwitch_level then 
      d23:=0;
    end when;
    when boost_panel3_cont_pwm1_vgate>boost_panel3_boost1_controlledIdealClosingSwitch_level then 
      d24:=1;
    elsewhen boost_panel3_cont_pwm1_vgate<=boost_panel3_boost1_controlledIdealClosingSwitch_level then 
      d24:=0;
    end when;
    when boost_panel4_boost1_idealDiode_s<0 then 
      d25:=1;
    elsewhen boost_panel4_boost1_idealDiode_s>=0 then 
      d25:=0;
    end when;
    when boost_panel4_boost1_idealDiode_off>0.5 then 
      d26:=1;
    elsewhen boost_panel4_boost1_idealDiode_off<0.5 then 
      d26:=0;
    end when;
    when boost_panel4_boost1_idealDiode_off>0.5 then 
      d27:=1;
    elsewhen boost_panel4_boost1_idealDiode_off<0.5 then 
      d27:=0;
    end when;
    when boost_panel4_cont_pwm1_vgate>boost_panel4_boost1_controlledIdealClosingSwitch_level then 
      d28:=1;
    elsewhen boost_panel4_cont_pwm1_vgate<=boost_panel4_boost1_controlledIdealClosingSwitch_level then 
      d28:=0;
    end when;
    when boost_panel4_cont_pwm1_vgate>boost_panel4_boost1_controlledIdealClosingSwitch_level then 
      d29:=1;
    elsewhen boost_panel4_cont_pwm1_vgate<=boost_panel4_boost1_controlledIdealClosingSwitch_level then 
      d29:=0;
    end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={boost_panel1_mmt1_potActFiltrada,batteryAndBockBoost1_voltageControl_intErr,batteryAndBockBoost1_battery_Qt,batteryAndBockBoost1_battery1_Qt,batteryAndBockBoost1_battery2_Qt,boost_panel2_mmt1_potActFiltrada,boost_panel3_mmt1_potActFiltrada,boost_panel4_mmt1_potActFiltrada,capacitor2_v,boost_panel4_capacitor1_v,boost_panel4_boost1_inductor_i,boost_panel3_capacitor1_v,boost_panel3_boost1_inductor_i,boost_panel2_capacitor1_v,boost_panel2_boost1_inductor_i,boost_panel1_capacitor1_v,boost_panel1_boost1_inductor_i,batteryAndBockBoost1_bookBoostPWMcontrol1_intErr,batteryAndBockBoost1_bookBoostDB_inductor_i},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=0.5,
		Tolerance={1e-5},
		AbsTolerance={1e-5}
	));
end SistHibGen1batery_qss;
