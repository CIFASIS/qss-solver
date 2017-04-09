model boost
	parameter Real C = 1e-4, L = 1e-4, R = 10, U = 24, T = 1e-4, DC = 0.5, ROn = 1e-5, ROff = 1e5;
	discrete Real Rd(start=1e5), Rs(start=1e-5), nextT(start=T),lastT,diodeon;
	Real uC,iL,iD,s;



	 equation
		iD=(Rs*iL-uC)/(Rd+Rs);
s=diodeon*iD+(1-diodeon)*iD*Rd;
	 	der(uC) = (iD - uC/R)/C;
		der(iL) =  (U-Rs*(iL-iD))/L;
	   
	algorithm
	
		when time > nextT then
		lastT:=nextT;
		nextT:=nextT+T;
		Rs := ROn;
	end when;

	when time - lastT-DC*T>0 then
		  Rs := ROff;
	end when;

	when s>0.6 then
  Rd:=ROn;
  diodeon:=1;
end when;
when s<0 then
		  Rd := ROff;
 		diodeon:=0;
	end when;
	annotation(

	experiment(
		MMO_Description="Boost Circuit.",
		MMO_Solver=LIQSS2,
		MMO_Output={uC,iL},
		StartTime=0,
		StopTime=0.01,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end boost;
