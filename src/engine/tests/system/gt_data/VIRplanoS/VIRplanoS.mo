model VIRplanoS

//SIN INMUNIDAD (S START=0.1) , 50% BARBIJOS (re, tr )  ,  VENTILADO medio )
function SRND
    input Real y;
    output Real r;
  external "C" r=srnd(y)  annotation(IncludeDirectory="modelica://VIRplanoS",
  Include="#include \"srnd.c\"");
  end SRND;
  function RND
    input Real y;
    output Real r;
  external "C" r=rnd(y)  annotation(IncludeDirectory="modelica://VIRplanoS",
  Include="#include \"rnd.c\"");
  end RND;
import math;
  constant Integer N = 4; //maximum number of people in the room
  Real V[N], H[N](each start = 1), I[N], M[N], F[N], R[N], E[N](each start = 1), P[N](each start = 1), A[N](each start = 1), S[N](each start = 0.1), D[N];    //// viral load, healthy cells, infected cells, activated antigen presenting cells, interferon, resistant cells, effector cells, plasma cells, antibodies, antigenic distance, dead cells
   parameter Real gv[N](each fixed=false) , gva[N](each fixed=false), gvh[N](each fixed=false) , av[N](each fixed=false), av1[N](each fixed=false), av2[N](each fixed=false), bhd[N](each fixed=false), ar[N](each fixed=false), ghv[N](each fixed=false), bhf[N](each fixed=false) , bie[N](each fixed=false) , ai[N](each fixed=false), bmd[N](each fixed=false) , bmv[N](each fixed=false) , am[N](each fixed=false), bf[N](each fixed=false) , cf[N](each fixed=false), bfh[N](each fixed=false) , af[N](each fixed=false), bem[N](each fixed=false) , bei[N](each fixed=false) , ae[N](each fixed=false) , bpm[N](each fixed=false) , ap[N], ba[N](each fixed=false) , gav[N](each fixed=false) , aa[N](each fixed=false) , r[N](each fixed=false) ;  // parameters characterizing disease processe
  parameter Real  re[N](each fixed=false) , tr[N](each fixed=false), cr = 100; //reception rate of viral load,transmission rate of viral load,decay rate of viral load inside the room
 Real Vr, Vtot, Vpar[N];   //total viral load inside the room, viral load of people inside the room, Aux Var
 discrete Real inside[N],inst[N];  // state: inside(1) o outside(0) of the room; time of change of state
 discrete Real Su,In1,Rec; //Susceptible, Infected , Recovered people 
 discrete Real u,day[N]; //day=first day of non-contagion after infection
 

initial algorithm
 u :=rand(1);

//parameter definition
   for i in 1:N loop
    gv[i] := 510;
    gva[i] := 619.2;
    gvh[i] := 1.02;
    av[i] := 1.7;
    av1[i] := 100;
    av2[i] := 23000;
    bhd[i] := 4;
    ar[i] :=1;
    ghv[i] := 0.34;
    bhf[i] := 0.01;
    bie[i] := 0.066;
    ai[i] := 1.5;
    bmd[i]:= 1;
    bmv[i] := 0.0037;
    am[i] := 1;
    bf[i] := 250000;
    cf[i] := 2000;
    bfh[i] := 17;
    af[i] := 8;
    bem[i] := 8.3;
    bei[i] := 2.72;
    ae[i] := 0.4;
    bpm[i] := 11.5;
    ap[i] := 0.4;
    ba[i] := 0.043;
    gav[i] := 146.2;
    aa[i] := 0.043;
    r[i] := 3e-5;
  end for;

// initialization of people's parameters
  for i in 1:N/2 loop
    re[i]:= rand(0.22) + 0.18;
    tr[i]:= re[i];
  end for;
  for i in N/2+1:N loop
    re[i]:= rand(0.06) + 0.04;
    tr[i]:= re[i];
  end for;
    V[1] := 10; //one infected
   inside[1]:= 1;
  
  for i in 1:N loop
    inst[i] := rand(2);
    day[i] :=1000;
  end for;

for i in 1:2 loop //(N*1/100) loop     //1% of people start infected   
V[i] :=10;
inside[i] :=1;
end for;

Su := N;

equation
for i in 1:N loop
  der(V[i]) = gv[i] * I[i] - gva[i] * S[i] * A[i] * V[i] - gvh[i] * H[i] * V[i] - av[i] * V[i] - av1[i] * V[i] / (1 + av2[i] * V[i]) + inside[i]*re[i] * Vr;
  der(H[i]) = bhd[i] * D[i] * (H[i] + R[i]) + ar[i] * R[i] - ghv[i] * V[i] * H[i] - bhf[i] * F[i] * H[i];
  der(I[i]) = ghv[i] * V[i] * H[i] - bie[i] * E[i] * I[i] - ai[i] * I[i];
  der(M[i]) = (bmd[i] * D[i] + bmv[i] * V[i]) * (1 - M[i]) - am[i] * M[i];
  der(F[i]) = bf[i] * M[i] + cf[i] * I[i] - bfh[i] * H[i] * F[i] - af[i] * F[i];
  der(R[i]) = bhf[i] * F[i] * H[i] - ar[i] * R[i];
  der(E[i]) = bem[i] * M[i] * E[i] - bei[i] * I[i] * E[i] + ae[i] * (1 - E[i]);
  der(P[i]) = bpm[i] * M[i] * P[i] + ap[i] * (1 - P[i]);
  der(A[i]) = ba[i] * P[i] - gav[i] * S[i] * A[i] * V[i] - aa[i] * A[i];
  der(S[i]) = r[i] * P[i] * (1 - S[i]);
  D[i] = 1 - H[i] - R[i] - I[i];
end for;
  der(Vr)=sum(Vpar) *100/N- cr * Vr;
  
for i in 1:N loop
   Vpar[i]=V[i]*tr[i]*inside[i];
  end for;

  //Vtot= sum(Vpar);

 algorithm
u:=u;

for i in 1:N loop
  when time > inst[i] then
      if inside[i]==0 then
       inside[i]:= 1- inside[i];
       inst[i]:=inst[i]+rand(0.0833); 
     else
      inside[i]:= 1- inside[i];
      inst[i]:=inst[i]+ rand(0.2075)+0.833;   
end if;
  end when;
end for;

 for i in 1:N loop
  when V[i] > 1 then
   Su:=Su-1;
   In1:=In1+1;
  day[i]:=time+10;
  end when;
 when time > day[i] then
    In1:=In1-1;
	   Rec:=Rec+1;
end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={In1,Rec,Su,Vr},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		MMO_RandomSeed=1,
		StartTime=0.0,
		StopTime=60,
		Tolerance={1e-2},
		AbsTolerance={1e-6}
	));
end VIRplanoS;
