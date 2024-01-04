model volume_properties

import retQSS;

Real x, y, z, vx, vy, vz;

parameter Real Bx = 0;
parameter Real By = 0;
parameter Real Bz = 0.001;

parameter Real c  = 299.792458;
parameter Real m  = 0.510998910;
parameter Real q = 1.;
parameter Real gamma2 = 22.36627204213;

parameter Real v0 = 0.999*c;

parameter Real invMGamma2 = 1 / (m * gamma2);
parameter Real qc2 = q*89875.5178737;
parameter Real coeff = qc2 * invMGamma2;

parameter Real delta = 0.1;

discrete Boolean status;
discrete Integer volumeID;
discrete Real cz, K, next_check;

initial algorithm 
    x  := 0;
    y  := 0;
    z  := 0;

    K := 1;

    vx := 0.9797958971132712*v0;
    vy := 0;
    vz := 0.2*v0;

    next_check := delta;

    status := retQSS_setUp(1, "geom/cube_800.vtk", "volume_properties");

    if status == false then
        status := retQSS_fail();
    end if;

    volumeID := particle_currentVolumeID(1);

equation
    der(x)  = vx;
    der(y)  = vy;
    der(z)  = vz*K;
    der(vx) = coeff * (Bz*vy - By*vz);
    der(vy) = coeff * (Bx*vz - Bz*vx);
    der(vz) = coeff * (By*vx - Bx*vy);

  algorithm
    when time > particle_nextCrossingTime(1,x,y,z,vx,vy,vz) then
        volumeID := particle_currentVolumeID(1);

        if volumeID > 0 then
            status := volume_centroid(volumeID);
            cz := getZ();

            if cz >= 120 then
                K := 2;
            end if;
            if cz >= 400 then
                K := 0.5;
            end if;
        end if;
    end when;

    when time > next_check then
        status := particle_checkPosition(1);
        next_check := time + delta;
    end when;
    
annotation(
    experiment(
        MMO_Description="Particle reacting to properties in volumes",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_PartitionMethod=Metis,
        MMO_Output={volumeID, cz},
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=20,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end volume_properties;
