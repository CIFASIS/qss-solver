model virus_replication
 import math;
 constant Integer N=10;
constant Integer M=10000; //number of samples
constant Integer SizeW=10000001;
 Real I(start=0), X[N]; // random input , concentration of material in compartment i 
 parameter Real mu=1, sigma=0.5, omega=1; // mean, noise magnitude , reversion strength of the Ornstein-Uhlenbeck process 
parameter Real tf=100; 
parameter Real  k=1;
parameter Real h(fixed=false); //progression rate of material from one compartment to the nex
  discrete Real dW;
parameter Real W[SizeW](each fixed=false);
parameter Real dWval[M](each fixed=false);
parameter Real TEvent[M];
parameter Integer di=SizeW/M;
parameter Real hmin(fixed=false);

initial algorithm
h:=tf/M;
hmin:=tf/SizeW;
W[1]:=normal(1);
for i in 2:SizeW loop
  W[i]:=W[i-1]+normal(1)*sqrt(hmin); //generate wiener process samples with dt=1e-4
end for;
for i in 1:M loop
 dWval[i]:=W[i*di+1]-W[(i-1)*di+1]; //compute the random increments
  TEvent[i]:=i*h;
 end for;
dW:=dWval[1];

equation

der(I)=-omega*(I-mu)+sigma*dW/h;
der(X[1]) =I-k*X[1];

for i in 2:N loop
  der(X[i])=k*X[i-1] - k*X[i];
end for;

algorithm 

for i in  1:M loop
  when time>TEvent[i]-h then
    dW:=dWval[i];
   end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS1,
		MMO_Period={100/1000},
		MMO_PartitionMethod=Metis,
		MMO_Output={X},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		MMO_RandomSeed=1,
		StartTime=0.0,
		StopTime=100,
		Tolerance={0},
		AbsTolerance={1e-2}
	));
end virus_replication;
