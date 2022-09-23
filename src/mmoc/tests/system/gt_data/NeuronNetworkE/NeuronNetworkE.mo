model NeuronNetworkE

  import math;

  constant Integer N=10000;
 constant Integer M=5; //maximum distance
  parameter Integer ind1[N](each fixed=false); //first individual connection
  parameter Integer ind2[N](each fixed=false);// second individual connection
 parameter Integer ind3[N](each fixed=false);// third individual connection
 parameter Integer ind4[N](each fixed=false);// fourth individual connection
 parameter Integer ind5[N](each fixed=false); // fifth individual connection
 Real V[N],Is[N]; // membrane potential , synaptic current
  discrete Real tnext[N](each start=1e-20),tnextr[N](each start=-1),active[N](each start=1);
  parameter Real taum=10e-3,Cm=250e-12,theta=-50e-3,taur=2e-3,taus=0.5e-3,Vr=-65e-3,EL=-65e-3;
  parameter Real vbg=10,kext=50,vext=vbg*kext, J[N](each fixed=false),Jmean=87.8e-12; 
 

initial algorithm

for i in 1:N-5*M loop
        ind1[i]:=i+random(M)+1;
        ind2[i]:=ind1[i]+random(M)+1;
        ind3[i]:=ind2[i]+random(M)+1;
        ind4[i]:=ind3[i]+random(M)+1;
        ind5[i]:=ind4[i]+random(M)+1;
  end for;

/*
for i in 1:N-5 loop
        ind1[i]:=i+1;
         ind2[i]:=i+2;
        ind3[i]:=i+3;
         ind4[i]:=i+4;        
        ind5[i]:=i+5;  
end for;
 ind1[N-4]:=N-4;
 ind2[N-4]:=N-3;
 ind3[N-4]:=N-2;
 ind4[N-4]:=N-1;
ind5[N-4]:=1;

 ind1[N-3]:=N-3;
 ind2[N-3]:=N-2;
 ind3[N-3]:=N-1;
 ind4[N-3]:=1;
ind5[N-3]:=2;

 ind1[N-2]:=N-2;
 ind2[N-2]:=N-1;
 ind3[N-2]:=1;
 ind4[N-2]:=2;
ind5[N-2]:=3;

 ind1[N-1]:=N-1;
 ind2[N-1]:=1;
 ind3[N-1]:=2;
 ind4[N-1]:=3;
ind5[N-1]:=4;

 ind1[N]:=1;
 ind2[N]:=2;
 ind3[N]:=3;
 ind4[N]:=4;
 ind5[N]:=5;
 */

for i in 1:N loop
  V[i]:=rand(1)*10e-3-65e-3;
  J[i]:=(rand(1)-0.5)*20e-12+87.8e-12;
  Is[i]:=rand(1)*10e-12+20e-12;
end for;

equation
 for i in 1:N loop
//		der(V[i])=(-(V[i])+Is[i])/taum*active[i]; 
		der(V[i])=(-(V[i]-EL)/taum+Is[i]/Cm)*active[i]; 
			der(Is[i])=-Is[i]/taus;
end for;

algorithm

for i in 1:N loop
  when V[i]>theta then
		    reinit(V[i],Vr);
     tnextr[i]:=time+taur;
     active[i]:=0;

    reinit(Is[ind1[i]],Is[ind1[i]]+J[i]*taum/taus);
    reinit(Is[ind2[i]],Is[ind2[i]]+J[i]*taum/taus);
    reinit(Is[ind3[i]],Is[ind3[i]]+J[i]*taum/taus);
    reinit(Is[ind4[i]],Is[ind4[i]]+J[i]*taum/taus);
    reinit(Is[ind5[i]],Is[ind5[i]]+J[i]*taum/taus);
  end when;

when time>tnext[i] then
    reinit(Is[i],(Is[i]+Jmean*taum/taus));
    tnext[i]:=time+exponential(1/vext);
  end when;


when time>tnextr[i] then
							active[i]:=1;
  end when;
end for; 

	
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_LPS=4,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=1e-5,
		MMO_Output={Is[1:N/10:N],V[1:N/10:N]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=0.1,
		Tolerance={1e-3},
		AbsTolerance={1e-12}
	));
end NeuronNetworkE;
