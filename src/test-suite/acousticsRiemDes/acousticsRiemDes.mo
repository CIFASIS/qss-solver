model acousticsRiemDes
//Este caso no plotea los w, pero en OMEdit si lo hace
  constant Integer N = 200;
  parameter Real K0 = 9.0, rho0 = 1.00, c = 3.0, Z0 = 3.0, u0 = 0.0, beta = 1000, x0 = 0.5, gam = 0.0;
  Real p[N], u[N], w1[N], w2[N];	//w1 y w2: invariantes de Riemann
initial algorithm
  for i in 1:N loop
    p[i] := 2.718 ^ (-beta * (1.0 * i / N - x0) ^ 2) * cos(gam * (1.0 * i / N - x0));
    u[i] := 0;
    w1[i] := 1 / 2 / Z0 * ((-p[i]) + u[i]);
    w2[i] := 1 / 2 / Z0 * (p[i] + u[i]);
  end for;
equation
//Resuelto en variables primitivas
  der(p[1]) = K0 * ((-u[1]) + u[N]) * N;
  der(u[1]) = 1 / rho0 * ((-p[2]) + p[1]) * N;
  for j in 2:N - 1 loop
    der(p[j]) = K0 * ((-u[j]) + u[j - 1]) * N;
    der(u[j]) = 1 / rho0 * ((-p[j + 1]) + p[j]) * N;
  end for;
  der(p[N]) = K0 * ((-u[N]) + u[N - 1]) * N;
  der(u[N]) = 1 / rho0 * ((-p[1]) + p[N]) * N;
//Resuelto en invariantes de Riemann: estos no los plotea acá pero sí en OMEdit.
  der(w1[1]) = (u0 - c) * ((-w1[2]) + w1[1]) * N;
  der(w2[1]) = (u0 + c) * ((-w2[1]) + w2[N]) * N;
  for j in 2:N - 1 loop
    der(w1[j]) = (u0 - c) * ((-w1[j + 1]) + w1[j]) * N;
    der(w2[j]) = (u0 + c) * ((-w2[j]) + w2[j - 1]) * N;
  end for;
  der(w1[N]) = (u0 - c) * ((-w1[1]) + w1[N]) * N;
  der(w2[N]) = (u0 + c) * ((-w2[N]) + w2[N - 1]) * N;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS3,
		MMO_Output={p[1:N/5:N]},
		StartTime=0.0,
		StopTime=1,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end acousticsRiemDes;
