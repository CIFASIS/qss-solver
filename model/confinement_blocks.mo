model confinement_blocks

import retQSS;

constant Integer N_VOLS = 240, N = 50; 
constant Integer AIR = 0, ROCK = 1;
constant Integer LOCKED = 0, UNLOCKED = 1;
constant Integer LOCK_THRESH = 5, REGULAR = 0, MESSIAH = 1, LOCK_TIME = 30;

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;

discrete Integer volumeID[N], prevVolumeID, vol_type, vol_status, next_lock, n_part;
discrete Real nx, ny, nz, dot;
discrete Integer particle_type;

initial algorithm 
    for i in 1:N loop
        x[i]  := random(-40,40);
        y[i]  := random(-80,20);
        z[i]  := 0;

        vx[i] := random(0,20);
        vy[i] := random(0,20);
        vz[i] := random(0,50);
    end for;

    status := retQSS_setUp(N, "geom/cube_240.vtk", "confinement_blocks");

    if status == false then
        status := retQSS_fail();
    end if;

    vol_type := volume_setProperty(0, "type", ROCK);
    vol_status := volume_setProperty(0, "status", UNLOCKED);

    for id in 1:N_VOLS loop
        vol_type := volume_setProperty(id, "type", AIR);
        vol_status := volume_setProperty(id, "status", UNLOCKED);
        if  modulus(id,25) == 0 then
            vol_type := volume_setProperty(id, "type", ROCK);
        end if;
        if  modulus(id,27) == 0 then
            vol_status := volume_setProperty(id, "status", LOCKED);
        end if;
    end for;

    for i in 1:N loop
        volumeID[i] := particle_currentVolumeID(i);
        particle_type := particle_setProperty(i, "type", REGULAR);
    end for;

    particle_type := particle_setProperty(N/2, "type", MESSIAH);
    next_lock := LOCK_TIME;

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
                prevVolumeID := particle_previousVolumeID(i);
                vol_status := volume_getProperty(prevVolumeID, "status");
                vol_type := volume_getProperty(volumeID[i], "type");
                n_part  := volume_countParticlesInside(volumeID[i]);
                particle_type := particle_getProperty(i, "type");

                if vol_type == ROCK or vol_status == LOCKED then
                    (nx, ny, nz) := particle_crossedFaceNormal(i);

                    dot := vx[i]*nx + vy[i]*ny + vz[i]*nz; 

                    reinit(vx[i], vx[i] - 2*dot*nx);
                    reinit(vy[i], vy[i] - 2*dot*ny);
                    reinit(vz[i], vz[i] - 2*dot*nz);
                elseif n_part >= LOCK_THRESH or particle_type == MESSIAH then
                    vol_status := volume_setProperty(volumeID[i], "status", UNLOCKED);
                end if; 
        end when;
    end for;

    when time > next_lock then
        for id in 1:N_VOLS loop
            if  modulus(id,27) == 0 then
                vol_status := volume_setProperty(id, "status", LOCKED);
            end if;
        end for;

        next_lock := time + LOCK_TIME;
    end when;

annotation(
    experiment(
        MMO_Description="Bouncing particles that can get trapped inside locked cells",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z,volumeID},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=150,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end confinement_blocks;
