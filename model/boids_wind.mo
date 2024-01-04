model boids_wind

import retQSS;

constant Integer N = 100;
constant Integer MAX_VOLUMES = 1000000;
constant Integer MAX_FACES_PER_VOL = 6;
constant Integer MAX_FACES = MAX_FACES_PER_VOL*MAX_VOLUMES;

discrete Real DEFAULT_RADIUS(start=50.),
              DEFAULT_SEPARATION_COEFF(start=0.05),
              DEFAULT_ALIGNMENT_COEFF(start=0.12),
              DEFAULT_COHESION_COEFF(start=0.09),
              DEFAULT_WIND_COEFF(start=1.),
              DEFAULT_SPEED(start=10.),
              dt(start=0.85);

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;
discrete Integer volumeID[N], num_neighbors;
discrete Integer nVols, nFaces, faceID, volID;
discrete Real next_update;
discrete Real cohesion_x, cohesion_y, cohesion_z;
discrete Real alignment_x, alignment_y, alignment_z;
discrete Real separation_x, separation_y, separation_z;
discrete Real nx, ny, nz, px, py, pz, ux, uy, uz;
discrete Real wind_x, wind_y, wind_z;
discrete Real new_vx, new_vy, new_vz, dot, result;


initial algorithm
    status := geometry_setUp("geom/cube_240.vtk");
    status := geometry_dumpSurface("cube_240_surface.vtk");

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

    status := particle_setUp(N, "boids_funcs");

    if status == false then
       status := retQSS_fail();
    end if;

    nVols := geometry_countVolumes();
    nFaces := geometry_countFaces();    

    // TODO: nested for loops!
    status := volume_setPropertyVector(0, "wind", 0, 0, 0);
    for v in 1:MAX_VOLUMES loop
        if v <= nVols then
            status := volume_setPropertyVector(v, "wind", 0, 0, 0);
        end if;
    end for;

    for f in 1:MAX_FACES loop
        if f <= nFaces then
            if face_isOnBoundary(f) then
                volID := face_primaryVolumeID(f);

                (ux, uy, uz) := volume_getPropertyVector(volID, "wind");
                (nx, ny, nz) := face_normal(f);

                status := volume_setPropertyVector(volID, "wind", ux-nx, uy-ny, uz-nz);
            end if;
        end if;
    end for;

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

            if volumeID[i] == 0 then
                (nx, ny, nz) := particle_crossedFaceNormal(i);

                dot := vx[i]*nx + vy[i]*ny + vz[i]*nz;

                (ux, uy, uz) := vectorWithNorm(
                                    vx[i]-2*dot*nx,
                                    vy[i]-2*dot*ny,
                                    vz[i]-2*dot*nz,
                                    DEFAULT_SPEED);
            else
                (wind_x, wind_y, wind_z) := volume_getPropertyVector(volumeID[i], "wind");

                new_vx := 
                    vx[i] + 
                    wind_x*DEFAULT_WIND_COEFF;

                new_vy := 
                    vy[i] + 
                    wind_y*DEFAULT_WIND_COEFF;

                new_vz := 
                    vz[i] + 
                    wind_z*DEFAULT_WIND_COEFF;

                (ux, uy, uz) := vectorWithNorm(new_vx, new_vy, new_vz, DEFAULT_SPEED);
            end if;

            reinit(vx[i], ux);
            reinit(vy[i], uy);
            reinit(vz[i], uz);
        end when;
    end for;

    when time > next_update then
        for i in 1:N loop
            num_neighbors := particleNeighborhood_countParticles(i);

            new_vx := vx[i];
            new_vy := vy[i];
            new_vz := vz[i];

            if num_neighbors > 0 then
                (num_neighbors, px, py, pz) := particleNeighborhood_averagePosition(i);
                cohesion_x := px - x[i];    
                cohesion_y := py - y[i];    
                cohesion_z := pz - z[i];

                (num_neighbors, alignment_x, alignment_y, alignment_z) := particleNeighborhood_averageVelocity(i);

                (num_neighbors, separation_x, separation_y, separation_z) := particleNeighborhood_repulsiveDirection(i);

                new_vx := 
                    new_vx + 
                    separation_x*DEFAULT_SEPARATION_COEFF +
                    alignment_x*DEFAULT_ALIGNMENT_COEFF +
                    cohesion_x*DEFAULT_COHESION_COEFF;

                new_vy := 
                    new_vy + 
                    separation_y*DEFAULT_SEPARATION_COEFF +
                    alignment_y*DEFAULT_ALIGNMENT_COEFF +
                    cohesion_y*DEFAULT_COHESION_COEFF;

                new_vz := 
                    new_vz + 
                    separation_z*DEFAULT_SEPARATION_COEFF +
                    alignment_z*DEFAULT_ALIGNMENT_COEFF +
                    cohesion_z*DEFAULT_COHESION_COEFF;
            end if;

            (wind_x, wind_y, wind_z) := volume_getPropertyVector(volumeID[i], "wind");

            new_vx := 
                new_vx + 
                wind_x*DEFAULT_WIND_COEFF;

            new_vy := 
                new_vy + 
                wind_y*DEFAULT_WIND_COEFF;

            new_vz := 
                new_vz + 
                wind_z*DEFAULT_WIND_COEFF;

            (ux, uy, uz) := vectorWithNorm(new_vx, new_vy, new_vz, DEFAULT_SPEED);

            reinit(vx[i], ux);
            reinit(vy[i], uy);
            reinit(vz[i], uz);
        end for;

       next_update := time + dt;
    end when;

annotation(
    experiment(
        MMO_Description="3D boids (with wind)",
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

end boids_wind;
