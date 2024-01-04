model pic
    import retQSS;

    constant Integer NX = 16, NY = 10, N_CELLS = NX*NY, N_PART = 27000;
    Real x[N_PART], y[N_PART], z[N_PART], vx[N_PART], vy[N_PART], vz[N_PART];

    parameter Real Q = 1.602e-19;
    parameter Real M = 32*1.661e-27;
    parameter Real EPS0 = 8.854e-12;
    parameter Real n0 = 1e12;
    parameter Real phi0 = 0;
    parameter Real Te = 1;
    parameter Real Ti = 0.1;
    parameter Real lD = sqrt(EPS0*Te/(n0*Q));
    parameter Real dx = lD;
    parameter Real dx2 = dx*dx;
    parameter Real Ly = (NY-1)*dx;
    parameter Real vth = sqrt(2*Q*Ti/M);
    parameter Real v_drift = 7000;
    parameter Real dt = 0.1*dx/v_drift;
    parameter Real np = 15;
    parameter Real flux = n0*v_drift*Ly;
    parameter Real npt = flux*dt;
    parameter Real np_insert = (NY-1)*np;
    parameter Real spwt = npt/np_insert;

    parameter Real Zero = 0;
    parameter Boolean status;

    discrete Integer volumeID[N_PART];
    discrete Integer Ex[N_PART], Ey[N_PART];
    discrete Real EX[N_CELLS], EY[N_CELLS], RHO[N_CELLS], PHI[N_CELLS];
    discrete Integer neighbors, solved;
    discrete Real phi, phi1, phi2, phi3, phi4;
    discrete Real rho, b;
    discrete Real E1, E2;
    discrete Real cx, cy;
    discrete Real next_t;
    discrete Real start_time[N_PART];

    initial algorithm
        next_t := 1e-7;

        for i in 1:N_PART loop
            Ex[i] := 0;
            Ey[i] := 0;

            if i <= np_insert then
                x[i] := random(0, dx);
                y[i] := random(0, (NY-1)*dx);
                z[i] := 0.5;
                vx[i] := v_drift + (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth;
                vy[i] := (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth*0.5;
                vz[i] := 0;
            else
                start_time[i] := div(i-1,np_insert)*dt;
            end if;
        end for;

        status := retQSSInit(N_PART, "geom/pic.vtk", "pic");

        if status == false then
            status := fail();
        end if;

        for i in 1:N_CELLS loop
            phi1 := setVolumeProperty(i, "phi", 0);
            rho := setVolumeProperty(i, "rho", 0);
            E1 := setVolumeProperty(i, "Ex", 0);
            E1 := setVolumeProperty(i, "Ey", 0);
        end for;

    equation
        for i in 1:N_PART loop
            der(x[i]) = vx[i];
            der(y[i]) = vy[i];
            der(z[i]) = vz[i];
            der(vx[i]) = Q*Ex[i]/M;
            der(vy[i]) = Q*Ey[i]/M;
            der(vz[i]) = 0;
        end for;

    algorithm

        for i in 1:N_PART loop
            when time > nextCrossingTimeOf(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
                volumeID[i] := currentVolumeIDOf(i);

                if volumeID[i] == 0 then
                    reinit(vx[i], 0);
                    reinit(vy[i], 0);
                    reinit(vz[i], 0);
                else
                    neighbors := getNeighborCount(volumeID[i]);
                    
                    rho := spwt * (particlesInsideVolume(volumeID[i]) / getVolumeCapacity(volumeID[i]));
                    phi := getVolumeProperty(volumeID[i], "phi");
                
                    if neighbors == 5 then
                        phi1 := getVolumeProperty(volumeID[i]-NY, "phi");
                        phi2 := getVolumeProperty(volumeID[i]+1, "phi");
                        phi3 := getVolumeProperty(volumeID[i]+NY, "phi");
                        phi4 := getVolumeProperty(volumeID[i]-1, "phi");

                        b   := -Q/EPS0 * (rho - n0*exp((phi-phi0)/Te));
                        phi := -dx2/4 * b + (phi4 + phi2 + phi3 + phi1)/4;

                        E1    := getVolumeProperty(volumeID[i]-NY, "phi");
                        E2    := getVolumeProperty(volumeID[i]+NY, "phi");
                        Ex[i] := (E1-E2) / (2*dx);

                        E1    := getVolumeProperty(volumeID[i]+1, "phi");
                        E2    := getVolumeProperty(volumeID[i]-1, "phi");
                        Ey[i] := (E1-E2) / (2*dx);
                    else
                        phi := 0;
                        Ex[i] := 0;
                        Ey[i] := 0;
                    end if;

                    phi := setVolumeProperty(volumeID[i], "phi", phi);
                    rho := setVolumeProperty(volumeID[i], "rho", rho);
                    Ex[i] := setVolumeProperty(volumeID[i], "Ex", Ex[i]);
                    Ey[i] := setVolumeProperty(volumeID[i], "Ey", Ey[i]);
                end if;
            end when;

            when time >= start_time[i] then
                reinit(x[i], random(0, dx));
                reinit(y[i], random(0, (NY-1)*dx));
                reinit(z[i], 0.5);

                reinit(vx[i], v_drift + (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth);
                reinit(vy[i], (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth*0.5);
                reinit(vz[i], 0);

                volumeID[i] := relocateParticle(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]);
            end when;
        end for;

        when time > next_t then
            for i in 1:N_CELLS loop
                PHI[i] := getVolumeProperty(i, "phi");
                RHO[i] := getVolumeProperty(i, "rho");
                EX[i] := getVolumeProperty(i, "Ex");
                EY[i] := getVolumeProperty(i, "Ey");
            end for;

            next_t := time + 1e-7;
            if next_t > 2*start_time[N_PART] then
                next_t := 1e20;
            end if;
        end when;

	annotation(

	experiment(
		MMO_Description="Simple PIC model",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Period={5e-3},
		MMO_LPS=0,
		MMO_DT_Min=2,
		MMO_Output={PHI,RHO,EX,EY},
		MMO_OutputType=CI_Step,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=2.12e-5,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));
end pic;
