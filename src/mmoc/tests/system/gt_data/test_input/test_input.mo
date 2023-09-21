model test_input

Real y;

equation
der(y) = cos(time);
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS3,
		MMO_PartitionMethod=Metis,
		MMO_Output={y},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=100,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end test_input;
