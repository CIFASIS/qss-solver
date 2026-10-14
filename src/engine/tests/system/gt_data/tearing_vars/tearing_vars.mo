model tearing_vars
  Real x,a;
  Real tearing_var_guess_1,tearing_var_res_1;
equation
  a = tearing_var_guess_1;
  tearing_var_res_1=x+a;
  tearing_var_res_1=0;
  der(x)=1+a;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=DASSL,
		MMO_Output={x},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end tearing_vars;
