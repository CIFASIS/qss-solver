model covid19

import retQSS;
import retQSS_covid19;
import retQSS_covid19_utils;
import retQSS_covid19_fsm;

/*
  This section loads parameters and constants from the parameters.config file
*/

constant Integer // size
	N = 168,
	GRID_DIVISIONS = 7;

// Initial conditions parameters
parameter Integer
	RANDOM_SEED = getIntegerModelParameter("RANDOM_SEED", 0),
	INITIAL_EXPOSED = getIntegerModelParameter("INITIAL_EXPOSED", 0),
	INITIAL_INFECTED = getIntegerModelParameter("INITIAL_INFECTED", 8),
	GRID_SCENARIO = getIntegerModelParameter("GRID_SCENARIO", 1), //0=open (homogeneous), 1=school (heterogeneous)
	FORCE_TERMINATION_AT = getRealModelParameter("FORCE_TERMINATION_AT", 5);

// Movements and velocity parameters
parameter Real
	DEFAULT_SPEED = getRealModelParameter("DEFAULT_SPEED", 30.0),
	SUPERSPREADER_PROB = 0.0, //no super spreaders
	SUPERSPREADER_ACCELERATION = 4.0,
	NO_VELOCITY_PROB = 0.0;

// Disease population and evolution parameters
parameter Real
	YOUNG_PROB = getRealModelParameter("YOUNG_PROB", 0.25),
	ADULT_PROB = getRealModelParameter("ADULT_PROB", 0.5),

	YOUNG_SYMPTOMATIC_PROB = getRealModelParameter("YOUNG_SYMPTOMATIC_PROB", 0.0),
	ADULT_SYMPTOMATIC_PROB = getRealModelParameter("ADULT_SYMPTOMATIC_PROB", 0.0),
	OLD_SYMPTOMATIC_PROB = getRealModelParameter("OLD_SYMPTOMATIC_PROB", 0.0),

	YOUNG_DEATH_PROB = getRealModelParameter("YOUNG_DEATH_PROB", 0.0),
	ADULT_DEATH_PROB = getRealModelParameter("ADULT_DEATH_PROB", 0.0),
	OLD_DEATH_PROB = getRealModelParameter("OLD_DEATH_PROB", 0.0),

	INCUBATION_TIME_M = 	getRealModelParameter("INCUBATION_TIME_M", 1.63),		// https://bmjopen.bmj.com/content/10/8/e039652
	INCUBATION_TIME_S = 	getRealModelParameter("INCUBATION_TIME_S", 0.5),		//
	PRESYMPTOMATIC_TIME_M = getRealModelParameter("PRESYMPTOMATIC_TIME_M", 2),		// https://www.researchgate.net/publication/344358423_SARS-CoV-2_transmission_dynamics_should_inform_policy/link/5fb1970492851cf24cd57da1/download
	PRESYMPTOMATIC_TIME_S = getRealModelParameter("PRESYMPTOMATIC_TIME_S", 0.5),	// CI95% [1, 3]
	SYMPTOMATIC_TIME_M = 	getRealModelParameter("SYMPTOMATIC_TIME_M", 10),		// https://www.researchgate.net/publication/344358423_SARS-CoV-2_transmission_dynamics_should_inform_policy/link/5fb1970492851cf24cd57da1/download
	SYMPTOMATIC_TIME_S = 	getRealModelParameter("SYMPTOMATIC_TIME_S", 1.5);		// CI95% [7, 13]

// Disease propagation parameters
parameter Real
	CLOSE_CONTACT_PROB = getRealModelParameter("CLOSE_CONTACT_PROB", 0.0), //No close contact / p2p contagion
	PARTICLE_TO_PARTICLE_INFECTION_PROB = getRealModelParameter("PARTICLE_TO_PARTICLE_INFECTION_PROB", 0.4),
	SYMPTOMATIC_CONTAGION_PROB = getRealModelParameter("SYMPTOMATIC_CONTAGION_PROB", 1.0),
	PRESYMPTOMATIC_CONTAGION_PROB = getRealModelParameter("PRESYMPTOMATIC_CONTAGION_PROB", SYMPTOMATIC_CONTAGION_PROB), //same as symptomatic
	ASYMPTOMATIC_CONTAGION_PROB = getRealModelParameter("ASYMPTOMATIC_CONTAGION_PROB", SYMPTOMATIC_CONTAGION_PROB), //same as symptomatic

	VOLUME_CONCENTRATION_LAMBDA = getRealModelParameter("VOLUME_CONCENTRATION_LAMBDA", 5.8), // JLJ Table SI-4 (classroom / prepandemic scenario) = lambda (hourly to daily conversion) 

	BREATHING_INTERVAL_TIME = getRealModelParameter("BREATHING_INTERVAL_TIME", 1.0 / 60.0 / 15.0), //~ 13.8 breaths per minute  https://my.clevelandclinic.org/health/articles/10881-vital-signs#:~:text=Respiratory%20rate%3A%20A%20person's%20respiratory,while%20resting%20is%20considered%20abnormal.

	SYMPTOMATIC_EMISSION_RATE = getRealModelParameter("SYMPTOMATIC_EMISSION_RATE", 18.6 * 1.0 * 1.0), // JLJ Table SI-4 (classroom / prepandemic scenario) = E_P0 * (1/breaths per hour) * r_E * (fe x fi)
	ASYMPTOMATIC_EMISSION_RATE = getRealModelParameter("ASYMPTOMATIC_EMISSION_RATE", SYMPTOMATIC_EMISSION_RATE), //same as symptomatic

	BREATHING_INHALATION_VOLUME = getRealModelParameter("BREATHING_INHALATION_VOLUME", 0.288 * 0.001 * 1.1 * 1.0); // JLJ Table SI-4 (classroom / prepandemic scenario) = B0 * (1/breaths per hour) * rB * fI

// Contact tracing parameters
parameter Real
	SYMPTOMATIC_DETECTION_PROB = getRealModelParameter("SYMPTOMATIC_DETECTION_PROB", 0.9),
	ASYMPTOMATIC_RANDOM_CONTACT_PROB = getRealModelParameter("ASYMPTOMATIC_RANDOM_CONTACT_PROB", 0.01),
	L1_SYNTOMATIC_DETECTION_PROB = getRealModelParameter("L1_SYNTOMATIC_DETECTION_PROB", 0.9),
	L2_SYNTOMATIC_DETECTION_PROB = getRealModelParameter("L2_SYNTOMATIC_DETECTION_PROB", 0.9),

	TEST_DELAY_TIME_MIN = 0.06,
	TEST_DELAY_TIME_MAX = 0.12,
	TESTED_POSITIVE_ISOLATION_TIME = getRealModelParameter("TESTED_POSITIVE_ISOLATION_TIME", 14.0),
	
	CONTACT_REMEMBER_PROB_BASE = 0.95,
	CONTACT_REMEMBER_PROB_DECAY = 0.95,
	
	L1_CONTACT_DESIRED_COUNT = getRealModelParameter("L1_CONTACT_DESIRED_COUNT", 0),
	L1_CONTACT_SUCESS_PROB = 0.95,
	L1_CONTACT_DELAY_MIN = getRealModelParameter("L1_CONTACT_DELAY_MIN", 0.5),
	L1_CONTACT_DELAY_MAX = getRealModelParameter("L1_CONTACT_DELAY_MAX", 1.0),
	L1_ISOLATION_TIME = getRealModelParameter("L1_ISOLATION_TIME", 20.0),
	
	L2_CONTACT_DESIRED_COUNT = getRealModelParameter("L2_CONTACT_DESIRED_COUNT", 0),
	L2_CONTACT_SUCESS_PROB = 0.95,
	L2_CONTACT_DELAY_MIN = getRealModelParameter("L2_CONTACT_DELAY_MIN", 1.0),
	L2_CONTACT_DELAY_MAX = getRealModelParameter("L2_CONTACT_DELAY_MAX", 2.0),
	L2_ISOLATION_TIME = getRealModelParameter("L2_ISOLATION_TIME", 20.0);


// Quarentine parameters
parameter Real
	QUARENTINE_START_WEEKS = getRealModelParameter("QUARENTINE_START_WEEKS", 2.0),
	QUARENTINE_DURATION_WEEKS = getRealModelParameter("QUARENTINE_DURATION_WEEKS", 0.0),
	QUARENTINE_PAUSE_WEEKS = getRealModelParameter("QUARENTINE_PAUSE_WEEKS", 2.0),
	QUARENTINE_ACCEPTANCE_RATIO = getRealModelParameter("QUARENTINE_ACCEPTANCE_RATIO", 0.90);

// School scenario paremeters (non homogenous scenario)
parameter Real
	HALLWAY_CLOSED_SPACE = getRealModelParameter("HALLWAY_CLOSED_SPACE", 0.0),
	VOLUME_HALLWAY_CONCENTRATION_LAMBDA = getRealModelParameter("VOLUME_HALLWAY_CONCENTRATION_LAMBDA", VOLUME_CONCENTRATION_LAMBDA),
	VOLUME_DIFFERENT_CLASSROOM_CONCENTRATION_LAMBDA = getRealModelParameter("VOLUME_DIFFERENT_CLASSROOM_CONCENTRATION_LAMBDA", VOLUME_CONCENTRATION_LAMBDA),
	BREAK_ENHANCEMENT_FACTOR = getRealModelParameter("BREAK_ENHANCEMENT_FACTOR", 1.0),
	TIME_BEFORE_BREAK = getRealModelParameter("TIME_BEFORE_BREAK", 1.25),
	BREAK_DURATION = getRealModelParameter("BREAK_DURATION", 0.25),
	BREAK_MOVEMENT_DURATION = 2.0 / 60.0;

// Output delta time parameter
parameter Real
	OUTPUT_UPDATE_DT = getRealModelParameter("OUTPUT_UPDATE_DT", 0.01);


// Grid constant
constant Integer
    VOLUMES_COUNT = GRID_DIVISIONS * GRID_DIVISIONS;


// School scenario paremeters (non homogenous scenario)
parameter Real
	INF = 1e20,
	EPS = 1e-5,
	PI = 3.1415926,
	PROGRESS_UPDATE_DT = 10.0,
	GRID_SIZE = getRealModelParameter("GRID_SIZE", 1.0),
	CELL_EDGE_LENGTH = GRID_SIZE / GRID_DIVISIONS,
	Z_COORD = CELL_EDGE_LENGTH / 2.0; 

/*
  Model variables
*/

// Particles position and movements variables
Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

// Volumes concentrations variables
Real volumeConcentration[VOLUMES_COUNT], volumeEmissionRate[VOLUMES_COUNT], volumeLambda[VOLUMES_COUNT];


/*
  Time array variables used on triggering events with "when" statements
*/

// This variable is used to simulate particles brething (they all breath at the same periodic time)
discrete Real nextBreathingTick;

// Disease evolution time-events variables
discrete Real expositionTime[N];
discrete Real infectionStartTime[N];
discrete Real symptomsStartTime[N];
discrete Real infectionFinishTime[N];

// Contact tracing time-events variables
discrete Real testResultTime[N];
discrete Real isolationFinishTime[N];
discrete Real level1ContactTime[N];
discrete Real level2ContactTime[N];

// Variables used to control and trigger quarentines
discrete Real quarentineStartTime, quarentineFinishTime;

// Variable used to control and trigger class breaks in the school scenario
discrete Real nextBreakTime;

// Useful variable for stopping the simulation when there is no more need to run
// Also it is used to set a maximum running time when developing or debugging
discrete Real terminateTime;


// Variables used to track statistics of infections, recoveries and isolations. These values are printed in the CSV output
discrete Integer infectedCount, recoveredCount, infectionsCount, isolatedCount;

// Variable used to control and trigger periodic output
discrete Real nextOutputTick;

// Variable used to control and trigger progress output in the terminal
discrete Real nextProgressTick;

// local variables
discrete Real _, normalX, normalY, ux, uy, volumeID;
discrete Boolean isolate;


initial algorithm

	// sets the log level from the config file
    _ := setDebugLevel(getIntegerModelParameter("LOG_LEVEL", INFO()));
    _ := debug(INFO(), time, "Starting initial algorithm", _, _, _, _);

	// sets the random seed from the config file
	_ := random_reseed(RANDOM_SEED);

	// sets several contagion and tracing constans from the config file
	_ := setContagionConstants(SUSCEPTIBLE(), UNKNOWN(), PRE_SYMPTOMATIC(), PRESYMPTOMATIC_CONTAGION_PROB, SYMPTOMATIC(),
		SYMPTOMATIC_CONTAGION_PROB, ASYMPTOMATIC(), ASYMPTOMATIC_CONTAGION_PROB, PARTICLE_TO_PARTICLE_INFECTION_PROB);
	_ := setContactsTracingConstants(1, L1_CONTACT_DESIRED_COUNT, CONTACT_REMEMBER_PROB_BASE, CONTACT_REMEMBER_PROB_DECAY, 
		L1_CONTACT_DELAY_MIN, L1_CONTACT_DELAY_MAX, L1_CONTACT_SUCESS_PROB);
	_ := setContactsTracingConstants(2, L2_CONTACT_DESIRED_COUNT, CONTACT_REMEMBER_PROB_BASE, CONTACT_REMEMBER_PROB_DECAY, 
		L2_CONTACT_DELAY_MIN, L2_CONTACT_DELAY_MAX, L2_CONTACT_SUCESS_PROB);
    _ := debug(INFO(), time, "Grid setup. Divisions = %d", GRID_DIVISIONS, _, _, _);

	// setup the grid in RETQSS as a simple grid using the constants from config file
    _ := geometry_gridSetUp(GRID_DIVISIONS, GRID_DIVISIONS, 1, CELL_EDGE_LENGTH);

	// setup the initial volumes properties considering the scenario set in the config file: homogenous or school
    for i in 1:VOLUMES_COUNT loop
		volumeConcentration[i] := 0.0;
		volumeEmissionRate[i] := 0.0;
		_ := volume_setProperty(i, "particleSpeed", DEFAULT_SPEED);
		_ := volume_setProperty(i, "particleEmissionRate", SYMPTOMATIC_EMISSION_RATE);
		_ := volume_setProperty(i, "particleBreathingInhalationVolume", BREATHING_INHALATION_VOLUME);

		if GRID_SCENARIO == 0 then
			_ := volume_setProperty(i, "isClosedSpace", 1);
  			_ := volume_setProperty(i, "isBlock", 0);
			volumeLambda[i] := VOLUME_CONCENTRATION_LAMBDA;
        elseif GRID_SCENARIO == 1 then // school floor scenario
        	_ := setClassroomBlock(i, 1);
        	if isSchoolHallway(i) == true then
				volumeLambda[i] := VOLUME_HALLWAY_CONCENTRATION_LAMBDA;
        		_ := volume_setProperty(i, "isClosedSpace", HALLWAY_CLOSED_SPACE);
			_ := volume_setProperty(i, "particleSpeed", random(0, DEFAULT_SPEED * 3));
			_ := volume_setProperty(i, "particleEmissionRate", SYMPTOMATIC_EMISSION_RATE*BREAK_ENHANCEMENT_FACTOR);
			_ := volume_setProperty(i, "particleBreathingInhalationVolume", BREATHING_INHALATION_VOLUME*BREAK_ENHANCEMENT_FACTOR);
        	else
        		if i == 15 then
 			//if (i == 15 or i == 17) then
 			//if (i == 15 or i == 17 or i == 19) then
 			//if (i == 15 or i == 17 or i == 19 or i == 21) then
 			//if (i == 15 or i == 17 or i == 19 or i == 21 or i == 29) then
 			//if (i == 15 or i == 17 or i == 19 or i == 21 or i == 29 or i == 31) then
 			//if (i == 15 or i == 17 or i == 19 or i == 21 or i == 29 or i == 31 or i == 33) then
 			//if (i == 15 or i == 17) or (i == 19 or i == 21) or (i == 29 or i == 31) or (i == 33 or i == 35) then
					volumeLambda[i] := VOLUME_DIFFERENT_CLASSROOM_CONCENTRATION_LAMBDA;
				else
					volumeLambda[i] := VOLUME_CONCENTRATION_LAMBDA;
				end if;
        		_ := volume_setProperty(i, "isClosedSpace", 1);
        	end if;
        end if;
    end for;

	// setup the initial particle position considering the scenario set in the config file: homogenous or school
	for i in 1:N loop
		if GRID_SCENARIO == 0 then
        	(x[i], y[i], z[i]) := randomXYPoint(GRID_SIZE, Z_COORD);
        elseif GRID_SCENARIO == 1 then
            (x[i], y[i], z[i]) := randomXYClassroom(GRID_SIZE/7, Z_COORD);
        end if;
    end for;

	// setup the particles in RETQSS
    _ := debug(INFO(), time, "Particles setup. N = %d", N,_,_,_);
	_ := setUpParticles(N, CELL_EDGE_LENGTH, GRID_DIVISIONS, x);

	// setup the particles initial state
    infectedCount := 0;
	for i in 1:N loop
        	(vx[i], vy[i], vz[i]) := randomXYVelocity(DEFAULT_SPEED, SUPERSPREADER_PROB, SUPERSPREADER_ACCELERATION, NO_VELOCITY_PROB);
		if i <= INITIAL_EXPOSED then
			expositionTime[i] := EPS;
		else
			expositionTime[i] := INF;
		end if;
		if i <= INITIAL_INFECTED then
			infectionStartTime[i] := EPS;
			_ := particle_setProperty(i, "status", EXPOSED());
			infectedCount := infectedCount + 1;
		else
			infectionStartTime[i] := INF;
			_ := particle_setProperty(i, "status", SUSCEPTIBLE());
		end if;
		symptomsStartTime[i] := INF;
		infectionFinishTime[i] := INF;
		testResultTime[i] := INF;
		level1ContactTime[i] := INF;
		level2ContactTime[i] := INF;
		isolationFinishTime[i] := INF;
		_ := particle_setProperty(i, "trackingStatus", UNKNOWN());
        _ := particle_setProperty(i, "infectionsCount", 0);
        _ := particle_setProperty(i, "acceptsQuarentine", randomBoolean(QUARENTINE_ACCEPTANCE_RATIO));
        _ := particle_setProperty(i, "volumeCrossingLastId", particle_currentVolumeID(i));
        _ := particle_setProperty(i, "volumeCrossingTime", 0.0);
        _ := particle_setProperty(i, "volumeCrossingConcentration", 0.0);
		_ := setRandomDiseaseOutcomesProperties(i, YOUNG_PROB, ADULT_PROB,
			YOUNG_SYMPTOMATIC_PROB, ADULT_SYMPTOMATIC_PROB, OLD_SYMPTOMATIC_PROB,
			YOUNG_DEATH_PROB,ADULT_DEATH_PROB,OLD_DEATH_PROB,
			INCUBATION_TIME_M, INCUBATION_TIME_S, PRESYMPTOMATIC_TIME_M, PRESYMPTOMATIC_TIME_S, SYMPTOMATIC_TIME_M, SYMPTOMATIC_TIME_S,
			SYMPTOMATIC_EMISSION_RATE, ASYMPTOMATIC_EMISSION_RATE, BREATHING_INHALATION_VOLUME);
		_ := particle_setProperty(i, "enteredVolumesCount", 0.0);
		_ := particle_setProperty(i, "bouncesCount", 0.0);
		_ := particle_setProperty(i, "initialX", x[i]);
		_ := particle_setProperty(i, "initialY", y[i]);
		_ := particle_setProperty(i, "initialVX", vx[i]);
		_ := particle_setProperty(i, "initialVY", vy[i]);
    end for;

	// initialize statistics tracking variables
    recoveredCount := 0;
    infectionsCount := 0;
    isolatedCount := 0;

	// initialize global time-triggers variables
	if QUARENTINE_DURATION_WEEKS > 0 then
	    quarentineStartTime := QUARENTINE_START_WEEKS * 7.0 + EPS;
	end if;
    quarentineFinishTime := 0;
    nextBreakTime := TIME_BEFORE_BREAK;
    nextOutputTick := EPS;
    nextProgressTick := EPS;
    nextBreathingTick := EPS;
    terminateTime := FORCE_TERMINATION_AT;
    _ := debug(INFO(), time, "Done initial algorithm",_,_,_,_);
    
/*
  Model's diferential equations: for particles movements and volumes concentration
*/
equation
    // newtonian position/velocity equations for each particle
    for i in 1:N loop
        der(x[i])  = vx[i];
        der(y[i])  = vy[i];
        der(z[i])  = vz[i];
        der(vx[i]) = 0.;
        der(vy[i]) = 0.;
        der(vz[i]) = 0.;
    end for;

    // box model concentration equations for each volume
    for i in 1:VOLUMES_COUNT loop
        der(volumeConcentration[i]) = - volumeConcentration[i] * volumeLambda[i];
	der(volumeEmissionRate[i]) = 0.;
	der(volumeLambda[i]) = 0.;
    end for;


/*
  Model's time events
*/
algorithm

	for i in 1:N loop

		//EVENT: particle enters a volume (it may bounce or triggers disease/tracing logics implemented in the library) 
		when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
			(expositionTime, normalX, normalY) := onNextCross(time, i, CLOSE_CONTACT_PROB);
			if normalX <> 0.0 or normalY <> 0.0 then
				ux := vx[i];
				uy := vy[i];
				(ux, uy) := naturalBounceVelocity(normalX, normalY, ux, uy);
				reinit(vx[i], ux);
				reinit(vy[i], uy);
			end if;
		end when;

		//EVENT: particle is exposed
		when time > expositionTime[i] then
			infectedCount := infectedCount + 1;
			(infectionStartTime[i], _) := onExposition(time, i);
		end when;
		
		//EVENT: particle infection start
		when time > infectionStartTime[i] then
			(infectionFinishTime[i], symptomsStartTime[i], testResultTime[i]) := onInfectionStart(time, i, ASYMPTOMATIC_RANDOM_CONTACT_PROB);
		end when;
		
		//EVENT: particle symptoms start (it may trigger the particle stop moving if the isolation starts)
		when time > symptomsStartTime[i] then
			(infectionFinishTime[i], testResultTime[i], level1ContactTime) := onSymptomsStart(time, i,
				SYMPTOMATIC_DETECTION_PROB, L1_SYNTOMATIC_DETECTION_PROB, L2_SYNTOMATIC_DETECTION_PROB,
				TEST_DELAY_TIME_MIN, TEST_DELAY_TIME_MAX);
			if not shouldMove(i) then
           		reinit(vx[i], 0.);
				reinit(vy[i], 0.);
		    	_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
				isolatedCount := isolatedCount + 1;
			end if;
		end when;
		
		//EVENT: particle infection finished (it may trigger the particle stop moving if died)
        when time > infectionFinishTime[i] then
			_ := onInfectionEnd(time, i);
			recoveredCount := recoveredCount + 1;
			infectionsCount := infectionsCount + particle_getProperty(i, "infectionsCount");
			if not shouldMove(i) then
         		reinit(vx[i], 0.);
				reinit(vy[i], 0.);
		    	_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
			end if;
        end when;

		//EVENT: particle test result arrived (it may trigger the particle if isolated and other tracing logics implemented in the library)
		when time > testResultTime[i] then
			(isolationFinishTime[i], _) := onTestResult(time, i, infectionFinishTime[i], TESTED_POSITIVE_ISOLATION_TIME);
			if shouldMove(i) then
		        (ux, uy) := randomXYVolumeVelocity(i);
			else
				ux := 0.;
				uy := 0.;
				isolatedCount := isolatedCount + 1;
			end if;
	        reinit(vx[i], ux);
			reinit(vy[i], uy);
			_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
		end when;

		//EVENT: particle was contacted as a level 1 case. The particle is stopped as it will be isolated.
		when time > level1ContactTime[i] then
			(isolationFinishTime[i], level2ContactTime) := onLevel1Contact(time, i, L1_ISOLATION_TIME);
     		reinit(vx[i], 0.);
			reinit(vy[i], 0.);
	    	_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
			isolatedCount := isolatedCount + 1;
		end when;

		//EVENT: particle was contacted as a level 2 case. The particle is stopped as it will be isolated.
		when time > level2ContactTime[i] then
			isolationFinishTime[i] := onLevel2Contact(time, i, L2_ISOLATION_TIME);
     		reinit(vx[i], 0.);
			reinit(vy[i], 0.);
	    	_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
			isolatedCount := isolatedCount + 1;
		end when;

		//EVENT: particle isolation is finished. It will start moving again in a random direction.
		when time > isolationFinishTime[i] then
			_ := onIsolationFinish(time, i);
			isolatedCount := isolatedCount - 1;
			if shouldMove(i) then
		        (ux, uy) := randomXYVolumeVelocity(i);
				reinit(vx[i], ux);
				reinit(vy[i], uy);
		    	_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
			end if;
		end when;

    end for;

	//EVENT: all particles breath. New emissions rate are computed and particles may become infected. 
	when time > nextBreathingTick then
		//nextBreathingTick := random(0.0, 0.01);
		//nextBreathingTick := time + nextBreathingTick;
		nextBreathingTick := time + BREATHING_INTERVAL_TIME;
		for i in 1:VOLUMES_COUNT loop
			_ := volume_setProperty(i, "newVolumeEmissionRate", 0);
		end for;
		for i in 1:N loop
			if onBreathe(time, i, volumeConcentration) then
				expositionTime[i] := time + EPS;
			end if;
		end for;
		for i in 1:VOLUMES_COUNT loop
			reinit(volumeConcentration[i], volume_getProperty(i, "newVolumeEmissionRate")*BREATHING_INTERVAL_TIME+volumeConcentration[i]);
		end for;
	end when;

	//EVENT: Quarentine starts. Particles are isolated according to their "acceptsQuarentine" property.
	when time > quarentineStartTime then
		_ := debug(INFO(), time, "Quarentine start", _, _, _, _);
		if QUARENTINE_PAUSE_WEEKS > 0 then
			quarentineFinishTime := quarentineStartTime + QUARENTINE_DURATION_WEEKS * 7.0 + EPS;
		end if;
	    for i in 1:N loop
			if particle_getProperty(i, "acceptsQuarentine") then
				_ := particle_setProperty(i, "vx", vx[i]);
		        _ := particle_setProperty(i, "vy", vy[i]);
				reinit(vx[i], 0);
				reinit(vy[i], 0);
				_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
		    end if;
	    end for;
	end when;

	//EVENT: Quarentine finished. Isolated particles will resume movement
	when time > quarentineFinishTime then
		_ := debug(INFO(), time, "Quarentine finished", _, _, _, _);
		if QUARENTINE_DURATION_WEEKS > 0 then
 		    quarentineStartTime := quarentineFinishTime + QUARENTINE_PAUSE_WEEKS * 7.0 + EPS;
		end if;
	    for i in 1:N loop
			if particle_getProperty(i, "acceptsQuarentine") then
				reinit(vx[i], particle_getProperty(i, "vx"));
				reinit(vy[i], particle_getProperty(i, "vy"));
				_ := particle_relocate(i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
		    end if;
	    end for;
	end when;

	//EVENT: Next break starts / part 1. All particles starts moving to the classroom door
	when time > nextBreakTime then
		if GRID_SCENARIO == 1 then
			_ := debug(INFO(), time, "On break, going to the door", _,_,_,_);
    		for i in 1:VOLUMES_COUNT loop
    			_ := setClassroomBlock(i, 0);
		    end for;
		    for i in 1:N loop
		    	(ux, uy) := getClassroomDoor(i, CELL_EDGE_LENGTH);
		    	reinit(vx[i], (ux-x[i])/BREAK_MOVEMENT_DURATION/1.02);
		    	reinit(vy[i], (uy-y[i])/BREAK_MOVEMENT_DURATION/1.02);

		    end for;
		end if;
	end when;
	//EVENT: Next break starts / part 2. All particles arrived the classroom door and will start moving randomly in the hallway
	when time > nextBreakTime + BREAK_MOVEMENT_DURATION then
		if GRID_SCENARIO == 1 then
				_ := debug(INFO(), time, "All in the hallway", _,_,_,_);
			    for i in 1:N loop
			    	ux := vx[i];
			    	uy := vy[i];
			        (ux, uy) := randomXYVolumeVelocity(i);
			    	reinit(vx[i], ux);
			    	reinit(vy[i], uy);
				volumeID := particle_currentVolumeID(i);
				_ := particle_setProperty(i, "emissionRate", volume_getProperty(volumeID, "particleEmissionRate"));
 				_ := particle_setProperty(i, "breathingInhalationVolume", volume_getProperty(volumeID, "particleBreathingInhalationVolume"));
			    end for;
		end if;
	end when;
	//EVENT: Current break finishes / part 1. All particles starts moving their corresponding classroom door
	when time > nextBreakTime + BREAK_DURATION then
		if GRID_SCENARIO == 1 then
			_ := debug(INFO(), time, "Finished break, reroute to door", _,_,_,_);
    		for i in 1:VOLUMES_COUNT loop
				if (i == 15 or i == 17) or (i == 19 or i == 21) or (i == 29 or i == 31) or (i == 33 or i == 35) then
					_ := volume_setProperty(i, "isBlock", 0);
				end if;
			end for;
		    for i in 1:N loop
		    	(ux, uy) := getClassroomDoor(i, CELL_EDGE_LENGTH);
		    	reinit(vx[i], (ux-x[i])/BREAK_MOVEMENT_DURATION/1.02);
		    	reinit(vy[i], (uy-y[i])/BREAK_MOVEMENT_DURATION/1.02);
		    end for;
		end if;
	end when;
	//EVENT: Current break finishes / part 2. All particles arrived their corresponding classroom door and starts moving towards its original place
	when time > nextBreakTime + BREAK_DURATION + BREAK_MOVEMENT_DURATION then
		if GRID_SCENARIO == 1 then
			_ := debug(INFO(), time, "Finished break, reroute to initial point", _,_,_,_);
		    for i in 1:N loop
		    	ux := particle_getProperty(i, "initialX") - x[i];
		    	uy := particle_getProperty(i, "initialY") - y[i];
		    	reinit(vx[i], ux/BREAK_MOVEMENT_DURATION);
		    	reinit(vy[i], uy/BREAK_MOVEMENT_DURATION);
		    end for;
		end if;
	end when;
	//EVENT: Current break finishes / part 2. All particles arrived at its original place and will start to move randomly
	when time > nextBreakTime + BREAK_DURATION + BREAK_MOVEMENT_DURATION * 2 then
		if GRID_SCENARIO == 1 then
			_ := debug(INFO(), time, "Finished break, close doors and start moving", _,_,_,_);
    		for i in 1:VOLUMES_COUNT loop
    			_ := setClassroomBlock(i, 1);
			end for;
		    for i in 1:N loop
		    	reinit(vx[i], particle_getProperty(i, "initialVX"));
		    	reinit(vy[i], particle_getProperty(i, "initialVY"));
			volumeID := particle_currentVolumeID(i);
			_ := particle_setProperty(i, "emissionRate", volume_getProperty(volumeID, "particleEmissionRate"));
 			_ := particle_setProperty(i, "breathingInhalationVolume", volume_getProperty(volumeID, "particleBreathingInhalationVolume"));
		    end for;
			nextBreakTime := nextBreakTime + TIME_BEFORE_BREAK + BREAK_DURATION;
		end if;
	end when;

	//EVENT: Next CSV output time: prints a new csv line and computes the next output time incrementing the variable
	when time > nextOutputTick then
		_ := outputCSV(time, N, x, y, VOLUMES_COUNT, volumeConcentration, recoveredCount, infectionsCount);
		nextOutputTick := time + OUTPUT_UPDATE_DT;
		if recoveredCount == infectedCount and terminateTime == 0 then
			terminateTime := time + nextOutputTick;
		end if;
	end when;

	//EVENT: Terminate time is reached, calling native function terminate()
	when time > terminateTime then
		terminate();
	end when;

	//EVENT: Next progress output time: prints a new line in stdout and computes the next output time incrementing the variable
  	when time > nextProgressTick then
		_ := debug(INFO(), time, "Progress checkpoint: infected = %d / recovered = %d / infections = %.0f / isolations = %.0f", infectedCount, recoveredCount, infectionsCount, isolatedCount);
        nextProgressTick := time + PROGRESS_UPDATE_DT;
	end when;
	

annotation(
	experiment(
		MMO_Description="Indirect infection of particles interacting through volumes.",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_PartitionMethod=Metis,
		MMO_Scheduler=ST_Binary,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=1000.0,
		Tolerance={1e-5},
		AbsTolerance={1e-8}
	));

end covid19;
