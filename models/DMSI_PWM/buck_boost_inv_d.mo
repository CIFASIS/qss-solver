model buck_boost_inv_d
  parameter Real R = 10, U = 24, T =1/20000, ROn = 1e-5, ROff = 1e5,Delay=1e-9;
  discrete Real nextT(start=T),lastT;

//buckboost1
  parameter Real C1 = 1e-4, L1 = 1e-4;
  discrete Real Rd12(start=1e5), Rs11(start=1e-5), Rs12(start=1e5), diodeon1;
  Real uC1,iL1,iD12,s1;
  discrete Real DC1(start=0.25);

//buckboost2
  parameter Real C2 = 1e-4, L2 = 1e-4;
  discrete Real Rd22(start=1e5), Rs21(start=1e-5), Rs22(start=1e5), diodeon2;
  Real uC2,iL2,iD22,s2;
  discrete Real DC2(start=0.25);

Real dU;
  discrete Real nextT0(start=1e5);
  discrete Real nextT1(start=1e5);
  discrete Real nextT2(start=1e5);

  equation
//buckboost1
    iD12=(Rs11*iL1-uC1-U)*Rs12/(Rs11*Rd12+Rs11*Rs12+Rd12*Rs12);
    s1=diodeon1*iD12+(1-diodeon1)*iD12*Rd12;
    der(uC1) = (iD12+iD12*Rd12/Rs12 - (uC1-uC2)/R)/C1;
    der(iL1) =  (-uC1-iD12*Rd12)/L1;

     dU=uC1-uC2;
//buckboost2
    iD22=(Rs21*iL2-uC2-U)*Rs22/(Rs21*Rd22+Rs21*Rs22+Rd22*Rs22);
    s2=diodeon2*iD22+(1-diodeon2)*iD22*Rd22;
    der(uC2) = (iD22+iD22*Rd22/Rs22 - (uC2-uC1)/R)/C2;
    der(iL2) =  (-uC2-iD22*Rd22)/L2;

  algorithm

    when time > nextT then
      lastT:=nextT;
      nextT:=nextT+T;
      Rs12 := ROff;
      Rs22 := ROff;
      DC1 := (sin(314*time)+1)*0.3+0.2;
      DC2 := 1-DC1;
      nextT0:= time+Delay;
    end when;
    when time -nextT0>0 then
      Rs11 := ROn;
      Rs21 := ROn;
    end when;

    when time - lastT-DC1*T>0 then
      Rs11 := ROff;
      nextT1:= time+Delay;
    end when;
    when time-nextT1>0 then
      Rs12 := ROn;
    end when;

    when time - lastT-DC2*T>0 then
      Rs21 := ROff;
      nextT2:= time+Delay;
    end when;
    when time-nextT2>0 then
      Rs22 := ROn;
    end when;

    when s1>0 then
      Rd12 :=ROn;
      diodeon1:=1;
    elsewhen s1<0 then
      Rd12 := ROff;
       diodeon1:=0;
    end when;

    when s2>0 then
      Rd22 :=ROn;
      diodeon2:=1;
    elsewhen s2<0 then
      Rd22 := ROff;
       diodeon2:=0;
    end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=CHEQSS2,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime= 0.1,
		Tolerance={0.001},
		AbsTolerance={1e-3}
	));
end buck_boost_inv_d;
