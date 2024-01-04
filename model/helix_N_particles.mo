model helix_N_particles
 
import retQSS;

constant Integer N = 2;

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

parameter Real Bx[N], By[N], Bz[N];

parameter Real c  = 299.792458;
parameter Real m  = 0.510998910;
parameter Real q = 1.;
parameter Real gamma2 = 22.36627204213;

parameter Real v0 = 0.999*c;

parameter Real invMGamma2 = 1 / (m * gamma2);
parameter Real qc2 = q*89875.5178737;
parameter Real coeff = qc2 * invMGamma2;

discrete Boolean status;
discrete Integer volumeID[N];

initial algorithm 
    Bx[1] := 0;
    By[1] := 0;
    Bz[1] := 0.001;

    Bx[2] := 0;
    By[2] := 0;
    Bz[2] := 0.01;

    x[1]  := 0;
    y[1]  := 0;
    z[1]  := 0;
    vx[1] := 0.9797958971132712*v0;
    vy[1] := 0;
    vz[1] := 0.2*v0;

    x[2]  := 1;
    y[2]  := 0;
    z[2]  := 0;
    vx[2] := 0.99498743710662*v0;
    vy[2] := 0;
    vz[2] := 0.1*v0;

    status := retQSS_setUp(N, "geom/cube_240.vtk", "helix_N_particles");

    if status == false then
        status := retQSS_fail();
    end if;

    volumeID[1] := particle_currentVolumeID(1);
    volumeID[2] := particle_currentVolumeID(2);

equation
    for i in 1:N loop
        der(x[i])  = vx[i];
        der(y[i])  = vy[i];
        der(z[i])  = vz[i];
        der(vx[i]) = coeff * (Bz[i]*vy[i] - By[i]*vz[i]);
        der(vy[i]) = coeff * (Bx[i]*vz[i] - Bz[i]*vx[i]);
        der(vz[i]) = coeff * (By[i]*vx[i] - Bx[i]*vy[i]);
    end for;

algorithm
    for i in 1:N loop
        when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
            volumeID[i] := particle_currentVolumeID(i);
        end when;
    end for;

annotation(
    experiment(
        MMO_Description="",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_PartitionMethod=Metis,
        MMO_Output={volumeID},
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=3,
        Tolerance={1e-7},
        AbsTolerance={1e-8}
    ));

end helix_N_particles;
