model TestRL3
  constant Integer N=100;
  Real iL[N];
  Real iR[N];
  Real uL[N];
  parameter Real L=1;
  parameter Real R=1;
  parameter Real L1=1;
  parameter Real U=1;
  parameter Real R0=1;
  Real tearing_var_guess_iR_N(start=1);
  Real tearing_var_res_iR_N(start=1);
equation
  iR[N] = tearing_var_guess_iR_N;
  for i in 99:99 loop
    iR[(-i)+198] = (iL[(-i)+198]+iR[(-i)+198+1]);
  end for;
  for i in 1:98 loop
    iR[(-i)+99] = (iL[(-i)+99]+iR[(-i)+99+1]);
  end for;
  uL[1] = (U+((iR[1]*R)*(-1)));
  for i in 1:1 loop
    uL[(-i)+2+1] = (((R*iR[(-i)+2+1])*(-1))+uL[(-i)+2]);
  end for;
  for i in 2:1:99 loop
    uL[i+1] = (((iR[i+1]*R)*(-1))+uL[i]);
  end for;
  tearing_var_res_iR_N = (((uL[N]*(-1))+(R0*iR[N]))+((R0*iL[N])*(-1)));
  tearing_var_res_iR_N = 0;
  for i in 1:1 loop
    der(iL[(-i)+2]) = (((L^(-1)))*uL[(-i)+2]);
  end for;
  for i in 2:1:100 loop
    der(iL[i]) = (((L^(-1)))*uL[i]);
  end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=DASSL,
		MMO_Output={iL[1]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=10,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end TestRL3;
