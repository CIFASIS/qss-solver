model buck_term

  parameter Real C = 1e-4, L = 1e-4, R0 = 10, U = 24, Ts = 1e-4, DC = 0.5, ROn = 1e-4, ROff = 1e4;
  discrete Real Rd(start=1e5), Rs(start=1e-5), nextT(start=Ts),lastT,diodeon;
  Real uC,iL,iD,s;
  discrete Real on(start=1), off(start=0);
  parameter Real ton = 1.3e-6, toff = 5.5e-6, vc0 = 0.2;
  Real SW, pS, iS, uS;

  constant Integer N=10;
  Real u[N](each start  = 298);
  parameter Real l = 0.025, c = 7.5, sigma = 8.61e-6;

equation
    
  iD=(iL*Rs-U)/(Rs+Rd);
  s=diodeon*iD+(1-diodeon)*iD*Rd;
  der(iL) = (-iD*Rd- uC)/L;
  der(uC) = (iL - uC/R0)/C;
  SW = Rs/1e4*1.1;
  pS = (on+off);
  der(u[1])= sigma*(pS*c+298-2*u[1]+u[2])/(l*l/N/N);
  for i in 2:N-1 loop
    der(u[i])=sigma*(u[i-1]-2*u[i]+u[i+1])/(l*l/N/N);
  end for;
  der(u[N])=sigma*(298-2*u[N]+u[N-1])/(l*l/N/N);

algorithm

  when time > nextT then
    lastT:=nextT;
    nextT:=nextT+Ts;
    Rs := ROn;
    on := 1e2;
  end when;

  when time - lastT-0.013*Ts>0 then
    on := 0;
    end when;

  when time - lastT-DC*Ts>0 then
    Rs := ROff;
    off := 1e2;
  end when;
  
  when time - lastT-DC*Ts-0.055*Ts>0 then
    off := 0;
  end when;

  when s>0 then
    Rd:=ROn;
    diodeon:=1;
  elsewhen s<0 then
    Rd := ROff;
    diodeon:=0;
  end when;
	annotation(

	experiment(
		MMO_Description="Buck converter.",
		MMO_Solver=LIQSS_BDF,
		MMO_SymDiff=false,
		MMO_Period={10/5000},
		MMO_PartitionMethod=Metis,
		MMO_Output={u[:]},
		MMO_OutputType=CI_Sampled,
		Jacobian=Sparse,
		MMO_BDF_PDepth=3,
		MMO_BDF_Max_Step=1,
		StartTime=0,
		StopTime=1,
		Tolerance={1e-6},
		AbsTolerance={1e-6}
	));
end buck_term;
