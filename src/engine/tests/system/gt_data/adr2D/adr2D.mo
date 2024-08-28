model adr2D
  parameter Real a=1,d=0.01,r=1000;
  constant Integer N=10;
  parameter Real dx=10/N;
 Real u[N,N];
equation
  der(u[1,1])=-a*(u[1,1]-1)/dx-a*(u[1,1]-1)/dx+d*(u[2,1]-2*u[1,1]+1)/dx^2+d*(u[1,2]-2*u[1,1]+1)/dx^2+r*(u[1,1]^2-u[1,1]^3);
  der(u[N,N])=-a*(u[N,N]-u[N-1,N])/dx-a*(u[N,N]-u[N,N-1])/dx+d*(2*u[N-1,N]-2*u[N,N])/dx^2+d*(2*u[N,N-1]-2*u[N,N])/dx^2+r*(u[N,N]^2-u[N,N]^3);
    der(u[1,N])=-a*(u[1,N]-1)/dx-a*(u[1,N]-u[1,N-1])/dx+d*(u[2,N]-2*u[1,N]+1)/dx^2+d*(2*u[1,N-1]-2*u[1,N])/dx^2+r*(u[1,N]^2-u[1,N]^3);
    der(u[N,1])=-a*(u[N,1]-u[N-1,1])/dx-a*(u[N,1]-1)/dx+d*(2*u[N-1,1]-2*u[N,1])/dx^2+d*(u[N,2]-2*u[N,1]+1)/dx^2+r*(u[N,1]^2-u[N,1]^3);

  for i in 2:N-1,j in 2:N-1 loop
    der(u[i,j])=-a*(u[i,j]-u[i-1,j])/dx-a*(u[i,j]-u[i,j-1])/dx+d*(u[i+1,j]-2*u[i,j]+u[i-1,j])/dx^2+d*(u[i,j+1]-2*u[i,j]+u[i,j-1])/dx^2+r*(u[i,j]^2-u[i,j]^3);
 end for;
   for j in 2:N-1 loop
    der(u[1,j])=-a*(u[1,j]-1)/dx-a*(u[1,j]-u[1,j-1])/dx+d*(u[2,j]-2*u[1,j]+1)/dx^2+d*(u[1,j+1]-2*u[1,j]+u[1,j-1])/dx^2+r*(u[1,j]^2-u[1,j]^3);
    der(u[N,j])=-a*(u[N,j]-u[N-1,j])/dx-a*(u[N,j]-u[N,j-1])/dx+d*(2*u[N-1,j]-2*u[1,j])/dx^2+d*(u[N,j+1]-2*u[N,j]+u[N,j-1])/dx^2+r*(u[N,j]^2-u[N,j]^3);
  end for; 
   for i in 2:N-1 loop
  der(u[i,1])=-a*(u[i,1]-u[i-1,1])/dx-a*(u[i,1]-1)/dx+d*(u[i+1,1]-2*u[i,1]+u[i-1,1])/dx^2+d*(u[i,2]-2*u[i,1]+1)/dx^2+r*(u[i,1]^2-u[i,1]^3); 
  der(u[i,N])=-a*(u[i,N]-u[i-1,N])/dx-a*(u[i,N]-u[i,N-1])/dx+d*(u[i+1,N]-2*u[i,N]+u[i-1,N])/dx^2+d*(2*u[i,N-1]-2*u[i,N])/dx^2+r*(u[i,N]^2-u[i,N]^3);
end for; 
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=CVODE_BDF,
		MMO_PartitionMethod=Scotch,
		MMO_LPS=4,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=1e-3,
		MMO_Output={u[2,2]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=10,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end adr2D;
