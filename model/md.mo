model md

import retQSS;
import retQSS_md;

constant Integer N = 100;

parameter Real
    DEFAULT_R_CUT = 0.2,
    DEFAULT_NEIGHBORBOOD_RADIUS = sqrt(3./100)+1e-5,
    DEFAULT_C = 1e-5,
    dt = 1.;

Real x[N], y[N], z[N],
     vx[N], vy[N], vz[N];

discrete Real ax[N], ay[N], az[N];

discrete Boolean status;
discrete Integer volumeID[N], num_neighbors, step;
discrete Real next_update, relocation[N];

discrete Real dot,nx,ny,nz,norm;
discrete Real px, py, pz, fx, fy, fz;


initial algorithm
    status := retQSS_setUpWithInitialConditions(N, "geom/md_box_3d.vtk", "../tmp_md/3d/0.dat", "md");

    for i in 1:N loop
        relocation[i] := 1e20;
    end for;

    step := 0;
    next_update := dt;
    status := volumeNeighborhood_toPeriodicRadial(DEFAULT_NEIGHBORBOOD_RADIUS);
    status := volumeNeighborhood_precomputeAll();

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
                relocation[i] := time;
            end if;
        end when;

        when time > relocation[i] then
            (px, py, pz) := particle_oppositePosition(i);

            reinit(x[i], px);
            reinit(y[i], py);
            reinit(z[i], pz);

            volumeID[i] := particle_relocate(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]);

            relocation[i] := 1e20;
        end when;
    end for;

    when time > next_update then
        status := particle_dumpAllCurrentValues("%d.dat", step);
        for i in 1:N loop
            ax[i] := 0;
            ay[i] := 0;
            az[i] := 0;

            (num_neighbors, fx, fy, fz) := particleNeighborhood_forEachParticle_1(
                                            i,
                                            "computeForce",
                                            DEFAULT_R_CUT);

            if num_neighbors > 0 then
                ax[i] := fx;
                ay[i] := fy;
                az[i] := fz; 
            end if;

            reinit(vx[i], vx[i]-DEFAULT_C*ax[i]);
            reinit(vy[i], vy[i]-DEFAULT_C*ay[i]);
            reinit(vz[i], vz[i]-DEFAULT_C*az[i]);
        end for;
        step := step + 1;
        next_update := dt*(step+1);
    end when;

annotation(

    experiment(
        MMO_Description="Introductory molecular dynamics example",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={0.5},
        MMO_PartitionMethod=Metis,
        //MMO_Output={x,y,z,vx,vy,vz},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Binary,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=2001,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end md;
