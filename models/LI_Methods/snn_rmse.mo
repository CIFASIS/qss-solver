model snn_rmse
import math;
  constant Integer N=1000,c=10;
 parameter Real cV=1e3, cI=1e9;
  Real V[N],Is[N];
  discrete Real tnext[N],tnextr[N](each start=-1),active[N](each start=1);
 discrete Real ies[N], es(start=0); // instants of emitted spikes, number of emitted spikes
  parameter Real taum=10e-3,Cm=250e-12,theta=-50e-3,taur=2e-3,taus=0.5e-3,Vr=-65e-3,EL=-65e-3;
  parameter Real vbg=10,kext=100,vext=vbg*kext,J[N](each fixed=false);
  parameter Real Jmean=(87.8e-12)/1,g=10;
  parameter Integer C[N,c*5](each fixed=false),Cn[N](each fixed=false), ce=c*0.8,ci=c*0.2, Ne=0.8*N,Ni=0.2*N,aux(fixed=false);
   initial algorithm
     for i in 1:N loop
      Is[i]:=rand(1)*1e-10*cI+1.7887;
      V[i]:=rand(2)*1e-3*cV-65;
       Cn[i]:=0;
      
      if i< N*0.8 then
	J[i]:=(Jmean+normal(Jmean*0.1/3))/c;         
      else
	J[i]:=-(Jmean*g+normal(Jmean*g*0.1/3))/c;       
      end if;
      
     tnext[i]:=exponential(1/vext);
    end for;
 
   for i in 1:N loop
      for j in 1:ce loop
         aux:=rand(Ne)+1;
         for k in 1:10 loop
             if C[aux,Cn[aux]]==i then
              	aux:=rand(Ne)+1;
             end if;
             if aux==i then
              	aux:=rand(Ne)+1;
             end if;
         end for;
       Cn[aux]:=Cn[aux]+1; 
       C[aux,Cn[aux]]:=i;
      end for;

     for j in 1:ci loop
        aux:=rand(Ni)+Ne+1;
         for k in 1:10 loop
             if C[aux,Cn[aux]]==i then
              	aux:=rand(Ni)+Ne+1;
             end if;
             if aux==i then
              	aux:=rand(Ni)+Ne+1;
             end if;
         end for;

       Cn[aux]:=Cn[aux]+1; 
       C[aux,Cn[aux]]:=i;
      end for;

     end for;    
    

  equation
   for i in 1:N loop
		   der(V[i])=(-(V[i]/cV-EL)/taum+Is[i]/Cm/cI)*cV*active[i];
		   der(Is[i])=-Is[i]/taus;
   end for;

algorithm
for i in 1:N loop
  when V[i]>theta*cV then
     reinit(V[i],Vr*cV);
    es:=es+1;
     tnextr[i]:=time+taur;
     active[i]:=0;
     for j in 1:Cn[i] loop
        reinit(Is[C[i,j]],Is[C[i,j]]+J[i]*cI*taum/taus);
     end for;
  end when annotation(	MMO_HD={(Is[C[i,j]],[1:N, 1:Cn[i]])},
		MMO_LHS_ST={(Is[C[i,j]],[1:N, 1:Cn[i]])},
		MMO_RHS_ST={(Is[C[i,j]],[1:N, 1:Cn[i]])} ) ;
  
  when time>tnext[i] then
      reinit(Is[i],Is[i]+Jmean*cI*taum/taus);
      tnext[i]:=time+exponential(1/vext);
  end when;
  
  when time>tnextr[i] then
  	 active[i]:=1;
  end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=EQSS2,
		MMO_Period={1e-3},
		MMO_PartitionMethod=Metis,
		MMO_LPS=2,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=0.005,
		MMO_Output={es},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		MMO_RandomSeed=59,
		StartTime=0.0,
		StopTime=0.05,
		Tolerance={0},
		AbsTolerance={1e-3}
	));
end snn_rmse;
