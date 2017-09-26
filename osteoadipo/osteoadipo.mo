model osteoadipo
	import math;
	// BioRica Constant cytoplasm
	parameter Real cytoplasm = 1.0;
	// BioRica Constant nucleus
	parameter Real nucleus = 1.0;
	// BioRica Constant k1
	parameter Real k1 = 0.182;
	// BioRica Constant k2
	parameter Real k2 = 0.0182;
	// BioRica Constant k3
	parameter Real k3 = 0.05;
	// BioRica Constant k4
	parameter Real k4 = 0.267;
	// BioRica Constant k5
	parameter Real k5 = 0.133;
	// BioRica Constant k_plus6
	parameter Real k_plus6 = 0.0909;
	// BioRica Constant k_minus6
	parameter Real k_minus6 = 0.909;
	// BioRica Constant k_plus7
	parameter Real k_plus7 = 1.0;
	// BioRica Constant k_minus7
	parameter Real k_minus7 = 50.0;
	// BioRica Constant k_plus8
	parameter Real k_plus8 = 1.0;
	// BioRica Constant k_minus8
	parameter Real k_minus8 = 120.0;
	// BioRica Constant k9
	parameter Real k9 = 206.0;
	// BioRica Constant k10
	parameter Real k10 = 206.0;
	// BioRica Constant k11
	parameter Real k11 = 0.417;
	// BioRica Constant V12
	parameter Real V12 = 0.423;
	// BioRica Constant k13
	parameter Real k13 = 0.000257;
	// BioRica Constant k14
	parameter Real k14 = 0.0000822;
	// BioRica Constant k21
	parameter Real k21 = 0.000001;
	// BioRica Constant k15
	parameter Real k15 = 0.167;
	// BioRica Constant k_plus16
	parameter Real k_plus16 = 1.0;
	// BioRica Constant k_minus16
	parameter Real k_minus16 = 30.0;
	// BioRica Constant k_plus17
	parameter Real k_plus17 = 1.0;
	// BioRica Constant k_minus17
	parameter Real k_minus17 = 1200.0;
	// BioRica Constant Vmax1
	parameter Real Vmax1 = 150.0;
	// BioRica Constant Km1
	parameter Real Km1 = 10.0;
	// BioRica Constant Ki
	parameter Real Ki = 9.0;
	// BioRica Constant Vmax2
	parameter Real Vmax2 = 15.0;
	// BioRica Constant Km2
	parameter Real Km2 = 8.0;
	// BioRica Constant kcat1
	parameter Real kcat1 = 1.5;
	// BioRica Constant Km3
	parameter Real Km3 = 15.0;
	// BioRica Constant Vmax3
	parameter Real Vmax3 = 45.0;
	// BioRica Constant Km4
	parameter Real Km4 = 15.0;
	// BioRica Constant kcat2
	parameter Real kcat2 = 1.5;
	// BioRica Constant Km5
	parameter Real Km5 = 15.0;
	// BioRica Constant Vmax4
	parameter Real Vmax4 = 45.0;
	// BioRica Constant Km6
	parameter Real Km6 = 15.0;
	// BioRica Constant kcat3
	parameter Real kcat3 = 1.5;
	// BioRica Constant Km7
	parameter Real Km7 = 15.0;
	// BioRica Constant Vmax5
	parameter Real Vmax5 = 45.0;
	// BioRica Constant Km8
	parameter Real Km8 = 15.0;
	// BioRica Constant kcat4
	parameter Real kcat4 = 1.5;
	// BioRica Constant Km9
	parameter Real Km9 = 9.0;
	// BioRica Constant k18
	parameter Real k18 = 0.15;
	// BioRica Constant k19
	parameter Real k19 = 39.0;
	// BioRica Constant Vmax6
	parameter Real Vmax6 = 45.0;
	// BioRica Constant Km10
	parameter Real Km10 = 12.0;
	// BioRica Constant kcat5
	parameter Real kcat5 = 0.6;
	// BioRica Constant n1
	parameter Real n1 = 2.0;
	// BioRica Constant Km11
	parameter Real Km11 = 15.0;
	// BioRica Constant k20
	parameter Real k20 = 0.015;
	// BioRica Constant kcat6
	parameter Real kcat6 = 1.5;
	// BioRica Constant Km12
	parameter Real Km12 = 15.0;
	// BioRica Constant kcat7
	parameter Real kcat7 = 1.5;
	// BioRica Constant Km13
	parameter Real Km13 = 15.0;
	// BioRica Constant Vmax7
	parameter Real Vmax7 = 45.0;
	// BioRica Constant Km14
	parameter Real Km14 = 15.0;
	// BioRica Constant normal_bCatenin_TCF
	parameter Real normal_bCatenin_TCF = 8.81;
	// BioRica Constant n
	parameter Real n = 2;
	// BioRica Constant ap
	parameter Real ap = 0.2;
	// BioRica Constant bp
	parameter Real bp = 0.5;
	// BioRica Constant mp
	parameter Real mp = 10;
	// BioRica Constant cpp
	parameter Real cpp = 0.1;
	// BioRica Constant kp
	parameter Real kp = 0.1;
	// BioRica Constant ao
	parameter Real ao = 0.1;
	// BioRica Constant aa
	parameter Real aa = 0.1;
	// BioRica Constant bo
	parameter Real bo = 1;
	// BioRica Constant ba
	parameter Real ba = 1;
	// BioRica Constant mo
	parameter Real mo = 1;
	// BioRica Constant ma
	parameter Real ma = 1;
	// BioRica Constant coo
	parameter Real coo = 0.1;
	// BioRica Constant caa
	parameter Real caa = 0.1;
	// BioRica Constant coa
	parameter Real coa = 0.1;
	// BioRica Constant cao
	parameter Real cao = 0.1;
	// BioRica Constant cop
	parameter Real cop = 0.5;
	// BioRica Constant cap
	parameter Real cap = 0.5;
	// BioRica Constant ko
	parameter Real ko = 0.1;
	// BioRica Constant ka
	parameter Real ka = 0.1;
	Real W_X17(start = 0.0);
	Real W_R20;
	Real W_X2(start = 0.0);
	Real W_R14;
	Real W_X20(start = 296.1137);
	Real W_X8(start = 0.0020);
	Real D_xa(start = 0);
	Real W_R30;
	Real W_X6(start = 0.0015);
	Real W_X27(start = 10.263);
	Real W_R26;
	Real W_X15(start = 3.4392);
	Real W_R7;
	Real W_X19(start = 6.486);
	Real W_R24;
	Real W_R8;
	Real W_X23(start = 2.1103);
	Real W_R10;
	Real W_R9;
	discrete Real W_ACTIVE;
	Real W_R22;
	Real W_X4(start = 0.0076);
	Real W_R16;
	Real W_bCatenin_TCF(start = 8.8121);
	Real W_R1;
	Real S_za;
	Real W_R18;
	Real W_R3;
	Real W_X28(start = 0.85544);
	Real W_X5(start = 49.1372);
	Real W_X12(start = 8.0e-4);
	Real W_R5;
	Real W_X9(start = 0.0020);
	Real W_R31;
	Real D_za;
	Real D_xo(start = 0);
	Real W_bCatenin(start = 42.7224);
	Real D_zo;
	Real W_R29;
	Real W_R15;
	Real W_R21;
	Real S_zo;
	Real W_X24(start = 180.9595);
	Real W_R19;
	Real W_R13;
	Real W_R6;
	Real W_R33;
	Real W_R11;
	Real W_R25;
	discrete Real INF(start = 1e20);
	discrete Real S__zd(start = 0);
	Real W_R17;
	discrete Real W_W(start = 0);
	Real W_R23;
	Real W_X22(start = 297.8897);
	Real W_X3(start = 0.0153);
	Real D_zd;
	Real W_X10(start = 0.9881);
	Real W_X7(start = 96.6019);
	Real W_X13X14;
	discrete Real event_to_adipo;
	discrete Real event_to_diff;
	Real W_R2;
	Real W_R4;
	Real W_X13(start = 6.1879);
	Real W_X25(start = 418.1788);
	Real W_R32;
	Real W_R27;
	Real W_zo;
	Real D_xp(start = 12);
	Real W_X16(start = 200.0);
	discrete Real S__za(start = 0);
	Real W_X18(start = 112.5585);
	Real S_zd;
	Real W_R28;
	Real W_X26(start = 0.8619);
	Real W_X21(start = 3.8863);
	Real W_X1(start = 100.0);
	Real R12;
	initial algorithm
	
//		event_to_diff := exponential(0.01);
	//	event_to_adipo := exponential(0.001);
	event_to_diff := 0.01;
		event_to_adipo := 0.001;
		
equation
	
		S_zd = S__zd;
		D_zd = S_zd;
		W_zo = W_ACTIVE;
		S_zo = W_zo;
		D_zo = S_zo;
		S_za = S__za;
		D_za = S_za;
		// BioRica Formula R1 instance: W_
		W_R1 = cytoplasm*k1*W_X1*W_W;
		// BioRica Formula R18 instance: W_
		W_R18 = cytoplasm*Vmax1*W_X16*W_W*1.0/(Km1+W_X16)*Ki*1.0/(Ki+W_X23);
		// BioRica Formula R2 instance: W_
		W_R2 = cytoplasm*k2*W_X2;
		// BioRica Formula R3 instance: W_
		W_R3 = cytoplasm*k3*W_X2*W_X4;
		// BioRica Formula R5 instance: W_
		W_R5 = cytoplasm*k5*W_X3;
		// BioRica Formula R8 instance: W_
		W_R8 = cytoplasm*(k_plus8*W_X3*W_bCatenin-k_minus8*W_X8);
		// BioRica Formula R4 instance: W_
		W_R4 = cytoplasm*k4*W_X4;
		// BioRica Formula R6 instance: W_
		W_R6 = cytoplasm*(k_plus6*W_X5*W_X6-k_minus6*W_X4);
		// BioRica Formula R32 instance: W_
		W_R32 = cytoplasm*kcat7*W_X23*W_X5*1.0/(Km13+W_X5);
		// BioRica Formula R7 instance: W_
		W_R7 = cytoplasm*(k_plus7*W_X7*W_X12-k_minus7*W_X6);
		// BioRica Formula R17 instance: W_
		W_R17 = cytoplasm*(k_plus17*W_X7*W_bCatenin-k_minus17*W_X15);
		// BioRica Formula R9 instance: W_
		W_R9 = cytoplasm*k9*W_X8;
		// BioRica Formula R10 instance: W_
		W_R10 = cytoplasm*k10*W_X9;
		// BioRica Formula R11 instance: W_
		W_R11 = cytoplasm*k11*W_X10;
		// BioRica Formula R13 instance: W_
		W_R13 = nucleus*k13*W_bCatenin;
		// BioRica Formula R14 instance: W_
		W_R14 = nucleus*(k14+k21*(W_bCatenin+W_bCatenin_TCF));
		// BioRica Formula R16 instance: W_
		W_R16 = nucleus*(k_plus16*W_bCatenin*W_X13-k_minus16*W_bCatenin_TCF);
		// BioRica Formula R15 instance: W_
		W_R15 = cytoplasm*k15*W_X12;
		// BioRica Formula R29 instance: W_
		W_R29 = cytoplasm*kcat5*pow(W_bCatenin_TCF,(n1))*1.0/(pow(Km11,(n1))+pow(W_bCatenin_TCF,(n1)));
		// BioRica Formula R19 instance: W_
		W_R19 = cytoplasm*Vmax2*W_X17*1.0/(Km2+W_X17);
		// BioRica Formula R20 instance: W_
		W_R20 = cytoplasm*kcat1*W_X17*W_X18*1.0/(Km3+W_X18);
		// BioRica Formula R27 instance: W_
		W_R27 = cytoplasm*(k18*W_X18*W_X25-k19*W_X24);
		// BioRica Formula R31 instance: W_
		W_R31 = cytoplasm*kcat6*W_X27*W_X18*1.0/(Km12+W_X18);
		// BioRica Formula R21 instance: W_
		W_R21 = cytoplasm*Vmax3*W_X19*1.0/(Km4+W_X19);
		// BioRica Formula R22 instance: W_
		W_R22 = cytoplasm*kcat2*W_X19*W_X20*1.0/(Km5+W_X20);
		// BioRica Formula R23 instance: W_
		W_R23 = cytoplasm*Vmax4*W_X21*1.0/(Km6+W_X21);
		// BioRica Formula R24 instance: W_
		W_R24 = cytoplasm*kcat3*W_X21*W_X22*1.0/(Km7+W_X22);
		// BioRica Formula R25 instance: W_
		W_R25 = cytoplasm*Vmax5*W_X23*1.0/(Km8+W_X23);
		// BioRica Formula R26 instance: W_
		W_R26 = cytoplasm*kcat4*W_X23*W_X24*1.0/(Km9+W_X24);
		// BioRica Formula R28 instance: W_
		W_R28 = cytoplasm*Vmax6*W_X26*1.0/(Km10+W_X26);
		// BioRica Formula R30 instance: W_
		W_R30 = cytoplasm*k20*W_X27;
		// BioRica Formula R33 instance: W_
		W_R33 = cytoplasm*Vmax7*W_X28*1.0/(Km14+W_X28);
		W_X13X14 = W_X13+W_bCatenin_TCF;
		R12 = cytoplasm*V12;
		der(W_X1) = (1*1.0/(cytoplasm))*((-1.0*W_R1)+(1.0*W_R2));
		der(W_X2) = (1*1.0/(cytoplasm))*((1.0*W_R1)+(-1.0*W_R2));
		der(W_X3) = (1*1.0/(cytoplasm))*((1.0*W_R4)+(-1.0*W_R5)+(-1.0*W_R8)+(1.0*W_R10));
		der(W_X4) = (1*1.0/(cytoplasm))*((-1.0*W_R3)+(-1.0*W_R4)+(1.0*W_R5)+(1.0*W_R6));
		der(W_X5) = (1*1.0/(cytoplasm))*((1.0*W_R3)+(-1.0*W_R6)+(-1.0*W_R32)+(1.0*W_R33));
		der(W_X6) = (1*1.0/(cytoplasm))*((1.0*W_R3)+(-1.0*W_R6)+(1.0*W_R7));
		der(W_X7) = (1*1.0/(cytoplasm))*((-1.0*W_R7)+(-1.0*W_R17));
		der(W_X8) = (1*1.0/(cytoplasm))*((1.0*W_R8)+(-1.0*W_R9));
		der(W_X9) = (1*1.0/(cytoplasm))*((1.0*W_R9)+(-1.0*W_R10));
		der(W_X10) = (1*1.0/(cytoplasm))*((1.0*W_R10)+(-1.0*W_R11));
		der(W_bCatenin) = (1*1.0/(nucleus))*((-1.0*W_R8)+(1.0*R12)+(-1.0*W_R13)+(-1.0*W_R16)+(-1.0*W_R17));
		der(W_X12) = (1*1.0/(cytoplasm))*((-1.0*W_R7)+(1.0*W_R14)+(-1.0*W_R15));
		der(W_X13) = (1*1.0/(nucleus))*((-1.0*W_R16));
		der(W_bCatenin_TCF) = (1*1.0/(nucleus))*((1.0*W_R16));
		der(W_X15) = (1*1.0/(cytoplasm))*((1.0*W_R17));
		der(W_X16) = (1*1.0/(cytoplasm))*((-1.0*W_R18)+(1.0*W_R19));
		der(W_X17) = (1*1.0/(cytoplasm))*((1.0*W_R18)+(-1.0*W_R19));
		der(W_X18) = (1*1.0/(cytoplasm))*((-1.0*W_R20)+(1.0*W_R21)+(1.0*W_R26)+(-1.0*W_R27)+(-1.0*W_R31));
		der(W_X19) = (1*1.0/(cytoplasm))*((1.0*W_R20)+(-1.0*W_R21)+(1.0*W_R31));
		der(W_X20) = (1*1.0/(cytoplasm))*((-1.0*W_R22)+(1.0*W_R23));
		der(W_X21) = (1*1.0/(cytoplasm))*((1.0*W_R22)+(-1.0*W_R23));
		der(W_X22) = (1*1.0/(cytoplasm))*((-1.0*W_R24)+(1.0*W_R25));
		der(W_X23) = (1*1.0/(cytoplasm))*((1.0*W_R24)+(-1.0*W_R25));
		der(W_X24) = (1*1.0/(cytoplasm))*((-1.0*W_R26)+(1.0*W_R27));
		der(W_X25) = (1*1.0/(cytoplasm))*((-1.0*W_R27)+(1.0*W_R28));
		der(W_X26) = (1*1.0/(cytoplasm))*((1.0*W_R26)+(-1.0*W_R28));
		der(W_X27) = (1*1.0/(cytoplasm))*((1.0*W_R29)+(-1.0*W_R30));
		der(W_X28) = (1*1.0/(cytoplasm))*((1.0*W_R32)+(-1.0*W_R33));
		der(D_xp) = (ap*pow(D_xp,n)+bp)*1.0/(mp+D_zd+cpp*pow(D_xp,n))-kp*D_xp;
		der(D_xo) = (ao*pow(D_xo,n)+bo+D_zo)*1.0/(mo+coo*pow(D_xo,n)+coa*pow(D_xa,n)+cop*pow(D_xp,n))-ko*D_xo;
		der(D_xa) = (aa*pow(D_xa,n)+ba+D_za)*1.0/(ma+caa*pow(D_xa,n)+cao*pow(D_xo,n)+cap*pow(D_xp,n))-ka*D_xa;
	algorithm
	
		when time > event_to_diff then
		if S__zd == 0 then
			S__zd := 	1;
		end if;
		if S__zd == 0 then
		//event_to_diff := time+exponential(0.01);
event_to_diff := time+0.01;
		else
		event_to_diff := INF;
		 end if;
		end when;
		
	when time > event_to_adipo  then
		if S__za == 0 then
			S__za := 	0.8;
		end if;
		if S__za == 0 then
		//event_to_adipo := time+exponential(0.001);
 event_to_adipo := time+0.001;
		else
		event_to_adipo := INF;
		 end if;
		end when;
		
		when (W_bCatenin_TCF >= 1.1*normal_bCatenin_TCF) then
			W_ACTIVE := 	0.8;
		elsewhen (W_bCatenin_TCF < 1.1*normal_bCatenin_TCF) then
			W_ACTIVE := 	0.0;
		end when;
		
		when (W_W >= 0) then
		if W_W == 0 and time >= 500 and time <= 1000 then
			W_W := 	1;
		end if;
		end when;
		
		when (W_W < 0) then
		if W_W == 0 and time >= 500 and time <= 1000 then
			W_W := 	1;
		end if;
		end when;
		
		when (time >= 500) then
		if W_W == 0 and time >= 500 and time <= 1000 then
			W_W := 	1;
		end if;
		end when;
		
		when (time <= 1000) then
		if W_W == 0 and time >= 500 and time <= 1000 then
			W_W := 	1;
		end if;
		end when;
		
		when (W_W >= 1) then
		if W_W == 1 and time > 1000 then
			W_W := 	0;
		end if;
		end when;
		
		when (W_W < 1) then
		if W_W == 1 and time > 1000 then
			W_W := 	0;
		end if;
		end when;
		
		when (time > 1000) then
		if W_W == 1 and time > 1000 then
			W_W := 	0;
		end if;
		end when;
		
	annotation(

	experiment(
		MMO_Description="osteoadipo automatic model translation.",
		MMO_Solver=LIQSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Output={S__zd,S__za,W_W,W_X1,W_X2,W_X3,W_X4,W_X5,W_X6,W_X7,W_X8,W_X9,W_X10,W_bCatenin,W_X12,W_X13,W_bCatenin_TCF,W_X15,W_X16,W_X17,W_X18,W_X19,W_X20,W_X21,W_X22,W_X23,W_X24,W_X25,W_X26,W_X27,W_X28,D_xp,D_xo,D_xa},
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=2500,
		Tolerance={1e-3},
		AbsTolerance={1e-3}
	));
end osteoadipo;
