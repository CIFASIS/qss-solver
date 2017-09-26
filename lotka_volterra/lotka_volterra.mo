model lotka_volterra
	Real x[2];
	initial algorithm 
		x[1] := 0.5;
		x[2] := 0.5;
	equation
		der(x[1]) = 0.1 * x[1] - 0.1 * x[1]*x[2];
		der(x[2]) = 0.1 * x[1]*x[2] - 0.1 * x[2];
	annotation(

	experiment(
		MMO_Description="Lotka Volterra model",
		MMO_Solver=QSS3,
		MMO_PartitionMethod=Metis,
		MMO_Output={x[:]},
		Jacobian=Dense,
		StartTime= 0.0,
		StopTime= 300.0,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-6}
	));
end lotka_volterra;
