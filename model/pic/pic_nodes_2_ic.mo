model pic_nodes_2

    import retQSS;
    import retQSS_PIC;

    constant Integer MIDPOINT_EVT = 0, ENTRY_EVT = 1, BIRTH_EVT = 2;
    constant Integer NX = 16, NY = 10, N_NODES = NX*NY, N_CELLS = (NX-1)*(NY-1), N_PART = 27000;
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
    parameter Real dt = 0.1*dx/v_drift; // 1.0620389946518801e-07
    parameter Real np = 15;
    parameter Real flux = n0*v_drift*Ly;
    parameter Real npt = flux*dt;
    parameter Real np_insert = (NY-1)*np;
    parameter Real spwt = npt/np_insert;

    discrete Real rho_vertex1[N_PART];
    discrete Real rho_vertex2[N_PART];
    discrete Real rho_vertex3[N_PART];
    discrete Real rho_vertex4[N_PART];
    discrete Integer volumeID[N_PART], prev_volumeID[N_PART];
    discrete Real Ex[N_PART], Ey[N_PART];
    discrete Real start_time[N_PART];
    discrete Real evt_time[N_PART];
    discrete Integer evt_type[N_PART];
    discrete Real RHO[2*N_NODES], PHI[2*N_NODES];

    discrete Boolean status;
    discrete Integer solved;
    discrete Real rho, phi, cap;
    discrete Real E1;
    discrete Real area, x1, x2, y1, y2, _dummy1, _dummy2;
    discrete Real next_t, system_t;

    initial algorithm
        next_t := 1e20;
        system_t := dt;

        status := retQSS_setUpWithInitialConditions(
                    N_PART,
                    "geom/pic_nodes.vtk",
                    "data/particles.txt",
                    "pic_nodes_2_ic");

        if status == false then
            status := retQSS_fail();
        end if;

        for i in 1:N_PART loop
            Ex[i] := 0;
            Ey[i] := 0;

            evt_type[i] := BIRTH_EVT;

            // Initialization of first particle cloud.
            if i <= np_insert then
                evt_time[i] := 0;
            else
                vx[i] := 0;
                vy[i] := 0;
                vz[i] := 0;

                start_time[i] := div(i-1,np_insert)*dt;
                evt_time[i] := 1e20;
            end if;

            rho_vertex1[i] := 0;
            rho_vertex2[i] := 0;
            rho_vertex3[i] := 0;
            rho_vertex4[i] := 0;

        end for;

        for i in 1:2*N_NODES loop
            PHI[i] := -1;
            RHO[i] := -1;            
        end for;

        for i in 1:N_CELLS loop
            // Properties are flagged with -1 in those vertices that belong to
            // the lower plane of the grid, which will not be used in this
            // model.
            phi := volume_setVertexProperty(i, 1, "phi", -1);
            phi := volume_setVertexProperty(i, 2, "phi", -1);
            phi := volume_setVertexProperty(i, 3, "phi", -1);
            phi := volume_setVertexProperty(i, 4, "phi", -1);
            phi := volume_setVertexProperty(i, 5, "phi", 0);
            phi := volume_setVertexProperty(i, 6, "phi", 0);
            phi := volume_setVertexProperty(i, 7, "phi", 0);
            phi := volume_setVertexProperty(i, 8, "phi", 0);

            rho := volume_setVertexProperty(i, 1, "rho", -1);
            rho := volume_setVertexProperty(i, 2, "rho", -1);
            rho := volume_setVertexProperty(i, 3, "rho", -1);
            rho := volume_setVertexProperty(i, 4, "rho", -1);
            rho := volume_setVertexProperty(i, 5, "rho", 0);
            rho := volume_setVertexProperty(i, 6, "rho", 0);
            rho := volume_setVertexProperty(i, 7, "rho", 0);
            rho := volume_setVertexProperty(i, 8, "rho", 0);

            E1 := volume_setVertexProperty(i, 1, "Ex", -1);
            E1 := volume_setVertexProperty(i, 2, "Ex", -1);
            E1 := volume_setVertexProperty(i, 3, "Ex", -1);
            E1 := volume_setVertexProperty(i, 4, "Ex", -1);
            E1 := volume_setVertexProperty(i, 5, "Ex", 0);
            E1 := volume_setVertexProperty(i, 6, "Ex", 0);
            E1 := volume_setVertexProperty(i, 7, "Ex", 0);
            E1 := volume_setVertexProperty(i, 8, "Ex", 0);

            E1 := volume_setVertexProperty(i, 1, "Ey", -1);
            E1 := volume_setVertexProperty(i, 2, "Ey", -1);
            E1 := volume_setVertexProperty(i, 3, "Ey", -1);
            E1 := volume_setVertexProperty(i, 4, "Ey", -1);
            E1 := volume_setVertexProperty(i, 5, "Ey", 0);
            E1 := volume_setVertexProperty(i, 6, "Ey", 0);
            E1 := volume_setVertexProperty(i, 7, "Ey", 0);
            E1 := volume_setVertexProperty(i, 8, "Ey", 0);
        end for;

    equation
        // Equations of motion of particles.
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

            // Event 1: particle enters a new cell.
            // Actions:
            //  * Find approximate time of half of its travel through current
            //    cell.
            //  * If it is exiting the geometry, kill it and remove charge from
            //    previous cell.
            when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
                volumeID[i] := particle_currentVolumeID(i);
                evt_time[i] := time+0.1*dt;
                evt_type[i] := ENTRY_EVT;

                if volumeID[i] == 0 then
                    reinit(vx[i], 0);
                    reinit(vy[i], 0);
                    reinit(vz[i], 0);

                    evt_time[i] := time;
                end if;
            end when;

            // Event 2: particle is halfway through current cell.
            // Actions:
            //  * Remove charge from last event.
            //  * Deposit charge in current cell by computing the areas of the
            //    rectangles determined by current particle position and cell
            //    limits.
            when time >= evt_time[i] then
                if evt_type[i] == ENTRY_EVT then
                    volumeID[i] := particle_previousVolumeID(i);
                else
                    volumeID[i] := particle_currentVolumeID(i);
                end if;

                rho := volume_getVertexProperty(volumeID[i], 5, "rho") - rho_vertex1[i];
                rho := volume_setVertexProperty(volumeID[i], 5, "rho", max2(rho,0));

                rho := volume_getVertexProperty(volumeID[i], 6, "rho") - rho_vertex2[i];
                rho := volume_setVertexProperty(volumeID[i], 6, "rho", max2(rho,0));

                rho := volume_getVertexProperty(volumeID[i], 7, "rho") - rho_vertex3[i];
                rho := volume_setVertexProperty(volumeID[i], 7, "rho", max2(rho,0));

                rho := volume_getVertexProperty(volumeID[i], 8, "rho") - rho_vertex4[i];
                rho := volume_setVertexProperty(volumeID[i], 8, "rho", max2(rho,0));

                Ex[i] := 0;
                Ey[i] := 0;
                volumeID[i] := particle_currentVolumeID(i);

                if evt_type[i] == ENTRY_EVT then
                    evt_time[i] := time + (particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) - time) / 2;
                    evt_type[i] := MIDPOINT_EVT;
                else
                    evt_time[i] := 1e20;
                end if;

                if volumeID[i] > 0 then
                    (x1, y1, _dummy1) := volume_vertexCoordinates(volumeID[i], 5);
                    (x2, _dummy1, _dummy2) := volume_vertexCoordinates(volumeID[i], 6);
                    (_dummy1, y2, _dummy2) := volume_vertexCoordinates(volumeID[i], 7);

                    cap := volume_capacity(volumeID[i]);

                    area := (x2-x[i])*(y2-y[i]) / cap;
                    rho := volume_getVertexProperty(volumeID[i], 5, "rho");
                    rho_vertex1[i] := spwt * (area/cap);
                    rho := volume_setVertexProperty(volumeID[i], 5, "rho", rho + rho_vertex1[i]);
                    Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 5, "Ex") * area;
                    Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 5, "Ey") * area;

                    area := (x[i]-x1)*(y2-y[i]) / cap;
                    rho := volume_getVertexProperty(volumeID[i], 6, "rho");
                    rho_vertex2[i] := spwt * (area/cap);
                    rho := volume_setVertexProperty(volumeID[i], 6, "rho", rho + rho_vertex2[i]);
                    Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 6, "Ex") * area;
                    Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 6, "Ey") * area;

                    area := (x2-x[i])*(y[i]-y1) / cap;
                    rho := volume_getVertexProperty(volumeID[i], 7, "rho");
                    rho_vertex3[i] := spwt * (area/cap);
                    rho := volume_setVertexProperty(volumeID[i], 7, "rho", rho + rho_vertex3[i]);
                    Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 7, "Ex") * area;
                    Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 7, "Ey") * area;

                    area := (x[i]-x1)*(y[i]-y1) / cap;
                    rho := volume_getVertexProperty(volumeID[i], 8, "rho");
                    rho_vertex4[i] := spwt * (area/cap);
                    rho := volume_setVertexProperty(volumeID[i], 8, "rho", rho + rho_vertex4[i]);
                    Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 8, "Ex") * area;
                    Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 8, "Ey") * area;
                end if;
            end when;

            // Event 3: particle creation.
            // Actions:
            //  * TODO.
            when time >= start_time[i] then
                (x1, y1, x2) := particle_initialVelocity(i);

                reinit(vx[i], x1);
                reinit(vy[i], y1);
                reinit(vz[i], x2);

                Ex[i] := 0;
                Ey[i] := 0;

                volumeID[i] := particle_currentVolumeID(i);

                (x1, y1, _dummy1) := volume_vertexCoordinates(volumeID[i], 5);
                (x2, _dummy1, _dummy2) := volume_vertexCoordinates(volumeID[i], 6);
                (_dummy1, y2, _dummy2) := volume_vertexCoordinates(volumeID[i], 7);
                cap := volume_capacity(volumeID[i]);

                area := (x2-x[i])*(y2-y[i]) / cap;
                rho := volume_getVertexProperty(volumeID[i], 5, "rho");
                rho_vertex1[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 5, "rho", rho + rho_vertex1[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 5, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 5, "Ey") * area;

                area := (x[i]-x1)*(y2-y[i]) / cap;
                rho := volume_getVertexProperty(volumeID[i], 6, "rho");
                rho_vertex2[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 6, "rho", rho + rho_vertex2[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 6, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 6, "Ey") * area;

                area := (x2-x[i])*(y[i]-y1) / cap;
                rho := volume_getVertexProperty(volumeID[i], 7, "rho");
                rho_vertex3[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 7, "rho", rho + rho_vertex3[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 7, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 7, "Ey") * area;

                area := (x[i]-x1)*(y[i]-y1) / cap;
                rho := volume_getVertexProperty(volumeID[i], 8, "rho");
                rho_vertex4[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 8, "rho", rho + rho_vertex4[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 8, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 8, "Ey") * area;
            end when;
        end for;

        // Event 4: solving of the linear system that approximates the
        // potential from the charge density on each cell. The electric field
        // is also updated inside the external function called here.
        when time > system_t then
            system_t := time + dt;
            solved := solvePotentialInNodes(NY, Q/EPS0, n0, phi0, Te);
            if not solved then
                status := retQSS_fail();
            end if;
            next_t := time;
        end when;

        // Event 5: sampling of charge density and potential for output
        // purposes.
        when time > next_t then
            for i in 1:2*N_NODES loop
                (_dummy1, _dummy2, x1) := vertex_coordinates(i);
                if x1 > 0 then
                    PHI[i] := vertex_getProperty(i, "phi");
                    RHO[i] := vertex_getProperty(i, "rho") + 1e4;
                end if;
            end for;
            next_t := 1e20;
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
        StopTime=2.13e-5,
        Tolerance={1e-6},
        AbsTolerance={1e-9}
    ));
end pic_nodes_2;
