model spl_neurons_layers
	function laymap
		input Real ind;
		input Real layers;
		input Real neurons;
		output Real dest;
		external "C" dest=layermap(ind,layers,neurons) annotation(Include = "#include <../../src/test-suite/spl_neurons_layers/layermap.c>");
	end laymap;
     import math;
	constant Integer N=1000;
	constant Integer NEX=4*N/5;
	constant Integer NIN=N-NEX;
	constant Integer LAY=5;
	constant Integer LS=N/LAY;//layer size
	constant Integer M=201;
	Real v[N](each start=-60),gex[N],ginh[N];
	parameter Real tau=20, vrest=-60, vthres=-50,Trefrac=5,Eex=0,Einh=-80,tauex=5,tauinh=10,dgex=0.5,dgin=1.25;
	discrete Real  active[N](start=1),tfire[N](start=10000);
	discrete Real  nextev[M](each start=rand(10));
	parameter Real Period=10;
	parameter Integer SNmap1[N];
      parameter Integer NNmap1[N],NNmap2[N], NNmap3[N], NNmap4[N],NNmap5[N],NNmap6[N], NNmap7[N], NNmap8[N],NNmap9[N], NNmap10[N];
      parameter Integer NNmap11[N],NNmap12[N], NNmap13[N], NNmap14[N],NNmap15[N],NNmap16[N], NNmap17[N], NNmap18[N],NNmap19[N], NNmap20[N];
initial algorithm
	for i in 1:M loop
		SNmap1[i]:=laymap(0,LAY,N);
	end for;
	for i in 1:N loop
		NNmap1[i]:=laymap(i,LAY,N);
		NNmap2[i]:=laymap(i,LAY,N);
		NNmap3[i]:=laymap(i,LAY,N);
		NNmap4[i]:=laymap(i,LAY,N);
		NNmap5[i]:=laymap(i,LAY,N);
		NNmap6[i]:=laymap(i,LAY,N);
		NNmap7[i]:=laymap(i,LAY,N);
		NNmap8[i]:=laymap(i,LAY,N);
		NNmap9[i]:=laymap(i,LAY,N);
		NNmap10[i]:=laymap(i,LAY,N);
		NNmap11[i]:=laymap(i,LAY,N);
		NNmap12[i]:=laymap(i,LAY,N);
		NNmap13[i]:=laymap(i,LAY,N);
		NNmap14[i]:=laymap(i,LAY,N);
		NNmap15[i]:=laymap(i,LAY,N);
		NNmap16[i]:=laymap(i,LAY,N);
		NNmap17[i]:=laymap(i,LAY,N);
		NNmap18[i]:=laymap(i,LAY,N);
		NNmap19[i]:=laymap(i,LAY,N);
		NNmap20[i]:=laymap(i,LAY,N);
	end for;
equation
	for i  in 1:N loop
		der(v[i])=active[i]*((vrest-v[i])+gex[i]*(Eex-v[i])+ginh[i]*(Einh-v[i]))/tau;
		der(gex[i])=-gex[i]/tauex;
		der(ginh[i])=-ginh[i]/tauinh;
	end for;
algorithm
	for i in 1:NEX loop
		when v[i]>vthres then
			reinit(v[i],vrest);
			reinit(gex[NNmap1[i]],gex[NNmap1[i]]+dgex);
			reinit(gex[NNmap2[i]],gex[NNmap2[i]]+dgex);
			reinit(gex[NNmap3[i]],gex[NNmap3[i]]+dgex);
			reinit(gex[NNmap4[i]],gex[NNmap4[i]]+dgex);
			reinit(gex[NNmap5[i]],gex[NNmap5[i]]+dgex);
			reinit(gex[NNmap6[i]],gex[NNmap6[i]]+dgex);
			reinit(gex[NNmap7[i]],gex[NNmap7[i]]+dgex);
			reinit(gex[NNmap8[i]],gex[NNmap8[i]]+dgex);
			reinit(gex[NNmap9[i]],gex[NNmap9[i]]+dgex);
			reinit(gex[NNmap10[i]],gex[NNmap10[i]]+dgex);
			reinit(gex[NNmap11[i]],gex[NNmap11[i]]+dgex);
			reinit(gex[NNmap12[i]],gex[NNmap12[i]]+dgex);
			reinit(gex[NNmap13[i]],gex[NNmap13[i]]+dgex);
			reinit(gex[NNmap14[i]],gex[NNmap14[i]]+dgex);
			reinit(gex[NNmap15[i]],gex[NNmap15[i]]+dgex);
			reinit(gex[NNmap16[i]],gex[NNmap16[i]]+dgex);
			reinit(gex[NNmap17[i]],gex[NNmap17[i]]+dgex);
			reinit(gex[NNmap18[i]],gex[NNmap18[i]]+dgex);
			reinit(gex[NNmap19[i]],gex[NNmap19[i]]+dgex);
			reinit(gex[NNmap20[i]],gex[NNmap20[i]]+dgex);
			active[i]:=0;
			tfire[i]:=time;
		end when;
	end for;

	for i in NEX+1:N loop
		when v[i]>vthres then
			reinit(v[i],vrest);
			reinit(ginh[NNmap1[i]],ginh[NNmap1[i]]+dgin);
			reinit(ginh[NNmap2[i]],ginh[NNmap2[i]]+dgin);
			reinit(ginh[NNmap3[i]],ginh[NNmap3[i]]+dgin);
			reinit(ginh[NNmap4[i]],ginh[NNmap4[i]]+dgin);
			reinit(ginh[NNmap5[i]],ginh[NNmap5[i]]+dgin);
			reinit(ginh[NNmap6[i]],ginh[NNmap6[i]]+dgin);
			reinit(ginh[NNmap7[i]],ginh[NNmap7[i]]+dgin);
			reinit(ginh[NNmap8[i]],ginh[NNmap8[i]]+dgin);
			reinit(ginh[NNmap9[i]],ginh[NNmap9[i]]+dgin);
			reinit(ginh[NNmap10[i]],ginh[NNmap10[i]]+dgin);
			reinit(ginh[NNmap11[i]],ginh[NNmap11[i]]+dgin);
			reinit(ginh[NNmap12[i]],ginh[NNmap12[i]]+dgin);
			reinit(ginh[NNmap13[i]],ginh[NNmap13[i]]+dgin);
			reinit(ginh[NNmap14[i]],ginh[NNmap14[i]]+dgin);
			reinit(ginh[NNmap15[i]],ginh[NNmap15[i]]+dgin);
			reinit(ginh[NNmap16[i]],ginh[NNmap16[i]]+dgin);
			reinit(ginh[NNmap17[i]],ginh[NNmap17[i]]+dgin);
			reinit(ginh[NNmap18[i]],ginh[NNmap18[i]]+dgin);
			reinit(ginh[NNmap19[i]],ginh[NNmap19[i]]+dgin);
			reinit(ginh[NNmap20[i]],ginh[NNmap20[i]]+dgin);
			active[i]:=0;
			tfire[i]:=time;
		end when;
	end for;

	for i in 1:N loop
		when time>tfire[i]+Trefrac then
			active[i]:=1;
		end when;
	end for;
	for i in 1:M loop
		when time>nextev[i] then
			reinit(gex[SNmap1[i]],gex[SNmap1[i]]+dgex);
			nextev[i]:=time+rand(Period);
		end when;
	end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS3,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={v[N]},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=1000,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end spl_neurons_layers;
