model acousticsRiemann
	//Este caso NO corre
	constant Integer N = 100;
	parameter Real K0=1.0,rho0=1.00,c0,Z0,u0=0,beta=100,x0=0.5,gam=0.0,x[N],inVal=0,outVal=1;
	parameter Real p0[N];	//Variables naturales
	Real w1[N],w2[N]; //Invariantes de Riemann
	initial algorithm
	c0:=sqrt(K0/rho0);
		Z0:=rho0*c0;
		for i in 1:N loop
			x[i]:=1.0*i/N;
			p0[i]:=2.718^(-beta*(x[i]-x0)^2) *cos(gam*(x[i]-x0));
			w1[i]:=1/2/Z0*(-p0[i]);
			w2[i]:=1/2/Z0*p0[i];
		end for;
	

	equation
	/*	for i in 1:N loop
			p[i]=Z0*(w2[i]-w1[i]); 			u[i]=(w2[i]+w1[i]);
		end for;*/
//Upwind:
		der(w1[1])=(u0 - c0)*(-w1[2]+w1[1])*N;	//Condicion de circuito cerrado
		der(w2[1])=(u0 + c0)*(-w2[1]+w2[N])*N;
		for j in 2:N-1 loop
			der(w1[j])=(u0 - c0)*(-w1[j+1]+w1[j])*N;
			der(w2[j])=(u0 + c0)*(-w2[j]+w2[j-1])*N;
		end for;
		der(w1[N])=(u0 - c0)*(-w1[1]+w1[N])*N;
		der(w2[N])=(u0 + c0)*(-w2[N]+ w2[N-1])*N;
	annotation(

	experiment(
		MMO_Description="Advection-Reaction Equation.",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_Period={1e-1},
		MMO_Output={w1[1:N/2:N],w2[1:N/2:N]},
		MMO_OutputType=CI_Sampled,
		StartTime=0.0,
		StopTime=100,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end acousticsRiemann;
