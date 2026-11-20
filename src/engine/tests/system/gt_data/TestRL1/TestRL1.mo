model TestRL1
  constant Integer N=100;
  parameter Real U0=1;
  Real iL[N];
  Real Ua[N];
  Real Uc[N];
  parameter Real Ra=1;
  parameter Real Rb=1;
  parameter Real Rc=1;
  parameter Real L=1;
  Real tearing_var_guess_Ua[N-1](start=1);
  Real tearing_var_res_Ua[N-1](start=1);
  Real tearing_var_guess_Uc_N(start=1);
  Real tearing_var_res_Uc_N(start=1);
equation
  for i in 1:1:1 loop
    Ua[i] = tearing_var_guess_Ua[i];
    Uc[i] = (((((Ra*iL[i])+(tearing_var_guess_Ua[i]*(-1)))+((iL[i+1]*Ra)*(-1)))*Rc)*((Ra^(-1))));
    tearing_var_res_Ua[i] = (((((Rb*Ra)*iL[i])+((tearing_var_guess_Ua[i]*Ra)*(-1)))+((Rb*tearing_var_guess_Ua[i])*(-1)))*((Ra^(-1))));
    tearing_var_res_Ua[i] = 0;
  end for;
  for i in 2:1:99 loop
    Ua[i] = tearing_var_guess_Ua[i];
    Uc[i] = ((Rc*(((iL[i]*Ra)+(tearing_var_guess_Ua[i]*(-1)))+((iL[i+1]*Ra)*(-1))))*((Ra^(-1))));
    tearing_var_res_Ua[i] = ((((Ra^(-1)))*(((Rb*tearing_var_guess_Ua[i])+(tearing_var_guess_Ua[i]*Ra))+(((Rb*iL[i])*Ra)*(-1))))*(-1));
    tearing_var_res_Ua[i] = 0;
  end for;
  der(iL[1]) = (((Ua[1]+(U0*(-1)))*((L^(-1))))*(-1));
  for i in 2:1:2 loop
    der(iL[i]) = (((L^(-1)))*(Uc[i-1]+(Ua[i]*(-1))));
  end for;
  for i in 3:1:99 loop
    der(iL[i]) = ((((L^(-1)))*(Ua[i]+(Uc[i-1]*(-1))))*(-1));
  end for;
  Uc[N] = tearing_var_guess_Uc_N;
  Ua[N] = ((Rb*Uc[N])*((Rc^(-1))));
  tearing_var_res_Uc_N = ((((Rc^(-1)))*(((Uc[N]*Ra)+(((iL[N]*Rc)*Ra)*(-1)))+(Ua[N]*Rc)))*((Ra^(-1))));
  tearing_var_res_Uc_N = 0;
  for i in 100:100 loop
    der(iL[(-i)+200]) = ((((L^(-1)))*(Ua[(-i)+200]+(Uc[(-i)+200-1]*(-1))))*(-1));
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
		StopTime=100,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end TestRL1;
