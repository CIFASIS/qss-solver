model myBurgers
	constant Integer N = 1000;
	parameter Real beta=100,x0=0.5,pi=3.14159,x[N],inVal=1,outVal=1;
	discrete Real ap[N](start=0);
	Real u[N];
	initial algorithm
	for i in 1:N loop
		x[i]:=1.0*i/N;
	//	u[i]:=sin(pi*2*x[i])+2; //Condición inicial que mantiene u>0 en todo el dominio
		u[i]:=sin(pi*2*x[i]); //Condición inicial con u<0	 //Sin la suma convexa, el esquema falla.
      if u[i] > 0 then
          ap[i]:=1;    
      else
      ap[i]:=0;
    end if;
	end for;
		
	equation
//Eq. Burgers invíscida: u_t+(1/2)*(u^2)_x=0
//Término viscoso: eps*uxx (término fuente a la derecha)
//Forma cuasi-lineal: u_t+u*u_x=0		(lamb=u)
//La velocidad de propagación es igual a la variable.
//Upwind:
//		  der(u[1])=(u[1]+u[N])/2*(-u[1]+u[N])*N;	//Condición periódica para u>0
		der(u[1])=ap[1]*(u[1]+u[N])/2*(-u[1]+u[N])*N+(1-ap[1])*(u[2]+u[1])/2*(-u[2]+u[1])*N;	//No puedo hacer andar esta suma convexa.
		for j in 2:N-1 loop
	//	  der(u[j])=(u[j]+u[j-1])/2*(-u[j]+u[j-1])*N;
		  der(u[j])=ap[j]*(u[j]+u[j-1])/2*(-u[j]+u[j-1])*N+(1-ap[j])*(u[j+1]+u[j])*(-u[j+1]+u[j])*N; //No puedo hacer andar esta suma convexa.
		end for;
		  der(u[N])=ap[N]*(u[N]+u[N-1])/2*(-u[N]+u[N-1])*N+(1-ap[N])*(u[1]+u[N])*(-u[1]+u[N])*N; //No puedo hacer andar esta suma convexa.
algorithm
  for i in 1:N loop
      when u[i] > 0 then
          ap[i]:=1;    elsewhen u[i] < 0 then
      ap[i]:=0;
    end when;
  end for;

//Lax-Wendroff: esquema 2 orden: Problema para implementarlo: necesito afectar el avance temporal con elementos de la discretización espacial
	annotation(

	experiment(
		MMO_Description="Burgers Equation",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={u[1:N/5:N]},
		MMO_OutputType=CI_Sampled,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=1.0,
		Tolerance={1e-5},
		AbsTolerance={1e-5}
	));
end myBurgers;
