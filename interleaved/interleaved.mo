model interleaved
	constant Integer N = 4;
	parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.5, ROn = 1e-5, ROff = 1e5;
	discrete Real Rd[N](each start=1e5), Rs[N](each start=1e5), nextT(start=T),lastT;
	Real uC,iL[N];

	 equation
		der(uC) = (sum(iL) - uC/R)/C;
	for i in 1:N loop
//der(iL[i]) = time - lastT-T*(i-1)/N-DC*T/N-0.01*T;
	    der(iL[i]) = (((U/Rs[i]) - iL[i]) * (Rs[i]*Rd[i]/(Rs[i]+Rd[i])) - uC)/L;
	end for;
	   
algorithm
	when time >= nextT then
		lastT:=nextT;
		nextT:=nextT+T;
	end when;
	for i in 1:N loop
		when time - lastT-T*(i-1)/N-0.01*T>0 then
		  Rs[i] := ROn;
		  Rd[i] := ROff;
		end when;
	end for;
	for i in 1:N loop
		when time - lastT-T*(i-1)/N-DC*T/N-0.01*T>0 then
		  Rs[i] := ROff;
		  Rd[i] := ROn;
		end when;
	end for;
	for i in 1:N loop
		when iL[i]<0 then
		  Rd[i] := ROff;
		end when;
	end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={uC,iL},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=0.01,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end interleaved;
