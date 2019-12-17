model advectionFlux
  constant Integer N = 100, M = 101;
  parameter Real L = 1.0, v = 1.0, beta = 100.0, x0 = 0.5, diff = 0.0000, gam = 0.0, x[N], inVal = 0, outVal = 1;
  Real u[N], F[M];
initial algorithm
  for i in 1:N / 2 loop
    x[i] := 1.0 * i / N;
    u[i] := 1;
  end for;
  //		u[i]:=2.718^(-beta*(x[i]-x0)^2)*cos(gam*(x[i]-x0)); //Condici�n inicial: Gaussiana con m�ximo=1
equation
  //Upwind advection
  F[1] = v * inVal;
  for j in 2:M loop
    F[j] = v * u[j - 1];
  end for;
  der(u[1]) = -(F[2] - F[1]) * N / L;
  for j in 2:N loop
    der(u[j]) = -(F[j + 1] - F[j]) * N / L;
    //  der(u[j]) = -((v*u[j])-(v*u[j-1]))*N/L;
end for;
	annotation(

	experiment(
		MMO_Description=" Advection Equation.",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={u[N]},
		Jacobian=Dense,
		StartTime= 0.0,
		StopTime= 1,
		Tolerance={1e-5},
		AbsTolerance={1e-5}
	));
end advectionFlux;
