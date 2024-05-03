#ifndef _RETQSS_MD_H_
#define _RETQSS_MD_H_

#include <retqss_types.hh>

#ifdef __cplusplus

#include <vector>
#include <retqss_cgal_main_types.hh>
#include <retqss_particle_neighbor.hh>
#include <retqss_particle_neighborhood.hh>

extern "C"
{

bool computeForce(
        retQSS::ParticleNeighbor*,
        const std::vector<double>&,
        Vector_3&);

Vector_3 computeForceOnNeighborhood(
        retQSS::ParticleNeighborhood*,
        double);
#endif


int dummy();

#ifdef __cplusplus
}
#endif

#endif
