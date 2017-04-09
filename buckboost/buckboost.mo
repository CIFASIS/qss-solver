model buckboost
	parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.25, ROn = 1e-5, ROff = 1e5;
	discrete Real Rd, Rs, nextT,lastT,diodeon;
	Real uC,iL,iD;

	initial algorithm 
	 nextT:=T;
	 Rs:=1e5;
	 Rd:=1e5;

	 equation
		iD=(Rs*iL-uC-U)/(Rd+Rs);
	 	der(uC) = (iD - uC/R)/C;
		der(iL) =  (-uC-iD*Rd)/L;
	   
	algorithm
	
	when time > nextT then
		lastT:=nextT;
		nextT:=nextT+T;
		Rs := ROn;
	end when;

	when time - lastT-DC*T>0 then
		  Rs := ROff;
		  Rd := ROn;
		diodeon:=1;
	end when;

	when iD*diodeon<0 then
		  Rd := ROff;
		diodeon:=0;
	end when;

	annotation(
	experiment(
		MMO_Description="Buck-Boost Circuit.",
		MMO_Solver=LIQSS2,
		MMO_Output={uC,iL},
		StartTime=0,
		StopTime=0.01,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end buckboost;
