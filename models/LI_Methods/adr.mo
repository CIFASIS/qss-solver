model adr
  parameter Real a=1,d=0.1,r=100;
  constant Integer N=100;//1000;
  parameter Real dx=10/N;
 Real u[N];
equation
  der(u[1])=-a*(u[1]-1)/dx+d*(u[2]-2*u[1]+1)/dx^2+r*(u[1]^2-u[1]^3);
  der(u[N])=-a*(u[N]-u[N-1])/dx+d*(2*u[N-1]-2*u[N])/dx^2+r*(u[N]^2-u[N]^3);
  for i in 2:N-1 loop
    der(u[i])=-a*(u[i]-u[i-1])/dx+d*(u[i+1]-2*u[i]+u[i-1])/dx^2+r*(u[i]^2-u[i]^3);
  end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=EQSS2,
		MMO_Period={1e-3},
		MMO_PartitionMethod=Metis,
		MMO_Output={u[N/10:N/10:N]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		MMO_CVODEMaxOrder= 1,
		StartTime=0.0,
		StopTime=3,
		Tolerance={1e-2},
		AbsTolerance={1e-4}
	));
end adr;
