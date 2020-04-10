model aircont
  import math;
  constant Integer N = 200;
  parameter Real CAP[N], RES[N], POT[N], THA = 32,pmax=0,Kp=1,Ki=1,tref=20;
  Real th[N];
  Real ierr;
  discrete Real ptotal,ptotals;
  discrete Real on[N];
  discrete Real dtref,pref(start=0.5);
  discrete Real nextSample(start=1);
  discrete Real noise(start=rand(2)-1);


  initial algorithm
  for i in 1:N loop
    th[i] := rand(4)+ 18;
    CAP[i] := rand(100)+ 550;
    RES[i] := rand(0.4)+ 1.8;
    POT[i] := rand(0.2)+ 13;
    pmax:=pmax+POT[i];
  end for;

  for i in 1:N loop
   if th[i] - tref   - 0.5> 0 then
        on[i] := 1;
        ptotal := ptotal + POT[i];
    end if;
  end for;

  equation
  for i in 1:N loop
    der(th[i]) = (THA/RES[i]-POT[i]*on[i]-th[i]/RES[i]+noise/RES[i])/CAP[i];
  end for;
  der(ierr) = pref-ptotals/pmax;

  algorithm
  for i in 1:N loop
    when th[i] - tref -dtref + on[i] - 0.5 > 0 then
        on[i] := 1;
        ptotal := ptotal + POT[i];
    elsewhen th[i] - tref -dtref + on[i] - 0.5 < 0 then
        on[i] := 0;
        ptotal := ptotal - POT[i];
    end when;
  end for;
  when time > 1000 then
    pref := 0.4;
  end when;
  when time > 2000 then
    pref := 0.5;
  end when;
  when time > nextSample then
    nextSample := nextSample+1;
    noise := rand(2)-1;
    ptotals := ptotal;
    dtref := Kp*(ptotals/pmax-pref)-Ki*ierr;
  end when;
	annotation(

	experiment(
		MMO_Description="Control of the power consumption of a large populaion of  air conditioners.",
		MMO_Solver=DOPRI,
		MMO_PartitionMethod=Metis,
		MMO_Output={ptotal},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=3000,
		Tolerance={1e-4},
		AbsTolerance={1e-4}
	));
end aircont;
