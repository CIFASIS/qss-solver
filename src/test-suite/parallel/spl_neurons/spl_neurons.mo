model spl_neurons
	import math;
	constant Integer N=3000,M=200,C=100;
	Real v[N](each start=-60),gex[N],ginh[N];
parameter Real tau=20, vrest=-60, vthres=-50,Trefrac=5,Eex=0,Einh=-80,tauex=5,tauinh=10,dgex=0.4,dgin=1.6;
	discrete Real  active[N](each start=1),tfire[N](each start=10000);
	discrete Real  nextev[M](each start=random(10));
	parameter Real Period=10,lastTime=100;
	parameter Integer SNmap1[N];
parameter Integer NNmap1[N],NNmap2[N], NNmap3[N], NNmap4[N],NNmap5[N],NNmap6[N], NNmap7[N], NNmap8[N],NNmap9[N], NNmap10[N];
parameter Integer NNmap11[N],NNmap12[N], NNmap13[N], NNmap14[N],NNmap15[N],NNmap16[N], NNmap17[N], NNmap18[N],NNmap19[N], NNmap20[N];
parameter Real ex[N];
discrete Real exReinit[N];
discrete Real inhReinit[N];
discrete Real nextGlobalTime[M];
discrete Real inputs[M];
initial algorithm

	for i in 1:M loop
	SNmap1[i]:=random(N);
	end for;

for i in 1:N loop
NNmap1[i] := random(C);
NNmap2[i] := random(C);
NNmap3[i] := random(C);
NNmap4[i] := random(C);
NNmap5[i] := random(C);
NNmap6[i] := random(C);
NNmap7[i] := random(C);
NNmap8[i] := random(C);
NNmap9[i] := random(C);
NNmap10[i] := random(C);
NNmap11[i] := random(C);
NNmap12[i] := random(C);
NNmap13[i] := random(C);
NNmap14[i] := random(C);
NNmap15[i] := random(C);
NNmap16[i] := random(C);
NNmap17[i] := random(C);
NNmap18[i] := random(C);
NNmap19[i] := random(C);
NNmap20[i] := random(C);
end for;

	for i in 1:N loop
		ex[i]:=random(100);
	end for;

equation
	for i  in 1:N loop
		der(v[i])=active[i]*((vrest-v[i])+gex[i]*(Eex-v[i])+ginh[i]*(Einh-v[i]))/tau;
		der(gex[i])=-gex[i]/tauex;
		der(ginh[i])=-ginh[i]/tauinh;
	end for;

algorithm

	for i in 1:N loop
		when v[i]*ex[i]>vthres then
			reinit(v[i],vrest);
			reinit(ginh[i],0);
			reinit(gex[i],0);
exReinit[NNmap1[i]] := 1;
exReinit[NNmap2[i]] := 1;
exReinit[NNmap3[i]] := 1;
exReinit[NNmap4[i]] := 1;
exReinit[NNmap5[i]] := 1;
exReinit[NNmap6[i]] := 1;
exReinit[NNmap7[i]] := 1;
exReinit[NNmap8[i]] := 1;
exReinit[NNmap9[i]] := 1;
exReinit[NNmap10[i]] := 1;
exReinit[NNmap11[i]] := 1;
exReinit[NNmap12[i]] := 1;
exReinit[NNmap13[i]] := 1;
exReinit[NNmap14[i]] := 1;
exReinit[NNmap15[i]] := 1;
exReinit[NNmap16[i]] := 1;
exReinit[NNmap17[i]] := 1;
exReinit[NNmap18[i]] := 1;
exReinit[NNmap19[i]] := 1;
exReinit[NNmap20[i]] := 1;
			active[i]:=0;
			tfire[i]:=time;
end when;
end for;

	for i in 1:N loop
		when v[i]*(1-ex[i])>vthres then
			reinit(v[i],vrest);
			reinit(ginh[i],0);
			reinit(gex[i],0);
inhReinit[NNmap1[i]] := 1;
inhReinit[NNmap2[i]] := 1;
inhReinit[NNmap3[i]] := 1;
inhReinit[NNmap4[i]] := 1;
inhReinit[NNmap5[i]] := 1;
inhReinit[NNmap6[i]] := 1;
inhReinit[NNmap7[i]] := 1;
inhReinit[NNmap8[i]] := 1;
inhReinit[NNmap9[i]] := 1;
inhReinit[NNmap10[i]] := 1;
inhReinit[NNmap11[i]] := 1;
inhReinit[NNmap12[i]] := 1;
inhReinit[NNmap13[i]] := 1;
inhReinit[NNmap14[i]] := 1;
inhReinit[NNmap15[i]] := 1;
inhReinit[NNmap16[i]] := 1;
inhReinit[NNmap17[i]] := 1;
inhReinit[NNmap18[i]] := 1;
inhReinit[NNmap19[i]] := 1;
inhReinit[NNmap20[i]] := 1;
			active[i]:=0;
			tfire[i]:=time;
end when;
end for;

	
for i in 1:N loop
when exReinit[i] > 0.5 then
   			reinit(gex[i],gex[i]+dgex);
   exReinit[i]:=0;
end when;
end for;

for i in 1:N loop
when   inhReinit[i] > 0.5 then
   reinit(ginh[i],ginh[i]+dgin);
   inhReinit[i]:=0;
end when;
end for;

	for i in 1:N loop
		when time>tfire[i]+Trefrac then
			active[i]:=1;
		end when;
	end for;

for i in 1:M loop
		when time>nextev[i]+nextGlobalTime[i] then
			  exReinit[SNmap1[i]] := 1;
   inputs[i] := inputs[i] + 1;
    nextev[i]:=time+10*sin(i*inputs[i])^2;
		end when;
	end for;

for i in 1:M loop
		when time > lastTime then
   nextGlobalTime[i] := 1e10;			  
		end when;
	end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Period={300/5000},
		MMO_Parallel=true,
		MMO_PartitionMethod=MetisVol,
		MMO_LPS=4,
		MMO_DT_Synch=SD_DT_Fixed,
		MMO_DT_Min=0.00001,
		MMO_Output={v[1:N/20:N]},
		MMO_OutputType=CI_Sampled,
		StartTime=0.0,
		StopTime=300,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end spl_neurons;
