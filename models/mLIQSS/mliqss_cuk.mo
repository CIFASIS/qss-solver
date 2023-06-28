model mliqss_cuk
  constant Integer N = 4;
  parameter Real R = 10, U = 24, T = 1e-4, DC = 0.25, ROn = 1e-5, ROff = 1e5;
  parameter Real C1=1e-4,L1=1e-4, C2 = 1e-4, L2 = 1e-4;
  discrete Real Rd(start=1e5), Rs(start=1e-5), nextT(start=T),lastT,diodeon;
  Real uC1[N],iL1[N],uC2,iL2[N],iD[N],s[N];

  equation
for i in 1:N loop
    s[i]=diodeon*iD[i]+(1-diodeon)*iD[i]*Rd;
    iD[i]=(Rs*(iL1[i]+iL2[i])-uC1[i])/(Rs+Rd);
    der(iL1[i]) =  (U-uC1[i]-iD[i]*Rs)/L1;
    der(iL2[i]) =  (-uC2-iD[i]*Rs)/L2;
     der(uC1[i]) = (iD[i] - iL2[i])/C1;
end for;
     der(uC2) = (sum(iL2) - uC2/R)/C2;

algorithm
    when time > nextT then
      lastT:=nextT;
      nextT:=nextT+T;
      Rs := ROn;
    end when;

    when time - lastT-DC*T>0 then
      Rs := ROff;
    end when;

for i in 1:N loop
    when s[i]>0 then
      Rd:=ROn;
      diodeon:=1;
    elsewhen s[i]<0 then
      Rd := ROff;
       diodeon:=0;
    end when;
end for;
	annotation(

	experiment(
		MMO_Description="Cuk converter with non-solved stiffness.",
		MMO_Solver=mLIQSS2,
		MMO_PartitionMethod=Metis,
		MMO_Output={uC1,iL1,uC2,iL2},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0,
		StopTime=0.02,
		Tolerance={ 1e-3},
		AbsTolerance={ 1e-3}
	));
end mliqss_cuk;
