model when_for_test

constant Integer N = 2;

discrete Real times1[N,N];

discrete Real times2[N];

algorithm
for i in 1:N loop
		when time > times1[i,1] then
for j in 1:N loop
		times1[i,j] := 10;
end for;			
		end when;
end for;

for i in 1:N loop
		when time > times1[i,1] then
for j in 1:N loop
		times1[j,i] := 10;
end for;			
		end when;
end for;

for i in 1:N loop
		when time > times1[i,1] then
for j in 1:N loop
		times1[i,i] := 10;
end for;			
		end when;
end for;

for i in 1:N loop
		when time > times1[i,1] then
for j in 1:N loop
		times1[j,j] := 10;
end for;			
		end when;
end for;


for i in 1:N loop
		when time > times2[i] then
for j in 1:N loop
		times2[j] := 10;
end for;			
		end when;
end for;
	
	annotation(

	experiment(
		MMO_Description="Test",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Period={0.2},
		MMO_PartitionMethod=Metis,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=0.4,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));
end when_for_test;
