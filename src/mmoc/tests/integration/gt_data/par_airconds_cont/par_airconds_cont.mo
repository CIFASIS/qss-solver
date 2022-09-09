model par_airconds_cont
  function getSection
    input Integer x;
    output Real y;
    external "C" y = getSection(x) annotation(
                                Include="#include \"sections.c\"");
  end getSection;

  import math;
  constant Integer N = 4000;
  constant Integer SECTIONS = 4;
  parameter Real CAP[N], RES[N], POT[N], THA = 32,pmax=0,Kp=1,Ki=1,tref=20;
  parameter Integer sections[N];
  parameter Integer sections_init[SECTIONS];
  parameter Integer sections_end[SECTIONS];
  Real th[N];
  Real ierr;
  discrete Real ptotals;
  discrete Real partTotal[SECTIONS];
  discrete Real sendPartTotal[SECTIONS];
  Real ptotal;
    
  discrete Real on[N];
  discrete Real dtref,pref(start=0.5);
  discrete Real nextSample(start=1);
  discrete Real partSample[SECTIONS](each start=1);
  discrete Real noise[N];
  discrete Real update[SECTIONS];
  discrete Real sampleNoise[N];
  discrete Real inputs[N];

  initial algorithm
    for i in 1:N loop
      th[i] := random(4)+ 18;
      CAP[i] := random(100)+ 550;
      RES[i] := random(0.4)+ 1.8;
      POT[i] := random(0.2)+ 13;
      pmax:=pmax+POT[i];
      noise[i] := 2*sin(i)^2;
      sampleNoise[i] := random(2);
      sections[i] := getSection(i);
    end for;

		   for i in 1:SECTIONS loop
												sections_init[i] := (i-1)*N/SECTIONS;
       sections_end[i] := i*N/SECTIONS;
    end for;


    for i in 1:N loop
      if th[i] - tref   - 0.5> 0 then
        on[i] := 1;
        ptotal := ptotal + POT[i];
      end if;
    end for;

  equation
    for i in 1:N loop
      der(th[i]) = (THA/RES[i]-POT[i]*on[i]-th[i]/RES[i]+noise[i]/RES[i])/CAP[i];
    end for;
    der(ierr) = pref-ptotals/pmax;
   der(ptotal) = 0;

  algorithm
    for i in 1:SECTIONS loop
      when time > partSample[i] then
        partSample[i] := partSample[i]+1;
        update[i] := 1;
        sendPartTotal[i] := partTotal[i];
														reinit(ptotal, ptotal + sendPartTotal[i]);        
        partTotal[i] := 0;
      end when  annotation(MMO_Event_Id="Ev_1",
																																																			  																																MMO_HH={(Ev_6[_d2],[1:SECTIONS, sections_init[i]:sections_end[i]])});
    end for;

    for i in 1:SECTIONS loop
      when update[i] > 0.5 then
        reinit(ptotal, ptotal + sendPartTotal[i]);
        update[i] := 0;
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
      ptotals := ptotal;
      dtref := Kp*(ptotals/pmax-pref)-Ki*ierr;
	end when;

    for i in 1:N loop
      when th[i] - tref -dtref + on[i] - 0.5 > 0 then
        on[i] := 1;
        partTotal[sections[i]]:= partTotal[sections[i]] + POT[i];
      elsewhen th[i] - tref -dtref + on[i] - 0.5 < 0 then
        on[i] := 0;
        partTotal[sections[i]]:= partTotal[sections[i]] - POT[i];
       end when annotation(MMO_Event_Id="Ev_6",
                                                  MMO_LHS_DSC={partTotal[sections[i]]},
																																																																																						MMO_HH={Ev_1[sections[i]]}	);
		
    end for;

    for i in 1:N loop
      when time > sampleNoise[i] then
        sampleNoise[i] := sampleNoise[i] + 1;
         inputs[i] := inputs[i] + 1;
        noise[i] := 2*sin(i*inputs[i])^2;
      end when;
    end for;
	annotation(

	experiment(
		MMO_Description="Control of the power consumption of a large populaion of  air conditioners.",
		MMO_Solver=QSS3,
		MMO_Period={3000/5000},
		MMO_Parallel=true,
		MMO_PartitionMethod=Scotch,
		MMO_LPS=4,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=10,
		MMO_Output={ptotals},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=3000,
		Tolerance={1e-4},
		AbsTolerance={1e-4}
	));
end par_airconds_cont;
