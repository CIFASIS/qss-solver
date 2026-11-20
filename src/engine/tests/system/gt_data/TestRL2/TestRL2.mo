model TestRL2
  constant Integer N=100;
  Real iL[N];
  Real iR[N];
  Real uL[N];
  parameter Real L=1;
  parameter Real R=1;
  parameter Real L1=1;
  parameter Real I=1;
  parameter Real R0=1;
equation
  iR[N] = ((I*(-1))+iL[N]);
  for i in 99:99 loop
    iR[(-i)+198] = (iL[(-i)+198]+iR[(-i)+198+1]);
  end for;
  for i in 1:98 loop
    iR[(-i)+99] = (iL[(-i)+99]+iR[(-i)+99+1]);
  end for;
  uL[1] = (((iR[1]*R)*(-1))+((iR[1]*R0)*(-1)));
  for i in 1:1 loop
    der(iL[(-i)+2]) = (((L^(-1)))*uL[(-i)+2]);
  end for;
  for i in 1:1:99 loop
    uL[i+1] = (((R*iR[i+1])*(-1))+uL[i]);
  end for;
  for i in 2:100 loop
    der(iL[(-i)+102]) = (((L^(-1)))*uL[(-i)+102]);
  end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=DASSL,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_Output={iL[N]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=1,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end TestRL2;
