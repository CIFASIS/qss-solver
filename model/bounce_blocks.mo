model bounce_blocks

import retQSS;

constant Integer AIR = 0, ROCK = 1, N_VOLS = 240, N = 5; 

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;
discrete Integer volumeID, vol_type;
discrete Real nx, ny, nz, dot;


initial algorithm 
    for i in 1:N loop
            x[i]  := random(-40,40);
            y[i]  := random(-80,20);
            z[i]  := 0;

            vx[i] := random(0,10);
            vy[i] := random(0,10);
            vz[i] := random(0,20);
    end for;

    status := retQSS_setUp(N, "geom/cube_240.vtk", "bounce_blocks");

    if status == false then
        status := retQSS_fail();
    end if;

    vol_type := volume_setProperty(0, "type", ROCK);
    for id in 1:N_VOLS loop
        vol_type := volume_setProperty(id, "type", AIR);
        if  modulus(id,17) == 0 then
            vol_type := volume_setProperty(id, "type", ROCK);
        end if;
    end for;

    for i in 1:N loop
        volumeID := particle_currentVolumeID(i);
    end for;

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
            volumeID := particle_currentVolumeID(i);
            vol_type := volume_getProperty(volumeID, "type");

            if vol_type == ROCK then
                (nx, ny, nz) := particle_crossedFaceNormal(i);

                dot := vx[i]*nx + vy[i]*ny + vz[i]*nz; 

                reinit(vx[i], vx[i] - 2*dot*nx);
                reinit(vy[i], vy[i] - 2*dot*ny);
                reinit(vz[i], vz[i] - 2*dot*nz);
            end if; 
        end when;
    end for;

annotation(
    experiment(
        MMO_Description="Particles bouncing against certain blocks",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=100,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end bounce_blocks;
