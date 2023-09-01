model mliqss_test
Real x1, x2;

initial equation
x1 = 4;
x2 = -4;

equation
der(x1) = -x1 -x2 + 0.2;
der(x2) = x1 - x2 + 1.2;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=mLIQSS,
		MMO_PartitionMethod=Metis,
		MMO_Output={x1, x2},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=80,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end mliqss_test;
