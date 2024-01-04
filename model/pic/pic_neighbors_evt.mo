model pic_neighbors_evt
    import retQSS;
    import retQSS_PIC;

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

    discrete Boolean status;

    discrete Real rho_neigh1[N_PART], rho_neigh2[N_PART], rho_neigh3[N_PART], rho_neigh4[N_PART], rho_neigh5[N_PART], evt_time[N_PART];
    discrete Integer volumeID[N_PART];
    discrete Real Ex[N_PART], Ey[N_PART];
    discrete Real RHO[N_CELLS], PHI[N_CELLS];
    discrete Integer neighbors, solved;
    discrete Real phi, phi1, phi2, phi3, phi4;
    discrete Real rho, b, cap;
    discrete Real E1, E2;
    discrete Real cx, cy;
    discrete Real next_t, system_t;
    discrete Real start_time[N_PART];

    discrete Integer neighborID1, neighborID2, neighborID3, neighborID4;
    discrete Real d, d1, d2, d3, d4;
    discrete Real q1, q2, q3, q4;
    discrete Real c1, c2, c3, c4;

    parameter Real rq = 0.5;

    initial algorithm
        next_t := 1e-7;
        system_t := dt;

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

            rho_neigh1[i] := 0;
            rho_neigh2[i] := 0;
            rho_neigh3[i] := 0;
            rho_neigh4[i] := 0;
            rho_neigh5[i] := 0;

            evt_time[i] := 1e20;
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
                evt_time[i] := time + (nextCrossingTimeOf(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) - time) / 2;

                if volumeID[i] == 0 then
                    reinit(vx[i], 0);
                    reinit(vy[i], 0);
                    reinit(vz[i], 0);
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

            when time >= evt_time[i] then
                volumeID[i] := previousVolumeIDOf(i);
                neighbors := getNeighborCount(volumeID[i]);

                if neighbors == 5 then
                    rho := getVolumeProperty(volumeID[i], "rho");
                    rho := setVolumeProperty(volumeID[i], "rho", rho - rho_neigh5[i]);

                    neighborID1 := getNeighborID(volumeID[i], 1);
                    rho := getVolumeProperty(neighborID1, "rho");
                    rho := setVolumeProperty(neighborID1, "rho", rho - rho_neigh1[i]);

                    neighborID2 := getNeighborID(volumeID[i], 2);
                    rho := getVolumeProperty(neighborID2, "rho");
                    rho := setVolumeProperty(neighborID2, "rho", rho - rho_neigh2[i]);

                    neighborID3 := getNeighborID(volumeID[i], 3);
                    rho := getVolumeProperty(neighborID3, "rho");
                    rho := setVolumeProperty(neighborID3, "rho", rho - rho_neigh3[i]);

                    neighborID4 := getNeighborID(volumeID[i], 4);
                    rho := getVolumeProperty(neighborID4, "rho");
                    rho := setVolumeProperty(neighborID4, "rho", rho - rho_neigh4[i]);
                end if;

                d := 0;

                volumeID[i] := currentVolumeIDOf(i);
                neighbors := getNeighborCount(volumeID[i]);
                evt_time[i] := 1e20;

                if volumeID[i] > 0 and neighbors == 5 then
                    neighborID1 := getNeighborID(volumeID[i], 1);
                    d1 := distanceToVolume(neighborID1, x[i], y[i], z[i]);
                    d := d + d1;

                    neighborID2 := getNeighborID(volumeID[i], 2);
                    d2 := distanceToVolume(neighborID2, x[i], y[i], z[i]);
                    d := d + d2;

                    neighborID3 := getNeighborID(volumeID[i], 3);
                    d3 := distanceToVolume(neighborID3, x[i], y[i], z[i]);
                    d := d + d3;

                    neighborID4 := getNeighborID(volumeID[i], 4);
                    d4 := distanceToVolume(neighborID4, x[i], y[i], z[i]);
                    d := d + d4;

                    c1 := (dx - d1) / d;
                    c2 := (dx - d2) / d;
                    c3 := (dx - d3) / d;
                    c4 := (dx - d4) / d;

                    q1 := (1-rq) * c1;
                    q2 := (1-rq) * c2;
                    q3 := (1-rq) * c3;
                    q4 := (1-rq) * c4;

                    rho := getVolumeProperty(neighborID1, "rho");
                    cap := getVolumeCapacity(neighborID1);
                    rho_neigh1[i] := spwt * (q1/cap);
                    rho := setVolumeProperty(neighborID1, "rho", rho + rho_neigh1[i]);

                    rho := getVolumeProperty(neighborID2, "rho");
                    cap := getVolumeCapacity(neighborID2);
                    rho_neigh2[i] := spwt * (q2/cap);
                    rho := setVolumeProperty(neighborID2, "rho", rho + rho_neigh2[i]);

                    rho := getVolumeProperty(neighborID3, "rho");
                    cap := getVolumeCapacity(neighborID3);
                    rho_neigh3[i] := spwt * (q3/cap);
                    rho := setVolumeProperty(neighborID3, "rho", rho + rho_neigh3[i]);

                    rho := getVolumeProperty(neighborID4, "rho");
                    cap := getVolumeCapacity(neighborID4);
                    rho_neigh4[i] := spwt * (q4/cap);
                    rho := setVolumeProperty(neighborID4, "rho", rho + rho_neigh4[i]);

                    rho := getVolumeProperty(volumeID[i], "rho");
                    cap := getVolumeCapacity(volumeID[i]);
                    rho_neigh5[i] := spwt * (rq/cap);
                    rho := setVolumeProperty(volumeID[i], "rho", rho + rho_neigh5[i]);


                    Ex[i] := 0;
                    Ey[i] := 0;

                    Ex[i] := Ex[i] + getVolumeProperty(volumeID[i], "Ex") * rq;
                    Ey[i] := Ey[i] + getVolumeProperty(volumeID[i], "Ey") * rq;

                    Ex[i] := Ex[i] + getVolumeProperty(neighborID1, "Ex") * q1;
                    Ey[i] := Ey[i] + getVolumeProperty(neighborID1, "Ey") * q1;

                    Ex[i] := Ex[i] + getVolumeProperty(neighborID2, "Ex") * q2;
                    Ey[i] := Ey[i] + getVolumeProperty(neighborID2, "Ey") * q2;

                    Ex[i] := Ex[i] + getVolumeProperty(neighborID3, "Ex") * q3;
                    Ey[i] := Ey[i] + getVolumeProperty(neighborID3, "Ey") * q3;

                    Ex[i] := Ex[i] + getVolumeProperty(neighborID4, "Ex") * q4;
                    Ey[i] := Ey[i] + getVolumeProperty(neighborID4, "Ey") * q4;
                end if;
            end when;
        end for;

        when time > system_t then
            system_t := time + dt;
            solved := solvePotential(NY, Q/EPS0, n0, phi0, Te);
            if not solved then
                status := fail();
            end if;
        end when;

        when time > next_t then
            for i in 1:N_CELLS loop
                PHI[i] := getVolumeProperty(i, "phi");
                RHO[i] := getVolumeProperty(i, "rho") + 1e4;
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
		MMO_Output={PHI,RHO},
		MMO_OutputType=CI_Step,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=2.12e-5,
		Tolerance={1e-6},
		AbsTolerance={1e-9}
	));
end pic_neighbors_evt;
