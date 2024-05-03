#include "retqss_md.h"

#include <retqss_model_api.h>

#include <cmath>


extern "C"
{

bool computeForce(
        retQSS::ParticleNeighbor *neighbors,
        const std::vector<double> &args,
        Vector_3 &result)
{
    auto r_cut = args[0];

    auto v_pq = neighbors->shortest_vector();
    auto distance = neighbors->distance();

    if(distance > r_cut)
        return false;

    result += std::exp(-distance) * (v_pq/distance);
        
    return true;
}

Vector_3 computeForceOnNeighborhood(
        retQSS::ParticleNeighborhood *neighborhood,
        double r_cut)
{
    Vector_3 force(0,0,0);

    for(auto neighbor : *neighborhood)
    {
        auto v_pq = neighbor->shortest_vector();
        auto distance = neighbor->distance();

        if(distance > r_cut)
            continue;

        force += std::exp(-distance) * (v_pq/distance);
    }

    return force;
}

int dummy()
{
    return 0;
}

}
