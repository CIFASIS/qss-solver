model cuk2
	parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.25, ROn = 1e-5, ROff = 1e5;
	parameter Real C1=1e-4,L1=1e-4;
	discrete Real Rd, Rs, nextT,lastT,diodeon;
	Real uC1,iL1,uC,iL,iD,phi;

	initial algorithm 
	 nextT:=T;
	 Rs:=1e5;
	 Rd:=1e5;

	 equation
		iL1=(L*phi+L*iL)/L1;
		iD=(Rs*(iL+iL1)-uC1)/(Rd+Rs);
	 	der(uC1) = (iD - iL)/C1;
		der(phi) = ( U+uC-uC1)/L;
	 	der(uC) = (iL - uC/R)/C;
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

	when diodeon*iD<0 then
		  Rd := ROff;
		 diodeon:=0;
	end when;
	annotation(

	experiment(
		MMO_Description="Cuk converter with change of variables.",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={iL,iL1,uC,uC1},
		Jacobian=Dense,
		StartTime=0,
		StopTime=0.01,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end cuk2;
