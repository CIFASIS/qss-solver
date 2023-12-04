model boids

// Import package containing retQSS API definition.
import retQSS;

constant Integer
    // Number of birds in the system.
    N = 170;

discrete Real 
    // Interval between rule evaluations.
    dt(start=1.),
    // Multiplicative coefficient for separation rule.
    DEFAULT_SEPARATION_COEFF(start=0.05),
    // Multiplicative coefficient for alignment rule.
    DEFAULT_ALIGNMENT_COEFF(start=0.12),
    // Multiplicative coefficient for cohesion rule.
    DEFAULT_COHESION_COEFF(start=0.09),
    // Radius of volume neighborhood to compute neighboring boids.
    DEFAULT_RADIUS(start=10),
    // Constant speed for all boids.
    DEFAULT_SPEED(start=7.);
    // The following values can be used for a proper parameterization of the
    // model when switching to the sphere geometry.
    //DEFAULT_RADIUS(start=0.2),
    //DEFAULT_SPEED(start=0.1);

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;
discrete Integer volumeID[N], num_neighbors;
discrete Real next_update; // Time of next rule evaluation.
discrete Real cohesion_x, cohesion_y, cohesion_z;
discrete Real alignment_x, alignment_y, alignment_z;
discrete Real separation_x, separation_y, separation_z;
discrete Real new_vx, new_vy, new_vz;
discrete Real px, py, pz, ux, uy, uz;


initial algorithm
    // Set up retQSS geometry.
    status := geometry_setUp("geom/torus.vtk");
    // Use the following line to switch to a sphere geometry. 
    //status := geometry_setUp("geom/sphere.vtk");

    // Define initial position and velocity for the N boids.
    for i in 1:N loop
            // Put the i-th boid in a random polyhedral volume.
            volumeID[i] := geometry_randomVolumeID();

            // Compute a random point inside this volume.
            (px, py, pz) := volume_randomPoint(volumeID[i]);
            x[i]  := px;
            y[i]  := py;
            z[i]  := pz;

            // Compute a random initial direction.
            (ux, uy, uz) := randomVectorWithNorm(DEFAULT_SPEED);
            vx[i] := ux;
            vy[i] := uy;
            vz[i] := uz;
    end for;

    // Initialize retQSS particles.
    status := particle_setUp(N, "boids");

    if status == false then
       status := retQSS_fail();
    end if;

    next_update := dt;

    // Set radial volume neighborhood algorithm.
    status := volumeNeighborhood_toRadial(DEFAULT_RADIUS);

equation
    // Equations of motion of the N boids.
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
        // When the event is triggered, the i-th boid has just crossed a volume
        // boundary, entering a new volume.
        when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
           volumeID[i] := particle_currentVolumeID(i);

            if volumeID[i] == 0 then
                // When the boid has just left the geometry, make it bounce
                // against the last crossed face so that it remains inside.
                (new_vx, new_vy, new_vz) := particle_reflectiveBounceWithSpeed(i,DEFAULT_SPEED);

                // Reinitialize velocity after the new reflected direction.
                reinit(vx[i], new_vx);
                reinit(vy[i], new_vy);
                reinit(vz[i], new_vz);
            end if;
        end when;
    end for;

    // When this event is triggered, the velocity of every boid is successively
    // updated by evaluating the corresponding three rules.
    when time > next_update then
        for i in 1:N loop
            num_neighbors := particleNeighborhood_countParticles(i);

            // Do nothing if current boid has no neighbors.
            if num_neighbors > 0 then
                // 1. Cohesion rule: compute center of mass of neighboring
                // boids and orient current boid in that direction.
                (num_neighbors, px, py, pz) := particleNeighborhood_averagePosition(i);
                cohesion_x := px - x[i];    
                cohesion_y := py - y[i];    
                cohesion_z := pz - z[i];

                // 2. Alignment rule: synchronize current boid direction with
                // the average direction of neighboring boids.
                (num_neighbors, alignment_x, alignment_y, alignment_z) := particleNeighborhood_averageVelocity(i);

                // 3. Separation rule: avoid crowding neighbors by steering
                // away from them.
                (num_neighbors, separation_x, separation_y, separation_z) := particleNeighborhood_repulsiveDirection(i);

                // Now update velocity components.
                new_vx := 
                    vx[i] + 
                    separation_x*DEFAULT_SEPARATION_COEFF +
                    alignment_x*DEFAULT_ALIGNMENT_COEFF +
                    cohesion_x*DEFAULT_COHESION_COEFF;

                new_vy := 
                    vy[i] + 
                    separation_y*DEFAULT_SEPARATION_COEFF +
                    alignment_y*DEFAULT_ALIGNMENT_COEFF +
                    cohesion_y*DEFAULT_COHESION_COEFF;

                new_vz := 
                    vz[i] + 
                    separation_z*DEFAULT_SEPARATION_COEFF +
                    alignment_z*DEFAULT_ALIGNMENT_COEFF +
                    cohesion_z*DEFAULT_COHESION_COEFF;

                // Set target speed and reinitialize velocity state variables.
                (ux, uy, uz) := vectorWithNorm(new_vx, new_vy, new_vz, DEFAULT_SPEED);

                reinit(vx[i], ux);
                reinit(vy[i], uy);
                reinit(vz[i], uz);
            end if;
        end for;

        // Set time of next update.
        next_update := time + dt;
    end when;

annotation(
    experiment(
        MMO_Description="3D boids",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z,vx,vy,vz},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=100,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end boids;
