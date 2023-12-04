// Ball bouncing inside a 3D box.
//  * Acceleration in the z axis determined by gravity force.
//  * When ground is hit, the ball bounces and loses energy.
//  * When a wall is hit, it just 'bounces' back without losing energy.
model bouncing_ball

// Import package containing retQSS API definition.
import retQSS;

Real x, y, z, vx, vy, vz;

discrete Integer bounce;
discrete Integer volumeID;
discrete Real nx, ny, nz;
discrete Boolean status;

initial algorithm 
    // Set initial position and velocity.
    x  := 8;
    y  := 2.5;
    z  := 21;

    vx := 2;
    vy := 2;
    vz := 0;

    // Initialize retQSS geometry and set up one particle.
    status := retQSS_setUp(1, "geom/bouncing_ball.vtk", "bouncing_ball");

    if status == false then
        status := retQSS_fail();
    end if;

    bounce := 0;
    volumeID := particle_currentVolumeID(1);

equation
    // Equations of motion of the ball.
    der(x) = vx;
    der(y) = vy;
    der(z) = vz;
    der(vx) = 0;
    der(vy) = 0;
    der(vz) = -9.8 - 0.1*vz - bounce * (z*1e6 + 100*vz); 

algorithm
    // When the event is triggered, the ball has just crossed one of the
    // boundaries of the box.
    when time > particle_nextCrossingTime(1,x,y,z,vx,vy,vz) then
        // Retreive ID of the new volume.
        volumeID := particle_currentVolumeID(1);

        // Retrieve outward normal vector to the face recently crossed.
        (nx, ny, nz) := particle_crossedFaceNormal(1);

        // Volume 0 represents the exterior of the geometry.
        //  * Ball is hitting the ground: it should bounce and lose energy.
        if volumeID == 0 and nz <> 0  then
            bounce := 1;
        //  * Ball is entering the box after bouncing against a wall.
        elseif volumeID > 0 and nz <> 0 then
            bounce := 0;
        //  * Ball is hitting a wall parallel to the y axis.
        elseif volumeID == 0 and nx <> 0 then
            reinit(vx, -1*vx);
        //  * Ball is hitting a wall parallel to the x axis.
        elseif volumeID == 0 and ny <> 0 then
            reinit(vy, -1*vy);
        end if; 
    end when;

annotation(
    experiment(
        MMO_Description="Ball bouncing inside a 3D box",
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

end bouncing_ball;
