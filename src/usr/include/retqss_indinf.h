#ifndef _RETQSS_INDINF_H_
#define _RETQSS_INDINF_H_

#include <retqss_types.hh>

#ifdef __cplusplus

#include <unordered_map>
#include <list>

enum InfectionStatus {
  Infected,
  NotInfected,
  Recovered,
  Dead,
  Detected,
  Isolated
};

struct ContactData
{
    ParticleID q;
    VolumeID v;
    InfectionStatus p_status, q_status;
    InfectionStatus v_status;
    double time;
};

typedef std::list<ContactData> ContactDataList;
typedef std::unordered_map<ParticleID, ContactDataList> ContactDataMap;

extern "C"
{
#endif

Bool indinf_infectVolume(
    VolumeID v,
    double *evt_times, double *det_times,
    double infection_prob,
    double t,
    double min_evt_time, double max_evt_time,
    double min_detection_time, double max_detection_time);

Bool indinf_dumpOutput(const char *filename);

Bool indinf_checkVolumeInfection(VolumeID v);

Bool indinf_updateContacts(ParticleID p, VolumeID v, double time);

Bool indinf_isolateContacts(ParticleID p, int max);

Bool indinf_dumpGraph(const char *filename, int type);

#ifdef __cplusplus
}
#endif

#endif
