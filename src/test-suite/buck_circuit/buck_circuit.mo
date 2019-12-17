model buck_circuit
	parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.5, ROn = 1e-5, ROff = 1e5;
	discrete Real Rd, Rs, nextT,lastT;
	Real uC,iL;

	initial algorithm 
	 nextT:=T;
	 lastT:=0;
	 uC:=0;
	 Rs:=1e5;
	 Rd:=1e5;
	  iL:=0;

	 equation
	 	der(uC) = (iL - uC/R)/C;
		der(iL) = (((U/Rs) - iL) * (Rs*Rd/(Rs+Rd)) - uC)/L;
	   
	algorithm
	
	when time > nextT then
		lastT:=nextT;
		nextT:=nextT+T;
		Rs := ROn;
		Rd := ROff;
	end when;

	when time - lastT-DC*T>0 then
		  Rs := ROff;
		  Rd := ROn;
	end when;

	when iL<0 then
		  Rd := ROff;
	end when;
	annotation(

	experiment(
		MMO_Description="Buck converter working in discontinuous mode.",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={uC,iL},
		Jacobian=Dense,
		StartTime=0,
		StopTime=0.01,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end buck_circuit;
