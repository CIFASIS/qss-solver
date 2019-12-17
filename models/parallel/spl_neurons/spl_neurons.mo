model spl_neurons
  function randomC
    input Integer x;
    output Real y;
    external "C" y = randomC(x) annotation(
                                Include="#include \"rneurons.c\"");
  end randomC;

  function randomN
    output Real y;
    external "C" y = randomN() annotation(
                                Include="#include \"rneurons.c\"");
  end randomN;

  function random10
    output Real y;
    external "C" y = random10() annotation(
                                Include="#include \"rneurons.c\"");
  end random10;

  function random100
    input Integer x;
    output Real y;
    external "C" y = random100(x) annotation(
                                Include="#include \"rneurons.c\"");
  end random100;

  constant Integer N=300000,M=60000;
  Real v[N](each start=-60),gex[N],ginh[N];
  parameter Real tau=20, vrest=-60, vthres=-50,Trefrac=5,Eex=0,Einh=-80,tauex=5,tauinh=10,dgex=0.4,dgin=1.6;
  discrete Real  active[N](each start=1),tfire[N](each start=10000);
  discrete Real  nextev[M](each start=random10());
  parameter Real Period=10,lastTime=100;
  parameter Integer SNmap1[N];
  parameter Integer NNmap1[N],NNmap2[N], NNmap3[N], NNmap4[N],NNmap5[N],NNmap6[N], NNmap7[N], NNmap8[N],NNmap9[N], NNmap10[N];
  parameter Integer NNmap11[N],NNmap12[N], NNmap13[N], NNmap14[N],NNmap15[N],NNmap16[N], NNmap17[N], NNmap18[N],NNmap19[N], NNmap20[N];
  parameter Integer NNmap21[N];
  parameter Integer NNmap22[N];
  parameter Integer NNmap23[N];
  parameter Integer NNmap24[N];
  parameter Integer NNmap25[N];
  parameter Integer NNmap26[N];
  parameter Integer NNmap27[N];
  parameter Integer NNmap28[N];
  parameter Integer NNmap29[N];
  parameter Integer NNmap30[N];
  parameter Integer NNmap31[N];
  parameter Integer NNmap32[N];
  parameter Integer NNmap33[N];
  parameter Integer NNmap34[N];
  parameter Integer NNmap35[N];
  parameter Integer NNmap36[N];
  parameter Integer NNmap37[N];
  parameter Integer NNmap38[N];
  parameter Integer NNmap39[N];
  parameter Integer NNmap40[N];
  parameter Integer NNmap41[N];
  parameter Integer NNmap42[N];
  parameter Integer NNmap43[N];
  parameter Integer NNmap44[N];
  parameter Integer NNmap45[N];
  parameter Integer NNmap46[N];
  parameter Integer NNmap47[N];
  parameter Integer NNmap48[N];
  parameter Integer NNmap49[N];
  parameter Integer NNmap50[N];
  parameter Integer NNmap51[N];
  parameter Integer NNmap52[N];
  parameter Integer NNmap53[N];
  parameter Integer NNmap54[N];
  parameter Integer NNmap55[N];
  parameter Integer NNmap56[N];
  parameter Integer NNmap57[N];
  parameter Integer NNmap58[N];
  parameter Integer NNmap59[N];
  parameter Integer NNmap60[N];
  parameter Integer NNmap61[N];
  parameter Integer NNmap62[N];
  parameter Integer NNmap63[N];
  parameter Integer NNmap64[N];
  parameter Integer NNmap65[N];
  parameter Integer NNmap66[N];
  parameter Integer NNmap67[N];
  parameter Integer NNmap68[N];
  parameter Integer NNmap69[N];
  parameter Integer NNmap70[N];
  parameter Integer NNmap71[N];
  parameter Integer NNmap72[N];
  parameter Integer NNmap73[N];
  parameter Integer NNmap74[N];
  parameter Integer NNmap75[N];
  parameter Integer NNmap76[N];
  parameter Integer NNmap77[N];
  parameter Integer NNmap78[N];
  parameter Integer NNmap79[N];
  parameter Integer NNmap80[N];
  parameter Integer NNmap81[N];
  parameter Integer NNmap82[N];
  parameter Integer NNmap83[N];
  parameter Integer NNmap84[N];
  parameter Integer NNmap85[N];
  parameter Integer NNmap86[N];
  parameter Integer NNmap87[N];
  parameter Integer NNmap88[N];
  parameter Integer NNmap89[N];
  parameter Integer NNmap90[N];
  parameter Integer NNmap91[N];
  parameter Integer NNmap92[N];
  parameter Integer NNmap93[N];
  parameter Integer NNmap94[N];
  parameter Integer NNmap95[N];
  parameter Integer NNmap96[N];
  parameter Integer NNmap97[N];
  parameter Integer NNmap98[N];
  parameter Integer NNmap99[N];
  parameter Integer NNmap100[N];
  parameter Integer NNmap101[N];
  parameter Integer NNmap102[N];
  parameter Integer NNmap103[N];
  parameter Integer NNmap104[N];
  parameter Integer NNmap105[N];
  parameter Integer NNmap106[N];
  parameter Integer NNmap107[N];
  parameter Integer NNmap108[N];
  parameter Integer NNmap109[N];
  parameter Integer NNmap110[N];
  parameter Integer NNmap111[N];
  parameter Integer NNmap112[N];
  parameter Integer NNmap113[N];
  parameter Integer NNmap114[N];
  parameter Integer NNmap115[N];
  parameter Integer NNmap116[N];
  parameter Integer NNmap117[N];
  parameter Integer NNmap118[N];
  parameter Integer NNmap119[N];
  parameter Integer NNmap120[N];
  parameter Integer NNmap121[N];
  parameter Integer NNmap122[N];
  parameter Integer NNmap123[N];
  parameter Integer NNmap124[N];
  parameter Integer NNmap125[N];
  parameter Integer NNmap126[N];
  parameter Integer NNmap127[N];
  parameter Integer NNmap128[N];
  parameter Integer NNmap129[N];
  parameter Integer NNmap130[N];
  parameter Integer NNmap131[N];
  parameter Integer NNmap132[N];
  parameter Integer NNmap133[N];
  parameter Integer NNmap134[N];
  parameter Integer NNmap135[N];
  parameter Integer NNmap136[N];
  parameter Integer NNmap137[N];
  parameter Integer NNmap138[N];
  parameter Integer NNmap139[N];
  parameter Integer NNmap140[N];
  parameter Integer NNmap141[N];
  parameter Integer NNmap142[N];
  parameter Integer NNmap143[N];
  parameter Integer NNmap144[N];
  parameter Integer NNmap145[N];
  parameter Integer NNmap146[N];
  parameter Integer NNmap147[N];
  parameter Integer NNmap148[N];
  parameter Integer NNmap149[N];
  parameter Integer NNmap150[N];
  parameter Integer NNmap151[N];
  parameter Integer NNmap152[N];
  parameter Integer NNmap153[N];
  parameter Integer NNmap154[N];
  parameter Integer NNmap155[N];
  parameter Integer NNmap156[N];
  parameter Integer NNmap157[N];
  parameter Integer NNmap158[N];
  parameter Integer NNmap159[N];
  parameter Integer NNmap160[N];
  parameter Integer NNmap161[N];
  parameter Integer NNmap162[N];
  parameter Integer NNmap163[N];
  parameter Integer NNmap164[N];
  parameter Integer NNmap165[N];
  parameter Integer NNmap166[N];
  parameter Integer NNmap167[N];
  parameter Integer NNmap168[N];
  parameter Integer NNmap169[N];
  parameter Integer NNmap170[N];
  parameter Integer NNmap171[N];
  parameter Integer NNmap172[N];
  parameter Integer NNmap173[N];
  parameter Integer NNmap174[N];
  parameter Integer NNmap175[N];
  parameter Integer NNmap176[N];
  parameter Integer NNmap177[N];
  parameter Integer NNmap178[N];
  parameter Integer NNmap179[N];
  parameter Integer NNmap180[N];
  parameter Integer NNmap181[N];
  parameter Integer NNmap182[N];
  parameter Integer NNmap183[N];
  parameter Integer NNmap184[N];
  parameter Integer NNmap185[N];
  parameter Integer NNmap186[N];
  parameter Integer NNmap187[N];
  parameter Integer NNmap188[N];
  parameter Integer NNmap189[N];
  parameter Integer NNmap190[N];
  parameter Integer NNmap191[N];
  parameter Integer NNmap192[N];
  parameter Integer NNmap193[N];
  parameter Integer NNmap194[N];
  parameter Integer NNmap195[N];
  parameter Integer NNmap196[N];
  parameter Integer NNmap197[N];
  parameter Integer NNmap198[N];
  parameter Integer NNmap199[N];
  parameter Integer NNmap200[N];
  parameter Real ex[N];
  discrete Real exReinit[N];
  discrete Real inhReinit[N];
  discrete Real nextGlobalTime[M];
  discrete Real inputs[M];
  initial algorithm
    for i in 1:M loop
      SNmap1[i]:=randomN();
    end for;

    for i in 1:N loop
      NNmap1[i] := randomC(i);
      NNmap2[i] := randomC(i);
      NNmap3[i] := randomC(i);
      NNmap4[i] := randomC(i);
      NNmap5[i] := randomC(i);
      NNmap6[i] := randomC(i);
      NNmap7[i] := randomC(i);
      NNmap8[i] := randomC(i);
      NNmap9[i] := randomC(i);
      NNmap10[i] := randomC(i);
      NNmap11[i] := randomC(i);
      NNmap12[i] := randomC(i);
      NNmap13[i] := randomC(i);
      NNmap14[i] := randomC(i);
      NNmap15[i] := randomC(i);
      NNmap16[i] := randomC(i);
      NNmap17[i] := randomC(i);
      NNmap18[i] := randomC(i);
      NNmap19[i] := randomC(i);
      NNmap20[i] := randomC(i);
      NNmap21[i] := randomC(i);
      NNmap22[i] := randomC(i);
      NNmap23[i] := randomC(i);
      NNmap24[i] := randomC(i);
      NNmap25[i] := randomC(i);
      NNmap26[i] := randomC(i);
      NNmap27[i] := randomC(i);
      NNmap28[i] := randomC(i);
      NNmap29[i] := randomC(i);
      NNmap30[i] := randomC(i);
      NNmap31[i] := randomC(i);
      NNmap32[i] := randomC(i);
      NNmap33[i] := randomC(i);
      NNmap34[i] := randomC(i);
      NNmap35[i] := randomC(i);
      NNmap36[i] := randomC(i);
      NNmap37[i] := randomC(i);
      NNmap38[i] := randomC(i);
      NNmap39[i] := randomC(i);
      NNmap40[i] := randomC(i);
      NNmap41[i] := randomC(i);
      NNmap42[i] := randomC(i);
      NNmap43[i] := randomC(i);
      NNmap44[i] := randomC(i);
      NNmap45[i] := randomC(i);
      NNmap46[i] := randomC(i);
      NNmap47[i] := randomC(i);
      NNmap48[i] := randomC(i);
      NNmap49[i] := randomC(i);
      NNmap50[i] := randomC(i);
      NNmap51[i] := randomC(i);
      NNmap52[i] := randomC(i);
      NNmap53[i] := randomC(i);
      NNmap54[i] := randomC(i);
      NNmap55[i] := randomC(i);
      NNmap56[i] := randomC(i);
      NNmap57[i] := randomC(i);
      NNmap58[i] := randomC(i);
      NNmap59[i] := randomC(i);
      NNmap60[i] := randomC(i);
      NNmap61[i] := randomC(i);
      NNmap62[i] := randomC(i);
      NNmap63[i] := randomC(i);
      NNmap64[i] := randomC(i);
      NNmap65[i] := randomC(i);
      NNmap66[i] := randomC(i);
      NNmap67[i] := randomC(i);
      NNmap68[i] := randomC(i);
      NNmap69[i] := randomC(i);
      NNmap70[i] := randomC(i);
      NNmap71[i] := randomC(i);
      NNmap72[i] := randomC(i);
      NNmap73[i] := randomC(i);
      NNmap74[i] := randomC(i);
      NNmap75[i] := randomC(i);
      NNmap76[i] := randomC(i);
      NNmap77[i] := randomC(i);
      NNmap78[i] := randomC(i);
      NNmap79[i] := randomC(i);
      NNmap80[i] := randomC(i);
      NNmap81[i] := randomC(i);
      NNmap82[i] := randomC(i);
      NNmap83[i] := randomC(i);
      NNmap84[i] := randomC(i);
      NNmap85[i] := randomC(i);
      NNmap86[i] := randomC(i);
      NNmap87[i] := randomC(i);
      NNmap88[i] := randomC(i);
      NNmap89[i] := randomC(i);
      NNmap90[i] := randomC(i);
      NNmap91[i] := randomC(i);
      NNmap92[i] := randomC(i);
      NNmap93[i] := randomC(i);
      NNmap94[i] := randomC(i);
      NNmap95[i] := randomC(i);
      NNmap96[i] := randomC(i);
      NNmap97[i] := randomC(i);
      NNmap98[i] := randomC(i);
      NNmap99[i] := randomC(i);
      NNmap100[i] := randomC(i);
      NNmap101[i] := randomC(i);
      NNmap102[i] := randomC(i);
      NNmap103[i] := randomC(i);
      NNmap104[i] := randomC(i);
      NNmap105[i] := randomC(i);
      NNmap106[i] := randomC(i);
      NNmap107[i] := randomC(i);
      NNmap108[i] := randomC(i);
      NNmap109[i] := randomC(i);
      NNmap110[i] := randomC(i);
      NNmap111[i] := randomC(i);
      NNmap112[i] := randomC(i);
      NNmap113[i] := randomC(i);
      NNmap114[i] := randomC(i);
      NNmap115[i] := randomC(i);
      NNmap116[i] := randomC(i);
      NNmap117[i] := randomC(i);
      NNmap118[i] := randomC(i);
      NNmap119[i] := randomC(i);
      NNmap120[i] := randomC(i);
      NNmap121[i] := randomC(i);
      NNmap122[i] := randomC(i);
      NNmap123[i] := randomC(i);
      NNmap124[i] := randomC(i);
      NNmap125[i] := randomC(i);
      NNmap126[i] := randomC(i);
      NNmap127[i] := randomC(i);
      NNmap128[i] := randomC(i);
      NNmap129[i] := randomC(i);
      NNmap130[i] := randomC(i);
      NNmap131[i] := randomC(i);
      NNmap132[i] := randomC(i);
      NNmap133[i] := randomC(i);
      NNmap134[i] := randomC(i);
      NNmap135[i] := randomC(i);
      NNmap136[i] := randomC(i);
      NNmap137[i] := randomC(i);
      NNmap138[i] := randomC(i);
      NNmap139[i] := randomC(i);
      NNmap140[i] := randomC(i);
      NNmap141[i] := randomC(i);
      NNmap142[i] := randomC(i);
      NNmap143[i] := randomC(i);
      NNmap144[i] := randomC(i);
      NNmap145[i] := randomC(i);
      NNmap146[i] := randomC(i);
      NNmap147[i] := randomC(i);
      NNmap148[i] := randomC(i);
      NNmap149[i] := randomC(i);
      NNmap150[i] := randomC(i);
      NNmap151[i] := randomC(i);
      NNmap152[i] := randomC(i);
      NNmap153[i] := randomC(i);
      NNmap154[i] := randomC(i);
      NNmap155[i] := randomC(i);
      NNmap156[i] := randomC(i);
      NNmap157[i] := randomC(i);
      NNmap158[i] := randomC(i);
      NNmap159[i] := randomC(i);
      NNmap160[i] := randomC(i);
      NNmap161[i] := randomC(i);
      NNmap162[i] := randomC(i);
      NNmap163[i] := randomC(i);
      NNmap164[i] := randomC(i);
      NNmap165[i] := randomC(i);
      NNmap166[i] := randomC(i);
      NNmap167[i] := randomC(i);
      NNmap168[i] := randomC(i);
      NNmap169[i] := randomC(i);
      NNmap170[i] := randomC(i);
      NNmap171[i] := randomC(i);
      NNmap172[i] := randomC(i);
      NNmap173[i] := randomC(i);
      NNmap174[i] := randomC(i);
      NNmap175[i] := randomC(i);
      NNmap176[i] := randomC(i);
      NNmap177[i] := randomC(i);
      NNmap178[i] := randomC(i);
      NNmap179[i] := randomC(i);
      NNmap180[i] := randomC(i);
      NNmap181[i] := randomC(i);
      NNmap182[i] := randomC(i);
      NNmap183[i] := randomC(i);
      NNmap184[i] := randomC(i);
      NNmap185[i] := randomC(i);
      NNmap186[i] := randomC(i);
      NNmap187[i] := randomC(i);
      NNmap188[i] := randomC(i);
      NNmap189[i] := randomC(i);
      NNmap190[i] := randomC(i);
      NNmap191[i] := randomC(i);
      NNmap192[i] := randomC(i);
      NNmap193[i] := randomC(i);
      NNmap194[i] := randomC(i);
      NNmap195[i] := randomC(i);
      NNmap196[i] := randomC(i);
      NNmap197[i] := randomC(i);
      NNmap198[i] := randomC(i);
      NNmap199[i] := randomC(i);
      NNmap200[i] := randomC(i);
    end for;

    for i in 1:N loop
      ex[i]:=random100(i);
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
        exReinit[NNmap21[i]] := 1;
        exReinit[NNmap22[i]] := 1;
        exReinit[NNmap23[i]] := 1;
        exReinit[NNmap24[i]] := 1;
        exReinit[NNmap25[i]] := 1;
        exReinit[NNmap26[i]] := 1;
        exReinit[NNmap27[i]] := 1;
        exReinit[NNmap28[i]] := 1;
        exReinit[NNmap29[i]] := 1;
        exReinit[NNmap30[i]] := 1;
        exReinit[NNmap31[i]] := 1;
        exReinit[NNmap32[i]] := 1;
        exReinit[NNmap33[i]] := 1;
        exReinit[NNmap34[i]] := 1;
        exReinit[NNmap35[i]] := 1;
        exReinit[NNmap36[i]] := 1;
        exReinit[NNmap37[i]] := 1;
        exReinit[NNmap38[i]] := 1;
        exReinit[NNmap39[i]] := 1;
        exReinit[NNmap40[i]] := 1;
        exReinit[NNmap41[i]] := 1;
        exReinit[NNmap42[i]] := 1;
        exReinit[NNmap43[i]] := 1;
        exReinit[NNmap44[i]] := 1;
        exReinit[NNmap45[i]] := 1;
        exReinit[NNmap46[i]] := 1;
        exReinit[NNmap47[i]] := 1;
        exReinit[NNmap48[i]] := 1;
        exReinit[NNmap49[i]] := 1;
        exReinit[NNmap50[i]] := 1;
        exReinit[NNmap51[i]] := 1;
        exReinit[NNmap52[i]] := 1;
        exReinit[NNmap53[i]] := 1;
        exReinit[NNmap54[i]] := 1;
        exReinit[NNmap55[i]] := 1;
        exReinit[NNmap56[i]] := 1;
        exReinit[NNmap57[i]] := 1;
        exReinit[NNmap58[i]] := 1;
        exReinit[NNmap59[i]] := 1;
        exReinit[NNmap60[i]] := 1;
        exReinit[NNmap61[i]] := 1;
        exReinit[NNmap62[i]] := 1;
        exReinit[NNmap63[i]] := 1;
        exReinit[NNmap64[i]] := 1;
        exReinit[NNmap65[i]] := 1;
        exReinit[NNmap66[i]] := 1;
        exReinit[NNmap67[i]] := 1;
        exReinit[NNmap68[i]] := 1;
        exReinit[NNmap69[i]] := 1;
        exReinit[NNmap70[i]] := 1;
        exReinit[NNmap71[i]] := 1;
        exReinit[NNmap72[i]] := 1;
        exReinit[NNmap73[i]] := 1;
        exReinit[NNmap74[i]] := 1;
        exReinit[NNmap75[i]] := 1;
        exReinit[NNmap76[i]] := 1;
        exReinit[NNmap77[i]] := 1;
        exReinit[NNmap78[i]] := 1;
        exReinit[NNmap79[i]] := 1;
        exReinit[NNmap80[i]] := 1;
        exReinit[NNmap81[i]] := 1;
        exReinit[NNmap82[i]] := 1;
        exReinit[NNmap83[i]] := 1;
        exReinit[NNmap84[i]] := 1;
        exReinit[NNmap85[i]] := 1;
        exReinit[NNmap86[i]] := 1;
        exReinit[NNmap87[i]] := 1;
        exReinit[NNmap88[i]] := 1;
        exReinit[NNmap89[i]] := 1;
        exReinit[NNmap90[i]] := 1;
        exReinit[NNmap91[i]] := 1;
        exReinit[NNmap92[i]] := 1;
        exReinit[NNmap93[i]] := 1;
        exReinit[NNmap94[i]] := 1;
        exReinit[NNmap95[i]] := 1;
        exReinit[NNmap96[i]] := 1;
        exReinit[NNmap97[i]] := 1;
        exReinit[NNmap98[i]] := 1;
        exReinit[NNmap99[i]] := 1;
        exReinit[NNmap100[i]] := 1;
        exReinit[NNmap101[i]] := 1;
        exReinit[NNmap102[i]] := 1;
        exReinit[NNmap103[i]] := 1;
        exReinit[NNmap104[i]] := 1;
        exReinit[NNmap105[i]] := 1;
        exReinit[NNmap106[i]] := 1;
        exReinit[NNmap107[i]] := 1;
        exReinit[NNmap108[i]] := 1;
        exReinit[NNmap109[i]] := 1;
        exReinit[NNmap110[i]] := 1;
        exReinit[NNmap111[i]] := 1;
        exReinit[NNmap112[i]] := 1;
        exReinit[NNmap113[i]] := 1;
        exReinit[NNmap114[i]] := 1;
        exReinit[NNmap115[i]] := 1;
        exReinit[NNmap116[i]] := 1;
        exReinit[NNmap117[i]] := 1;
        exReinit[NNmap118[i]] := 1;
        exReinit[NNmap119[i]] := 1;
        exReinit[NNmap120[i]] := 1;
        exReinit[NNmap121[i]] := 1;
        exReinit[NNmap122[i]] := 1;
        exReinit[NNmap123[i]] := 1;
        exReinit[NNmap124[i]] := 1;
        exReinit[NNmap125[i]] := 1;
        exReinit[NNmap126[i]] := 1;
        exReinit[NNmap127[i]] := 1;
        exReinit[NNmap128[i]] := 1;
        exReinit[NNmap129[i]] := 1;
        exReinit[NNmap130[i]] := 1;
        exReinit[NNmap131[i]] := 1;
        exReinit[NNmap132[i]] := 1;
        exReinit[NNmap133[i]] := 1;
        exReinit[NNmap134[i]] := 1;
        exReinit[NNmap135[i]] := 1;
        exReinit[NNmap136[i]] := 1;
        exReinit[NNmap137[i]] := 1;
        exReinit[NNmap138[i]] := 1;
        exReinit[NNmap139[i]] := 1;
        exReinit[NNmap140[i]] := 1;
        exReinit[NNmap141[i]] := 1;
        exReinit[NNmap142[i]] := 1;
        exReinit[NNmap143[i]] := 1;
        exReinit[NNmap144[i]] := 1;
        exReinit[NNmap145[i]] := 1;
        exReinit[NNmap146[i]] := 1;
        exReinit[NNmap147[i]] := 1;
        exReinit[NNmap148[i]] := 1;
        exReinit[NNmap149[i]] := 1;
        exReinit[NNmap150[i]] := 1;
        exReinit[NNmap151[i]] := 1;
        exReinit[NNmap152[i]] := 1;
        exReinit[NNmap153[i]] := 1;
        exReinit[NNmap154[i]] := 1;
        exReinit[NNmap155[i]] := 1;
        exReinit[NNmap156[i]] := 1;
        exReinit[NNmap157[i]] := 1;
        exReinit[NNmap158[i]] := 1;
        exReinit[NNmap159[i]] := 1;
        exReinit[NNmap160[i]] := 1;
        exReinit[NNmap161[i]] := 1;
        exReinit[NNmap162[i]] := 1;
        exReinit[NNmap163[i]] := 1;
        exReinit[NNmap164[i]] := 1;
        exReinit[NNmap165[i]] := 1;
        exReinit[NNmap166[i]] := 1;
        exReinit[NNmap167[i]] := 1;
        exReinit[NNmap168[i]] := 1;
        exReinit[NNmap169[i]] := 1;
        exReinit[NNmap170[i]] := 1;
        exReinit[NNmap171[i]] := 1;
        exReinit[NNmap172[i]] := 1;
        exReinit[NNmap173[i]] := 1;
        exReinit[NNmap174[i]] := 1;
        exReinit[NNmap175[i]] := 1;
        exReinit[NNmap176[i]] := 1;
        exReinit[NNmap177[i]] := 1;
        exReinit[NNmap178[i]] := 1;
        exReinit[NNmap179[i]] := 1;
        exReinit[NNmap180[i]] := 1;
        exReinit[NNmap181[i]] := 1;
        exReinit[NNmap182[i]] := 1;
        exReinit[NNmap183[i]] := 1;
        exReinit[NNmap184[i]] := 1;
        exReinit[NNmap185[i]] := 1;
        exReinit[NNmap186[i]] := 1;
        exReinit[NNmap187[i]] := 1;
        exReinit[NNmap188[i]] := 1;
        exReinit[NNmap189[i]] := 1;
        exReinit[NNmap190[i]] := 1;
        exReinit[NNmap191[i]] := 1;
        exReinit[NNmap192[i]] := 1;
        exReinit[NNmap193[i]] := 1;
        exReinit[NNmap194[i]] := 1;
        exReinit[NNmap195[i]] := 1;
        exReinit[NNmap196[i]] := 1;
        exReinit[NNmap197[i]] := 1;
        exReinit[NNmap198[i]] := 1;
        exReinit[NNmap199[i]] := 1;
        exReinit[NNmap200[i]] := 1;
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
        inhReinit[NNmap21[i]] := 1;
        inhReinit[NNmap22[i]] := 1;
        inhReinit[NNmap23[i]] := 1;
        inhReinit[NNmap24[i]] := 1;
        inhReinit[NNmap25[i]] := 1;
        inhReinit[NNmap26[i]] := 1;
        inhReinit[NNmap27[i]] := 1;
        inhReinit[NNmap28[i]] := 1;
        inhReinit[NNmap29[i]] := 1;
        inhReinit[NNmap30[i]] := 1;
        inhReinit[NNmap31[i]] := 1;
        inhReinit[NNmap32[i]] := 1;
        inhReinit[NNmap33[i]] := 1;
        inhReinit[NNmap34[i]] := 1;
        inhReinit[NNmap35[i]] := 1;
        inhReinit[NNmap36[i]] := 1;
        inhReinit[NNmap37[i]] := 1;
        inhReinit[NNmap38[i]] := 1;
        inhReinit[NNmap39[i]] := 1;
        inhReinit[NNmap40[i]] := 1;
        inhReinit[NNmap41[i]] := 1;
        inhReinit[NNmap42[i]] := 1;
        inhReinit[NNmap43[i]] := 1;
        inhReinit[NNmap44[i]] := 1;
        inhReinit[NNmap45[i]] := 1;
        inhReinit[NNmap46[i]] := 1;
        inhReinit[NNmap47[i]] := 1;
        inhReinit[NNmap48[i]] := 1;
        inhReinit[NNmap49[i]] := 1;
        inhReinit[NNmap50[i]] := 1;
        inhReinit[NNmap51[i]] := 1;
        inhReinit[NNmap52[i]] := 1;
        inhReinit[NNmap53[i]] := 1;
        inhReinit[NNmap54[i]] := 1;
        inhReinit[NNmap55[i]] := 1;
        inhReinit[NNmap56[i]] := 1;
        inhReinit[NNmap57[i]] := 1;
        inhReinit[NNmap58[i]] := 1;
        inhReinit[NNmap59[i]] := 1;
        inhReinit[NNmap60[i]] := 1;
        inhReinit[NNmap61[i]] := 1;
        inhReinit[NNmap62[i]] := 1;
        inhReinit[NNmap63[i]] := 1;
        inhReinit[NNmap64[i]] := 1;
        inhReinit[NNmap65[i]] := 1;
        inhReinit[NNmap66[i]] := 1;
        inhReinit[NNmap67[i]] := 1;
        inhReinit[NNmap68[i]] := 1;
        inhReinit[NNmap69[i]] := 1;
        inhReinit[NNmap70[i]] := 1;
        inhReinit[NNmap71[i]] := 1;
        inhReinit[NNmap72[i]] := 1;
        inhReinit[NNmap73[i]] := 1;
        inhReinit[NNmap74[i]] := 1;
        inhReinit[NNmap75[i]] := 1;
        inhReinit[NNmap76[i]] := 1;
        inhReinit[NNmap77[i]] := 1;
        inhReinit[NNmap78[i]] := 1;
        inhReinit[NNmap79[i]] := 1;
        inhReinit[NNmap80[i]] := 1;
        inhReinit[NNmap81[i]] := 1;
        inhReinit[NNmap82[i]] := 1;
        inhReinit[NNmap83[i]] := 1;
        inhReinit[NNmap84[i]] := 1;
        inhReinit[NNmap85[i]] := 1;
        inhReinit[NNmap86[i]] := 1;
        inhReinit[NNmap87[i]] := 1;
        inhReinit[NNmap88[i]] := 1;
        inhReinit[NNmap89[i]] := 1;
        inhReinit[NNmap90[i]] := 1;
        inhReinit[NNmap91[i]] := 1;
        inhReinit[NNmap92[i]] := 1;
        inhReinit[NNmap93[i]] := 1;
        inhReinit[NNmap94[i]] := 1;
        inhReinit[NNmap95[i]] := 1;
        inhReinit[NNmap96[i]] := 1;
        inhReinit[NNmap97[i]] := 1;
        inhReinit[NNmap98[i]] := 1;
        inhReinit[NNmap99[i]] := 1;
        inhReinit[NNmap100[i]] := 1;
        inhReinit[NNmap101[i]] := 1;
        inhReinit[NNmap102[i]] := 1;
        inhReinit[NNmap103[i]] := 1;
        inhReinit[NNmap104[i]] := 1;
        inhReinit[NNmap105[i]] := 1;
        inhReinit[NNmap106[i]] := 1;
        inhReinit[NNmap107[i]] := 1;
        inhReinit[NNmap108[i]] := 1;
        inhReinit[NNmap109[i]] := 1;
        inhReinit[NNmap110[i]] := 1;
        inhReinit[NNmap111[i]] := 1;
        inhReinit[NNmap112[i]] := 1;
        inhReinit[NNmap113[i]] := 1;
        inhReinit[NNmap114[i]] := 1;
        inhReinit[NNmap115[i]] := 1;
        inhReinit[NNmap116[i]] := 1;
        inhReinit[NNmap117[i]] := 1;
        inhReinit[NNmap118[i]] := 1;
        inhReinit[NNmap119[i]] := 1;
        inhReinit[NNmap120[i]] := 1;
        inhReinit[NNmap121[i]] := 1;
        inhReinit[NNmap122[i]] := 1;
        inhReinit[NNmap123[i]] := 1;
        inhReinit[NNmap124[i]] := 1;
        inhReinit[NNmap125[i]] := 1;
        inhReinit[NNmap126[i]] := 1;
        inhReinit[NNmap127[i]] := 1;
        inhReinit[NNmap128[i]] := 1;
        inhReinit[NNmap129[i]] := 1;
        inhReinit[NNmap130[i]] := 1;
        inhReinit[NNmap131[i]] := 1;
        inhReinit[NNmap132[i]] := 1;
        inhReinit[NNmap133[i]] := 1;
        inhReinit[NNmap134[i]] := 1;
        inhReinit[NNmap135[i]] := 1;
        inhReinit[NNmap136[i]] := 1;
        inhReinit[NNmap137[i]] := 1;
        inhReinit[NNmap138[i]] := 1;
        inhReinit[NNmap139[i]] := 1;
        inhReinit[NNmap140[i]] := 1;
        inhReinit[NNmap141[i]] := 1;
        inhReinit[NNmap142[i]] := 1;
        inhReinit[NNmap143[i]] := 1;
        inhReinit[NNmap144[i]] := 1;
        inhReinit[NNmap145[i]] := 1;
        inhReinit[NNmap146[i]] := 1;
        inhReinit[NNmap147[i]] := 1;
        inhReinit[NNmap148[i]] := 1;
        inhReinit[NNmap149[i]] := 1;
        inhReinit[NNmap150[i]] := 1;
        inhReinit[NNmap151[i]] := 1;
        inhReinit[NNmap152[i]] := 1;
        inhReinit[NNmap153[i]] := 1;
        inhReinit[NNmap154[i]] := 1;
        inhReinit[NNmap155[i]] := 1;
        inhReinit[NNmap156[i]] := 1;
        inhReinit[NNmap157[i]] := 1;
        inhReinit[NNmap158[i]] := 1;
        inhReinit[NNmap159[i]] := 1;
        inhReinit[NNmap160[i]] := 1;
        inhReinit[NNmap161[i]] := 1;
        inhReinit[NNmap162[i]] := 1;
        inhReinit[NNmap163[i]] := 1;
        inhReinit[NNmap164[i]] := 1;
        inhReinit[NNmap165[i]] := 1;
        inhReinit[NNmap166[i]] := 1;
        inhReinit[NNmap167[i]] := 1;
        inhReinit[NNmap168[i]] := 1;
        inhReinit[NNmap169[i]] := 1;
        inhReinit[NNmap170[i]] := 1;
        inhReinit[NNmap171[i]] := 1;
        inhReinit[NNmap172[i]] := 1;
        inhReinit[NNmap173[i]] := 1;
        inhReinit[NNmap174[i]] := 1;
        inhReinit[NNmap175[i]] := 1;
        inhReinit[NNmap176[i]] := 1;
        inhReinit[NNmap177[i]] := 1;
        inhReinit[NNmap178[i]] := 1;
        inhReinit[NNmap179[i]] := 1;
        inhReinit[NNmap180[i]] := 1;
        inhReinit[NNmap181[i]] := 1;
        inhReinit[NNmap182[i]] := 1;
        inhReinit[NNmap183[i]] := 1;
        inhReinit[NNmap184[i]] := 1;
        inhReinit[NNmap185[i]] := 1;
        inhReinit[NNmap186[i]] := 1;
        inhReinit[NNmap187[i]] := 1;
        inhReinit[NNmap188[i]] := 1;
        inhReinit[NNmap189[i]] := 1;
        inhReinit[NNmap190[i]] := 1;
        inhReinit[NNmap191[i]] := 1;
        inhReinit[NNmap192[i]] := 1;
        inhReinit[NNmap193[i]] := 1;
        inhReinit[NNmap194[i]] := 1;
        inhReinit[NNmap195[i]] := 1;
        inhReinit[NNmap196[i]] := 1;
        inhReinit[NNmap197[i]] := 1;
        inhReinit[NNmap198[i]] := 1;
        inhReinit[NNmap199[i]] := 1;
        inhReinit[NNmap200[i]] := 1;
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
      when inhReinit[i] > 0.5 then
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
    MMO_PartitionMethod=Manual,
    MMO_LPS=62,
    MMO_DT_Min=0.5,
    MMO_Output={v[1:N/20:N]},
    MMO_OutputType=CI_Sampled,
    StartTime=0.0,
    StopTime=300,
    Tolerance={1e-3},
    AbsTolerance={1e-3}
  ));
end spl_neurons;