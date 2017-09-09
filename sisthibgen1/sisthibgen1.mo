model sisthibgen1
	function ipvsolver
		input Real aVpv;
		input Real bNp;
		input Real cNs;
		input Real dTpv;
		input Real elambdaph;
		input Real femax;
		input Real gitmax;
		input Real hIpvguess;
		output Real iIpv;
		external "C" iIpv=ipvsolver(aVpv,bNp,cNs,dTpv,elambdaph,femax,gitmax,hIpvguess) annotation(Include = "#include <../../src/test-suite/sisthibgen1/ipvsolver.c>");
	end ipvsolver;
//    import gustavo;
constant Integer NboostSerie=1;
constant Integer NgenPVparalelo=1;
 constant Integer Nboost=2; //Cantidad total de Convertidores Boost (NboostSerie*NgenPVparalelo)

//Parametros de las fuentes boost

 parameter Real Cboost = 4.7e-3;
 parameter Real Lboost = 15e-3;
 parameter Real R = 1000.0;
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

//Parámetros de las celdas solares
constant Integer Npaneles=2; //paneles (Nboostserie*NgenPVparalelo)
parameter Real Cpvin=4.7e-3; //Filtro de entrada
constant Integer Ns=60;//Numero de celdas en serie por panel
constant Integer Np=1;//Número de celdas en paralelo por panel
discrete Real Tpv; // Temperatura de operación de los paneles
Real Ipv[Npaneles]; //Corriente generada por el panel
Real Vpv[Npaneles](each start=2);//Tension generada por el panel
discrete Real  lambdaph[Npaneles]; //Radiacteón [mW/cm2]

//Parametros del Capacitor, la linea, la carga y la bateria
parameter Real Cbus=1e-4;//4700e-6;
parameter Real Rshunt=0.01; //Resistencia de shunt de la coneccion de la boost con la linea de CC [ohm]
discrete Real Rdshunt[Nboost](each start=1e5);
parameter Real iLoad=0.1;
Real iBatery;
Real Ubus (start=20);
Real Ibatery;

//Parametros de control
parameter Real TContMaxPot;
discrete Real nextTContMaxPot[Nboost];

//Real ControlIVlinea;
//parameter Real VlineaRef=50;
discrete Real VpvRef[Nboost](each start=35);       //Tension de referencia para el control de tension de los paneles
Real VpvrefVpvIntegral[Nboost];
parameter Real UlineRef=48;
Real eUlineIntegral;

//variables intermedias
discrete Real Ipvguess[Npaneles](each start=0); 
Real PotPanel[Npaneles]; //Potencia generada por cada panel
discrete Real DiodePanel[Npaneles](each start=1);   //Estado de conduccion del diodo del modelo del panel
discrete Real sumAproxInt[Npaneles];
Real VpvFiltrada[Npaneles];

//MPPT algorithm variables
discrete Real LastPotPanel[Npaneles](each start=0);
discrete Real ActualPotPanel[Npaneles](each start=0);
discrete Real deltaPot[Npaneles](each start=0);
discrete Real LastVpv[Npaneles](each start=0);
discrete Real ActualVpv[Npaneles](each start=0);
discrete Real deltaVpv[Npaneles](each start=0);
parameter Real deltaVpvRefMPPT=0.5;
Real suma[Npaneles];

initial algorithm 

Tpv:=273+25.0;                       //Debe ingresarce la Temperatura de los paneles [K]

for i in 1:Npaneles loop
	lambdaph[i]:=80;			
	TContMaxPot:=0.2;
	nextTContMaxPot[i]:=TContMaxPot;
end for;

for i in 1:Nboost loop

  	Rsboost[i]:=1e5;
  	Rdboost[i]:=1e5;
  	Tboost[i]:=2e-5;
  	DCboost[i]:=0.9;
	nextTboost[i]:=Tboost[i];
end for;


//=================================================================
equation 
//=================================================================

for i in 1:Npaneles loop				//Ecuaciones correspondientes a los paneles
	Ipv[i]=ipvsolver(Vpv[i],Np,Ns,Tpv,lambdaph[i],1e-5,6,Ipvguess[i]);
	der(Vpv[i])=(Ipv[i]-iLboost[i])/Cpvin;
	PotPanel[i]=Vpv[i]*Ipv[i];
	iDboost[i]=((Rsboost[i])*iLboost[i]-80-0*uCboost[i])/((Rdboost[i])+(Rsboost[i]));
	uDboost[i]=iDboost[i]*Rdboost[i];
	der(uCboost[i]) = (iDboost[i] - 0*uCboost[i]/R-0*(uCboost[i]-Ubus)/0.1)/Cboost;
	der(iLboost[i]) =  (Vpv[i]-Rsboost[i]*(iLboost[i]-iDboost[i]))/Lboost;
	der(VpvrefVpvIntegral[i])=VpvRef[i]-Vpv[i];
	der(VpvFiltrada[i])=(Vpv[i]-VpvFiltrada[i])*100;
suma[i] = (uCboost[i]-Ubus)/0.1;
end for;
der (eUlineIntegral)=UlineRef-Ubus;
Ibatery=2*eUlineIntegral+0.05*(UlineRef-Ubus);
//der(Ubus)=((uCboost[1]-Ubus)/0.1+(uCboost[2]-Ubus)/0.1+Ibatery)/Cbus;

der(Ubus)=(sum(suma)+Ibatery)/Cbus;


//====================================================
algorithm 
//====================================================

for i in 1:Npaneles loop
	when time > nextTboost[i] then
		lastTboost[i]:=nextTboost[i];
 nextTboost[i]:=nextTboost[i]+Tboost[i];
		Ipvguess[i]:=Ipv[i];
		Rsboost[i] := ROn;
		sumAproxInt[i]:=sumAproxInt[i]+(VpvRef[i]-Vpv[i])*Tboost[i];
		DCboost[i]:=0.85-0.3*(VpvRef[i]-Vpv)-1*sumAproxInt[i]*6;
//		DCboost[i]:=0.8-0.1*(VpvRef[i]-Vpv[i])-VpvrefVpvIntegral[i]*6;
		if (DCboost[i]>0.98) then
			DCboost[i]:=0.98;
		end if;
		if (DCboost[i]<0.4) then
			DCboost[i]:=0.4;
		end if;
	end when;
end for;

for i in 1:Npaneles loop  
  when time- lastTboost[i]-DCboost[i]*Tboost[i]>0 then
      Rsboost[i] := ROff;
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


for i in 1:Npaneles loop
	when time > nextTContMaxPot[i] then
		nextTContMaxPot[i]:=nextTContMaxPot[i]+TContMaxPot;
		LastPotPanel[i]:=ActualPotPanel[i];
		ActualPotPanel[i]:=PotPanel[i];
		deltaPot[i]:=ActualPotPanel[i]-LastPotPanel[i];

		//algoritmo MPPT
		
		LastVpv[i]:=ActualVpv[i];
		ActualVpv[i]:=VpvRef[i];
		deltaVpv[i]:=ActualVpv[i]-LastVpv[i];	
//		LastIpv[i]:=ActualIpv[i];
//		ActualIpv[i]:=Ipv[i];
//		deltaIpv[i]:=ActualIpv[i]-LastIpv[i];

		if(abs(deltaVpv[i])>0.1*deltaVpvRefMPPT) then
//		if(deltaVpv[i]<>0) then
			if(deltaPot[i]<>0) then
				if(deltaPot[i]/deltaVpv[i]>0) then
					VpvRef[i]:=VpvRef[i]+deltaVpvRefMPPT;
				else
					VpvRef[i]:=VpvRef[i]-deltaVpvRefMPPT;
				end if;
			end if;
			VpvRef[i]:=30;
		end if;
			




/*		
		if (deltaPot[i]>0 and VpvRef[i]<10) then
			VpvRef[i]:=VpvRef[i]+1;
		else
			if(VpvRef[i]>1) then
				VpvRef[i]:=VpvRef[i]-1;
			end if;
		end if;
*/
	end when;
end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=LIQSS2,
		MMO_MinStep=1e-12,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={Ipv[1],ActualPotPanel[1],Vpv[1],DCboost[1],VpvRef[1],VpvFiltrada[1],deltaPot[1]},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=5,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end sisthibgen1;
