model allow_discrete_integer
  constant Integer N=1000;
  Real u[N];
  discrete Integer d;

equation
  for i in 1:N loop
    der(u[i])=time;
  end for;

algorithm
  when time > 0 then
    reinit(u[d],0);
  end when;
	annotation(
	experiment(
		MMO_Description="Use time variable in initialization code.",

		MMO_Solver=QSS,
		MMO_PartitionMethod=Metis,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		MMO_RandomSeed=0,
		StartTime=0,
		StopTime=10,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end allow_discrete_integer;
