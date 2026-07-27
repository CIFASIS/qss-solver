model buck_inv_delay_2d
  parameter Real R = 10, U = 24, T = 1/1000, ROn = 1e-5, ROff = 1e5, Delay = 1e-9;
  discrete Real lastT, DC1(start = 0.1), DC2(start = 0.9), nextT(start = T);
  //buck1
  parameter Real C1 = 1e-4, L1 = 1e-4;
  discrete Real Rd12(start = 1e5), Rd11(start = 1e5), Rs11(start = 1e-5), Rs12(start = 1e5), diodeon12, diodeon11;
  Real uC1, iL1, iD12, s12,iD11,s11;
  
  //buck2
  parameter Real C2 = 1e-4, L2 = 1e-4;
  discrete Real Rd22(start = 1e5), Rd21(start = 1e5), Rs21(start = 1e-5), Rs22(start = 1e5), diodeon22, diodeon21;
  Real uC2, iL2, iD22, s22,iD21,s21;
  discrete Real nextT0(start = 1e5);
  discrete Real nextT1(start = 1e5);
  discrete Real nextT2(start = 1e5);
  Real dU;
equation
//buck1
  iD12 = (iL1*(Rs11*Rd11/(Rs11+Rd11)) - U)*Rs12/((Rs11*Rd11/(Rs11+Rd11))*Rs12 + (Rs11*Rd11/(Rs11+Rd11))*Rd12 + Rs12*Rd12);
  s12 = diodeon12*iD12 + (1 - diodeon12)*iD12*Rd12;
  iD11=(-((iL1*(Rs11*Rd11/(Rs11+Rd11)) - U)*Rs12/((Rs11*Rd11/(Rs11+Rd11))*Rs12 + (Rs11*Rd11/(Rs11+Rd11))*Rd12 + Rs12*Rd12))*Rd12-U)/Rd11;
  s11= diodeon11*iD11 + (1 - diodeon11)*iD11*Rd11;
  der(iL1) = (-iD12*Rd12 - uC1)/L1;
  der(uC1) = (iL1 - (uC1 - 1*uC2)/R)/C1;
//buck2
  iD22 = (iL2*(Rs21*Rd21/(Rs21+Rd21)) - U)*Rs22/((Rs21*Rd21/(Rs21+Rd21))*Rs22 + (Rs21*Rd21/(Rs21+Rd21))*Rd22 + Rs22*Rd22);
  s22 = diodeon22*iD22 + (1 - diodeon22)*iD22*Rd22;
  iD21=(-((iL2*(Rs21*Rd21/(Rs21+Rd21)) - U)*Rs22/((Rs21*Rd21/(Rs21+Rd21))*Rs22 + (Rs21*Rd21/(Rs21+Rd21))*Rd22 + Rs22*Rd22))*Rd22-U)/Rd21;
  s21= diodeon21*iD21 + (1 - diodeon21)*iD21*Rd21;
  der(iL2) = (-iD22*Rd22 - uC2)/L2;
  der(uC2) = (iL2 - (uC2 - 1*uC1)/R)/C2;
  dU = uC1 - uC2;
algorithm
  when time > nextT then //PWM start period
    lastT := nextT;      //Last PWM period start time
    nextT := nextT + T;  //Next PWM period start time
    Rs12 := ROff;        //Set T12 and T22 to the OFF state
    Rs22 := ROff;
    DC1 := (sin(314*time) + 1)*0.3 + 0.2;  //Buck1 Duty cycle
    DC2 := 1 - DC1;         //Buck2 Duty cycle
    nextT0 := time + Delay; //Delay before seting T11
                            //and T21 to the On state
  end when;
  when time - nextT0 > 0 then
    Rs11 := ROn;  //Set T11 and T21 to the On state
    Rs21 := ROn;
  end when;
  when time - lastT - DC1*T > 0 then // PWM1 edge transition
    Rs11 := ROff;           //Set T11 to the OFF state
    nextT1 := time + Delay; //Delay before seting On T12
  end when;
  when time - nextT1 > 0 then
    Rs12 := ROn;            //Set T12 to the On state
  end when;
  when time - lastT - DC2*T > 0 then //PWM2 edge transition
    Rs21 := ROff;           //Set T21 to the OFF state
    nextT2 := time + Delay; //Delay before seting On T22
  end when;
  when time - nextT2 > 0 then
    Rs22 := ROn;    //Set T22 to the On state
  end when;
//Buck 1 Diode state commander
  when s12 > 0 then //Diode12 conduction state control
    Rd12 := ROn;
    diodeon12 := 1;
  elsewhen s12 < 0 then 
    Rd12 := ROff;
    diodeon12 := 0;
  end when;
  when s11 > 0 then //Diode11 conduction state control
    Rd11 := ROn;
    diodeon11 := 1;
  elsewhen s11 < 0 then
    Rd11 := ROff;
    diodeon11 := 0;
  end when;
  
//Buck 2 Diode state commander
  when s22 > 0 then //Diode22 conduction state control
    Rd22 := ROn;
    diodeon22 := 1;
  elsewhen s22 < 0 then
    Rd22 := ROff;
    diodeon22 := 0;
  end when;
  when s21 > 0 then //Diode21 conduction state control
    Rd21 := ROn;
    diodeon21 := 1;
  elsewhen s21 < 0 then
    Rd21 := ROff;
    diodeon21 := 0;
  end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=CHEQSS2,
		MMO_Output={dU},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end buck_inv_delay_2d;
