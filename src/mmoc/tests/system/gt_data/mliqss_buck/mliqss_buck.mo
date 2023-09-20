model mliqss_buck
  import math;

  parameter Real C = 1e-4, L = 1e-4, U = 24, T = 1e-4, dutyCycle = 0.5, ROn = 1e-5, ROff = 1e5;
  discrete Real R(start=10);
  discrete Real DC(start=0.5);
  discrete Real Rd(start=1e5), Rs(start=1e-5), nextT(start=T),lastT,diodeon;
  Real uC,iL,iD,s;
  Real Vd;
  Real Vsw;
 Real dc(start=0.65);   //Borrar
 discrete Real ptedc(start=0.5); 


//parametros del modelo pormediado
    Real K;
    discrete Real Kadj(start=1);


//Parametros del Algoritmo supervisor
  parameter Real alpha=1e-2;  //umbral porcentual para pasar a modelo promediado
  parameter Real sigma=1e-1; //Umbral porcentual para pasar a modelo switcheado
  discrete Real riL(start=3.5); //iL Ripple amplitude
  discrete Real rUc(start=0.6);//uC Ripple amplitude

  parameter Real deltaDC=1e-2;// Variacion absoluta en DC para pasar a modelo switcheado
  discrete Integer modSEL(start=1); 			//1:modelo switcheado 0:modelo promediado
  discrete Boolean toAvEnableI(start=false);
  discrete Boolean toAvEnableU(start=false);
  discrete Boolean toAverageEnabled(start=false);
  discrete Real toSwEnabled(start=0);
  discrete Real lastTaverage(start=0);

  Real meanIL(start=0);
  discrete Real meanILdisc(start=0);
  discrete Real lastMeanILdisc(start=1e9);
  discrete Real lastIL;
  discrete Real meanILdiff;


  Real meanUC(start=0);
  discrete Real meanUCdisc(start=0);
  discrete Real lastMeanUCdisc(start=1e9);
  discrete Real lastUC;
  discrete Real meanUCdiff;

  Real avSwVoltage(start=0);
  discrete Real avSwVoltageDisc(start=0);
  Real avDiVoltage(start=0);
  discrete Real avDiVoltageDisc(start=0);

  discrete Real lastDC;
  discrete Real toSwNextT(start=1e9);


  equation
    der(dc)=ptedc;
    K=(1-dc)/dc;
    iD=modSEL*(iL*Rs-U)/(Rs+Rd);
    s=diodeon*iD+(1-diodeon)*iD*Rd;
    Vd=iD*Rd*modSEL-U/(1+K*Kadj)*(1-modSEL);
    Vsw=modSEL*(iL-iD)*Rs;
    der(uC) = (iL - uC/R)/C;

    der(iL) = (-Vd- uC)/L;


    der(meanIL)=modSEL*(iL-lastIL)/T;
    der(meanUC)=modSEL*(uC-lastUC)/T;

    der(avSwVoltage)=modSEL*Vsw/T;
    der(avDiVoltage)=modSEL*Vd/T;
																			
//maxIL=bandera*iL;

  algorithm
    when time*modSEL > nextT then
      DC:=dc;
      if DC>0.99 then
	DC:=0.99;
      else
	if DC<0.01 then
		DC:=0.01;
	end if;
      end if;
      lastT:=nextT;
      nextT:=nextT+T;
      Rs := ROn;

//Calculo parametros del modelo promediado

//      K:=(1-DC)/DC;

//Comienzan calculos del algoritmo supervisor
//bandera:=0;
//maxILd:=0;
      lastMeanILdisc:=meanILdisc;
      meanILdisc:=meanIL+lastIL;
      meanILdiff:=fabs(meanILdisc-lastMeanILdisc);
      reinit(meanIL,0);
      lastIL:=iL;

      lastMeanUCdisc:=meanUCdisc;
      meanUCdisc:=meanUC+lastUC;
      meanUCdiff:=fabs(meanUCdisc-lastMeanUCdisc);
      reinit(meanUC,0);
      lastUC:=uC;

      avDiVoltageDisc:=avDiVoltage;
      reinit(avDiVoltage,0);
      avSwVoltageDisc:=avSwVoltage;
      reinit(avSwVoltage,0);
      if (time-lastTaverage)>1.5*T then
	Kadj:=(-avSwVoltageDisc / avDiVoltageDisc) / K;
      end if;

      toAvEnableI:=false;
      toAvEnableU:=false;
      toAverageEnabled:=false;
      if meanILdiff<alpha*riL then
                toAvEnableI:=true;
      end if;
      if meanUCdiff< alpha*rUc then
                toAvEnableU:=true;
      end if;
      if toAvEnableU and toAvEnableI  then
                toAverageEnabled:=true;
      end if;
      if toAverageEnabled then
                modSEL:=0;	          //Se selecciona el modelo promediado
                Rs:=ROff;
                reinit(iL,meanILdisc);
                reinit(uC,meanUCdisc);
                lastDC:=DC;
     end if;
    end when;


    when (time - lastT-DC*T)*modSEL>0 then
      Rs := ROff;
    end when;					
    when s>0 then
      Rd:=ROn;
      diodeon:=1;
    elsewhen s<0 then
      Rd := ROff;
      diodeon:=0;
    end when;
//==================================================================================
//Algoritmo para pasar a modelo switcheado
//==================================================================================
    
    when (uC-meanUCdisc)*(1-modSEL)>sigma*rUc then
if time<16 then
	toSwEnabled:=1;
	toSwNextT:=T*(1+floor(time/T));
end if;
    end when; 
    when (meanUCdisc-uC)*(1-modSEL)>sigma*rUc then
if time<16 then
	toSwEnabled:=2;
	toSwNextT:=T*(1+floor(time/T));
end if;
    end when; 


    when (iL-meanILdisc)*(1-modSEL)>sigma*riL then
if time<16 then
	toSwEnabled:=3;
	toSwNextT:=T*(1+floor(time/T));
end if;
    end when; 
    when (meanILdisc-iL)*(1-modSEL)>sigma*riL then
if time<16 then
	toSwEnabled:=4;
	toSwNextT:=T*(1+floor(time/T));
end if;
    end when; 


    when (DC-lastDC)*(1-modSEL)>deltaDC then
if time<16 then
	toSwEnabled:=5;
	toSwNextT:=T*(1+floor(time/T));
end if;
    end when; 
    when (lastDC-DC)*(1-modSEL)>deltaDC then
if time<16 then
	toSwEnabled:=6;
	toSwNextT:=T*(1+floor(time/T));
end if;
    end when; 

    when time>toSwNextT then
	modSEL:=1;
	nextT:=time;
	reinit(iL,(lastIL-meanILdisc)+iL);
	reinit(uC,lastUC-meanUCdisc+uC);       
	lastTaverage:=time;
	toSwEnabled:=0;
    end when;
 
    when time>0 then
       //   DC:=dutyCycle;
	ptedc:=0.5;
	rUc:=0.6;
	riL:=3.5;
    end when;

    when time>0.11 then
	ptedc:=0;
//reinit(dc,0.7);
//	rUc:=0.6;
//	riL:=3.5;
    end when;

    when time>0.15 then
       //   DC:=dutyCycle;
	reinit(dc,0.4);
	rUc:=0.6;
	riL:=3.5;
    end when;


    when time>0.6 then //0.00501235     
	R:=2;
    end when;
//iL,meanILdisc,uC,meanUCdisc,K,Vd,Vsw,avSwVoltageDisc,avDiVoltageDisc,Kadj,modSEL,toSwEnabled,bandera,meanIL,iD,Rs,Rd,modSEL,toAvEnableI,toAvEnableU,maxILd,minILd,bandera,maxIL,DC
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=mLIQSS2,
		MMO_Period={0.0003/10},
		MMO_PartitionMethod=Metis,
		MMO_Output={uC,iL,modSEL},
		Jacobian=Dense,
		MMO_BDF_PDepth=1,
		MMO_BDF_Max_Step=0,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-3},
		AbsTolerance={1e-6}
	));
end mliqss_buck;
