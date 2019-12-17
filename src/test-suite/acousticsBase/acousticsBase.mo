model acoustics
	constant Integer N = 200;
	parameter Real K0=1.0,rho0=1.00,c0,Z0,u0=0,beta=1000,x0=0.25,gam=00.0,x[N],inVal=0,outVal=1;
	Real p[N],u[N](each start=0.0);
	initial algorithm
	for i in 1:N loop
		x[i]:=1.0*i/N;
		p[i]:=2.718^(-beta*(x[i]-x0)^2); //para cond oscilatoria: *cos(gam*(x[i]-x0))
//		u[i]:=2^2;
	end for;
	c0:=sqrt(K0/rho0);
	Z0:=rho0*c0;
		
	equation
//Upwind: (extraído de Leveque FVMbook, eqs 2.52)
		der(p[1])=K0*(-u[1]+u[N])*N;	//Condicion de circuito cerrado para presión
		der(u[1])=1/rho0*(-p[2]+p[1])*N;	//Condicion de circuito cerrado para velocidad
		for j in 2:N-1 loop
			der(p[j])=K0*(-u[j]+u[j-1])*N;
		 	der(u[j])=1/rho0*(-p[j+1]+p[j])*N;
		end for;
		der(p[N])=K0*(-u[N]+u[N-1])*N;
		der(u[N])=1/rho0*(-p[1]+p[N])*N;
	annotation(

	experiment(
		MMO_Description="Linear acoustics.",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={p[1:N/5:N],u[100]},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=1,
		Tolerance={1e-5},
		AbsTolerance={1e-5}
	));
end acoustics;
