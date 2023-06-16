model mliqss_adr
  constant Integer N = 1000;
  parameter Real a=1;
  parameter Real d=0.001;
  parameter Real r=1000;
  parameter Real dx=10/N;
  Real u[N];

  initial algorithm
  for i in 1:N/5 loop
    u[i]:=1;
  end for;

  equation
  der(u[1])=-a*(u[1]-1)/dx + d*(u[2]-2*u[1] + 1)/(dx^2)+ r*(u[1]^2)*(1-u[1]);
  der(u[N])=-a*(u[N]-u[N-1])/dx + d*(u[N-1]-2*u[N] + u[N-1])/(dx^2) + r*(u[N]^2)*(1-u[N]);
  for i in 2:N-1 loop
    der(u[i])=-a*(u[i]-u[i-1])/dx +d*(u[i + 1]-2*u[i] + u[i-1])/(dx^2) +r*(u[i]^2)*(1-u[i]);
  end for;
	annotation(

	experiment(
		MMO_Description="Advection",
		MMO_Solver=mLIQSS2,
		MMO_PartitionMethod=Scotch,
		MMO_LPS=2,
		MMO_DT_Min=2,
		MMO_Output={u[1:100:N]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=10,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end mliqss_adr;
