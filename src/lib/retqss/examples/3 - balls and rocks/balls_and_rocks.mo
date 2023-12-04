// Balls moving in a geometry with rocks that make them bounce.
model balls_and_rocks

// Import package containing retQSS API definition.
import retQSS;

constant Integer
    // Number of particles in the system.
    N = 5,
    // Volumes that can be penetrated by balls.
    AIR = 0,
    // Volumes that make balls bounce against them.
    ROCK = 1,
    // Maximum number of supported volumes.
    MAX_VOLS = 10000;

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;
discrete Integer volumeID, volType, numVolumes;
discrete Real nx, ny, nz, dot;


initial algorithm 
    // Define initial position and velocity for the N balls.
    for i in 1:N loop
        // Random point within the geometry boundaries and on the ground.
        // More advanced models will use the API to get random points
        // inside random volumes, making them geometry-agnostic (see e.g.
        // boids model).
        x[i]  := random(-40,40);
        y[i]  := random(-80,20);
        z[i]  := 0;

        // Random velocity.
        vx[i] := random(0,10);
        vy[i] := random(0,10);
        vz[i] := random(0,20);
    end for;

    // Initialize retQSS geometry and set up N particles.
    status := retQSS_setUp(N, "geom/balls_and_rocks.vtk", "balls_and_rocks");

    if status == false then
        status := retQSS_fail();
    end if;

    // Set volume type using the property API.
    numVolumes := geometry_countVolumes();
    for v in 0:MAX_VOLS loop
        if v <= numVolumes then
            volType := volume_setProperty(v, "type", AIR);
            // Rocks are volumes whose IDs are divisible by 17.
            if  modulus(v,17) == 0 then
                volType := volume_setProperty(v, "type", ROCK);
            end if;
        end if;
    end for;

equation
    // Equations of motion of the N balls.
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
        // When the event is triggered, the i-th ball has just crossed a volume
        // boundary, entering a new volume.
        when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
            // Retrieve new volume ID and its type.
            volumeID := particle_currentVolumeID(i);
            volType := volume_getProperty(volumeID, "type");

            // Rock hit. Make the ball bounce updating its velocity via the
            // reflection equation.
            if volType == ROCK then
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
        MMO_Description="Balls moving in a geometry with rocks that make them bounce",
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

end balls_and_rocks;
