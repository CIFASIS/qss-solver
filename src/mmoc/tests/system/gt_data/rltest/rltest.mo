model rltest
  constant Integer N=10;
  Real x1[N],x2[1];
  Real a1[N];
  parameter Real R=1,C=1,L=1;
equation
  a1[1]=((10-x1[1])^3)/R;
  for i in 1:N-1 loop
    a1[i+1]=((x1[i]-x1[i+1])^3)/R;
  end for;
for i in 1:N-1 loop
    der(x1[i])=((a1[i]-a1[i+1])^3)/C;
  end for;
   der(x1[N])=(a1[N]-x2[1])/C;
  der(x2[1])=x1[N]/L;
  
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=CVODE_BDF,
		MMO_PartitionMethod=Metis,
		MMO_Output={x1[1:N],x2[1]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1000,
		Tolerance={1e-6},
		AbsTolerance={1e-6}
	));
end rltest;
