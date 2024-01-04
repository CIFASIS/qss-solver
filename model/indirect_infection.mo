model indirect_infection

import retQSS;
import retQSS_indinf;

constant Integer
    GRID_SIZE = 100,
    CELL_EDGE_LENGTH = 4,
    NX = GRID_SIZE/CELL_EDGE_LENGTH,
    NY = GRID_SIZE/CELL_EDGE_LENGTH,
    MAX_VOLS = NX*NY;

constant Integer N = 200;

constant Integer
  INFECTED = 0,
  NOT_INFECTED = 1,
  RECOVERED = 2,
  DEAD = 3,
  DETECTED = 4,
  ISOLATED = 5;

parameter Real INF, dt, dtOutput, Z_COORD, DEFAULT_SPEED;
parameter Real PARTICLE_INFECTION_PROB, VOLUME_INFECTION_PROB, DEATH_PROB, DETECTION_PROB;
parameter Real MIN_EVENT_TIME, MAX_EVENT_TIME;
parameter Real MIN_RESIDUAL_TIME, MAX_RESIDUAL_TIME, SUPERSPREADER_COEFF;
parameter Real MIN_DETECTION_TIME, MAX_DETECTION_TIME;
parameter Real TRACING_MAX;  // Cuantos contactos se van a aislar (entre los mas recientes)

parameter Real stopTime(start=300.); // Si se cambia, actualizarlo tambien en las annotations.

Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

discrete Boolean status;
discrete Real eventTime[N], detectionTime[N], infectedUntil[MAX_VOLS];
discrete Integer volumeID[N], particleStatus, volumeStatus;
discrete Real ux, uy, uz, result, out_x, out_y, out_z;
discrete Real nextVolumeInfectionTimeUpdate;
discrete Real nextOutputUpdate, residualTime, speed;


initial algorithm
    dt := 0.01;
    dtOutput := 0.2;
    INF := 1e20;
    Z_COORD := 0.9;

    DEFAULT_SPEED := 2.5;

    PARTICLE_INFECTION_PROB := 0.4;
    VOLUME_INFECTION_PROB := 0.3;
    DEATH_PROB := 0.;
    DETECTION_PROB := 0.;

    MIN_EVENT_TIME := 12.;
    MAX_EVENT_TIME := 18.;
    MIN_RESIDUAL_TIME := 2.;
    MAX_RESIDUAL_TIME := 5.;

    MIN_DETECTION_TIME := 1.;
    MAX_DETECTION_TIME := 4.;

    TRACING_MAX := 5;

    SUPERSPREADER_COEFF := 3.;

    status := geometry_setUp("geom/indinf_4.vtk");

    for i in 1:N loop
        volumeID[i] := geometry_randomVolumeID();

        (ux, uy, uz) := volume_randomPoint(volumeID[i]);
        x[i]  := ux;
        y[i]  := uy;
        z[i]  := Z_COORD;

        speed := DEFAULT_SPEED;
        if i == 1 then
            speed := SUPERSPREADER_COEFF*speed;
        end if;
        (ux, uy, uz) := vectorWithNorm(random(-1.,1.), random(-1.,1.), 0, speed);

        vx[i] := ux;
        vy[i] := uy;
        vz[i] := 0;
    end for;

    status := particle_setUp(N, "indirect_infection");

    if status == false then
       status := retQSS_fail();
    end if;

    for i in 1:MAX_VOLS loop
        volumeStatus := volume_setProperty(i, "status", NOT_INFECTED);
        result := volume_setProperty(i, "infectedUntil", 0.);
    end for;

    particleStatus := particle_setProperty(1, "status", INFECTED);
    eventTime[1] := random(MIN_EVENT_TIME, MAX_EVENT_TIME);
    for i in 2:N loop
            particleStatus := particle_setProperty(i, "status", NOT_INFECTED);
            eventTime[i] := 0.;
            detectionTime[i] := 1e20;
    end for;

    volumeID[1] := particle_currentVolumeID(1);
    volumeStatus := volume_setProperty(volumeID[1], "status", INFECTED);
    residualTime := random(MIN_RESIDUAL_TIME, MAX_RESIDUAL_TIME);
    result := volume_setProperty(volumeID[1], "infectedUntil", residualTime);
    status := infectVolume(volumeID[1], eventTime, detectionTime, PARTICLE_INFECTION_PROB/2., 0., MIN_EVENT_TIME, MAX_EVENT_TIME, MIN_DETECTION_TIME, MAX_DETECTION_TIME);

    nextVolumeInfectionTimeUpdate := dt;
    nextOutputUpdate := dtOutput;

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
            particleStatus := particle_getProperty(i, "status");

            if volumeID[i] == 0 then
                (ux, uy, uz) := particle_crossedFaceNormal(i);

                if ux == 0 then
                    ux := random(0., 1.);
                    uy := -1*uy;
                elseif uy == 0 then
                    ux := -1*ux;
                    uy := random(0., 1.);
                end if;

                speed := DEFAULT_SPEED;
                if i == 1 then
                    speed := SUPERSPREADER_COEFF*speed;
                end if;
                (ux, uy, uz) := vectorWithNorm(ux, uy, 0, speed);

                reinit(vx[i], ux);
                reinit(vy[i], uy);
            elseif particle_previousVolumeID(i) <> 0 then
                status := updateContacts(i, volumeID[i], time);
                volumeStatus := volume_getProperty(volumeID[i], "status");
                if particleStatus == INFECTED then
                    status := infectVolume(volumeID[i], eventTime, detectionTime, PARTICLE_INFECTION_PROB, time, MIN_EVENT_TIME, MAX_EVENT_TIME, MIN_DETECTION_TIME, MAX_DETECTION_TIME);
                    if MAX_RESIDUAL_TIME > 0 then
                        result := random(0., 1.);
                        if result < VOLUME_INFECTION_PROB then
                            volumeStatus := volume_setProperty(volumeID[i], "status", INFECTED);
                            residualTime := time + random(MIN_RESIDUAL_TIME, MAX_RESIDUAL_TIME);
                            result := volume_setProperty(volumeID[i], "infectedUntil", residualTime);
                        end if;
                    end if;
                elseif particleStatus == NOT_INFECTED and volumeStatus == INFECTED then
                    result := volume_getProperty(volumeID[i], "infectedUntil");
                    if result >= time then
                        result := random(0., 1.);
                        if result < PARTICLE_INFECTION_PROB then
                            particleStatus := particle_setProperty(i, "status", INFECTED);
                            detectionTime[i] := time + random(MIN_DETECTION_TIME, MAX_DETECTION_TIME);
                            eventTime[i] := time + random(MIN_EVENT_TIME, MAX_EVENT_TIME);
                        end if;
                    end if;
                end if;
            end if;
        end when;
    end for;

    for i in 1:N loop
        when time > eventTime[i] then
            result := random(0., 1.);
            if result < DEATH_PROB then
                status := particle_kill(i);
                particleStatus := particle_setProperty(i, "status", DEAD);
            else
                particleStatus := particle_setProperty(i, "status", RECOVERED);
            end if;
            eventTime[i] := INF;
        end when;

        when time > detectionTime[i] then
            result := random(0., 1.);
            if result < DETECTION_PROB then
                particleStatus := particle_setProperty(i, "status", DETECTED);
                status := isolateContacts(i, TRACING_MAX);
            end if;            
        end when;
    end for;

    for i in 1:MAX_VOLS loop
        when time > infectedUntil[i] then
            status := checkVolumeInfection(i);
            if status then
                result := random(0., 1.);
                if result < VOLUME_INFECTION_PROB then
                    volumeStatus := volume_setProperty(i, "status", INFECTED);
                    residualTime := time + random(MIN_RESIDUAL_TIME, MAX_RESIDUAL_TIME);
                    result := volume_setProperty(i, "infectedUntil", residualTime);
                else
                    volumeStatus := volume_setProperty(i, "status", NOT_INFECTED);
                    result := volume_setProperty(i, "infectedUntil", 0.);
                end if;
            else
                volumeStatus := volume_setProperty(i, "status", NOT_INFECTED);
                result := volume_setProperty(i, "infectedUntil", 0.);
            end if;
            infectedUntil[i] := result;
        end when;
    end for;

    when time > nextVolumeInfectionTimeUpdate then
        for i in 1:MAX_VOLS loop
            infectedUntil[i] := volume_getProperty(i, "infectedUntil");
        end for;
        nextVolumeInfectionTimeUpdate := time + dt;
    end when;

    when time > nextOutputUpdate then
        if time+nextOutputUpdate >= stopTime then
            status := dumpGraph("contacts_inf.adjlist", INFECTED);
            status := dumpGraph("contacts_ninf.adjlist", NOT_INFECTED);
        end if;

        status := dumpOutput("indirect_infection_output.dat");
        nextOutputUpdate := time + dtOutput;
    end when;

annotation(
	experiment(
		MMO_Description="Indirect infection of particles interacting through volumes.",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Period={0.2},
		MMO_PartitionMethod=Metis,
		MMO_Output={x,y,vx,vy,volumeID},
		MMO_OutputType=CI_Sampled,
		MMO_Scheduler=ST_Binary,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=300,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));

end indirect_infection;
