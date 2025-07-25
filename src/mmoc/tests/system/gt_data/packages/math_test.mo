model math_test
  import math;
  Real th[10];

  equation
    for i in 1:10 loop
      der(th[i]) = sin(1);
    end for;
	annotation(

	experiment(
		MMO_Description="Power consumption  in a large population of air conditioners.",
		MMO_Solver=QSS2,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=3000,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end math_test;
