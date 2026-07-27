model cuk2_inv_delay
  parameter Real R = 10, U = 24, T = 1/20000, ROn = 1e-5, ROff = 1e5, Delay = 1e-9;
  discrete Real nextT(start = T), lastT;
  Real dU;
  discrete Real nextT0(start = 1e5);
  discrete Real nextT1(start = 1e5);
  discrete Real nextT2(start = 1e5);
  //cuk1 converter
  parameter Real C10 = 1e-4, L10 = 1e-4;
  parameter Real C11 = 1e-4, L11 = 1e-4;
  discrete Real Rd12(start = 1e5), Rs11(start = 1e-5), Rs12(start = 1e5), diodeon1;
  Real uC11, iL11, uC10, iL10, iD1, phi1, s1;
  discrete Real DC1(start = 0.25);
  // cuk2 converter
  parameter Real C20 = 1e-4, L20 = 1e-4;
  parameter Real C21 = 1e-4, L21 = 1e-4;
  discrete Real Rd22(start = 1e5), Rs21(start = 1e-5), Rs22(start = 1e5), diodeon2;
  Real uC21, iL21, uC20, iL20, iD2, phi2, s2;
  discrete Real DC2(start = 0.25);
equation
//cuk1
  iL11 = (L10*phi1 + L10*iL10)/L11;
  iD1 = (Rs11*(iL10 + iL11) - uC11)*Rs12/(Rs11*Rs12 + Rs11*Rd12 + Rs12*Rd12);
  s1 = diodeon1*iD1 + (1 - diodeon1)*iD1*Rd12;
  der(uC11) = (iD1 + iD1*Rd12/Rs12 - iL10)/C11;
  der(phi1) = (U + uC10 - uC11)/L10;
  der(uC10) = (iL10 - (uC10 - uC20)/R - 0*uC10/R)/C10;
  der(iL10) = ((-uC10) - iD1*Rd12)/L10;
//cuk2
  iL21 = (L20*phi2 + L20*iL20)/L21;
  iD2 = (Rs21*(iL20 + iL21) - uC21)*Rs22/(Rs21*Rs22 + Rs21*Rd22 + Rs22*Rd22);
  s2 = diodeon2*iD2 + (1 - diodeon2)*iD2*Rd22;
  der(uC21) = (iD2 + iD2*Rd22/Rs22 - iL20)/C21;
  der(phi2) = (U + uC20 - uC21)/L20;
  der(uC20) = (iL20 - (uC20 - uC10)/R - 0*uC20/R)/C20;
  der(iL20) = ((-uC20) - iD2*Rd22)/L20;
  dU = uC10 - uC20;
algorithm
  when time > nextT then
    lastT := nextT;
    nextT := nextT + T;
    Rs12 := ROff;
    Rs22 := ROff;
    DC1 := (sin(314*time) + 1)*0.3 + 0.2;
    DC2 := 1 - DC1;
    nextT0 := time + Delay;
  end when;
  when time - nextT0 > 0 then
    Rs11 := ROn;
    Rs21 := ROn;
  end when;
  when time - lastT - DC1*T > 0 then
    Rs11 := ROff;
    nextT1 := time + Delay;
  end when;
  when time - nextT1 > 0 then
    Rs12 := ROn;
  end when;
  when time - lastT - DC2*T > 0 then
    Rs21 := ROff;
    nextT2 := time + Delay;
  end when;
  when time - nextT2 > 0 then
    Rs22 := ROn;
  end when;
  when s1 > 0 then
    Rd12 := ROn;
    diodeon1 := 1;
  elsewhen s1 < 0 then
    Rd12 := ROff;
    diodeon1 := 0;
  end when;
  when s2 > 0 then
    Rd22 := ROn;
    diodeon2 := 1;
  elsewhen s2 < 0 then
    Rd22 := ROff;
    diodeon2 := 0;
  end when;

	annotation(
	experiment(
		MMO_Description="",
		MMO_Solver=CHEQSS2,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end cuk2_inv_delay;
