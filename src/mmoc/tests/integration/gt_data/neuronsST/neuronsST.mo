model neuronsST
  import math;
  constant Integer N=10,c=5;
  Real V[N],Is[N];
  discrete Real tnext(start=1e-20),tnextr[N](each start=1),active[N](each start=1);
  discrete Real xi[N];
  parameter Real taum=10e-3,Cm=250e-12,theta=-50e-3,taur=2e-3,taus=0.5e-3,Vr=-65e-3,EL=-65e-3;
  parameter Real vbg=10,kext=10000,vext=vbg*kext,J[N](each fixed=false);
  parameter Real Jmean=87.8e-12, mu=Jmean*taum*vbg*kext,sigma=sqrt(mu*Jmean),dt=1e-4;
  parameter Integer C[N,c](each fixed=false),Cn[N](each fixed=false);
  
   initial algorithm

     for i in 1:N loop
      xi[i]:=(rand(1)-0.5)*sqrt(dt);      
      V[i]:=rand(1)*10e-3-65e-3;
      Cn[i]:=integer(rand(c+1));
      J[i]:=(rand(1)-0.5)*20e-12+87.8e-12;
    end for; 
     for i in 1:N loop
       
       for j in 1:Cn[i] loop
         C[i,j]:=rand_int(N, i)+1;
       end for;
    end for;  
equation
for i in 1:N loop
		der(V[i])=(-(V[i])+Is[i])/taum*active[i];
			der(Is[i])=(-Is[i]+mu+sigma*sqrt(taum)*xi[i]/dt)/taus+1e-80*tnext;
end for;

algorithm

 when time>tnext then
    for i in 1:N loop
      xi[i]:=(rand(1)-0.5)*sqrt(dt);
	   end for;  
    tnext:=time+dt;
  end when;

for i in 1:N loop
  when V[i]>theta then
		    reinit(V[i],Vr);
     tnextr[i]:=time+taur;
     active[i]:=0;
     for j in 1:Cn[i] loop
        reinit (Is[C[i,j]],Is[C[i,j]]+J[i]*taum/taus);
     end for;
  end when annotation(MMO_HD={(Is[C[i,j]],[1:N, 1:Cn[i]])});

  when time>tnextr[i] then
							active[i]:=1;
  end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_Period={1e-4},
		MMO_PartitionMethod=Metis,
		MMO_Output={V[1], Is[1]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-12}
	));
end neuronsST;
