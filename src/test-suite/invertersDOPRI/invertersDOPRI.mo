model invertersDOPRI
  constant Integer N = 501;
  Real x[N + 1];
  discrete Real nextchange(start = 5),uslope;
  discrete Real satx[N],satdx[N];
  parameter Real UOP = 5,G = 100,UTH = 1;
initial algorithm
  for i in 1:N / 2 loop
      x[2 * i]:=5;
    x[2 * i + 1]:=0.006247;
  end for;
equation
  der(x[1]) = uslope;
  for i in 2:N + 1 loop
  der(x[i]) = UOP - x[i] - G * (satx[i - 1] * (x[i - 1] - UTH) ^ 2 - satdx[i - 1] * (x[i - 1] - x[i] - UTH) ^ 2);

  end for;
algorithm
  for i in 1:N loop
      when x[i] > UTH then
          satx[i]:=1;    elsewhen x[i] <= UTH then
      satx[i]:=0;
    end when;
  end for;
  for i in 1:N loop
      when x[i] - x[i + 1] > UTH then
          satdx[i]:=1;    elsewhen x[i] - x[i + 1] <= UTH then
      satdx[i]:=0;
    end when;
  end for;
  when time > nextchange then
      if nextchange == 5 then 
      uslope:=1;
      nextchange:=10;
    elseif nextchange == 10 then
      uslope:=0;
      nextchange:=15;

    elseif nextchange == 15 then
      uslope:=-2.5;
      nextchange:=17;
    else
      uslope:=0;
      nextchange:=10000000000.0;
    end if;  
  end when;
	annotation(

	experiment(
		MMO_Description="A chain of logical inverters",
		MMO_Solver=DOPRI,
		MMO_Period={200/5000},
		MMO_Output={x[N],x[1]},
		MMO_OutputType=CI_Sampled,
		StartTime= 0,
		StopTime= 200,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end invertersDOPRI;
