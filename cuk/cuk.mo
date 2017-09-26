model cuk
	parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.25, ROn = 1e-5, ROff = 1e5;
	parameter Real C1=1e-4,L1=1e-4;
	discrete Real Rd, Rs, nextT,lastT;
	Real uC1,iL1,uC,iL,iD;

	initial algorithm 
	 nextT:=T;
	 Rs:=1e5;
	 Rd:=1e5;

	 equation
		iD=(Rs*(iL+iL1)-uC1)/(Rd+Rs);
	 	der(uC1) = (iD - iL)/C1;
		der(iL1) =  (U-uC1-iD*Rd)/L1;
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
	end when;

	when iD<0 then
		  Rd := ROff;
	end when;
	annotation(

	experiment(
		MMO_Description="Cuk converter with non-solved stiffness.",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={uC,iL,uC1,iL1},
		Jacobian=Dense,
		StartTime=0,
		StopTime=0.001,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end cuk;
