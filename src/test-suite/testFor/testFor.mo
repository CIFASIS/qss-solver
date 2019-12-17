model testFor
	Real u1[2];
  parameter Real ax=1,ay=1,r=1000;
  constant Integer N = 100, M=100;
  parameter Real dx=10/N;
  parameter Real dy=10/M;

initial algorithm
	for i in 1:2 loop
		u1[i]:=1;
	end for;
equation
 der(u1[1])=(-u1[1]*ax/dx)+(-u1[1]*ay/dy) /*+ r*(u1[1]^2- u1[1]^3)*/;
 for i in 2:2 loop
    der(u1[i])=(-u1[i]+u1[i-1])*ax/dx+(-u1[i]*ay/dy) /*+ r*(u1[1]^2- u1[1]^3)*/;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_Output={u1[:]},
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end testFor;
