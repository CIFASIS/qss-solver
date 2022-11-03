model NeuralNetwork1
import math;
  constant Integer N=100;
   constant Integer c=10;
 parameter Real cV=1e3, cI=1e9;
  Real V[N],Is[N];
  discrete Real tnext[N],tnextr[N](each start=-1),active[N](each start=1),Islast[N],tlast[N];
  parameter Real taum=10e-3,Cm=250e-12,theta=-50e-3,taur=2e-3,taus=0.5e-3,Vr=-65e-3,EL=-65e-3;
  parameter Real vbg=8,kext=940,vext=vbg*kext,J[N](each fixed=false);
  parameter Real Jmean=(87.8e-12)/1,g=4;
  parameter Integer C[N,c*2](each fixed=false),Cn[N](each fixed=false), ce=c*0.8,ci=c*0.2, Ne=0.8*N,Ni=0.2*N,aux(fixed=false);
   initial algorithm
     for i in 1:N loop
      Islast[i]:=rand(1)*1e-10*cI+0.4;
//      V[i]:=rand(2)*1e-3*cV-65;
      V[i]:=rand(15)-65;
     Cn[i]:=0;
      
      if i< N*0.8 then
	J[i]:=(Jmean+normal(Jmean*0.1/3));         
      else
	J[i]:=-(Jmean*g+normal(Jmean*g*0.1/3));       
      end if;
      
     tnext[i]:=exponential(1/vext);
    end for;
 
   for i in 1:N loop
      for j in 1:ce loop
         aux:=rand(Ne)+1;
         for k in 1:10 loop
            if Cn[aux]>=2*c then
              	aux:=rand(Ne)+1;
            end if;
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
            if Cn[aux]>=2*c then
              	aux:=rand(Ni)+Ne+1;
            end if;
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
     Is[i]=Islast[i]*exp(-(time-tlast[i])/taus);
		  
     der(V[i])=(-(V[i]/cV-EL)/taum+Is[i]/Cm/cI)*cV*active[i];
   end for;

algorithm
for i in 1:N loop
  when V[i]>theta*cV then
     reinit(V[i],Vr*cV);
     tnextr[i]:=time+taur;
     active[i]:=0;
     for j in 1:Cn[i] loop
        Islast[C[i,j]]:=Islast[C[i,j]]*exp(-(time-tlast[C[i,j]])/taus)+J[C[i,j]]*cI;
        tlast[C[i,j]]:=time;
     end for;
  end when annotation(	MMO_HD={(V[C[i,j]],[1:N, 1:Cn[i]])}) ;
 
  
  when time>tnext[i] then
    Islast[i]:=Islast[i]*exp(-(tnext-tlast)/taus)+Jmean*cI;
    tlast[i]:=time;
    tnext[i]:=time+exponential(1/vext);
  end when;
  
  when time>tnextr[i] then
  	 active[i]:=1;
  end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS2,
		MMO_Period={1e-4},
		MMO_PartitionMethod=Metis,
		MMO_LPS=2,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=0.005,
		MMO_Output={V[1:N/10:N]},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		MMO_RandomSeed=1,
		StartTime=0.0,
		StopTime=0.1,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end NeuralNetwork1;
