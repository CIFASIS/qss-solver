model advection
//Modelo de adveccion pura upwind
	constant Integer N = 10000;
	parameter Real alpha=100,u=1.0, qBC=1.0,x0=0.5,x[N],beta=0.0;
	Real q[N];

	initial algorithm

//Pulso Gaussiano
	for i in 1:N loop
		x[i]:=1.0*i/N;
		q[i]:=2.718^(-alpha*(x[i]-x0)^2);	//Gaussiana
	end for;
/*
//Escalon
	for i in 1:N/2 loop	
		q[i]:=1;
	end for;
*/
	equation
//Condiciones de contorno cíclicas
		der(q[1])=u*(-q[1]+q[N])*N+beta*(q[2]-2*q[1]+q[N])*N^2;
		der(q[N])=u*(-q[N]+q[N-1])*N+beta*(q[1]-2*q[N]+q[N-1])*N^2;
		for i in 2:(N-1) loop
			der(q[i])=u*(-q[i]+q[i-1])*N+beta*(q[i+1]-2*q[i]+q[i-1])*N^2;
		end for;
	
	annotation(

	experiment(
		MMO_Description="Advection equation",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_Output={q[N/2]},
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-5},
		AbsTolerance={1e-5}
	));
end advection;
