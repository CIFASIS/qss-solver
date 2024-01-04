model boids_funcs

import retQSS;
import retQSS_boids;

constant Integer N = 170;

discrete Real DEFAULT_RADIUS(start=7.),
              DEFAULT_SEPARATION_COEFF(start=0.05),
              DEFAULT_ALIGNMENT_COEFF(start=0.12),
              DEFAULT_COHESION_COEFF(start=0.09),
              DEFAULT_SPEED(start=7.),
              dt(start=1.);

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;
discrete Integer volumeID[N], num_neighbors;
discrete Real next_update;
discrete Real cohesion_x, cohesion_y, cohesion_z;
discrete Real alignment_x, alignment_y, alignment_z;
discrete Real separation_x, separation_y, separation_z;
discrete Real nx, ny, nz, px, py, pz;
discrete Real new_vx, new_vy, new_vz, dot, result;


initial algorithm
    status := geometry_setUp("geom/torus.vtk");

    for i in 1:N loop
            volumeID[i] := geometry_randomVolumeID();

            (px, py, pz) := volume_randomPoint(volumeID[i]);
            x[i]  := px;
            y[i]  := py;
            z[i]  := pz;

            (px, py, pz) := randomVectorWithNorm(DEFAULT_SPEED);
            vx[i] := px;
            vy[i] := py;
            vz[i] := pz;
    end for;

    status := particle_setUp(N, "boids_funcs");

    if status == false then
       status := retQSS_fail();
    end if;

    for i in 1:N loop
            result := particle_setProperty(i, "separation_coeff", DEFAULT_SEPARATION_COEFF);
            result := particle_setProperty(i, "alignment_coeff", DEFAULT_ALIGNMENT_COEFF);
            result := particle_setProperty(i, "cohesion_coeff", DEFAULT_COHESION_COEFF);
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

                (px, py, pz) := vectorWithNorm(
                                    vx[i]-2*dot*nx,
                                    vy[i]-2*dot*ny,
                                    vz[i]-2*dot*nz,
                                    DEFAULT_SPEED);

                reinit(vx[i], px);
                reinit(vy[i], py);
                reinit(vz[i], pz);
            end if;
        end when;
    end for;

    when time > next_update then
        for i in 1:N loop
            num_neighbors := particleNeighborhood_countParticles(i);

            if num_neighbors > 0 then
                (num_neighbors, px, py, pz) := particleNeighborhood_forEachParticle_1(i, "boids_forEach", num_neighbors);

                new_vx := vx[i] + px - x[i]*DEFAULT_COHESION_COEFF;
                new_vy := vy[i] + py - y[i]*DEFAULT_COHESION_COEFF;
                new_vz := vz[i] + pz - z[i]*DEFAULT_COHESION_COEFF;

                (px, py, pz) := vectorWithNorm(new_vx, new_vy, new_vz, DEFAULT_SPEED);

                reinit(vx[i], px);
                reinit(vy[i], py);
                reinit(vz[i], pz);
            end if;
        end for;

       next_update := time + dt;
    end when;

annotation(
    experiment(
        MMO_Description="3D boids (with particle neighborhood iteration)",
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

end boids_funcs;
