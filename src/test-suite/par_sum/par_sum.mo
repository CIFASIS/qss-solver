model par_sum
		  parameter Real P[10];
   discrete Real d;
	  Real a;
   Real b[10];
   Real c[10];
initial algorithm
		 for i in 1:10 loop
	   P[i]:=i;
  end for;
equation
   for i in 1:10 loop
   b[i] = i;
der(c[i])=i;
   end for;
	  der(a)=sum(P);
algorithm 
  when time > 0 then
    d:=max(P);
    d:=min(P);
    d:=sum(P);
    d:=product(P);
    d:=P*b;
    d:=max(b);
    d:=min(b);
    d:=sum(b);
    d:=product(b);
    d:=b*P;
    d:=max(c);
    d:=min(c);
    d:=sum(c);
    d:=product(c);
    d:=b*c;
  end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS3,
		MMO_PartitionMethod=Metis,
		MMO_Output={c},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end par_sum;
