model init_code_time
  constant Integer N=1000;
  Real u[N];

initial algorithm
  for i in 1:N-1 loop 
     u[i]:=time; 
  end for;
  u[N]:= time;

equation
  for i in 1:N loop
    der(u[i])=time;
  end for;
	annotation(

	experiment(
		MMO_Description="Use time variable in initialization code.",
		MMO_Solver=QSS1,
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
end init_code_time;
