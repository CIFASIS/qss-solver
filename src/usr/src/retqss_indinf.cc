#include "retqss_indinf.h"

#include <retqss_model_api.h>
#include <retqss_interface.hh>
#include <retqss_utilities.hh>

#include <cmath>
#include <fstream>

ContactDataMap contact_data;


extern "C"
{

Bool indinf_infectVolume(
    VolumeID v,
    double *evt_times, double *detection_times,
    double infection_prob,
    double t,
    double min_evt_time, double max_evt_time,
    double min_detection_time, double max_detection_time)
{
    auto geometry = RETQSS()->geometry();
    auto tracker = RETQSS()->tracker();
    
    auto volume = geometry->get_volume(v);
    auto particle_IDs = volume->particles_inside();

    for(auto p : particle_IDs)
    {
        auto particle = tracker->get_particle(p);
        auto status = particle->get_property("status");

        if(status == NotInfected)
        {
            auto q = retQSS::random_double(0., 1.);
            if(q < infection_prob)
            {
                particle->set_property("status", Infected);
                evt_times[p] = t + retQSS::random_double(min_evt_time, max_evt_time);
                detection_times[p] = t + retQSS::random_double(min_detection_time, max_detection_time);
            }
        }
    }

    return true;
}

Bool indinf_checkVolumeInfection(VolumeID v)
{
    auto geometry = RETQSS()->geometry();
    auto tracker = RETQSS()->tracker();
    auto vol = geometry->get_volume(v);
    auto particle_IDs = vol->particles_inside();

    for(auto p : particle_IDs)
    {
        auto particle = tracker->get_particle(p);
        auto status = particle->get_property("status");

        if(status == Infected)
            return true;
    }

    return false;
}

Bool indinf_updateContacts(ParticleID p, VolumeID v, double time)
{
    auto geometry = RETQSS()->geometry();
    auto vol = geometry->get_volume(v);
    int v_status = vol->get_property("status");
    auto particle_IDs = vol->particles_inside();
    auto int_p = RETQSS_TO_INTERNAL(p);
    int p_status = RETQSS()->particle_get_property(int_p, "status"), q_status;

    auto it = contact_data.insert(std::pair<ParticleID, ContactDataList>(int_p, ContactDataList())).first;

    for(auto q : particle_IDs)
        if(int_p != q)
        {
            q_status = RETQSS()->particle_get_property(q, "status");
         
            ContactData data;
            data.q = q;
            data.v = v;
            data.p_status = InfectionStatus(p_status);
            data.q_status = InfectionStatus(q_status);
            data.v_status = InfectionStatus(v_status);
            data.time = time;

            it->second.push_back(data);
        }

    return true;
}

Bool indinf_isolateContacts(ParticleID p, int max)
{
    auto int_p = RETQSS_TO_INTERNAL(p);
    auto it = contact_data.find(int_p);

    if(it != contact_data.end())
    {
        auto &contacts = it->second;
        auto rit = contacts.rbegin();
        auto rend = contacts.rend();
        for(; max > 0 && rit != rend; rit++, max--)
        {
            auto q = rit->q;
            RETQSS()->particle_set_property(q, "status", Isolated);
        }
    }

    return true;
}

Bool indinf_dumpGraph(const char *filename, int type)
{
    std::ofstream file(filename, std::ofstream::out);  
    auto tracker = RETQSS()->tracker();

    for(retQSS::Internal_ParticleID p = 0; p < tracker->num_particles(); p++)
    {
        file << RETQSS_TO_PUBLIC(p);

        auto it = contact_data.find(p);
        if(it != contact_data.end())
        {
            for(auto data : it->second)
                if(data.v_status == type)
                    file << " " << RETQSS_TO_PUBLIC(data.q);
        }

        file << std::endl;
    } 

    return true;
}

Bool indinf_dumpOutput(const char *filename)
{
    std::ofstream file(filename, std::ofstream::out | std::ofstream::app);
    auto tracker = RETQSS()->tracker();
    auto geometry = RETQSS()->geometry();

    int infected = 0;
    int recovered = 0;
    int not_infected = 0;
    int dead = 0;

    for(retQSS::Internal_ParticleID p = 0; p < tracker->num_particles(); p++)
    {
        auto particle = tracker->get_particle(p);
        auto status = particle->get_property("status");

        if(status == Infected || status == Detected)
            infected++;
        else if(status == NotInfected)
            not_infected++;
        else if(status == Recovered)
            recovered++;
        else if(status == Dead)
            dead++;
    }

    file
        << infected << " "
        << not_infected << " "
        << recovered << " "
        << dead << std::endl;

    for(retQSS::Internal_ParticleID p = 0; p < tracker->num_particles(); p++)
    {
        auto particle = tracker->get_particle(p);
        auto status = particle->get_property("status");

        if(p > 0) file << " ";
        file << status;
    }

    file << std::endl;

    for(VolumeID v = 1; v < geometry->num_volumes(); v++)
    {
        auto vol = geometry->get_volume(v);
        auto status = vol->get_property("status");

        if(v > 1) file << " ";
        file << status;
    }

    file << std::endl;

    file.close();

    return true;
}

}
