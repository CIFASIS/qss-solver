model rays

import retQSS;

constant Integer N = 100; 

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Real coeff_x[N], coeff_y[N], coeff_z[N];
parameter Real target_x[N], target_y[N], target_z[N];
parameter Real k;

discrete Boolean status;
discrete Integer volumeID[N], n1, n2;

initial algorithm 
    for i in 1:N loop
        x[i]  := random(-30,80);
        y[i]  := random(-60,-1);
        z[i]  := random(-30,30);

        target_x[i] := random(0,50);
        target_y[i] := 0;
        target_z[i] := random(0,50);

        k := random(0.5, 5);
        coeff_x[i] := (target_x[i] - x[i]) * k;
        coeff_y[i] := (target_y[i] - y[i]) * k;
        coeff_z[i] := (target_z[i] - z[i]) * k;

        vx[i] := coeff_x[i];
        vy[i] := coeff_y[i];
        vz[i] := coeff_z[i];
    end for;

    n1 := 0;
    n2 := 0;

    status := retQSS_setUp(N, "geom/rays.vtk", "rays");

    if status == false then
        status := retQSS_fail();
    end if;

    for i in 1:N loop
        volumeID[i] := particle_currentVolumeID(i);
    end for;

equation
    for i in 1:N loop
        der(x[i])  = coeff_x[i];
        der(y[i])  = coeff_y[i];
        der(z[i])  = coeff_z[i];
        der(vx[i]) = 0;
        der(vy[i]) = 0;
        der(vz[i]) = 0;
    end for;

algorithm
    for i in 1:N loop
        when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
            volumeID[i] := particle_currentVolumeID(i);
            n1 := volume_countParticlesInside(1);
            n2 := volume_countParticlesInside(2);

            if volumeID[i] == 1 then
                coeff_x[i] := 0;
                coeff_y[i] := random(10, 50);
                coeff_z[i] := 0;
            end if;
        end when;
    end for;

annotation(
    experiment(
        MMO_Description="",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={0.1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x,y,z,n1,n2},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=20,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end rays;
