model train4
  constant Integer cars=10;
  Real l[cars],vl[cars],w[cars];

  discrete Real noslip[cars](each start=1);
  discrete Real tau_mot(start=190000);
 //car parameters
  parameter Real g=9.81,m=126000,L=20,kc=1e7,bc=1e7;
 //wheel parameters
 parameter Real R=1,J=100,bslip=1e-3,bnoslip=1,baxle=1e-4,vslipmax =0.1,fric_coef_dyn = 0.6;
//track parameters
 parameter Real r=10e3,phi=0.0001;

//algebraic variables
  Real dxdl[cars],dydl[cars],dzdl[cars];
  Real x[cars],y[cars],z[cars];
	 Real fl[cars],fx[cars],fy[cars],fz[cars];
 Real fric[cars],vs[cars];

initial algorithm

for i in 1:cars loop
  l[i]:=(i-1)*L;
end for;  


equation
  x[1]=r*sin(l[1]/r)*cos(phi);
  y[1]=r*cos(l[1]/r);
  z[1]=r*sin(l[1]/r)*sin(phi);
  fx[1]=0;
  fy[1]=0;
  fz[1]=-m*g;
 dxdl[1]=cos(l[1]/r)*cos(phi);
 dydl[1]=-sin(l[1]/r);
  dzdl[1]=cos(l[1]/r)*sin(phi);
 
 vs[1]=R*w[1]-vl[1];
 fric[1] = noslip[1] * bnoslip * vs[1]* m*g + (1 - noslip[1]) * (fric_coef_dyn * vs[1] * m*g+ bslip *vs[1] * m*g);

 fl[1]=dxdl[1]*fx[1]+dydl[1]*fy[1]+dzdl[1]*fz[1]+fric[1]+kc*(l[2]-l[1]-L)+bc*(vl[2]-vl[1]);
 
 der(w[1])=(tau_mot-R*fric[1]-baxle*m*g*w[1])/J;
 der(l[1])=vl[1];
  der(vl[1])=fl[1]/m;

for i in 2:cars-1 loop 
  x[i]=r*sin(l[i]/r)*cos(phi);
  y[i]=r*cos(l[i]/r);
  z[i]=r*sin(l[i]/r)*sin(phi);
  fx[i]=0;
  fy[i]=0;
  fz[i]=-m*g;
 dxdl[i]=cos(l[i]/r)*cos(phi);
 dydl[i]=-sin(l[i]/r);
  dzdl[i]=cos(l[i]/r)*sin(phi);
 
 vs[i]=R*w[i]-vl[i];
 fric[i] = noslip[i] * bnoslip * vs[i]* m*g + (1 - noslip[i]) * (fric_coef_dyn * vs[i] * m*g+ bslip *vs[i] * m*g);

 fl[i]=dxdl[i]*fx[i]+dydl[i]*fy[i]+dzdl[i]*fz[i]+fric[i]+kc*(l[i+1]+l[i-1]-2*l[i])+bc*(vl[i+1]+vl[i-1]-2*vl[i]);
 
 der(w[i])=(-R*fric[i]-baxle*m*g*w[i])/J;
 der(l[i])=vl[i];
  der(vl[i])=fl[i]/m;
end for;

 x[cars]=r*sin(l[cars]/r)*cos(phi);
  y[cars]=r*cos(l[cars]/r);
  z[cars]=r*sin(l[cars]/r)*sin(phi);
  fx[cars]=0;
  fy[cars]=0;
  fz[cars]=-m*g;
 dxdl[cars]=cos(l[cars]/r)*cos(phi);
 dydl[cars]=-sin(l[cars]/r);
  dzdl[cars]=cos(l[cars]/r)*sin(phi);
 
 vs[cars]=R*w[cars]-vl[cars];
 fric[cars] = noslip[cars] * bnoslip * vs[cars]* m*g + (1 - noslip[cars]) * (fric_coef_dyn * vs[cars] * m*g+ bslip *vs[cars] * m*g);

 fl[cars]=dxdl[cars]*fx[cars]+dydl[cars]*fy[cars]+dzdl[cars]*fz[cars]+fric[cars]+kc*(l[cars-1]-l[cars]+L)+bc*(vl[cars-1]-vl[cars]);
 
 der(w[cars])=(-R*fric[cars]-baxle*m*g*w[cars])/J;
 der(l[cars])=vl[cars];
  der(vl[cars])=fl[cars]/m;


 algorithm

  

for i in 1:cars loop
 when vs[i] > vslipmax then
    noslip[i] := 0;
  elsewhen vs[i] < vslipmax then
    noslip[i] := 1;
  end when;
  when vs[i] < -vslipmax then
    noslip[i] := 0;
   elsewhen vs[i] >-vslipmax then
    noslip[i] := 1;
  end when;
end for;
when time>30 then
    tau_mot:=0;
  end when;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_Period={1000/5000},
		MMO_Output={vl[1]},
		MMO_OutputType=CI_Sampled,
		StartTime=0.0,
		StopTime=1000,
		Tolerance={1e-4},
		AbsTolerance={1e-6}
	));
end train4;
