model paneles
//Parametros de las fuentes boost
 constant Integer Nboost=2;
 parameter Real Cboost = 1e-4;
 parameter Real Lboost = 1e-4;
 parameter Real R = 100.0;
 parameter Real ROn = 1e-5;
 parameter Real ROff = 1e5;
 parameter Real Tboost[Nboost];
 discrete Real DCboost[Nboost];
 discrete Real Rdboost[Nboost](each start=1e5);
 discrete Real Rsboost[Nboost](each start=1e5);
 discrete Real nextTboost[Nboost];
 discrete Real lastTboost[Nboost];
 Real uCboost[Nboost];
 Real iLboost[Nboost];
 Real iDboost[Nboost];
 Real uDboost[Nboost];
 Real U[Nboost];
 Real Uline (start=0);

//Parámetros de las celdas solares
constant Integer Npaneles=2; //paneles
constant Integer Ns=36;//Numero de celdas en serie por panel
constant Integer Np=36;//Número de celdas en paralelo por panel
parameter Real q=1.6e-19; // [C]
parameter Real Ac=1.6;
parameter Real K=1.3805e-23; // [Nm/K]
parameter Real K1=0.0017;// [ A/oC]
parameter Real Ior=2.0793e-6;// [A]
parameter Real Tref=303;// [K]
parameter Real Eg=1.1;// [V]
parameter Real Isc=3.27;// [A]  
Real Ipv[Npaneles]; //Corriente generada por el panel
Real Vpv[Npaneles];//Tension generada por el panel
discrete Real Iph[Npaneles]; //Corriente por radiación
discrete Real Irs; //Corriente de saturación inversa
discrete Real Tpv; // Temperatura de operación de los paneles
discrete Real  lambdaph[Npaneles]; //Radiacteón [mW/cm2]
parameter Real Rspv;
Real PotPanel[Npaneles]; //Potencia generada por cada panel
discrete Real DiodePanel[Npaneles](each start=1);   //Estado de conduccion del diodo del modelo del panel

//Parametros del Capacitor, la linea, la carga y la bateria
parameter Real Cbatery=4700e-6;
parameter Real Rshunt=0.0001; //Resistencia de shunt de la coneccion de la boost con la linea de CC [ohm]
discrete Real Rdshunt[Nboost](each start=1e5);
parameter Real iLoad=0.1;
Real iBatery;

//variables intermedias
Real Ishunt[Npaneles];
discrete Real LastPotPanel[Nboost](each start=0);
discrete Real ActualPot[Nboost](each start=0);
discrete Real deltaPot[Nboost];
Real PotMedia[Nboost];
Real Vpvmedia[Nboost];
discrete Real Vpvdiscreta[Nboost];
discrete Real VpvRef[Nboost](each start=3);       //Tension de referencia para el control de tension de los paneles

//Parametros de control
parameter Real TContMaxPot;
discrete Real nextTContMaxPot[Nboost];
Real control[Nboost];
Real ControlIVlinea;
parameter Real VlineaRef=10;


initial algorithm 

Tpv:=273+50.0;                       //Debe ingresarce la Temperatura de los paneles [K]
for i in 1:Npaneles loop
  TContMaxPot:=0.5;
  nextTContMaxPot[i]:=0;
  nextTboost[i]:=1e-4;
  Rsboost[i]:=1e5;
  Rdboost[i]:=1e5;
  Tboost[i]:=1e-4;
  DCboost[i]:=0.9;
//  VpvRef[i]:=1;
  lambdaph[i]:=8;
  Rdshunt[i]:=1e5;
//  PotMedia[i]:=0;
//  VpvRef[i]:=1;
end for;

//DCboost[1]:=0.7;
//DCboost[2]:=0.3;
VpvRef[1]:=1;
VpvRef[2]:=2;
//DCboost[3]:=0.7;
//DCboost[4]:=0.9;

//lambdaph[1]:=6.5;			//Debe ingresarce la radiacion medida [mW/cm2]
//lambdaph[2]:=10;
//lambdaph[3]:=6.5;
//lambdaph[4]:=6.5;
Rspv:=1.0;

Irs:=Ior*(Tpv/Tref)^3*exp(q*Eg*(1/Tref-1/Tpv)/K/Ac);

for i in 1:Npaneles loop
	Iph[i]:=(Isc+K1*(Tpv-Tref))*lambdaph[i]/100;
end for;


equation 

for i in 1:Npaneles loop				//Ecuaciones correspondientes a los paneles
    	Ipv[i]=iLboost[i];
    	Vpv[i]=( log(DiodePanel*(Np*Iph[i]-Ipv[i])/(Np*Irs)+1)*K*Ac*Tpv/q-Ipv[i]*Rspv/Np )*Ns;
    	U[i]=Vpv[i];
	//PotPanel[i]=U[i]*Ipv[i];
    	iDboost[i]=((Rsboost[i])*iLboost[i]-uCboost[i])/((Rdboost[i])+(Rsboost[i]));
    	uDboost[i]=iDboost[i]*Rdboost[i];
    	der(uCboost[i]) = (iDboost[i] - uCboost[i]/R-(uCboost[i]-Uline)/(Rshunt+Rdshunt[i]))/Cboost;
    	der(iLboost[i]) =  (U[i]-Rsboost[i]*(iLboost[i]-iDboost[i]))/Lboost;
	Ishunt[i]=(uCboost[i]-Uline)/(Rshunt+Rdshunt[i]);

	PotPanel[i]=Uline*Ishunt[i];
//	PotPanel[i]=Vpv[i]*iLboost[i];
//	deltaPot[i]=PotPanel[i]-LastPotPanel[i];
//	deltaPot[i]=PotMedia[i]/time-LastPotPanel[i];
	der(control[i])=VpvRef[i]-Vpvdiscreta[i];
	der(PotMedia[i])=PotPanel[i]/TContMaxPot;
	der(Vpvmedia[i])=Vpv[i]/Tboost[i];
end for;


iBatery=-(VlineaRef-Uline)*1-ControlIVlinea;
//der(Uline)=((uCboost[1]-Uline)/(Rshunt+Rdshunt[1])+(uCboost[2]-Uline)/(Rshunt+Rdshunt[2])-iLoad-iBatery)/Cbatery;
der(Uline)=(sum(Ishunt)-iLoad-iBatery)/Cbatery;
der(ControlIVlinea)=VlineaRef-Uline;


algorithm 

for i in 1:Nboost loop
  when time > nextTContMaxPot[i] then
		nextTContMaxPot[i]:=nextTContMaxPot[i]+TContMaxPot;
		LastPotPanel[i]:=ActualPot[i];
		ActualPot[i]:=PotMedia[i];
		deltaPot[i]:=ActualPot[i]-LastPotPanel[i];
		reinit(	PotMedia[i],0);
		if ActualPot[i]<25 then
			if (deltaPot[i]>0 and VpvRef[i]<10) then
				VpvRef[i]:=VpvRef[i]+1;
			else
				if(VpvRef[i]>1) then
					VpvRef[i]:=VpvRef[i]-1;
				end if;
			end if;
		end if;
		VpvRef[i]:=2;

  end when;
end for;

for i in 1:Nboost loop  
  when time > nextTboost[i] then
      lastTboost[i]:=nextTboost[i];
      nextTboost[i]:=nextTboost[i]+Tboost[i];
      Rsboost[i] := ROn;
	Vpvdiscreta[i]:=Vpvmedia[i];
	reinit(Vpvmedia[i],0);

	DCboost[i]:=-0.01*(VpvRef[i]-Vpvdiscreta[i])-control[i]*0.7;
     	if (DCboost[i]>0.98) then
			DCboost[i]:=0.98;
	end if;
	if (DCboost[i]<0.02) then
			DCboost[i]:=0.02;
	end if;
  end when;
end for;
  
for i in 1:Nboost loop  
  when time- lastTboost[i]-DCboost[i]*Tboost[i]>0 then
      Rsboost[i] := ROff;
  end when;
end for;  

for i in 1:Nboost loop
	when Np*Iph[i]-Ipv[i]<0.1 then
		DiodePanel[i]:=0;
	end when;
end for;  

for i in 1:Nboost loop
	when Np*Iph[i]-Ipv[i]>0.1 then
		DiodePanel[i]:=1;
	end when;
end for;  

for i in 1:Nboost loop  
  when iDboost[i]<0 then
      Rdboost[i] := ROff;
  end when;
end for;

for i in 1:Nboost loop
  when uDboost[i]>0 then
      Rdboost[i] := ROn;
  end when;
end for;

for i in 1:Nboost loop
  when uCboost[i]-Uline>0.1 then
		Rdshunt[i]:=ROn;
  end when;
end for;

for i in 1:Nboost loop
  when (uCboost[i]-Uline)/Rdshunt<0.1 then
		Rdshunt[i]:=ROff;
  end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={Uline,DCboost},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=1,
		Tolerance={1e-4},
		AbsTolerance={1e-4}
	));
end paneles;
