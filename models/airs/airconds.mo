model airconds
  import math;
  constant Integer N = 20000;
  parameter Real CAP[N], RES[N], POT[N], THA = 32,pmax=0;
  Real th[N];
  discrete Real ptotal;
  discrete Real on[N];
  discrete Real tref[N];
  discrete Real nextSample[N];
  discrete Real noise[N];

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
  end for;
  for i in 1:N loop
if th[i] - tref[i]  - 0.5 > 0 then
        on[i] := 1;
          ptotal := ptotal + POT[i];
    elseif th[i] - tref[i] < 0 then
              on[i] := 0;

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
        ptotal := ptotal + POT[i];
    elsewhen th[i] - tref[i] + on[i] - 0.5 < 0 then
              on[i] := 0;
         ptotal := ptotal - POT[i];
    end when;
  end for;
  for i in 1:N loop
    when time > 1000 then
      tref[i] := 20.5;
    end when;
    when time > 2000 then
      tref[i] := 20;
    end when;
  end for;
  for i  in 1:N loop
    when time > nextSample[i] then
      nextSample[i] := nextSample[i]+1;
      noise[i] := rand(2)-1;
    end when;
  end for;
	annotation(

	experiment(
		MMO_Description="Power consumption  in a large population of air conditioners.",
		MMO_Solver=QSS3,
		MMO_Parallel=true,
		MMO_PartitionMethod=Metis,
		MMO_LPS=4,
		MMO_DT_Min=2,
		MMO_Output={ptotal,th[1]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=3000,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end airconds;
