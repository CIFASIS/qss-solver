model GOLDBETER
	// BioRica Constant vm1
	parameter Real vm1 = 3.0;
	// BioRica Constant vm3
	parameter Real vm3 = 1.0;
	// BioRica Constant vi
	parameter Real vi = 0.025;
	// BioRica Constant vd
	parameter Real vd = 0.25;
	// BioRica Constant Kd
	parameter Real Kd = 0.02;
	// BioRica Constant kd
	parameter Real kd = 0.01;
	// BioRica Constant k1
	parameter Real k1 = 0.005;
	// BioRica Constant k2
	parameter Real k2 = 0.005;
	// BioRica Constant k3
	parameter Real k3 = 0.005;
	// BioRica Constant k4
	parameter Real k4 = 0.005;
	// BioRica Constant kc
	parameter Real kc = 0.5;
	// BioRica Constant v2
	parameter Real v2 = 1.5;
	// BioRica Constant v4
	parameter Real v4 = 0.5;
	Real c(start = 0.01);
	Real x(start = 0.01);
	Real m(start = 0.01);
	equation
	
		der(c) = vi-vd*x*(c*1.0/(Kd+c))-kd*c;
		der(m) = (c*1.0/(kc+c))*vm1*(1-m)*1.0/(k1+(1-m))-v2*m*1.0/(k2+m);
		der(x) = m*vm3*(1-x)*1.0/(k3+(1-x))-v4*x*1.0/(k4+x);
	annotation(

	experiment(
		MMO_Description="GOLDBETER automatic model translation.",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={c,m,x},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=400,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end GOLDBETER;
