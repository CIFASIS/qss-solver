model airconds
	import math;
	constant Integer N = 2000;
	parameter Real CAP[N], RES[N], POT[N], THA = 32,pmax=0;
	Real th[N];
discrete Real on[N];
discrete Real tref[N];
discrete Real nextSample[N];
discrete Real noise[N];
discrete Real nextTref[N]; 
	
	initial algorithm
	for i in 1:N loop
		th[i] := rand(4)+ 18;
		CAP[i] := rand(100)+ 550;
		RES[i] := rand(0.4)+ 1.8;
		POT[i] := rand(2)+ 13;
		pmax:=pmax+POT[i];
		nextSample[i] := 1;
		noise[i] := rand(2)-1;
		tref[i] := 20;
 nextTref[i] := 1000;
	end for;
	for i in 1:N loop
if th[i] - tref[i]  - 0.5 > 0 then
				on[i] := 1;
end if;
end for;

	equation
	for i in 1:N loop
		der(th[i]) = (THA/RES[i]-POT[i]*on[i]-th[i]/RES[i]+noise[i]/RES[i])/CAP[i];
	end for;
	
	algorithm
	for i in 1:N loop
		when th[i] - tref[i] + on[i] - 0.5 > 0 then
				on[i] := 1;
		elsewhen th[i] - tref[i] + on[i] - 0.5 < 0 then
				on[i] := 0;
		end when;
	end for;
	for i in 1:N loop
		when time > nextTref[i] then
   if (nextTref[i] == 1000) then			
     tref[i] := 20.5;
   else
      tref[i] := 20;
    end if;
   nextTref[i] := 2000;
		end when;
	end for;
	for i  in 1:N loop
		when time > nextSample[i] then
			nextSample[i] := nextSample[i]+1;
			noise[i] := 2*abs(sin(i*time))-1;
		end when;
	end for;
	annotation(

	experiment(
		MMO_Description="Power consumption  in a large population of air conditioners.",
		MMO_Solver=QSS2,
		MMO_Period={3000/5000},
		MMO_Parallel=true,
		MMO_PartitionMethod=HMetis,
		MMO_LPS=4,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=3000,
		MMO_Output={th[1]},
		StartTime=0,
		StopTime=3000,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end airconds;
