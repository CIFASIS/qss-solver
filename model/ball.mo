model ball

import retQSS;

Real x, y, z, vx, vy, vz;

discrete Integer bounce;
discrete Integer volumeID;
discrete Real nx, ny, nz;
discrete Boolean status;

initial algorithm 
    x  := 8;
    y  := 2.5;
    z  := 21;

    vx := 2;
    vy := 2;
    vz := 0;

    status := retQSS_setUp(1, "geom/ball.vtk", "ball");

    if status == false then
        status := retQSS_fail();
    end if;

    bounce := 0;
    volumeID := particle_currentVolumeID(1);

equation
    der(x) = vx;
    der(y) = vy;
    der(z) = vz;
    der(vx) = 0;
    der(vy) = 0;
    der(vz) = -9.8 - 0.1*vz - bounce * (z*1e6 + 100*vz); 

algorithm
    when time > particle_nextCrossingTime(1,x,y,z,vx,vy,vz) then
        volumeID := particle_currentVolumeID(1);

        (nx, ny, nz) := particle_crossedFaceNormal(1);

        if volumeID == 0 and nz <> 0  then
            bounce := 1;
        elseif volumeID > 0 and nz <> 0 then
            bounce := 0;
        elseif volumeID == 0 and nx <> 0 then
            reinit(vx, -1*vx);
        elseif volumeID == 0 and ny <> 0 then
            reinit(vy, -1*vy);
        end if; 
    end when;

annotation(
    experiment(
        MMO_Description="Ball bouncing",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=20,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end ball;
