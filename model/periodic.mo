model periodic

import retQSS;

constant Integer N = 25, N_VOLS = 240;

discrete Real
    REFLECTIVE(start=1.),
    PERIODIC(start=2.),
    DEFAULT_SPEED(start=12.);

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status, onBoundary;
discrete Integer volumeID[N], previousVolumeID, volType;
discrete Real dot, ux, uy, uz, px, py, pz;


initial algorithm
    status := geometry_setUp("geom/cube_240.vtk");

    for i in 1:N loop
        volumeID[i] := geometry_randomVolumeID();

        (px, py, pz) := volume_randomPoint(volumeID[i]);
        x[i]  := px;
        y[i]  := py;
        z[i]  := pz;

        (ux, uy, uz) := randomVectorWithNorm(DEFAULT_SPEED);
        vx[i] := ux;
        vy[i] := uy;
        vz[i] := uz;
    end for;

    for i in 1:N_VOLS loop
        onBoundary := volume_isOnBoundary(i);
        if onBoundary and (modulus(i,11) == 0 or modulus(i,31) == 0) then
            status := volume_setProperty(i, "type", PERIODIC);
        else
            status := volume_setProperty(i, "type", REFLECTIVE);
        end if;
    end for;

    status := particle_setUp(N, "periodic");

    if status == false then
       status := retQSS_fail();
    end if;

equation
    for i in 1:N loop
        der(x[i])  = vx[i];
        der(y[i])  = vy[i];
        der(z[i])  = vz[i];
        der(vx[i]) = 0;
        der(vy[i]) = 0;
        der(vz[i]) = 0;
    end for;

algorithm
    for i in 1:N loop
        when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
           volumeID[i] := particle_currentVolumeID(i);

            if volumeID[i] == 0 then
                previousVolumeID := particle_previousVolumeID(i);
                volType := volume_getProperty(previousVolumeID, "type");

                if volType == PERIODIC then
                    (px, py, pz) := particle_oppositePosition(i);
                
                    reinit(x[i], px);
                    reinit(y[i], py);
                    reinit(z[i], pz);
                else
                    (ux, uy, uz) := particle_crossedFaceNormal(i);

                    dot := vx[i]*ux + vy[i]*uy + vz[i]*uz;

                    (px, py, pz) := vectorWithNorm(
                                        vx[i]-2*dot*ux,
                                        vy[i]-2*dot*uy,
                                        vz[i]-2*dot*uz,
                                        DEFAULT_SPEED);

                    reinit(vx[i], px);
                    reinit(vy[i], py);
                    reinit(vz[i], pz);
                end if;
            end if;
        end when;
    end for;

annotation(
    experiment(
        MMO_Description="Single particle in geometry with periodic boundaries",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z,vx,vy,vz,volumeID},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=100,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end periodic;
