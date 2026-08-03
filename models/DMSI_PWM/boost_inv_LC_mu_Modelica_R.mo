model boost_inv_LC_mu_Modelica_R
  /************************************************************************
    * To execute this model using CheQSS method go to:                                            *
    *       -- File -> Settings                                                                                                          *
    *       -- Add --replace-algs to **Compiler** options in the Falgs section.            *
    ************************************************************************/ 
 
  parameter Real ROn = 1e-5, ROff = 1e5;
  //Load parameters
  Real R(start = 32.3);
  discrete Real lastR(start = 32.3);
  discrete Real timeLastR(start = 0);
  discrete Real pteR(start = 0);
  //DC voltage source parameters
  Real U(start = 48);
  discrete Real lastU(start = 48);
  discrete Real timeLastU(start = 0);
  discrete Real pteU(start = 0);
  //commutation parameters
  Real T(start = 5e-5);
  discrete Real lastTc(start = 5e-5);
  discrete Real timeLastTc(start = 0);
  discrete Real pteTc(start = 0);
  discrete Real nextT(start = 5e-5), lastT;
  Real dU;
  //boost1
  parameter Real C1 = 30e-6, L1 = 150e-6;
  discrete Real Rd12(start = 1e5), Rs11(start = 1e-5), Rs12(start = 1e5), diodeon1;
  Real uC1, iL1, iD1, s1;
  discrete Real DC1(start = 0.25);
  //boost2
  parameter Real C2 = 30e-6, L2 = 150e-6;
  discrete Real Rd22(start = 1e5), Rs21(start = 1e-5), Rs22(start = 1e5), diodeon2;
  Real uC2, iL2, iD2, s2;
  discrete Real DC2(start = 0.25);
  //Referenses
  parameter Real Vdc = 226;
  //Output voltage Offset of V01 and V02
  parameter Real VrefMax = 220;
  //reference signal amplitude
  parameter Real frecRef = 50;
  //reference signal frequency
  Real Vref, V01ref, V02ref;
  //filters Variables;
  parameter Real Tf = 0.000025;
  //Voltage control 1 Variables
  Real iL1ref;
  Real V01filt(start = 110);
  Real i01filt;
  Real PiV1intErr;
  Real PiVOut1;
  Real PiVerr1;
  Real satVInV1;
  parameter Real kpv = 0.059;
  parameter Real kiv = 4.99e-4;
  parameter Real satUppIL1Ref = 1000;
  parameter Real satLowIL1Ref = -100;
  discrete Real SatV1(start = 0);
  discrete Real satValV1(start = 0);
  //Voltage control 2 Variables
  Real iL2ref;
  Real V02filt(start = 110);
  Real i02filt;
  Real PiV2intErr;
  Real PiVOut2;
  Real PiVerr2;
  Real satVInV2;
  parameter Real satUppIL2Ref = 1000;
  parameter Real satLowIL2Ref = -100;
  discrete Real SatV2(start = 0);
  discrete Real satValV2(start = 0);
  //Current control 1 Variables
  Real iL1Filt;
  Real PiI1intErr;
  Real PiIOut1;
  Real PiIerr1;
  Real satIInDC1;
  parameter Real kpi = 3.529;
  parameter Real kii = 8.44e-5;
  parameter Real satUppDC1Ref = 0.95;
  parameter Real satLowDC1Ref = 0.05;
  discrete Real SatI1(start = 0);
  discrete Real satValDC1(start = 0);
  Real DC1ref;
  //Current control 2 Variables
  Real iL2Filt;
  Real PiI2intErr;
  Real PiIOut2;
  Real PiIerr2;
  Real satIInDC2;
  parameter Real satUppDC2Ref = 0.95;
  parameter Real satLowDC2Ref = 0.05;
  discrete Real SatI2(start = 0);
  discrete Real satValDC2(start = 0);
  Real DC2ref;
equation
//================
//VoRefCreator
//================
  Vref = VrefMax*sin(2*3.1416*frecRef*time);
  V01ref = Vdc + Vref/sqrt(2);
  V02ref = uC1 - sqrt(2)*Vref;
  U = lastU + pteU*(time - timeLastU);
//======================
//VoltageLoop 1
//======================
  der(V01filt) = (uC1 - V01filt)/Tf;
  der(i01filt) = (dU/R - i01filt)/Tf;
  PiVerr1 = V01ref - V01filt;
  der(PiV1intErr) = PiVerr1*(1 - SatV1);
  PiVOut1 = kiv*PiV1intErr + kpv*PiVerr1;
  satVInV1 = (PiVOut1 + i01filt)*V01filt/U;
  iL1ref = satVInV1*(1 - SatV1) + satValV1;
//======================
//VoltageLoop 2
//======================
  der(V02filt) = (uC2 - V02filt)/Tf;
  der(i02filt) = (-dU/R - i02filt)/Tf;
  PiVerr2 = V02ref - V02filt;
  der(PiV2intErr) = PiVerr2*(1 - SatV2);
  PiVOut2 = kiv*PiV2intErr + kpv*PiVerr2;
  satVInV2 = (PiVOut2 + i02filt)*V02filt/U;
  iL2ref = satVInV2*(1 - SatV2) + satValV2;
//======================
//Current Loop 1
//======================
  der(iL1Filt) = (iL1 - iL1Filt)/Tf;
  PiIerr1 = iL1ref - iL1Filt;
  der(PiI1intErr) = PiIerr1*(1 - SatI1);
  PiIOut1 = kii*PiI1intErr + kpi*PiIerr1;
  satIInDC1 = (U - PiIOut1)/(abs(V01filt) + 1e-8);
  DC1ref = satIInDC1*(1 - SatI1) + satValDC1;
//======================
//Current Loop 2
//======================
  der(iL2Filt) = (iL2 - iL2Filt)/Tf;
  PiIerr2 = iL2ref - iL2Filt;
  der(PiI2intErr) = PiIerr2*(1 - SatI2);
  PiIOut2 = kii*PiI2intErr + kpi*PiIerr2;
  satIInDC2 = (U - PiIOut2)/(abs(V02filt) + 1e-8);
  DC2ref = satIInDC2*(1 - SatI2) + satValDC2;
//======================
//boost1
//======================
  iD1 = (Rs11*iL1 - uC1)*Rs12/(Rs11*Rs12 + Rs11*Rd12 + Rs12*Rd12);
  s1 = diodeon1*iD1 + (1 - diodeon1)*iD1*Rd12;
  der(uC1) = (iD1 + iD1*Rd12/Rs12 - (uC1 - uC2)/R)/C1;
  der(iL1) = (U - Rs11*(iL1 - (iD1 + iD1*Rd12/Rs12)))/L1;
//======================
//boost 2
//======================
  iD2 = (Rs21*iL2 - uC2)*Rs22/(Rs21*Rs22 + Rs21*Rd22 + Rs22*Rd22);
  s2 = diodeon2*iD2 + (1 - diodeon2)*iD2*Rd22;
  der(uC2) = (iD2 + iD2*Rd22/Rs22 - (uC2 - uC1)/R)/C2;
  der(iL2) = (U - Rs21*(iL2 - (iD2 + iD2*Rd22/Rs22)))/L2;
  R = lastR + pteR*(time - timeLastR);
  T = 1/20000;
  dU = uC1 - uC2;
algorithm
//======================
//Voltage control1
//======================
  when satVInV1 > satUppIL1Ref then
    SatV1 := 1;
    satValV1 := satUppIL1Ref;
  end when;
  when satVInV1 < satUppIL1Ref then
    SatV1 := 0;
    satValV1 := 0;
  end when;
  when satVInV1 < satLowIL1Ref then
    SatV1 := 1;
    satValV1 := satLowIL1Ref;
  end when;
  when satVInV1 > satLowIL1Ref then
    SatV1 := 0;
    satValV1 := 0;
  end when;
//======================
//Voltage control 2
//======================
  when satVInV2 > satUppIL2Ref then
    SatV2 := 1;
    satValV2 := satUppIL1Ref;
  end when;
  when satVInV2 < satUppIL2Ref then
    SatV2 := 0;
    satValV2 := 0;
  end when;
  when satVInV2 < satLowIL2Ref then
    SatV2 := 1;
    satValV2 := satLowIL2Ref;
  end when;
  when satVInV2 > satLowIL2Ref then
    SatV2 := 0;
    satValV2 := 0;
  end when;
//======================
//Current control  1
//======================
  when satIInDC1 > satUppDC1Ref then
    SatI1 := 1;
    satValDC1 := satUppDC1Ref;
  end when;
  when satIInDC1 < satUppDC1Ref then
    SatI1 := 0;
    satValDC1 := 0;
  end when;
  when satIInDC1 < satLowDC1Ref then
    SatI1 := 1;
    satValDC1 := satLowDC1Ref;
  end when;
  when satIInDC1 > satLowDC1Ref then
    SatI1 := 0;
    satValDC1 := 0;
  end when;
//======================
//Current control  1
//======================
  when satIInDC2 > satUppDC2Ref then
    SatI2 := 1;
    satValDC2 := satUppDC2Ref;
  end when;
  when satIInDC2 < satUppDC2Ref then
    SatI2 := 0;
    satValDC2 := 0;
  end when;
  when satIInDC2 < satLowDC2Ref then
    SatI2 := 1;
    satValDC2 := satLowDC2Ref;
  end when;
  when satIInDC2 > satLowDC2Ref then
    SatI2 := 0;
    satValDC2 := 0;
  end when;
//=========================
//New PWM start Period
//=========================
  when time > nextT then
    lastT := nextT;
    nextT := nextT + T;
    Rs11 := ROff;
    Rs12 := ROn;
    Rs21 := ROff;
    Rs22 := ROn;
    DC1 := DC1ref;
    DC2 := DC2ref;
  end when;
  when time - lastT - DC1*T > 0 then
    Rs11 := ROn;
    Rs12 := ROff;
  end when;
  when time - lastT - DC2*T > 0 then
    Rs21 := ROn;
    Rs22 := ROff;
  end when;
  when s1 > 0.6 then
    Rd12 := ROn;
    diodeon1 := 1;
  end when;
  when s1 < 0 then
    Rd12 := ROff;
    diodeon1 := 0;
  end when;
  when s2 > 0.6 then
    Rd22 := ROn;
    diodeon2 := 1;
  end when;
  when s2 < 0 then
    Rd22 := ROff;
    diodeon2 := 0;
  end when;
//====================================
//Perturbation introduced
//====================================
//-----------------------------
//DC Voltage source perturbation
//-----------------------------
  when time > 0 then
    lastU := 48;
    pteU := 0;
    timeLastU := 0;
  end when;
  when time > 0.1021 then
    lastU := 48;
    pteU := -(48*0.2)/0.01;
    timeLastU := 0.1021;
  end when;
  when time > 0.1021 + 0.01 then
    lastU := 48*0.8;
    pteU := 0;
    timeLastU := 0.1021 + 0.01;
  end when;
  when time > 0.15 then
    lastU := 48;
    pteU := 0;
    timeLastU := 0.15;
  end when;
//-----------------------------
//Load perturbation
//-----------------------------
  when time > 0 then
    lastR := 32.3;
    pteR := 0;
    timeLastR := 0;
  end when;
  when time > 0.51 then
    lastR := 32.3;
    pteR := (32.3*0.2)/0.015;
    timeLastR := 0.51;
  end when;
  when time > 0.51 + 0.015 then
    lastR := 32.3*1.2;
    pteR := 0;
    timeLastR := 0.51 + 0.015;
  end when;
  when time > 0.55 then
    lastR := 32.3;
    pteR := 0;
    timeLastR := 0.55;
  end when;
//-----------------------------
//Commutation frequency perturbation
//-----------------------------
  when time > 0 then
    lastTc := 1/20000;
    pteTc := 0;
    timeLastTc := 0;
  end when;
  when time > 0.887 then
    lastTc := 1/20000;
    pteTc := (0.000005 - 0.00005)/0.04;
    timeLastTc := 0.887;
  end when;
  when time > 0.887 + 0.04 then
    lastTc := 0.000005;
    pteTc := 0;
    timeLastTc := 0.887 + 0.04;
  end when;
  when time > 0.94 then
    lastTc := 1/20000;
    pteTc := 0;
    timeLastTc := 0.94;
  end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=DASSL,
    MMO_SymDiff=false,
		MMO_Period={1e-6},
		MMO_Output={dU,iL1},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));

end boost_inv_LC_mu_Modelica_R;
