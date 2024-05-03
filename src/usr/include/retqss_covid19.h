#ifndef _RETQSS_COVID19_H_
#define _RETQSS_COVID19_H_

#include <retqss_types.hh>

#ifdef __cplusplus

#include <unordered_map>
#include <list>

struct ContagionConstants
{
	int suceptibleStatus;
	int unknownTrackingStatus;
	std::unordered_map<int, double> statusContagionProbability;
	double particleToParticleInfectionProbability;
};

struct TracingConstants
{
	int desiredContactsCount;
	double contactRememberedProbBase;
	double contactRememberedProbDecay;
	double delayTimeMin;
	double delayTimeMax;
	double successProbability;
};

struct ContactData
{
    ParticleID q;
    VolumeID v;
    double time;
};

typedef std::list<ContactData> ContactDataList;

extern "C"
{
#endif

int covid19_setDebugLevel(
	int level);

int covid19_debug(
	int level,
	double time,
	const char *format,
	int int1,
	int int2,
	double double1,
	double double2);

double covid19_arrayGet(
	double *array,
	int index);

Bool covid19_arraySet(
	double *array,
	int index,
	double value);

int covid19_getIntegerModelParameter(
	const char *name,
	int defaultValue);

double covid19_getRealModelParameter(
	const char *name,
	double defaultValue);

Bool covid19_setUpParticles(
    int N,
    double cellEdgeLength,
    int gridDivisions,
    double *x);

Bool covid19_setContagionConstants(
	int suceptibleStatus,
	int unknownTrackingStatus,
	int presymptomaticStatus,
	double presymptomaticContagionProbability,
	int symptomaticStatus,
	double symptomaticContagionProbability,
	int asymptomaticStatus,
	double asymptomaticContagionProbability,
	double particleToParticleInfectionProbability);

Bool covid19_setContactsTracingConstants(
	int level,
	int desiredContactsCount,
	double contactRememberedProbBase,
	double contactRememberedProbDecay,
	double delayTimeMin,
	double delayTimeMax,
	double successProbability);

Bool covid19_addInfection(int p);

Bool covid19_interactWithParticlesInVolume(
	double time,
	ParticleID p,
	VolumeID v,
    double closeContactProbability,
    double *incubation_start_time);

int covid19_getContactsCount(ParticleID p);

int covid19_traceContacts(
	int level,
	double time,
	ParticleID p,
	double *contactTimeArray);

Bool covid19_outputCSV(double time, 
	int N,
	double *x,
	int V,
	double *volumeConcentration,
    int recoveredCount,
    int infectionsCount);

#ifdef __cplusplus
}
#endif

#endif
