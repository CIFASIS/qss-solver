package retQSS_covid19_fsm
/*
  This package defines the states of both FSMs used in the model
  SEIR-like model states: SUSCEPTIBLE, EXPOSED, PRE_SYMPTOMATIC, ASYMPTOMATIC, RECOVERED, DEAD
  Contact tracing model states: UNKNOWN, SUSPECTED, TESTED_POSITIVE, LEVEL_1_CONTACT, LEVEL_2_CONTACT, REMOVED
  
  The states are defined as a enums
*/


// SEIR-like model states 
function SUSCEPTIBLE 		output Integer state; algorithm state := 0; end SUSCEPTIBLE;
function EXPOSED 			output Integer state; algorithm state := 1; end EXPOSED;
function PRE_SYMPTOMATIC 	output Integer state; algorithm state := 2; end PRE_SYMPTOMATIC;
function SYMPTOMATIC		output Integer state; algorithm state := 3; end SYMPTOMATIC;
function ASYMPTOMATIC		output Integer state; algorithm state := 4; end ASYMPTOMATIC;
function RECOVERED			output Integer state; algorithm state := 5; end RECOVERED;
function DEAD				output Integer state; algorithm state := 6; end DEAD;


// Contact tracing model states
function UNKNOWN			output Integer state; algorithm state := 0; end UNKNOWN;
function SUSPECTED			output Integer state; algorithm state := 1; end SUSPECTED;
function TESTED_POSITIVE	output Integer state; algorithm state := 2; end TESTED_POSITIVE;
function LEVEL_1_CONTACT	output Integer state; algorithm state := 3; end LEVEL_1_CONTACT;
function LEVEL_2_CONTACT	output Integer state; algorithm state := 4; end LEVEL_2_CONTACT;
function REMOVED			output Integer state; algorithm state := 5; end REMOVED;

end retQSS_covid19_fsm;
