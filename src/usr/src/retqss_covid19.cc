#include "retqss_covid19.h"

#include <retqss_model_api.h>
#include <retqss_interface.hh>
#include <retqss_utilities.hh>

#include <cmath>
#include <fstream>
#include <set>
#include <chrono>
#include <ctime>
#include <cstddef>

int debugLevel;

ContagionConstants contagionConstants;
std::map<int, TracingConstants> tracingConstants;
std::unordered_map<ParticleID, ContactDataList> contactsHistory;

std::ofstream outputCSV("solution.csv");
bool started = false;

#define IC_FILE "initial_conditions.ic"
#define PARAMS_FILE "parameters.config"
#define DEBUG 1

extern "C"
{

int covid19_setDebugLevel(int level)
{
	debugLevel = level;
	return level;
}

int covid19_isDebugLevelEnabled(int level)
{
	return level <= debugLevel;
}

int covid19_debug(int level, double time, const char *format, int int1, int int2, double double1, double double2)
{
	if (covid19_isDebugLevelEnabled(level)) {
	    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	    char * ct = std::ctime(&current_time);
	    ct[strcspn(ct, "\n")] = '\0';
	    printf("[%s] (t=%.2f) ", ct, time);
	    printf(format, (int) int1, (int) int2, double1, double2);
	    printf("\n");
	}
	return level;
}

double covid19_arrayGet(double *array, int index)
{
	return array[index-1];
}

Bool covid19_arraySet(double *array, int index, double value)
{
	array[index-1] = value;
	return true;
}


std::string covid19_getParameter(const char *name) {
	std::ifstream is_file(PARAMS_FILE);
	std::string line;
	while( std::getline(is_file, line) )
	{
	  std::istringstream is_line(line);
	  std::string key;
	  if( std::getline(is_line, key, '=') && key == name)
	  {
	    std::string value;
	    if( std::getline(is_line, value) )
	      return value;
	  }
	}
	return std::string("");
}
int covid19_getIntegerModelParameter(const char *name, int defaultValue) {
	std::string value = covid19_getParameter(name);
	return value == "" ? defaultValue : std::stoi(value);
}

double covid19_getRealModelParameter(const char *name, double defaultValue) {
	std::string value = covid19_getParameter(name);
	return value == "" ? defaultValue : std::stof(value);
}

Bool covid19_setUpParticles(
    int N,
    double cellEdgeLength,
    int gridDivisions,
    double *x)
{
	std::ofstream output(IC_FILE);
	for(int p = 0; p < N; p++)
	{
		for(int c=0;c<6;c++){
			output << x[(p+c*N)*3] << " ";
		}
		double px = x[p*3] / cellEdgeLength;
		double py = x[(p+N)*3] / cellEdgeLength;
		int volumeID = ((int) px) * gridDivisions + ((int) py) + 1;
	    output << " " << volumeID << std::endl;
	}
	output.close();
	retQSS_particle_setUpFromFile(N, IC_FILE, "indirect_infection");
    return true;
}

Bool covid19_setContagionConstants(
	int suceptibleStatus,
	int unknownTrackingStatus,
	int presymptomaticStatus,
	double presymptomaticContagionProbability,
	int symptomaticStatus,
	double symptomaticContagionProbability,
	int asymptomaticStatus,
	double asymptomaticContagionProbability,
	double particleToParticleInfectionProbability)
{
	contagionConstants.suceptibleStatus = suceptibleStatus;
	contagionConstants.unknownTrackingStatus = unknownTrackingStatus;
	contagionConstants.statusContagionProbability.clear();
	contagionConstants.statusContagionProbability[presymptomaticStatus]=presymptomaticContagionProbability;
	contagionConstants.statusContagionProbability[symptomaticStatus]=symptomaticContagionProbability;
	contagionConstants.statusContagionProbability[asymptomaticStatus]=asymptomaticContagionProbability;
	contagionConstants.particleToParticleInfectionProbability=particleToParticleInfectionProbability;
	return true;
}

Bool covid19_setContactsTracingConstants(
	int level,
	int desiredContactsCount,
	double contactRememberedProbBase,
	double contactRememberedProbDecay,
	double delayTimeMin,
	double delayTimeMax,
	double successProbability)
{
	tracingConstants[level] = TracingConstants();
	tracingConstants[level].desiredContactsCount = desiredContactsCount;
	tracingConstants[level].contactRememberedProbBase = contactRememberedProbBase;
	tracingConstants[level].contactRememberedProbDecay = contactRememberedProbDecay;
	tracingConstants[level].delayTimeMin = delayTimeMin;
	tracingConstants[level].delayTimeMax = delayTimeMax;
	tracingConstants[level].successProbability = successProbability;
	return true;
}

Bool covid19_isContagious(int status){
	return contagionConstants.statusContagionProbability.find(status) != contagionConstants.statusContagionProbability.end();
}

Bool covid19_addInfection(int p){
    int count = RETQSS()->particle_get_property(p, "infectionsCount");
    RETQSS()->particle_set_property(p, "infectionsCount", count + 1);
    return true;
}

Bool covid19_tryToInfect(double time, int from, int from_status, int to, double *incubation_start_time)
{
	double infection_probability = contagionConstants.particleToParticleInfectionProbability * contagionConstants.statusContagionProbability[from_status];
	if(retQSS::random_double(0., 1.) < infection_probability && time < incubation_start_time[to]) {
		incubation_start_time[to] = time;
		if (covid19_isDebugLevelEnabled(DEBUG)) {
			covid19_debug(DEBUG, time, "Infection from %d to %d", from+1, to+1, 0, 0);
        }
        covid19_addInfection(from);
        return true;
    } else {
		return false;
	}
}

void covid19_addContact(
	double time,
	int p,
	int q,
	VolumeID volumeId)
{
	ContactDataList& contacts = contactsHistory[p];
	ContactData data;
	data.q = q;
	data.v = volumeId;
	data.time = time;
	contacts.push_back(data);
}

Bool covid19_onContact(double time,	int p, int p_status, int p_tracking_status, int q, double *incubation_start_time)
{
	if (p_tracking_status != contagionConstants.unknownTrackingStatus) {
		return false;
	}
	int q_tracking_status = RETQSS()->particle_get_property(q, "trackingStatus");
	if (q_tracking_status != contagionConstants.unknownTrackingStatus) {
		return false;
	}
	int q_status = RETQSS()->particle_get_property(q, "status");
	if (p_status == contagionConstants.suceptibleStatus && covid19_isContagious(q_status)) {
		return covid19_tryToInfect(time, q, q_status, p, incubation_start_time);
    } else if (q_status == contagionConstants.suceptibleStatus && covid19_isContagious(p_status)) {
		return covid19_tryToInfect(time, p, p_status, q, incubation_start_time);
    } else {
		return false;
    }
}

Bool covid19_interactWithParticlesInVolume(
	double time,
	ParticleID particleId,
	VolumeID volumeId,
	double close_contact_probability,
	double *incubation_start_time)
{
	int int_p = RETQSS_TO_INTERNAL(particleId);
	if (covid19_isDebugLevelEnabled(DEBUG)) {
		covid19_debug(DEBUG, time, "Particle %d entered volume %d", int_p, volumeId, 0, 0);
	}
	int p_status = RETQSS()->particle_get_property(int_p, "status");
	int p_tracking_status = RETQSS()->particle_get_property(int_p, "trackingStatus");
	auto geometry = RETQSS()->geometry();
	for(int int_q : geometry->get_volume(volumeId)->particles_inside()) {
		if(int_p != int_q && retQSS::random_double(0., 1.) < close_contact_probability) {
			covid19_addContact(time, int_p, int_q, volumeId);
			covid19_addContact(time, int_q, int_p, volumeId);
			covid19_onContact(time, int_p, p_status, p_tracking_status, int_q, incubation_start_time);
		}
	}
	return true;
}

int covid19_getContactsCount(ParticleID p)
{
    auto int_p = RETQSS_TO_INTERNAL(p);
	auto it = contactsHistory.find(int_p);
	if (it == contactsHistory.end()) {
		return 0;
	} else {
		return it->second.size();
	}
}

int covid19_traceContacts(
	int level,
	double time,
	ParticleID p,
	double *contactTimeArray)
{
	auto constants = tracingConstants[level];
    auto int_p = RETQSS_TO_INTERNAL(p);
    auto it = contactsHistory.find(int_p);
	int tracedContacts = 0;
    if (it != contactsHistory.end()) {
        auto &contacts = it->second;
        double rememberedProb = constants.contactRememberedProbBase;
        int remainingContacts = constants.desiredContactsCount;
		for(auto rit = contacts.rbegin(); rit != contacts.rend() && remainingContacts > 0 && rememberedProb > 1e-2; 
			rit++, rememberedProb *= constants.contactRememberedProbDecay) {
            if (retQSS::random_double(0., 1.) < rememberedProb) {
                remainingContacts--;
                if (retQSS::random_double(0., 1.) < constants.successProbability) {
                    auto q = rit->q;
                    auto newTime = time + retQSS::random_double(constants.delayTimeMin, constants.delayTimeMax);
                    if (newTime < contactTimeArray[q]) { 
						contactTimeArray[q] = newTime;
                    }
                    tracedContacts++;
                }
            }
        }
    }
	return tracedContacts;
}


Bool covid19_outputCSV(double time,
	int N,
	double *x,
	int V,
	double *volumeConcentration,
    int recoveredCount,
    int infectionsCount)
{
	if(!started) {
		outputCSV << "time";
		for(int i=1; i<=N; i++) {
			outputCSV << ",PX[" << i << "],PY[" << i << "],PS[" << i << "],PTS[" << i << "]";
		}
		for(int i=1; i<=V; i++){
			outputCSV << ",VS[" << i << "],VC[" << i << "]";
		}
		outputCSV << ",recoveredCount,infectionsCount";
		outputCSV << std::endl;
		started = true;
	} 
	outputCSV << std::fixed << std::setprecision(4) << time;
	for(int i=0; i < N; i++){
	    int pStatus = (int) RETQSS()->particle_get_property(i, "status");
	    int pTrackingStatus = (int) RETQSS()->particle_get_property(i, "trackingStatus");
		outputCSV << "," << x[i*3] << "," << x[(i+N)*3] << "," << pStatus << "," << pTrackingStatus;
	}
	outputCSV << std::setprecision(12);
	for(int i=0; i < V; i++){
	    int vClosed = RETQSS()->volume_get_property(i+1, "isClosedSpace")?1:0;
		outputCSV << "," << vClosed << "," << volumeConcentration[i*3];
	}
	outputCSV << "," << recoveredCount << "," << infectionsCount;
	outputCSV << std::endl;
	outputCSV.flush();
	return true;
}

}
