model buck
  parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.5, ROn = 1e-5, ROff = 1e5;
  discrete Real Rd(start=1e5), Rs(start=1e-5), nextT(start=T),lastT,diodeon;
  Real uC,iL,iD,s;


  equation
    iD=(iL*Rs-U)/(Rs+Rd);
    s=diodeon*iD+(1-diodeon)*iD*Rd;
    der(iL) = (-iD*Rd- uC)/L;
     der(uC) = (iL - uC/R)/C;

  algorithm

    when time > nextT then
      lastT:=nextT;
      nextT:=nextT+T;
      Rs := ROn;
    end when;

    when time - lastT-DC*T>0 then
      Rs := ROff;
    end when;

    when s>0 then
      Rd:=ROn;
      diodeon:=1;
    elsewhen s<0 then
      Rd := ROff;
       diodeon:=0;
    end when;
	annotation(

	experiment(
		MMO_Description="Buck converter.",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={uC,iL},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=0.01,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end buck;
