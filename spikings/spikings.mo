model spikings
	import math;
	constant Integer N=1000;
	Real v[N];
	Real u[N];
	parameter Real a=0.02,b=0.2;
	parameter Real c[N],d[N],I[N];
	parameter Real r[N];
	
	initial algorithm
	for i in 1:N loop
	  v[i]:=-60;
	  u[i]:=0;
	  r[i]:=rand(1);
	  c[i]:=-65+15*r[i]^2;
	  d[i]:=8-6*r[i]^2;
	  I[i]:=rand(10);
	end for;
	equation
	for i in 1:N loop
	  der(v[i])=0.04*v[i]*v[i]+5*v[i]+140-u[i]+I[i];
	  der(u[i]) = a*(b*v[i]-u[i]);
	end for;

	algorithm
	for i in 1:N loop
		when v[i] > 30 then
      		reinit(v[i],c[i]);
      		reinit(u[i],u[i]+d[i]);
    	end when;
	end for;
	annotation(

	experiment(
		MMO_Description="Spiking Neurons.",
		MMO_Solver=LIQSS2,
		MMO_Output={v[1],v[2]},
		StartTime=0,
		StopTime=1000,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end spikings;
