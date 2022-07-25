model adr
	constant Integer N=1000, CI = 1e-3;
  parameter Real a=1;
  parameter Real d[N];
  parameter Real r=1000;
  parameter Real L=10;
  parameter Real dx=L/N;
  parameter Real dx2=dx^2; 
  Real u[N];

initial algorithm
  // BDF computed variables.
  for i in N/2:N loop 
     d[i]:=10; 
  end for;
  
  // LIQSS2 computed variables.
  for i in 1:N/2-1 loop 
     d[i]:=1e-6;
  end for;


equation
  der(u[1])=-a*(u[1]-1)/dx+d[i]*(u[2]-2*u[1]+1)/dx2+r*(u[1]^2)*(1-u[1]);
  der(u[N])=-a*(u[N]-u[N-1])/dx+d[i]*(u[N]-2*u[N]+u[N-1])/dx2+r*(u[N]^2)*(1-u[N]);
  for i in 2:N-1 loop
    der(u[i])=-a*(u[i]-u[i-1])/dx+ d[i]*(u[i+1]-2*u[i]+u[i-1])/dx2+ r*(u[i]^2)*(1-u[i]);
  end for;
	annotation(

	experiment(
		MMO_Description="Advection Reaction Diffusion problem.",
		MMO_Solver=LIQSS_BDF,
		MMO_Period={10/5000},
		MMO_PartitionMethod=Metis,
		MMO_Output={u[750]},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		MMO_BDF_Part={u[N/2:N]},
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=10,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end adr;
