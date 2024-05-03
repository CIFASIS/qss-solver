#include <math.h>
#include <vector>

#include "retqss_boids.h"

#include <retqss_model_api.h>


extern "C"
{

int boids_update(int boid_id)
{
    int status = 0;
    int count = 0;
    int volume_id = retQSS_particle_currentVolumeID(boid_id), neighbor_id;

    double x, y, z, vx, vy, vz;
    double s_coeff = retQSS_particle_getProperty(boid_id, "separation_coeff");
    double a_coeff = retQSS_particle_getProperty(boid_id, "alignment_coeff");
    double c_coeff = retQSS_particle_getProperty(boid_id, "cohesion_coeff");

    Vector_3 separation(0,0,0), alignment(0,0,0), cohesion(0,0,0);

    retQSS_particle_currentPosition(boid_id, &x, &y, &z);
    Vector_3 boid_pos(x,y,z);

    retQSS_particle_currentVelocity(boid_id, &vx, &vy, &vz);
    Vector_3 boid_v(vx,vy,vz);

    for(int i = 1; i <= retQSS_volumeNeighborhood_countVolumes(volume_id); i++)
    {
        neighbor_id = retQSS_volumeNeighborhood_neighborID(volume_id, i);
        if(neighbor_id == 0) continue;

        count += process_volume(
                    neighbor_id,
                    boid_id,
                    separation,
                    alignment,
                    cohesion);
    }

    count += process_volume(
                volume_id,
                boid_id,
                separation,
                alignment,
                cohesion);

    if(count > 0)
    {
        status = 1;

        alignment /= count;

        cohesion /= count;
        cohesion -= boid_pos;

        boid_v += separation*s_coeff + alignment*a_coeff + cohesion*c_coeff;
    }

    retQSS_particle_setProperty(boid_id, "vx", boid_v[0]);
    retQSS_particle_setProperty(boid_id, "vy", boid_v[1]);
    retQSS_particle_setProperty(boid_id, "vz", boid_v[2]);

	return status;
}

int process_volume(
    int id,
    int boid_id,
    Vector_3 &separation,
    Vector_3 &alignment,
    Vector_3 &cohesion)
{
    double dist;
    double x, y, z, x1, y1, z1, vx, vy, vz;
    double view_angle = retQSS_particle_getProperty(boid_id, "view_angle") * (M_PI/180.);
    double radius = retQSS_particle_getProperty(boid_id, "neighborhood_radius");
    int count = 0;

    retQSS_particle_currentPosition(boid_id, &x, &y, &z);
    Vector_3 boid_pos(x,y,z);

    for(int j = 1; j <= retQSS_volume_countParticlesInside(id); j++)
    {
        int close_boid_id = retQSS_volume_IDOfParticleInside(id, j);
        if(close_boid_id == boid_id) continue;

        if(is_in_neighborhood(boid_id, close_boid_id, radius, view_angle, &dist))
        {
            retQSS_particle_currentPosition(close_boid_id, &x1, &y1, &z1);
            retQSS_particle_currentVelocity(close_boid_id, &vx, &vy, &vz);

            Vector_3 close_boid_pos(x1,y1,z1);
            Vector_3 close_boid_v(vx,vy,vz);

            separation += (boid_pos - close_boid_pos)/dist;
            alignment += close_boid_v;
            cohesion += close_boid_pos;
        
            count++;
        }
    }

    return count;
}

int is_in_neighborhood(
    int boid_id,
    int close_boid_id,
    double radius,
    double view_angle,
    double *distance)
{
    *distance = std::sqrt(retQSS_particle_squaredDistanceBetween(boid_id, close_boid_id));
    double angle = retQSS_particle_angleBetween(boid_id, close_boid_id);

    return *distance < radius && angle <= view_angle;
}

bool boids_forEach(
    ParticleID p,
    ParticleID q,
    const std::vector<double> &args,
    Vector_3 &result)
{
    double xp, yp, zp, xq, yq, zq, vxq, vyq, vzq;
    double n = args[0];

    double a = retQSS_particle_getProperty(p, "alignment_coeff");
    double c = retQSS_particle_getProperty(p, "cohesion_coeff");
    double s = retQSS_particle_getProperty(p, "separation_coeff");

    retQSS_particle_currentPosition(p, &xp, &yp, &zp);
    retQSS_particle_currentPosition(q, &xq, &yq, &zq);
    retQSS_particle_currentVelocity(q, &vxq, &vxq, &vzq);

    Vector_3 p_pos(xp,yp,zp);
    Vector_3 q_pos(xq,yq,zq);
    Vector_3 q_v(vxq,vyq,vzq);

    Vector_3 pq = p_pos - q_pos;
    double norm = std::sqrt(pq.squared_length());

    result += s*pq/norm;
    result += (a/n)*q_v;
    result += (c/n)*q_pos;

    return true;
}

}
