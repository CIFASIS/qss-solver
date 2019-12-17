model TYSON
	import math;
	// BioRica Constant k1
	parameter Real k1 = 0.015;
	// BioRica Constant k2
	parameter Real k2 = 0.0;
	// BioRica Constant k3
	parameter Real k3 = 200;
	// BioRica Constant k4
	parameter Real k4 = 180;
	// BioRica Constant k4prim
	parameter Real k4prim = 0.018;
	// BioRica Constant k5
	parameter Real k5 = 0.0;
	// BioRica Constant k6
	parameter Real k6 = 1.0;
	// BioRica Constant k7
	parameter Real k7 = 0.6;
	// BioRica Constant k8
	parameter Real k8 = 1000000.0;
	// BioRica Constant k9
	parameter Real k9 = 1000.0;
	Real CP(start = 0.75);
	Real yt;
	Real C2(start = 0.0);
	Real M(start = 0.0);
	Real pM(start = 0.25);
	Real CT;
	Real Y(start = 0.0);
	Real YT;
	Real F;
	Real yP(start = 0.0);
	equation
	
		// BioRica Formula CT instance: 
		CT = C2+CP+pM+M;
		// BioRica Formula YT instance: 
		YT = Y+yP+pM+M;
		// BioRica Formula F instance: 
		F = k4prim+k4*pow((M*1.0/CT),2);
		yt = YT;
		der(C2) = k6*M-k8*C2+k9*CP;
		der(CP) = -k3*CP*Y+k8*C2-k9*CP;
		der(pM) = k3*CP*Y+-pM*F+k5*M;
		der(M) = pM*F-k5*M-k6*M;
		der(Y) = k1-k2*Y-k3*CP*Y;
		der(yP) = k6*M-k7*yP;
	annotation(

	experiment(
		MMO_Description="TYSON automatic model translation.",
		MMO_Solver=LIQSS2,
		MMO_Output={C2,CP,pM,M,Y,yP},
		StartTime=0.0,
		StopTime=100,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end TYSON;
