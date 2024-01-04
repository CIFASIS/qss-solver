model boids_2d

import retQSS;
import retQSS_boids;

constant Integer N = 200;

discrete Real DEFAULT_VIEW_ANGLE(start=135), 
              DEFAULT_RADIUS(start=40),
              DEFAULT_SEPARATION_COEFF(start=0.007),
              DEFAULT_ALIGNMENT_COEFF(start=0.12),
              DEFAULT_COHESION_COEFF(start=0.05),
              DEFAULT_SPEED(start=5.),
              Z_COORD(start=5.),
              dt(start=1.);

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];
discrete Boolean status;
discrete Integer volumeID[N];
discrete Real next_update, px, py, pz;
discrete Real ux, uy, uz, dot, result;


initial algorithm
    status := geometry_setUp("geom/boids_2d.vtk");

    for i in 1:N loop
		    volumeID[i] := geometry_randomVolumeID();

            (px, py, pz) := volume_randomPoint(volumeID[i]);
		    x[i]  := px;
		    y[i]  := py;
		    z[i]  := Z_COORD;

            (ux, uy, uz) := randomVectorWithNorm(DEFAULT_SPEED);
            vx[i] := ux;
            vy[i] := uy;
            vz[i] := uz;
    end for;

    status := particle_setUp(N, "boids_2d");

    if status == false then
       status := retQSS_fail();
    end if;

    for i in 1:N loop
            result := particle_setProperty(i, "view_angle", DEFAULT_VIEW_ANGLE);
            result := particle_setProperty(i, "neighborhood_radius", DEFAULT_RADIUS);
            result := particle_setProperty(i, "separation_coeff", DEFAULT_SEPARATION_COEFF);
            result := particle_setProperty(i, "alignment_coeff", DEFAULT_ALIGNMENT_COEFF);
            result := particle_setProperty(i, "cohesion_coeff", DEFAULT_COHESION_COEFF);
    end for;

    next_update := dt;
    status := volumeNeighborhood_toVertexSharing();

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
                (ux, uy, uz) := particle_crossedFaceNormal(i);

                dot := vx[i]*ux + vy[i]*uy + vz[i]*uz;

                (ux, uy, uz) := vectorWithNorm(
                                    vx[i]-2*dot*ux,
                                    vy[i]-2*dot*uy,
                                    vz[i]-2*dot*uz,
                                    DEFAULT_SPEED);

                reinit(vx[i], ux);
                reinit(vy[i], uy);
                reinit(vz[i], uz);
            end if;
        end when;
    end for;

    when time > next_update then
        for i in 1:N loop
            status := updateBoid(i);
            if status == true then
                ux := particle_getProperty(i, "vx");
                uy := particle_getProperty(i, "vy");
                uz := particle_getProperty(i, "vz");

                (px, py, pz) := vectorWithNorm(ux, uy, uz, DEFAULT_SPEED);

                reinit(vx[i], px);
                reinit(vy[i], py);
                reinit(vz[i], pz);
            end if;
        end for;

       next_update := time + dt;
    end when;

annotation(
	experiment(
		MMO_Description="2D boids",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Period={1e-1},
		MMO_PartitionMethod=Metis,
		MMO_Output={x,y,vx,vy},
		MMO_OutputType=CI_Sampled,
		MMO_Scheduler=ST_Linear,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=100,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));

end boids_2d;
