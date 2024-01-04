model diffusion

import retQSS;

constant Integer N = 20;
Real x, y, z, vx, vy, vz;
Real u[N], u_alg[N];

parameter Real ppi = 3.14159265;
parameter Real tpi2 = 98.696044;
parameter Real Zero = 0;
parameter Real v0 = 35, K = 2.5;

discrete Boolean status;
discrete Integer volumeID, prevVolumeID;
discrete Real u_vol, coeff, new_vx;

initial algorithm
    x := 19.5;
    y := 0.5;
    z := 0.5;

    vx := -v0;
    vy := 0;
    vz := 0; 

    u[1] := 1;
    for i in 2:N-1 loop
        u[i] :=(10-i)/10;
    end for;
    u[N] := -1;

    status := retQSS_setUp(1, "geom/diffusion.vtk", "diffusion");

    if status == false then
        status := retQSS_fail();
    end if;

    volumeID := particle_currentVolumeID(1);

    for i in 1:N loop
        u_alg[i] := volume_setProperty(i, "u", u[i]);
    end for;

equation
    der(x) = vx;
    der(y) = vy;
    der(z) = vz;
    der(vx) = 0;
    der(vy) = 0;
    der(vz) = 0; 

    der(u[1]) = -(K/10) * u[1] + Zero*u_alg[1];
    u_alg[1] = volume_setProperty(1, "u", u[1]);
    for i in 2:N-1 loop
        der(u[i]) = ((i*i)/tpi2) * (u[i+1] - K*u[i] + u[i-1]) + Zero*u_alg[i];
        u_alg[i] = volume_setProperty(i, "u", u[i]);
    end for;
    der(u[N]) = ((K*(N-1)*(N-1))/tpi2) * (-u[N] + u[N-1]) + Zero*u_alg[N];
    u_alg[N] = volume_setProperty(N, "u", u[N]);

algorithm
    when time > particle_nextCrossingTime(1,x,y,z,vx,vy,vz) then
        volumeID := particle_currentVolumeID(1);
        prevVolumeID := particle_previousVolumeID(1);

        if volumeID == 0 then
            new_vx := -vx;
        elseif prevVolumeID == 0 then
            new_vx := vx;
        else
            u_vol := volume_getProperty(volumeID, "u");
            coeff := 1 - (u_vol+1)/4;
            new_vx := coeff*v0;
            if vx < 0 then
                new_vx := new_vx * -1;
            end if;
        end if;

        reinit(vx, new_vx); 
    end when;

annotation(
    experiment(
        MMO_Description="Heat diffusion",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={5e-3},
        MMO_PartitionMethod=Scotch,
        MMO_LPS=2,
        MMO_DT_Min=2,
        MMO_Output={x,vx,volumeID,u_vol,u_alg},
        MMO_OutputType=CI_Sampled,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=10,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));
end diffusion;
