model rectifierLIQSS3
	parameter Real Ron=1e-5,Roff=1e5,U=311,L=1e-3,R=10,w=314.16;
	Real iL,u;
	discrete Real Rd(start=1e5);
	
	equation
		der(u) = 1000*(U*sin(w*time)-u);
		der(iL) = (u-iL*(R+Rd))/L;
	algorithm
    when iL < 0 then
    	Rd:=Roff;
	end when;
    when u > 0 then
    	Rd:=Ron;
  	end when;
	annotation(

	experiment(
		MMO_Description="Half wave rectifier.",
		MMO_Solver=LIQSS3,
		MMO_SymDiff=false,
		MMO_Period={1/5000},
		MMO_PartitionMethod=Metis,
		MMO_Output={iL,u},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=1,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end rectifierLIQSS3;
