model advection
  parameter Real alpha=0.5,mu=1000;
  constant Integer N = 20000;
  Real u[N];
  initial algorithm
  for i in 1:N/3 loop
    u[i]:=1;
  end for;

  equation
    der(u[1])=(-u[1]+1)*N-mu*u[1]*(u[1]-alpha)*(u[1]-1);
    for j in 2:N loop
      der(u[j])=(-u[j]+u[j-1])*N-mu*u[j]*(u[j]-alpha)*(u[j]-1);
  end for;
	annotation(

	experiment(
		MMO_Description="Advection",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Scotch,
		MMO_LPS=2,
		MMO_DT_Min=2,
		MMO_Output={u[N]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end advection;
