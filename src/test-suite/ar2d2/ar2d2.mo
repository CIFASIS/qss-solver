model ar2d2
import file;
  parameter Real dT=1e-3;
  parameter Real ax=1,ay=1,r=1000;
  discrete Real sav,nextSample;
  constant Integer N = 1000, M=10;
  parameter Real dx=10/N;
  parameter Real dy=10/M;
  Real u1[N](each fixed=false);
  Real u2[N](each fixed=false);
  Real u3[N](each fixed=false);
  Real u4[N](each fixed=false);
  Real u5[N](each fixed=false);
  Real u6[N](each fixed=false);
  Real u7[N](each fixed=false);
  Real u8[N](each fixed=false);
  Real u9[N](each fixed=false);
  Real u10[N](each fixed=false);

initial algorithm
  for i in 1:N/10 loop
    u1[i]:=1;
  end for;
equation
  der(u1[1])=(-u1[1]*ax/dx)+(-u1[1]*ay/dy) + r*(u1[1]^2)*(1- u1[1]);
  der(u2[1])=(-u2[1]*ax/dx)+(-u2[1]+u1[1])*ay/dy + r*(u2[1]^2)*(2 - u2[1]);
  der(u3[1])=(-u3[1]*ax/dx)+(-u3[1]+u2[1])*ay/dy + r*(u3[1]^2)*(2 - u3[1]);
  der(u4[1])=(-u4[1]*ax/dx)+(-u4[1]+u3[1])*ay/dy + r*(u4[1]^2)*(2 - u4[1]);
  der(u5[1])=(-u5[1]*ax/dx)+(-u5[1]+u4[1])*ay/dy + r*(u5[1]^2)*(2 - u5[1]);
  der(u6[1])=(-u6[1]*ax/dx)+(-u6[1]+u5[1])*ay/dy + r*(u6[1]^2)*(2 - u6[1]);
  der(u7[1])=(-u7[1]*ax/dx)+(-u7[1]+u6[1])*ay/dy + r*(u7[1]^2)*(2 - u7[1]);
  der(u8[1])=(-u8[1]*ax/dx)+(-u8[1]+u7[1])*ay/dy + r*(u8[1]^2)*(2 - u8[1]);
  der(u9[1])=(-u9[1]*ax/dx)+(-u9[1]+u8[1])*ay/dy + r*(u9[1]^2)*(2 - u9[1]);
  der(u10[1])=(-u10[1]*ax/dx)+(-u10[1]+u9[1])*ay/dy + r*(u10[1]^2)*(2 - u10[1]);
  for i in 2:N loop
    der(u1[i])=(-u1[i]+u1[i-1])*ax/dx+(-u1[i]*ay/dy) + r*(u1[i]^2)*(1 - u1[i]);
    der(u2[i])=(-u2[i]+u2[i-1])*ax/dx+(-u2[i]+u1[i])*ay/dy + r*(u2[i]^2)*(1- u2[i]);
    der(u3[i])=(-u3[i]+u3[i-1])*ax/dx+(-u3[i]+u2[i])*ay/dy + r*(u3[i]^2)*(1- u3[i]);
    der(u4[i])=(-u4[i]+u4[i-1])*ax/dx+(-u4[i]+u3[i])*ay/dy + r*(u4[i]^2)*(1- u4[i]);
    der(u5[i])=(-u5[i]+u5[i-1])*ax/dx+(-u5[i]+u4[i])*ay/dy + r*(u5[i]^2)*(1- u5[i]);
    der(u6[i])=(-u6[i]+u6[i-1])*ax/dx+(-u6[i]+u5[i])*ay/dy + r*(u6[i]^2)*(1- u6[i]);
    der(u7[i])=(-u7[i]+u7[i-1])*ax/dx+(-u7[i]+u6[i])*ay/dy + r*(u7[i]^2)*(1- u7[i]);
    der(u8[i])=(-u8[i]+u8[i-1])*ax/dx+(-u8[i]+u7[i])*ay/dy + r*(u8[i]^2)*(1- u8[i]);
    der(u9[i])=(-u9[i]+u9[i-1])*ax/dx+(-u9[i]+u8[i])*ay/dy + r*(u9[i]^2)*(1- u9[i]);
    der(u10[i])=(-u10[i]+u10[i-1])*ax/dx+(-u10[i]+u9[i])*ay/dy + r*(u10[i]^2)*(1- u10[i]);
  end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={u5[1:N/5:N]},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=5,
		Tolerance={1e-3},
		AbsTolerance={1e-4}
	));
end ar2d2;
