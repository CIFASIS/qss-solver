model rectifier
	parameter Real Ron=1e-5,Roff=1e5,U=311,L=1e-3,R=10,w=314.16;
	Real iL,u;
	discrete Real Rd(start=1e-5);
	
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
		MMO_Solver=LIQSS2,
		MMO_Output={iL,u},
		StartTime=0,
		StopTime=1,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end rectifier;
