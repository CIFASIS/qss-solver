model for_stm_test

constant Integer N = 2;
	
Real ux;

discrete Real times[2];

equation
 der(ux) = 0.0;
	
algorithm
/*	for i in 1:2 loop
		when time > times[i] then
for j in 1:N loop	
		times[j] := 1;
end for;
		end when;
	end for;*/
	when time > 10 then
for j in 1:N loop	
		times[j] := 1;
end for;

end when;
	
	annotation(

	experiment(
		MMO_Description="Test",
		MMO_Solver=QSS2,
		MMO_Period={0.2},
		MMO_PartitionMethod=Metis,
		MMO_Output={times},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));
end for_stm_test;
