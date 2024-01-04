model boids_obstacles

import retQSS;

constant Integer N = 100;

discrete Real DEFAULT_RADIUS(start=50.),
              DEFAULT_SEPARATION_COEFF(start=0.05),
              DEFAULT_ALIGNMENT_COEFF(start=0.12),
              DEFAULT_COHESION_COEFF(start=0.09),
              DEFAULT_SPEED(start=10.),
              dt(start=1.);

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status, isObstacle;
discrete Integer volumeID[N], num_neighbors, nextID, boidsKilled;
discrete Real next_update;
discrete Real cohesion_x, cohesion_y, cohesion_z;
discrete Real alignment_x, alignment_y, alignment_z;
discrete Real separation_x, separation_y, separation_z;
discrete Real ux, uy, uz, px, py, pz;
discrete Real new_vx, new_vy, new_vz, dot, result;
discrete Real nextAvoidance[N], killTime[N];


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

    status := particle_setUp(N, "boids_obstacles");

    if status == false then
       status := retQSS_fail();
    end if;

    for i in 1:N loop
        volumeID[i] := particle_currentVolumeID(i);
        nextAvoidance[i] := 1e-10;
        killTime[i] := 1e20;
    end for;

    status := volume_setProperty(140, "obstacle", 1);
    status := volume_setProperty(60, "obstacle", 1);
    status := volume_setProperty(23, "obstacle", 1);
    status := volume_setProperty(160, "obstacle", 1);

    status := particle_dumpAllInitialConditions("boids_obstacles_ic.txt");

    boidsKilled := 0;
    next_update := dt;
    status := volumeNeighborhood_toRadial(DEFAULT_RADIUS);

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
            isObstacle := volume_hasProperty(volumeID[i], "obstacle");

            if isObstacle then
                killTime[i] := time;
            elseif volumeID[i] == 0 then
                (ux, uy, uz) := particle_crossedFaceNormal(i);

                dot := vx[i]*ux + vy[i]*uy + vz[i]*uz;

                (new_vx, new_vy, new_vz) := vectorWithNorm(
                                                vx[i]-2*dot*ux,
                                                vy[i]-2*dot*uy,
                                                vz[i]-2*dot*uz,
                                                DEFAULT_SPEED);

                reinit(vx[i], new_vx);
                reinit(vy[i], new_vy);
                reinit(vz[i], new_vz);
            else
                nextAvoidance[i] := time;
            end if;
        end when;

        when time > nextAvoidance[i] then
            nextID := particle_nextVolumeID(i);
            isObstacle := volume_hasProperty(nextID, "obstacle");

            if isObstacle then
                reinit(vx[i], -vx[i]);
                reinit(vy[i], -vy[i]);
                reinit(vz[i], -vz[i]);
            end if;

            nextAvoidance[i] := 1e20;
        end when;

        when time > killTime[i] then
            reinit(vx[i], 0);
            reinit(vy[i], 0);
            reinit(vz[i], 0);
            status := particle_kill(i);
            boidsKilled := boidsKilled + 1;
            nextAvoidance[i] := 1e20;
            killTime[i] := 1e20;
        end when;
    end for;

    when time > next_update then
        for i in 1:N loop
            status := particle_checkPosition(i);
            num_neighbors := particleNeighborhood_countParticles(i);

            if particle_isAlive(i) and num_neighbors > 0 then
                (num_neighbors, px, py, pz) := particleNeighborhood_averagePosition(i);
                cohesion_x := px - x[i];    
                cohesion_y := py - y[i];    
                cohesion_z := pz - z[i];

                (num_neighbors, alignment_x, alignment_y, alignment_z) := particleNeighborhood_averageVelocity(i);

                (num_neighbors, separation_x, separation_y, separation_z) := particleNeighborhood_repulsiveDirection(i);

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

                (ux, uy, uz) := vectorWithNorm(new_vx, new_vy, new_vz, DEFAULT_SPEED);

                reinit(vx[i], ux);
                reinit(vy[i], uy);
                reinit(vz[i], uz);

                nextAvoidance[i] := time;
            end if;
        end for;

       next_update := time + dt;
    end when;

annotation(
    experiment(
        MMO_Description="3D boids (with obstacles)",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z,vx,vy,vz,boidsKilled},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=100,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end boids_obstacles;
