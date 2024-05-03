package retQSS_covid19

import retQSS;
import retQSS_covid19_utils;
import retQSS_covid19_fsm;

/*
  This function is used to mirror disease's transmission parameters constants in C layer.
  These parameters like probabilities and status id are used when calling the external C function interactWithParticlesInVolume.
*/
function setContagionConstants
	input Integer susceptibleStatus;
	input Integer unknownTrackingStatus;
	input Integer presymptomaticStatus;
	input Real presymptomaticContagionProbability;
	input Integer symptomaticStatus;
	input Real symptomaticContagionProbability;
	input Integer asymptomaticStatus;
	input Real asymptomaticContagionProbability;
	input Real particleToParticleInfectionProbability;
	output Boolean _;
	external "C" _=covid19_setContagionConstants(susceptibleStatus,unknownTrackingStatus,presymptomaticStatus,presymptomaticContagionProbability,symptomaticStatus,symptomaticContagionProbability,asymptomaticStatus,asymptomaticContagionProbability,particleToParticleInfectionProbability) annotation(
	    Library="covid19",
	    Include="#include \"retqss_covid19.h\"");
end setStatusConstants;

/*
  This function is used to mirror contact tracing parameters constants in C layer.
  These parameters like contact counts, delays and probabilities are used when calling the external C function processContacts.
*/
function setContactsTracingConstants
	input Integer level;
	input Integer desiredContactsCount;
	input Real contactRememberedProbBase;
	input Real contactRememberedProbDecay;
	input Real delayTimeMin;
	input Real delayTimeMax;
	input Real successProbability;
	output Boolean status;
	external "C" status=covid19_setContactsTracingConstants(level, desiredContactsCount, contactRememberedProbBase, contactRememberedProbDecay, delayTimeMin, delayTimeMax, successProbability) annotation(
	    Library="covid19",
	    Include="#include \"retqss_covid19.h\"");
end setContactsTracingConstants;

/*
  Sets up retQSS-geometry and N particles in a square grid
*/
function setUpParticles
	input Integer N;
	input Real cellEdgeLength;
	input Integer gridDivisions;
	input Real x[1];
	output Boolean _;
	external "C" _=covid19_setUpParticles(N, cellEdgeLength, gridDivisions, x) annotation(
	    Library="covid19",
	    Include="#include \"retqss_covid19.h\"");
end setUpParticles;

/*
  Randomly sets the following desease's related outcome properties for a given particle:
    willHaveSymptoms, latencyTime, presymptomaticTime, symptomaticTime, willDie, emissionRate, breathingInhalationVolume
  This function is called at the beginning of the simulation in order to have repeatable results
*/
function setRandomDiseaseOutcomesProperties
	input Integer particleID;
	input Real youngProb;
	input Real adultProb;
	input Real youngSymptomaticProb;
	input Real adultSymptomaticProb;
	input Real oldSymptomaticProb;
	input Real youngDeathProb;
	input Real adultDeathProb;
	input Real oldDeathProb;
	input Real incubationTimeMu;
	input Real incubationTimeSigma;
	input Real presymptomaticTimeMu;
	input Real presymptomaticTimeSigma;
	input Real symptomaticTimeMu;
	input Real symptomaticTimeSigma;
	input Real symptomaticEmissionRate;
	input Real asymptomaticEmissionRate;
	input Real breathingInhalationVolume;
	output Boolean _;
protected
	Real symptomaticProb;
	Real dieProb;
	Boolean willDie;
	Real incubationTime;
	Real presymptomaticTime;
	Real symptomaticTime;
	Real latencyTime;
	Real emissionRate;
	Real efficiencyFactor;
	Real r;
algorithm
	r := random(0.0, 1.0);
	if r < youngProb then
		symptomaticProb := youngSymptomaticProb;
		dieProb := youngDeathProb;
	elseif r < youngProb + adultProb then
		symptomaticProb := adultSymptomaticProb;
		dieProb := adultDeathProb;
	else
		symptomaticProb := oldSymptomaticProb;
		dieProb := oldDeathProb;
	end if;
	_ := particle_setProperty(particleID, "willHaveSymptoms", randomBoolean(symptomaticProb));
	incubationTime := 0;//random_lognormal(incubationTimeMu, incubationTimeSigma);
	presymptomaticTime := 0;//random_normal(presymptomaticTimeMu, presymptomaticTimeSigma);
	if presymptomaticTime > incubationTime then
	    presymptomaticTime := incubationTime;
	end if;
	latencyTime := incubationTime - presymptomaticTime;
	_ := particle_setProperty(particleID, "latencyTime", latencyTime);
	_ := debug(DEBUG(), 0, "Incubation times for: %d. Latency: %f / Incubation: %f", particleID, _, latencyTime, incubationTime);
	symptomaticTime := 0;// random_normal(symptomaticTimeMu, symptomaticTimeSigma);
	if particle_getProperty(particleID, "willHaveSymptoms") then
		willDie := randomBoolean(dieProb/symptomaticProb);
		_ := particle_setProperty(particleID, "presymptomaticTime", presymptomaticTime);
		_ := particle_setProperty(particleID, "symptomaticTime", symptomaticTime);
		_ := debug(DEBUG(), 0, "Symptomatic times for: %d. Presymptomatic: %f / Symptomatic: %f", particleID, _, presymptomaticTime, symptomaticTime);
		emissionRate := symptomaticEmissionRate;
	else
		willDie := false;
		_ := particle_setProperty(particleID, "asymptomaticTime", presymptomaticTime + symptomaticTime);
		_ := debug(DEBUG(), 0, "Asymptomatic time for: %d. Asymptomatic: %f", particleID, _, particle_getProperty(particleID, "asymptomaticTime"), _);
		emissionRate := asymptomaticEmissionRate;
	end if;
    _ := particle_setProperty(particleID, "willDie", willDie);
	_ := particle_setProperty(particleID, "emissionRate", emissionRate);
	_ := particle_setProperty(particleID, "breathingInhalationVolume", breathingInhalationVolume);
end setRandomDiseaseOutcomesProperties;


/*
  Performs the person to person (non aerosol) contagion logic for a given particle P in a given volume V.
  This is an external C function because it needs to perform an iteration on a list, and the main logic is as follows
  - For each other particle Q in the volume V
    - Randomly choose whenever P and Q will have contact or not using closeContactProbability parameter
    - In case of contact
      - Add Q in P's contact list
      - Add P in Q's contact list
      - if P is susceptible and Q is contagious Q may infect P by setting incubationStartTime[P] = time
      - if Q is susceptible and P is contagious P may infect Q by setting incubationStartTime[Q] = time
*/
function interactWithParticlesInVolume
	input Real time;
	input Integer particleID;
	input Integer volumeID;
	input Real closeContactProbability;
	input Real incubationStartTime[1];
	output Boolean _;
	external "C" _=covid19_interactWithParticlesInVolume(time,particleID,volumeID,closeContactProbability,incubationStartTime) annotation(
	    Library="covid19",
	    Include="#include \"retqss_covid19.h\"");
end updateContacts;

/*
  Checks whenever a particle
    - enters a new volume and needs to interact with other particles in it
    - enters to the "outside volume" with id 0 and therefore needs to bounce.
  Returns the normal vector of the bouncing direction or (0,0) if not bounce was performed
*/
function onNextCross
	input Real time;
	input Integer particleID;
	input Real closeContactProbability;
	output Real expositionTime[1];
	output Real normalX;
	output Real normalY;
protected
	Integer volumeID;
	Boolean _;
algorithm
	volumeID := particle_currentVolumeID(particleID);
	normalX := 0;
	normalY := 0;
	if (volumeID == 0) or (volume_getProperty(volumeID, "isBlock") == 1) then
		_ := incrementProperty(particleID, "bouncesCount");
	    (normalX, normalY, _) := particle_crossedFaceNormal(particleID);
	else
		if particle_previousVolumeID(particleID) <> 0 then
			_ := incrementProperty(particleID, "enteredVolumesCount");
			_ := interactWithParticlesInVolume(time + EPS(), particleID, volumeID, closeContactProbability, expositionTime);
		end if;
	end if;
end onNextCross;

/*
  Performs a given particle breathing logic with Wellss Riley infection probability model
    - if the particle is infected on a closed volume it will emit infected quanta according to it's emissonRate property
    - if the particle is susceptible it may become randomly infected with 1 - exp(quantum) probability
*/
function onBreathe
	input Real time;
	input Integer particleID;
	input Real volumeConcentration[1];
	output Boolean infected;
protected
	Integer particleStatus;
	Integer particleTrackingStatus;
	Integer volumeID;
	Boolean isClosedSpace;
	Real emissionRate;
	Real volumeCapacity;
	Real breathingInhalationVolume;
	Real concentration;
	Real n;
	Real P;
	Integer infecterID;
	Boolean _;
algorithm
	_ := debug(DEBUG(), time, "Breath from %d %d %.4f", particleID, _, _, _);
	particleTrackingStatus := particle_getProperty(particleID, "trackingStatus");
	infected := false;
	if particleTrackingStatus == UNKNOWN() then
		particleStatus := particle_getProperty(particleID, "status");
		volumeID := particle_currentVolumeID(particleID);
		if volumeID <> 0 then
			isClosedSpace := volume_getProperty(volumeID, "isClosedSpace");
			if isClosedSpace and (particleStatus == PRE_SYMPTOMATIC() or particleStatus == PRE_SYMPTOMATIC() or particleStatus == ASYMPTOMATIC()) then
				volumeCapacity := volume_capacity(volumeID);
				emissionRate := particle_getProperty(particleID, "emissionRate") / volumeCapacity;
				_ := volume_setProperty(volumeID, "newVolumeEmissionRate", volume_getProperty(volumeID, "newVolumeEmissionRate") + emissionRate);
				_ := volume_setProperty(volumeID, "lastInfecterParticleID", particleID);
				_ := debug(DEBUG(), time, "Exhalation from %d in volume %d with rate %f and volume capacity: %f", particleID, volumeID, emissionRate, volumeCapacity);
			elseif particleStatus == SUSCEPTIBLE() then
				breathingInhalationVolume := particle_getProperty(particleID, "breathingInhalationVolume");
				concentration := equationArrayGet(volumeConcentration[1], volumeID);
				n := concentration * breathingInhalationVolume;
				P := 1.0 - exp(-n);
				_ := debug(DEBUG(), time, "Inhalation from %d in %d with c=%f and P=%f", particleID, volumeID, concentration, P);
				if randomBoolean(P) then // Wells Riley infection probability model
					infecterID := volume_getProperty(volumeID, "lastInfecterParticleID");
					_ := debug(DEBUG(), time, "Infection from %d to %d thorough volume %.0f", infecterID, particleID, volumeID, _);
					_ := incrementProperty(infecterID, "infectionsCount");
					infected := true;
				end if;
			end if;
		end if;
	end if;
end onBreathe;

/*
  Performs a given particle exposition event by setting the infectionStartTime according to the particle's latencyTime
*/
function onExposition
	input Real time;
	input Integer particleID;
	output Real infectionStartTime;
	output Boolean _;
protected
	Integer status;
algorithm
	_ := debug(DEBUG(), time, "Start incubation %d", particleID, EMPTY(),EMPTY(),EMPTY());
	status := particle_getProperty(particleID, "status");
	if status == SUSCEPTIBLE() then
		status := particle_setProperty(particleID, "status", EXPOSED());
		infectionStartTime := time + particle_getProperty(particleID, "latencyTime");
	end if;
end onExposition;

/*
  Performs a given particle infection start event by setting the infection finish time
  If the particle will have symptoms it also sets the symptoms start time.
*/
function onInfectionStart
	input Real time;
	input Integer particleID;
	input Real asymptomaticDetectionProbability;
	output Real infectionFinishTime;
	output Real symptomsStartTime;
	output Real testResultTime;
protected
	Boolean _;
	Integer status;
	Real infectionTime;
algorithm
	_ := debug(DEBUG(), time, "Started infection %d", particleID, _, _, _);
	if particle_getProperty(particleID, "status") == EXPOSED() then
	    if particle_getProperty(particleID, "willHaveSymptoms") then
	    	status := PRE_SYMPTOMATIC();
			symptomsStartTime := time + particle_getProperty(particleID, "presymptomaticTime");
    	else
	    	status := ASYMPTOMATIC();
			infectionTime := particle_getProperty(particleID, "asymptomaticTime");
			if randomBoolean(asymptomaticDetectionProbability) then
				testResultTime := time + random(0, infectionTime);
			end if;
			infectionFinishTime := time + infectionTime;
    	end if;
		_ := particle_setProperty(particleID, "status", status);
	end if;
end onInfectionStart;

/*
  Return the number of contacts for a given particle.
  External C function.
*/
function getContactsCount
	input Integer particleID;
	output Integer count;
	external "C" count=covid19_getContactsCount(particleID) annotation(
	    Library="covid19",
	    Include="#include \"retqss_covid19.h\"");
end getContactsCount;

/*
  Process the contacts tracing logic for a given particle P and a given contact tracing level L = {1, 2}
  This is an external C function because it needs to perform an iteration on a list, and the main logic is as follows
  - From all contacts Q of P from newer to older
    - Randomly choose if the P remembers being in contact with Q using "contact remember probability"
    - If P remembers Q, then Q will be traced with a "tracing probability success" and set its contactTime
  - Each level (1 or 2) have its own
    - remembering probability (exponential decay)
    - tracing success probability
    - contact tracing list length
*/
function processContacts
	input Integer level;
	input Real time;
	input Integer particleID;
	input Real contactTimeArray[1];
	output Integer tracedContacts;
	external "C" tracedContacts=covid19_traceContacts(level, time, particleID, contactTimeArray) annotation(
	    Library="covid19",
	    Include="#include \"retqss_covid19.h\"");
end processContacts;

/*
  Performs symptoms start event for a given particle:
  - changes the infection FSM to SYMPTOMATIC
  - sets the infectionFinishTime
  - schedules a test by setting testResultTime[particleID]
  - process its contacts as level 1 contact tracing
*/
function onSymptomsStart
	input Real time;
	input Integer particleID;
	input Real symptomaticDetectionProbability;
	input Real level1SymptomaticDetectionProbability;
	input Real level2SymptomaticDetectionProbability;
	input Real testDelayTimeMin;
	input Real testDelayTimeMax;
	output Real infectionFinishTime;
	output Real testResultTime;
	output Real level1ContactTime[1];
protected
	Boolean _;
	Integer status;
	Integer trackingStatus;
	Real detectionProbability;
	Integer tracedContacts;
algorithm
	_ := debug(DEBUG(), time, "Started symptoms %d", particleID, _, _, _);
	status := particle_getProperty(particleID, "status");
	if status == PRE_SYMPTOMATIC() then
		_ := particle_setProperty(particleID, "status", SYMPTOMATIC());
		infectionFinishTime := time + particle_getProperty(particleID, "symptomaticTime");
		trackingStatus := particle_getProperty(particleID, "trackingStatus");
		if trackingStatus == LEVEL_1_CONTACT() then
			detectionProbability := level1SymptomaticDetectionProbability;
		elseif trackingStatus == LEVEL_2_CONTACT() then
			detectionProbability := level2SymptomaticDetectionProbability;
		else
			detectionProbability := symptomaticDetectionProbability;
		end if;
		if randomBoolean(detectionProbability) then
			_ := particle_setProperty(particleID, "trackingStatus", SUSPECTED());
			testResultTime := time + random(testDelayTimeMin, testDelayTimeMax);
			tracedContacts := processContacts(1, time, particleID, level1ContactTime);
			_ := debug(DEBUG(), time, "Suspected id: %d / traced contacts: %d / infections: %.0f", particleID, tracedContacts, particle_getProperty(particleID, "infectionsCount"), _);
			_ := debug(DEBUG(), time, "Suspected id: %d / contacts: %d / volumes: %.0f / bounces: %.0f", particleID, getContactsCount(particleID), particle_getProperty(particleID, "enteredVolumesCount"), particle_getProperty(particleID, "bouncesCount"));
		end if;
	end if;
end onSymptomsStart;

/*
  Performs infection end event for a given particle by changing the infection FSM to RECOVERED or DEAD
*/
function onInfectionEnd
	input Real time;
	input Integer particleID;
	output Boolean dead;
protected
	Integer newStatus;
	Boolean _;
algorithm
	dead := particle_getProperty(particleID, "willDie");
	if dead then
		newStatus := DEAD();
	else
		newStatus := RECOVERED();
	end if;
	_ := particle_setProperty(particleID, "status", newStatus);
	_ := debug(DEBUG(), time, "Finished infection %d (dead = %d)", particleID, dead, _, _);
end onInfectionEnd;

/*
  Performs test result event for a given particle by changing contact tracing FSM
    - If the particle is infected, test result will be positive and the particle will isolate
    - If the particle is not infected, it will be released from all isolation / tracking
*/
function onTestResult
	input Real time;
	input Integer particleID;
	input Real infectionFinishTime;
	input Real asymptomaticIsolationTime;
	output Real isolationFinishTime;
	output Boolean _;
protected
	Integer status;
	Integer newStatus;
algorithm
	status := particle_getProperty(particleID, "status");
	_ := debug(DEBUG(), time, "Test result for %d status %d", particleID, status,EMPTY(),EMPTY());
	if status == PRE_SYMPTOMATIC() or status == SYMPTOMATIC() or status == ASYMPTOMATIC() then
		newStatus := TESTED_POSITIVE();
		if status == PRE_SYMPTOMATIC() or status == SYMPTOMATIC() then
			isolationFinishTime := time + asymptomaticIsolationTime;
		else
			isolationFinishTime := infectionFinishTime + EPS();
		end if;
	else
		newStatus := UNKNOWN();
	end if;
	_ := particle_setProperty(particleID, "trackingStatus", newStatus);
end onTestResult;

/*
  Performs level 1 contact event for a given particle:
    - Update contact tracing FSM to LEVEL_1 status
    - process the particle's contact as level 2 tracing
*/
function onLevel1Contact
	input Real time;
	input Integer particleID;
	input Real isolationTime;
	output Real isolationFinishTime;
	output Real level2ContactTime[1];
protected
	Integer tracedContacts;
	Boolean _;
algorithm
	_ := particle_setProperty(particleID, "trackingStatus", LEVEL_1_CONTACT());
	tracedContacts := processContacts(2, time, particleID, level2ContactTime);
	_ := debug(DEBUG(), time, "Level 1 contact for %d. traced contacts: %d", particleID, tracedContacts, _, _);
	isolationFinishTime := time + isolationTime;
end onLevel1Contact;

/*
  Performs level 2 contact event for a given particle by setting contact tracing FSM to LEVEL_2 status
*/
function onLevel2Contact
	input Real time;
	input Integer particleID;
	input Real isolationTime;
	output Real isolationFinishTime;
protected
	Boolean _;
algorithm
	_ := particle_setProperty(particleID, "trackingStatus", LEVEL_2_CONTACT());
	_ := debug(DEBUG(), time, "Level 2 contact for %d", particleID, _, _, _);
	isolationFinishTime := time + isolationTime;
end onLevel1Contact;

/*
  Performs isolation finish event for a given particle by updating contact tracing FSM to REMOVED status
*/
function onIsolationFinish
	input Real time;
	input Integer particleID;
	output Boolean _;
algorithm
	_ := debug(DEBUG(), time, "Isolation finish for %d", particleID, EMPTY(),EMPTY(),EMPTY());
	_ := particle_setProperty(particleID, "trackingStatus", REMOVED());
end onTestResult;

/*
  Returns whenever a particle should move or not for being isolated or dead.
*/
function shouldMove
	input Integer particleID;
	output Boolean result;
protected
	Integer status;
algorithm
	status := particle_getProperty(particleID, "status");
	if status == DEAD() then
		result := false;
	else
		status := particle_getProperty(particleID, "trackingStatus");
		if status == UNKNOWN() or status == REMOVED() then
			result := true;
		else
			result := false;
		end if;
	end if;
end shouldMove;

end retQSS_covid19;
