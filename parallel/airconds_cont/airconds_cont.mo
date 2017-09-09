model airconds_cont
function getSection
input Integer x;
output Real y;
external "C" y = getSection(x) annotation(
																Include="#include \"sections.c\""); 
end randomC;

	import math;
	constant Integer N = 20000;
constant Integer SECTIONS = 20;
	parameter Real CAP[N], RES[N], POT[N], THA = 32,pmax=0,Kp=1,Ki=1,tref=20;
parameter Integer sections[N];	
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
		th[i] := rand(4)+ 18;
		CAP[i] := rand(100)+ 550;
		RES[i] := rand(0.4)+ 1.8;
		POT[i] := rand(0.2)+ 13;
	pmax:=pmax+POT[i];
noise[i] := 2*sin(i)^2;	
sampleNoise[i] := rand(2);
sections[i] := getSection(i);
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
partTotal[i] := 0;
end when;
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
 end when;
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
		MMO_PartitionMethod=Patoh,
		MMO_LPS=4,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=1,
		MMO_Output={ptotal},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		StartTime=0,
		StopTime=3000,
		Tolerance={1e-4},
		AbsTolerance={1e-4}
	));
end airconds_cont;
